#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "BF.h"
#include "bplus_tree.h"
#include "scans_management.h"
#include "util.h"

int maxOpenFiles =  MAXOPENFILES;
void UpdateScan(int scandesc, int nextbyte, int nextblock);

void AM_Init() {
    /*Initialize BF level, global structures for open open_inds, open scans*/

    int i;
    BF_Init();
    AM_errno = 0;

    for (i = 0; i < MAXOPENFILES; i++) {
        if (open_inds[i] != NULL)
            free(open_inds[i]);
        open_inds[i] = NULL;
    }

    for (i = 0; i < MAXSCANS; i++){
        if (scans[i] != NULL)
            free(scans[i]);
        scans[i] = NULL;
    }

    open_indes = 0;
    open_scans = 0;

}

int AM_DestroyIndex(char *fileName) {
    /*Check if the file is open. If not destroy it, else error.
     *AM_OpenIndexScan will not open a scan if the index isn't open
     *already, so this function does not need to check for open scans*/

    int i;

    /* Open file check */
    for (i = 0; i < MAXOPENFILES; i++)
        if (open_inds[i] != NULL && !strcmp(open_inds[i]->filename, fileName)) {
            AM_errno = AME_F_ALREADY_OPEN;
            return -1;
        }

    remove(fileName);
    printf("Removing file %s\n", fileName);
    AM_errno = AME_OK;
    return AME_OK;
}

int AM_OpenIndex(char *fileName) {
    int i, filedesc;

    /*Make sure there's still space */
    if (open_indes == MAXOPENFILES) {
        AM_errno = AME_MAXOPENINDEX;
        return -1;

    }

    /*Open index, if there's still space, for the file fileName*/

    for (i = 0; i < MAXOPENFILES; i++) {
        if (open_inds[i] == NULL) {
            open_inds[i] = malloc(sizeof(index_info));
            assert(open_inds[i] != NULL);
            if ((filedesc = BF_OpenFile(fileName)) < 0) {
                AM_errno = AME_BF_OPEN;
                return -1;

            } else {
                /*Get info block, make sure this is a B+ tree index file*/
                char *info;
                if (BF_ReadBlock(filedesc, 0, (void **) &info) < 0) {
                    AM_errno = AME_BF_READBLOCK;
                    return -1;
                }
                if (info[0] != 'b') {
                    AM_errno = AME_NO_BTREE;
                    return -1;
                }
                strcpy(open_inds[i]->filename, fileName);
                open_inds[i]->filedesc = filedesc;
                open_indes++;
                break;
            }
        }
    }

    AM_errno = AME_OK;
    return i;
}

int AM_CloseIndex(int fileDesc) {
    /*Check if there's not an open scan for the file, close it and remove the
     *  equivalent entry from the array of open index*/

    int posindex = fileDesc, bfdesc, i;

    /* Desc in bounds check */
    if (fileDesc < 0 || fileDesc >= MAXOPENFILES) {
        AM_errno = AME_OUT_OF_BOUNDS;
        return -1;
    }

    /* Open index check */
    if (open_inds[posindex] == NULL) {
        AM_errno = AME_F_NOT_OPEN;
        return -1;
    }

    /* Open scan check */
    for (i = 0; i < MAXSCANS; i++)
        if (scans[i] != NULL && scans[i]->filedesc == fileDesc) {
            AM_errno = AME_OPENSCAN;
            return -1;
        }

    bfdesc = open_inds[posindex]->filedesc;
    if (BF_CloseFile(bfdesc) < 0) {
        AM_errno = AME_BF_CLOSE;
        return -1;
    }

    free(open_inds[posindex]);
    open_inds[posindex] = NULL;
    open_indes--;
    AM_errno = AME_OK;
    return 0;
}



int AM_OpenIndexScan(int fileDesc, int op, void *value) {
    /*Opens a scan for the file with descriptor fileDesc*/
    /*Search for the first record/block which satisfies the constraints*/
    int i = 0, scdesc = -1, blockid, rootid, offs, typ1, typ2, found = 0, last;
    int start;
    void *block, *infoblock;

    /* Max open scans check */
    if (open_scans == MAXSCANS) {
        AM_errno = AME_MAXOPENSCANS;
        return -1;
    }

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

    /* Correct op check */
    if (op != EQUAL && op != GREATER_THAN && op != GREATER_THAN_OR_EQUAL &&
        op != NOT_EQUAL && op != LESS_THAN && op != LESS_THAN_OR_EQUAL) {
        AM_errno = AME_WRONG_OP;
        return -1;
    }

    for (i = 0; i < MAXSCANS; i++) {
        if (scans[i] == NULL) {
            scans[i] = malloc(sizeof(scan_info));
            assert(scans[i] != NULL);
            scans[i]->filedesc = fileDesc;
            scans[i]->bfdesc = open_inds[fileDesc]->filedesc;
            scans[i]->op = op;
            scans[i]->value = value;
            scdesc = i;
            open_scans++;
            break;

        }
    }

    if (BF_ReadBlock(scans[scdesc]->bfdesc, 0, &infoblock) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    /*get root block id*/
    memcpy(&rootid, infoblock + ROOT, sizeof(int));

    /*get size of record, and the type of the key*/
    memcpy(&typ1, infoblock + LEN1, sizeof(int));
    memcpy(&typ2, infoblock + LEN2, sizeof(int));
    scans[scdesc]->v1_length = typ1;
    scans[scdesc]->v2_length = typ2;

    memcpy(&scans[scdesc]->v_type, infoblock + TYPE1, sizeof(char));

    offs = typ1 + typ2;

    if (BF_ReadBlock(scans[scdesc]->bfdesc, rootid, &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return -1;
    }

    scans[scdesc]->lastrecordoffset = BD_START;
    switch (scans[scdesc]->op) {
        case EQUAL:
        case GREATER_THAN:
        case GREATER_THAN_OR_EQUAL:
            /*Search for the data block*/
            scans[scdesc]->lastblock = Search(value, scans[scdesc]->bfdesc, NULL);
            if (BF_ReadBlock(scans[scdesc]->bfdesc, scans[scdesc]->lastblock, &block) < 0) {
                AM_errno = AME_BF_READBLOCK;
                return -1;
            }

            memcpy(&last, block + BD_ADDR, sizeof(int));
            for (i = BD_START; i < last; i += offs) {
                if (GrtOrEq(block+i, value, scans[scdesc]->v_type)) {
                    found = 1;
                    break;
                }

            }
            if (found)  scans[scdesc]->lastrecordoffset = i;
            break;
        case NOT_EQUAL:
        case LESS_THAN:
        case LESS_THAN_OR_EQUAL:
            scans[scdesc]->lastblock = 1;
            if (BF_ReadBlock(scans[scdesc]->bfdesc, scans[scdesc]->lastblock, &block) < 0) {
                AM_errno = AME_BF_READBLOCK;
                return -1;

            }
            scans[scdesc]->lastrecordoffset = BD_START;
            break;

    }

    AM_errno = AME_OK;
    return scdesc;
}

int AM_CloseIndexScan(int scanDesc) {
/* Close scanDesc scan if it exists */
    if (scanDesc < 0 || scanDesc >= MAXSCANS) {
        AM_errno = AME_OUT_OF_BOUNDS;
        return AM_errno;
    }

    if (scans[scanDesc] != NULL) {
        free(scans[scanDesc]);
        scans[scanDesc] = NULL;
        open_scans--;
        AM_errno = AME_OK;
        return AM_errno;
    }

    AM_errno = AME_CLOSESCAN;
    return AM_errno;

}

void *AM_FindNextEntry(int scanDesc) {

    /* Desc in bounds check */
    if (scanDesc < 0 || scanDesc >= MAXSCANS) {
        AM_errno = AME_OUT_OF_BOUNDS;
        return NULL;
    }

    /* Open scan check */
    if (scans[scanDesc] == NULL) {
        AM_errno = AME_CLOSESCAN;
        return NULL;
    }


    /*Get last block and last record saved on the scan struct*/
    /*If there are no other records, AME_EOF*/
    /*Else, check op cases*/

    int lastblock = scans[scanDesc]->lastblock, lastrec = scans[scanDesc]->lastrecordoffset;
    int nextemptybyte, nextblock;
    void *block;
    void *key, *value2;

    if (lastblock == -1) {
        AM_errno = AME_EOF;
        return NULL;
    }


    if (BF_ReadBlock(scans[scanDesc]->bfdesc, lastblock, &block) < 0) {
        AM_errno = AME_BF_READBLOCK;
        return NULL;
    }

    memcpy(&nextemptybyte, block + BD_ADDR, sizeof(int));
    memcpy(&nextblock, block + BD_RIGHT, sizeof(int));

    if (nextblock == -1) {
        /*If lastrecord is the next available free byte, then there are no more records, which means we
         *          * reached the end. Return NULL and AM_errno = AME_EOF.*/
        if (lastrec == nextemptybyte) {
            AM_errno = AME_EOF;
            return NULL;
        }
    }

    /*Query*/
    if (scans[scanDesc]->op == EQUAL) {
        if (!Equal(block + scans[scanDesc]->lastrecordoffset, scans[scanDesc]->value, scans[scanDesc]->v_type)) {
            /*No more equals, return NULL*/
            AM_errno = AME_EOF;
            return NULL;

        }
        /*Update scan, with the next record or/and data block, and return the corresponding value*/
        UpdateScan(scanDesc, nextemptybyte, nextblock);
        return block + lastrec + scans[scanDesc]->v1_length;

    } else if (scans[scanDesc]->op == NOT_EQUAL) {
        while (Equal(block + scans[scanDesc]->lastrecordoffset, scans[scanDesc]->value, scans[scanDesc]->v_type)) {
            /*Ignore all equals and update*/
            UpdateScan(scanDesc, nextemptybyte, nextblock);
            if (BF_ReadBlock(scans[scanDesc]->bfdesc, scans[scanDesc]->lastblock, &block) < 0) {
               AM_errno = AME_BF_READBLOCK;
                return NULL;
            }

            memcpy(&nextemptybyte, block + BD_ADDR, sizeof(int));
            memcpy(&nextblock, block + BD_RIGHT, sizeof(int));
        }
        /*Update scan, with the next record or/and data block, and return the corresponding value*/
        lastrec = scans[scanDesc]->lastrecordoffset;
        UpdateScan(scanDesc, nextemptybyte, nextblock);
        return block + lastrec + scans[scanDesc]->v1_length;

    } else if (scans[scanDesc]->op == LESS_THAN) {
        if (GrtOrEq(block + scans[scanDesc]->lastrecordoffset, scans[scanDesc]->value, scans[scanDesc]->v_type)) {
            /*key >= value, return NULL*/
            AM_errno = AME_EOF;
            return NULL;

        }
        /*Update scan, with the next record or/and data block, and return the corresponding value*/
        UpdateScan(scanDesc, nextemptybyte, nextblock);
        return block + lastrec + scans[scanDesc]->v1_length;

    } else if (scans[scanDesc]->op == LESS_THAN_OR_EQUAL) {
        if (Greater(block + scans[scanDesc]->lastrecordoffset, scans[scanDesc]->value, scans[scanDesc]->v_type)) {
            /*key > value, return NULL*/
            AM_errno = AME_EOF;
            return NULL;
        }
        /*Update scan, with the next record or/and data block, and return the corresponding value*/
        UpdateScan(scanDesc, nextemptybyte, nextblock);
        return block + lastrec + scans[scanDesc]->v1_length;

    } else if (scans[scanDesc]->op == GREATER_THAN) {
        /*Update scan, with the next record or/and data block, and return the corresponding value*/
       while (Equal(block + scans[scanDesc]->lastrecordoffset, scans[scanDesc]->value, scans[scanDesc]->v_type)) {
            /*Ignore all equals and update*/
            UpdateScan(scanDesc, nextemptybyte, nextblock);
            if (BF_ReadBlock(scans[scanDesc]->bfdesc, scans[scanDesc]->lastblock, &block) < 0) {
               AM_errno = AME_BF_READBLOCK;
                return NULL;
            }

            memcpy(&nextemptybyte, block + BD_ADDR, sizeof(int));
            memcpy(&nextblock, block + BD_RIGHT, sizeof(int));
        }
        lastrec = scans[scanDesc]->lastrecordoffset;
        UpdateScan(scanDesc, nextemptybyte, nextblock);
        return block + lastrec + scans[scanDesc]->v1_length;

    }
    else if (scans[scanDesc]->op == GREATER_THAN_OR_EQUAL) {
        /*Update scan, with the next record or/and data block, and return the corresponding value*/
        UpdateScan(scanDesc, nextemptybyte, nextblock);
        return block + lastrec + scans[scanDesc]->v1_length;

    }

}

void UpdateScan(int scandesc, int nextbyte, int nextblock) {
    scans[scandesc]->lastrecordoffset += scans[scandesc]->v1_length + scans[scandesc]->v2_length;
    if (scans[scandesc]->lastrecordoffset == nextbyte) {
        scans[scandesc]->lastblock = nextblock;
        scans[scandesc]->lastrecordoffset = BD_START;
    }

}
