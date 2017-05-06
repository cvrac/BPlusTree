#ifndef BF_H_
#define BF_H_

/* Error codes for BF layer */
#define BFE_OK                       0
#define BFE_NOMEM                   -1
#define BFE_CANNOTOPENFILE          -2
#define BFE_CANNOTCLOSEFILE         -3
#define BFE_CANNOTCREATEFILE        -4
#define BFE_INCOMPLETEREAD          -5
#define BFE_INCOMPLETEWRITE         -6
#define BFE_FILEEXISTS              -7
#define BFE_NOBUF                   -8
#define BFE_LISTERROR               -9
#define BFE_FILEOPEN                -10
#define BFE_FD                      -11
#define BFE_FILENOTEXISTS           -12
#define BFE_FTABFULL                -13
#define BFE_HEADOVERFLOW            -14
#define BFE_BLOCKFIXED              -15
#define BFE_BLOCKUNFIXED            -16
#define BFE_EOF                     -17
#define BFE_FILEHASFIXEDBLOCKS     -18
#define BFE_BLOCKFREE               -19
#define BFE_BLOCKINBUF              -20
#define BFE_BLOCKNOTINBUF           -21
#define BFE_INVALIDBLOCK            -22
#define BFE_CANNOTDESTROYFILE		-23

#define BLOCK_SIZE 1024

int BF_Errno;

void BF_Init();
int BF_CreateFile(const char* filename);
int BF_OpenFile(const char* filename);
int BF_CloseFile(const int fileDesc);
int BF_GetBlockCounter(const int fileDesc);
int BF_AllocateBlock(const int fileDesc);
int BF_ReadBlock(const int fileDesc, const int blockNumber, void** block);
int BF_WriteBlock(const int fileDesc, const int blockNumber);
void BF_PrintError(const char* message);

#endif /* BF_H_ */
