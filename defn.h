#ifndef DEFN_H_
#define DEFN_H_

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

#endif /* DEFN_H_ */
