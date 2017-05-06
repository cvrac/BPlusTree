#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_

#include "path.h"

#define INTEGER 'i'
#define FLOAT 'f'
#define STRING 'c'

/* Info in first block */
#define F_TYPE 0
#define TYPE1 1
#define LEN1 2
#define TYPE2 2+sizeof(int)
#define LEN2 3+sizeof(int)
#define ROOT 3+2*sizeof(int)

/* Info in index blocks */
#define BI_TYPE 0                    // Block type
#define BI_ADDR 1                    // Next free byte in block
#define BI_P0 (BI_ADDR+sizeof(int))  // ID of block pointed to by leftmost pointer

/* Info in data blocks */
#define BD_TYPE 0                      // Block type
#define BD_RIGHT 1                     // ID of next block to the right
#define BD_ADDR (BD_RIGHT+sizeof(int)) // Next free byte in block

#define BD_START (BD_ADDR + sizeof(int)) // Where data info ends

extern int AM_errno;

#define AME_OK 0
#define AME_EOF -1
#define AME_FILE_EXISTS -2
#define AME_WRONG_ATTR_TYPE -4
#define AME_WRONG_ATTR_LEN_I -5
#define AME_WRONG_ATTR_LEN_F -6
#define AME_WRONG_ATTR_LEN_C -7
#define AME_BF_CREATE -8
#define AME_BF_OPEN -9
#define AME_BF_ALLOCATEBLOCK -10
#define AME_BF_READBLOCK -11
#define AME_BF_WRITEBLOCK -12
#define AME_MAXOPENINDEX -13
#define AME_F_ALREADY_OPEN -14
#define AME_F_NOT_OPEN -15
#define AME_BF_CLOSE -16
#define AME_SCANDESC -17
#define AME_CLOSESCAN -18
#define AME_OPENSCAN -19
#define AME_NO_BTREE -20
#define AME_OUT_OF_BOUNDS -21
#define AME_WRONG_OP -22
#define AME_MAXOPENSCANS -23
#define AME_IDENTICAL -24

#define EQUAL 1
#define NOT_EQUAL 2
#define LESS_THAN 3
#define GREATER_THAN 4
#define LESS_THAN_OR_EQUAL 5
#define GREATER_THAN_OR_EQUAL 6

int Search(void *value1, int bf, Path *path);

#endif /* BPLUSTREE_H_ */
