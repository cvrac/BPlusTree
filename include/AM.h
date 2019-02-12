#ifndef AM_H_
#define AM_H_

void AM_Init( void );
int AM_CreateIndex(char *fileName, char attrType1, int attrLength1, char attrType2, int attrLength2);
int AM_DestroyIndex(char *fileName);
int AM_OpenIndex (char *fileName );
int AM_CloseIndex (int fileDesc);
int AM_InsertEntry(int fileDesc, void *value1, void *value2);
int AM_OpenIndexScan(int fileDesc, int op, void *value);
void *AM_FindNextEntry(int scanDesc);
int AM_CloseIndexScan(int scanDesc);
void AM_PrintError(char *errString);

#endif /* AM_H_ */
