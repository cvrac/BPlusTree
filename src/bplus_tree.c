#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#include "bplus_tree.h"
#include "AM.h"
#include "scans_management.h"
#include "BF.h"
#include "util.h"
#include "path.h"

extern int max_open_files;

/* ************************************************************************ */

int Create_Block(int bf, char b_type);
int Search(void *value1, int bf, Path *path);
int Insert_Record(int bf, int block_id, void *value1, void *value2);
int Split_Data_Block(void *value1, void *value2, int block_id, Path path, int bf);
int Split_Index_Block(void *value_key, int l_block_id, int r_block_id, int block_id, Path *path, int bf);
int Insert_Index(int bf, int l_block_id, int r_block_id, void *value, Path *p);
int Insert_Index_Root(int bf, int l_block, char *value, int r_block);
int Identical_Values(int bf, int block_id, int b1_pos, char *tmp, int remaining_values);

int AM_CreateIndex(char *fileName, char attrType1, int attrLength1, char attrType2, int attrLength2)
{ /* Create a new B+ tree index, initialise first information block */

    // Make sure file doesn't exist
    struct stat buf;
    if (stat (fileName, &buf) == 0) {
        AM_errno = AME_FILE_EXISTS;
        return -1;
    }

    // Parse attributes
    if (attrType1 != STRING && attrType1 != INTEGER && attrType1 != FLOAT ||
            attrType2 != STRING && attrType2 != INTEGER && attrType2 != FLOAT) {
        AM_errno = AME_WRONG_ATTR_TYPE;
        return -1;
    }
    if (attrType1 == INTEGER && attrLength1 != 4 ||
            attrType2 == INTEGER && attrLength2 != 4) {
        AM_errno = AME_WRONG_ATTR_LEN_I;
        return -1;
    }
    if (attrType1 == FLOAT && attrLength1 != 4 ||
            attrType2 == FLOAT && attrLength2 != 4) {
        AM_errno = AME_WRONG_ATTR_LEN_F;
        return -1;
    }
    if (attrType1 == STRING && (attrLength1 < 1 || attrLength1 > 255) ||
            attrType2 == STRING && (attrLength2 < 1 || attrLength2 > 255)) {
        AM_errno = AME_WRONG_ATTR_LEN_C;
        return -1;
    }

    // Create file
    if (BF_CreateFile(fileName) < 0) {
        AM_errno = AME_BF_CREATE;
        return -1;
    }
    int bf;
    if ((bf = BF_OpenFile(fileName)) < 0) {
        AM_errno = AME_BF_OPEN;
        return -1;
    }

    // First block allocation
    if (BF_AllocateBlock(bf) < 0) {
        AM_errno = AME_BF_ALLOCATEBLOCK;
        return -1;
    }
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    // Data information in the first block
    info[F_TYPE] = 'b';
    info[TYPE1] = attrType1;
    *(int *)(info+LEN1) = attrLength1;
    info[TYPE2] = attrType2;
    *(int *)(info+LEN2) = attrLength2;
    *(int *)(info+ROOT) = -1;

    if (BF_WriteBlock(bf, BF_GetBlockCounter(bf)-1) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }

    if (BF_CloseFile(bf) < 0) {
        AM_errno = AME_BF_CLOSE;
        return -1;
    }

    AM_errno = AME_OK;
    return 0;
}

int AM_InsertEntry(int fileDesc, void *value1, void *value2)
{ /* Insert value1-value2 in the right data block. If full, split it and insert an index to the
   * index block a level above. If full too, split too and insert an index to an even upper level.
   * Update root if necassary. */

    /* Desc in bounds check */
    if (fileDesc < 0 || fileDesc >= MAXOPENFILES) {
        AM_errno = AME_OUT_OF_BOUNDS;
        return -1;
    }

    /* Open index check */
    if (open_inds[fileDesc] == NULL) {
        AM_errno = AME_F_NOT_OPEN;
        return -1;
    }

    /* Open file */
    int bf = open_inds[fileDesc]->filedesc;

    /* Get info block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Add terminating null character to strings if they are larger than supported len */
    if (info[TYPE1] == STRING && strlen(value1) >= *(int *)(info+LEN1))
        *(char *)(value1 + *(int *)(info+LEN1)-1) = '\0';
    if (info[TYPE2] == STRING && strlen(value2) >= *(int *)(info+LEN2))
        *(char *)(value2 + *(int *)(info+LEN2)-1) = '\0';

    /* If there is no data */
    if (BF_GetBlockCounter(bf) == 1) {
        int b_dt;

        /* Create root data block */
        if ((b_dt = Create_Block(bf, 'd')) < 0)
            return -1;

        /* Insert data */
        if (Insert_Record(bf, b_dt, value1, value2) < 0)
            return -1;
        AM_errno = AME_OK;
        return AME_OK;
    }

    /* If there is data */
    Path path = path_Init();
    /* Find the data block where value1 is located and hold the path. 'block' will also point to it */
    int block_id = Search( value1, bf, &path);

    /* Get block */
    char *block;
    if (BF_ReadBlock(bf, block_id, (void **) &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Reached data block *
     * If there is space for entry, insert record */
    if (*(int *)(block+BD_ADDR) + *(int *)(info+LEN1) + *(int *)(info+LEN2) < BLOCK_SIZE) {
        if (Insert_Record(bf, block_id, value1, value2) < 0)
            return -1;
    }
    else {
        /* Or split data block */
        if (Split_Data_Block(value1, value2, block_id, path, bf) < 0)
            return -1;
    }

    path_Destroy(path);
    AM_errno = AME_OK;
    return AME_OK;
}

int Search(void *value1, int bf, Path *path)
{/* Search for the data block where value1 is located
  * Return the block ID, path will hold the index
  * block ID path to reach it */

    char *info;
    /* Get info  block */
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    char *block;
    /* Get root block */
    if (BF_ReadBlock(bf, *(int *)(info+ROOT), (void **) &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    int key;                             // Position in block corresponding to the key we are currently checking
    int pointer;                         // Position in block corresponding to the pointer placed left to the key we are checking
    int block_id = *(int *)(info+ROOT);  // Id of the block pointed to by pointer ; start from data root node in case there are no index blocks

    /* For all index blocks:
     * Iterate through the block to find a deeper block to search into */
    while (block[BI_TYPE] == 'i') {
        key = BI_P0 + sizeof(int);       // Start from the first key
        if (path != NULL) {
            path_Update(path, block_id); // Update path
        }

        /* While value1 >= key and there is another key-pointer duo in the block, try the next key */
        while (key < *(int *)(block+BI_ADDR) && GrtOrEq(value1, (void *) (block+key), info[TYPE1])) {
            key += (*(int *)(info+LEN1) + sizeof(int));   // Next key
        }

        block_id = *(int *)(block+key-sizeof(int)); // The block where the pointer leads

        /* Get the chosen block */
        if (BF_ReadBlock(bf, block_id, (void **) &block) < 0) {
            AM_errno = AME_BF_READBLOCK;
            return -1;
        }
    }
    return block_id; // Reached data block
}

int Split_Data_Block(void *value1, void *value2, int block_id, Path path, int bf)
{ /* Split data block in two, insert an index to the index block of the upper level */

    /* Get info block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Get block */
    char *block;
    if (BF_ReadBlock(bf, block_id, (void **) &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Get number of records to split on each block */
    int record_len = (*(int *)(info+LEN1) + *(int *)(info+LEN2)); // Total size of a record
    int n = (*(int *)(block+BD_ADDR) - (BD_START)) / record_len;  // Total records in block
    int n1 = (n+1) / 2; // Recommended number of records to keep in block1. Less than that in case that multiple
                        // identical values don't fit in block1, or more than that in case that multiple
                        // identical values don't fit in block2 and need to stay in block1.

    /* Hold a record, start with the one we need to insert */
    char *tmp = malloc(record_len);
    if (tmp== NULL) {
        return -1;
    }
    memcpy((void *) tmp, value1, *(int *)(info+LEN1));
    memcpy((void *) (tmp + *(int *)(info+LEN1)), value2, *(int *)(info+LEN2));

    /* For each record in original block, check if tmp can be put in the place of a record to maintain sorting.
     * If so, swap that record with tmp and repeat for all records in original block */
    int b1_pos = BD_START;
    int inserted = 0;          // Number of values block1 currenty has
    while (inserted < n1) {

        if (Less((void *) tmp, (void *) (block+b1_pos), info[TYPE1])) {
            /* Swap tmp here if sorting will be maintained */
            memswap((void *) tmp, (void *) (block+b1_pos), record_len);
        }

        /* If identical values (including tmp) are spotted, make sure they stay on this block if they fit. If not, go to the next */
        int identical = Identical_Values(bf, block_id, b1_pos, tmp, n-inserted); // Get number of identical values
        /* Make sure identical values are less or equal to the total block capacity */
        if (identical+1 > n) {
            AM_errno = AME_IDENTICAL;
            return -1;
        }

        /* Can't fit in original block ; put them in the next */
        if (identical+1 > n-inserted)
            break;
        else if (identical) {
            /* Can fit in orignal block ; they stay here (including tmp) */
            if (Equal((void *)(block+b1_pos), tmp, info[TYPE1])) {          // If tmp is one of the identical values
                b1_pos += identical*record_len;                             // Leave the ones already in their place alone
                memswap((void *) tmp, (void *) (block+b1_pos), record_len); // Swap the first non identical value with tmp
                b1_pos += record_len;                                       // Move to the next record
            }
            else
                b1_pos += ((identical+1)*record_len);                       // No swapping ; go right away to the next record
            inserted += (identical+1);                                      // Inserted this string of identical values
            continue;
        }
        b1_pos += record_len;                                               // Move to the next record
        inserted++;
    }
    *(int *)(block+BD_ADDR) = BD_START + inserted*record_len;   // Update next free space in original block.

    /* Get new block */
    int new_block_id = Create_Block(bf, 'd');
    char *new_block;
    if (BF_ReadBlock(bf, new_block_id, (void **) &new_block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* New block will point where the original pointed */
    *(int *)(new_block + BD_RIGHT) = *(int *)(block + BD_RIGHT);

    /* Original block will point to the new one */
    *(int *)(block + BD_RIGHT) = new_block_id;

    /* Insert remaining records at new block. Insert_Record will keep them sorted */
    int n2 = n-inserted;
    while (n2--) {
        Insert_Record(bf, new_block_id, (void *) (block+b1_pos), (void *) (block+b1_pos + *(int *)(info+LEN1)));
        b1_pos += record_len;   // Next record to insert from original block
    }

    /* Finally Insert tmp */
    Insert_Record(bf, new_block_id, (void *) tmp, (void *) (tmp + *(int *)(info+LEN1)));

    /* Write original block back */
    if (BF_WriteBlock(bf, block_id) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }

    /* Write new block back */
    if (BF_WriteBlock(bf, new_block_id) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }

    free(tmp);

    /* If the data block used to be the root, create a new index
     * as root and point to the two data blocks. Else insert index on index block above */
    if (path_isEmpty(path))
        Insert_Index_Root(bf, block_id, new_block+BD_START, new_block_id);
    else
        Insert_Index(bf, block_id, new_block_id, (void *)(new_block+BD_START), &path);
}

int Identical_Values(int bf, int block_id, int b1_pos, char *tmp, int remaining_values)
{ /* Return the number of pairs of consecutive identical values starting in block[pos],
   * including tmp value. (If there are n identical values, returns n-1) */

    /* Get root block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0)
        AM_errno = AME_BF_READBLOCK;

    /* Get block */
    char *block;
    if (BF_ReadBlock(bf, block_id, (void **) &block) < 0)
        AM_errno = AME_BF_READBLOCK;

    int identical = 0;
    int b1_initial_pos = b1_pos;       // The position of the value we check for equality with the others
    int iterate = remaining_values-1;
    b1_pos += (*(int *)(info+LEN1) + *(int *)(info+LEN2)); // Start comparing with the next value onwards
    while (iterate--) {
        if (!Equal((void *)(block+b1_pos), (void *)(block+b1_initial_pos), info[TYPE1]))
            break;
        identical++;
        b1_pos += (*(int *)(info+LEN1) + *(int *)(info+LEN2)); // Go to the next record
    }

    /* Also compare with tmp */
    if (Equal(tmp, (void *)(block+b1_initial_pos), info[TYPE1]))
        identical++;
    return identical;
}

int Split_Index_Block(void *value, int l_block_id, int r_block_id, int block_id, Path *path, int bf)
{ /* Split index block in two, send an index to the index block of the upper level */
    /* Get info block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Get block */
    char *block;
    if (BF_ReadBlock(bf, block_id, (void **) &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Get number of keys to split on each block */
    int pair_len = *(int *)(info+LEN1) + sizeof(int);                     // Total size of a pair
    int n = (*(int *)(block+BI_ADDR) - (BI_P0+sizeof(int))) / pair_len;   // Total pairs in block
    int n1 = n / 2; // Pairs to keep in block1

    /* Hold a pair, start with the one we need to insert */
    char *tmp = malloc(pair_len);
    if (tmp == NULL) {
        return -1;
    }
    memcpy((void *) tmp, value, *(int *)(info+LEN1));
    memcpy((void *) (tmp + *(int *)(info+LEN1)), &r_block_id, sizeof(int));

    /* For each pair's key in original block, check if tmp can be put in the place of a key to maintain sorting.
     * If so, swap that pair with tmp and repeat for all pairs in original block */
    int b1_pos = BI_P0+sizeof(int);
    int inserted = 0;          // Number of values block1 currently has
    while (inserted < n1) {
        if (Less((void *) tmp, (void *) (block+b1_pos), info[TYPE1]))  {
            memswap((void *) tmp, (void *) (block+b1_pos), pair_len);
            if (!inserted)                                            // If we inserted value in the first position,
                *(int *)(block+BI_P0) = l_block_id;                   // also update P0 pointer.
        }
        b1_pos += pair_len;                                           // Move to the next pair
        inserted++;
    }
    *(int *)(block+BI_ADDR) = BI_P0+sizeof(int) + inserted*pair_len;  // Update next free space in original block

    /* Swap tmp with the next value in original block if tmp is smaller */
    if (Less((void *) tmp, (void *) (block+b1_pos), info[TYPE1]))
        memswap((void *) tmp, (void *) (block+b1_pos), pair_len);

    /* Store the smallest value between the above. It will be sent to an upper level  in the tree for insertion */
    char *to_insert = malloc(*(int *)(info+LEN1));
    if (to_insert == NULL)
        return -1;
    memcpy(to_insert, block+b1_pos, *(int *)(info+LEN1));

    /* Move on to the next value in original block */
    b1_pos += pair_len;

    /* Get new block */
    int new_block_id = Create_Block(bf, 'i');
    char *new_block;
    if (BF_ReadBlock(bf, new_block_id, (void **) &new_block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Update new block's P0 */
    *(int *)(new_block+BI_P0) = *(int *)(block+b1_pos-sizeof(int));

    /* Insert remaining pairs at new block in the same manner */
    int n2 = n-inserted;
    int b2_pos = BI_P0+sizeof(int);
    while (--n2) {
        if (Less((void *) tmp, (void *) (block+b1_pos), info[TYPE1]))     // Keep the larger value in tmp
            memswap((void *) tmp, (void *) (block+b1_pos), pair_len);
        memcpy(new_block+b2_pos, block+b1_pos, pair_len);                 // Insert smaller value in new block
        b1_pos += pair_len;   // Next pair to insert from original block
        b2_pos += pair_len;   // Next position in new block
    }

    /* Finally Insert tmp */
    memcpy(new_block+b2_pos, tmp, pair_len);

    /* Update next free space in new block */
    *(int *)(new_block+BI_ADDR) = b2_pos;

    /* Write original block back */
    if (BF_WriteBlock(bf, block_id) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }

    /* Write new block back */
    if (BF_WriteBlock(bf, new_block_id) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }

    free(tmp);

    /* If the index block split used to be the root, create a new index
     * as root and point to the two index blocks */
    if (path_isEmpty(*path))
        Insert_Index_Root(bf, block_id, (void *)(to_insert), new_block_id);
    else
        Insert_Index(bf, block_id, new_block_id, (void *)(to_insert), path);
    free(to_insert);
}

int Insert_Record(int bf, int block_id, void *value1, void *value2)
{ /* Insert values in data block while keeping it sorted */

    /* Get root block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0)
        AM_errno = AME_BF_READBLOCK;

    /* Get data block */
    char *block;
    if (BF_ReadBlock(bf, block_id, (void **) &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    int addr = *(int *)(block+BD_ADDR); // Next free byte in block
    int key = BD_START;                 // Current key position
    int record_len = (*(int *)(info+LEN1) + *(int *)(info+LEN2)); // Total size of a record

    /* Find the right position to maintain sorting */
    while (key < addr && GrtOrEq(value1, (void *) (block+key), info[TYPE1])) {
        key += record_len;
    }

    if (key != addr) {
        memmove(block + key + record_len, block + key, addr-key);
    }

    /* Update next free byte */
    addr += record_len;
    memcpy(block+BD_ADDR, &addr, sizeof(int));

    /* Copy value1 */
    memcpy(block+key, value1, *(int *)(info+LEN1));

    /* Copy value2 */
    key += *(int *)(info+LEN1);    // Point to value2
    memcpy(block+key, value2, *(int *)(info+LEN2));

    /* Write block back */
    if (BF_WriteBlock(bf, block_id) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }
}

int Insert_Index(int bf, int l_block_id, int r_block_id, void *value, Path *p)
{ /* Insert value-pointer pair in index block while keeping it sorted.
   * In case insertion needs to happen in the first value spot in the block,
   * we also need to update the left pointer along with the right one */

    /* Get root block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0)
        AM_errno = AME_BF_READBLOCK;

    /* Get father of last block */
    int block_id = path_Pop(p);
    char *block;
    if (BF_ReadBlock(bf, block_id, (void **) &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    int pair_len = *(int *)(info+LEN1) + sizeof(int); // Total size of a value-pointer pair

    /* If value doesn's fit, split index block */
    if (*(int *)(block+BI_ADDR) + pair_len >= BLOCK_SIZE) {
        Split_Index_Block(value, l_block_id, r_block_id, block_id, p, bf);
        return 0;
    }

    /* Find the right position to maintain sorting */
    int addr = *(int *)(block+BI_ADDR); // Next free byte in block
    int key = BI_P0 + sizeof(int);      // Current key position
    while (key < addr && GrtOrEq(value, (void *) (block+key), info[TYPE1])) {
        key += pair_len;
    }

    /* If placing value in-between the others to maintain sorting ; move them to the right */
    if (key != addr) {
        memmove(block + key + pair_len, block + key, addr-key);
    }

    /* Update next free byte */
    addr += pair_len;
    *(int *)(block+BI_ADDR) = addr;

    /* Insert value */
    memcpy(block+key, value, *(int *)(info+LEN1));

    /* Update right pointer */
    *(int *)(block+key + *(int *)(info+LEN1)) = r_block_id;

    /* If value is put at the start of the index block, also update left pointer P0 */
    if (key == BI_P0 + sizeof(int)) {
        *(int *)(block + BI_P0) = l_block_id;
    }
    /* Write block back */
    if (BF_WriteBlock(bf, block_id) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }
}

int Insert_Index_Root(int bf, int l_block, char *value, int r_block)
{ /* Create index root and manage pointers */

    /* Get root block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0)
        AM_errno = AME_BF_READBLOCK;

    /* Allocate block and get it */
    int root_id = Create_Block(bf, 'i');
    char *root;
    if (BF_ReadBlock(bf, root_id, (void **) &root) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Initialise */
    *(int *)(root+BI_P0) = l_block;   // Left pointer
    memcpy(root+BI_P0+sizeof(int), value, *(int *)(info+LEN1)); // Value
    *(int *)(root+BI_P0+sizeof(int) + *(int *)(info+LEN1)) = r_block; // Right pointer
    *(int *)(root+BI_ADDR) = BI_P0+2*sizeof(int) + *(int *)(info+LEN1); // Next free byte in block

    /* Update info block with new root */
    *(int *)(info+ROOT) = root_id;

    /* Write index block back */
    if (BF_WriteBlock(bf, root_id) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }

    /* Write info block back */
    if (BF_WriteBlock(bf, 0) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }
}

int Create_Block(int bf, char b_type)
{ /* Allocates a block depending on argument b_type and returns its id.
   * 'd' : Data block allocation. The second block allocated in the program
   *       will be the root, so it handles that as well.
   * 'i' : Index block allocation. */

    /* Get info block */
    char *info;
    if (BF_ReadBlock(bf, 0, (void **) &info) < 0)
        AM_errno = AME_BF_READBLOCK;

    /* Block allocation */
    if (BF_AllocateBlock(bf) < 0) {
        AM_errno = AME_BF_ALLOCATEBLOCK;
        return -1;
    }
    char *block;
    if (BF_ReadBlock(bf, BF_GetBlockCounter(bf)-1, (void **) &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /* Data block */
    if (b_type == 'd') {
        /* Information in the data block */
        block[BD_TYPE] = b_type;              // Block type
        *(int *)(block + BD_RIGHT) = -1;      // Pointer to right block ; none yet
        *(int *)(block + BD_ADDR) = BD_START; // Next free byte in block

        /* Is it root? */
        if (BF_GetBlockCounter(bf) == 2) {
            /* Update root indication on info block */
            *(int *)(info+ROOT) = BF_GetBlockCounter(bf)-1;

            /* Write info block back */
            if (BF_WriteBlock(bf, 0) < 0) {
                AM_errno = AME_BF_WRITEBLOCK;
                return -1;
            }
        }
    }
    else if (b_type == 'i') {
        /* Index Block */
        block[BI_TYPE] = b_type;                       // Block type
        *(int *)(block+BI_P0) = -1;                    // Pointer P0 ; none yet
        *(int *)(block+BI_ADDR) = BI_P0+sizeof(int);   // Next free byte in block
    }

    /* Write block back */
    if (BF_WriteBlock(bf, BF_GetBlockCounter(bf)-1) < 0) {
        AM_errno = AME_BF_WRITEBLOCK;
        return -1;
    }
    return BF_GetBlockCounter(bf)-1;
}

void AM_PrintError(char *errString)
{
    printf("%s: ", errString);
    switch (AM_errno) {
        case AME_OK: printf("No error\n"); break;
        case AME_EOF: printf("Reached end of file\n"); break;
        case AME_BF_CREATE:
                      printf("Cannot create file\n");
                      BF_PrintError("Error in BF_CreateFile");
                      break;
        case AME_BF_OPEN:
                      printf("Cannot open file\n");
                      BF_PrintError("Error in BF_OpenFile\n");
                      break;
        case AME_BF_ALLOCATEBLOCK:
                      printf("Cannot allocate data info block\n");
                      BF_PrintError("Error in BF_AllocateBlock");
                      break;
        case AME_BF_READBLOCK:
                      printf("Cannot get pointer to block\n");
                      BF_PrintError("Error in BF_ReadBlock\n");
                      break;
        case AME_BF_WRITEBLOCK:
                      printf("Cannot save block\n");
                      BF_PrintError("Error in BF_WriteBlock\n");
                      break;
        case AME_FILE_EXISTS: printf("Index file already exists\n"); break;
        case AME_NO_BTREE: printf("This is not a B+ tree index file\n"); break;
        case AME_MAXOPENINDEX: printf("Reached max amount of open index files\n"); break;
        case AME_MAXOPENSCANS: printf("Reached max amount of open scan files\n"); break;
        case AME_WRONG_ATTR_TYPE: printf("Wrong attribute type: expected 'i' for int, 'f' for float or 'c' for string\n"); break;
        case AME_WRONG_ATTR_LEN_I : printf("Wrong integer length: expected 4\n"); break;
        case AME_WRONG_ATTR_LEN_F : printf("Wrong float length: expected 4\n"); break;
        case AME_WRONG_ATTR_LEN_C : printf("Wrong string length: expected 1-255\n"); break;
        case AME_WRONG_OP: printf("Wrong operation: expected EQUAL, GREATER_THAN, GREATER_THAN_OR_EQUAL, NOT_EQUAL, LESS_THAN, or LESS_THAN_OR_EQUAL"); break;
        case AME_F_ALREADY_OPEN: printf("File is already open\n"); break;
        case AME_F_NOT_OPEN: printf("File is not open\n"); break;
        case AME_CLOSESCAN: printf("Scan is closed\n"); break;
        case AME_OPENSCAN: printf("Scan is open\n"); break;
        case AME_OUT_OF_BOUNDS: printf("File descriptor out of bounds\n"); break;
        case AME_IDENTICAL: printf("Too many identical values in data block\n"); break;
    }
}
