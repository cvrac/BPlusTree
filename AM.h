#ifndef AM_H_
#define AM_H_

/* Error codes */

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

void AM_Init( void );


int AM_CreateIndex(
  char *fileName, /* όνομα αρχείου */
  char attrType1, /* τύπος πρώτου πεδίου: 'c' (συμβολοσειρά), 'i' (ακέραιος), 'f' (πραγματικός) */
  int attrLength1, /* μήκος πρώτου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c' */
  char attrType2, /* τύπος πρώτου πεδίου: 'c' (συμβολοσειρά), 'i' (ακέραιος), 'f' (πραγματικός) */
  int attrLength2 /* μήκος δεύτερου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c' */
);


int AM_DestroyIndex(
  char *fileName /* όνομα αρχείου */
);


int AM_OpenIndex (
  char *fileName /* όνομα αρχείου */
);


int AM_CloseIndex (
  int fileDesc /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
);


int AM_InsertEntry(
  int fileDesc, /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
  void *value1, /* τιμή του πεδίου-κλειδιού προς εισαγωγή */
  void *value2 /* τιμή του δεύτερου πεδίου της εγγραφής προς εισαγωγή */
);


int AM_OpenIndexScan(
  int fileDesc, /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
  int op, /* τελεστής σύγκρισης */
  void *value /* τιμή του πεδίου-κλειδιού προς σύγκριση */
);


void *AM_FindNextEntry(
  int scanDesc /* αριθμός που αντιστοιχεί στην ανοιχτή σάρωση */
);


int AM_CloseIndexScan(
  int scanDesc /* αριθμός που αντιστοιχεί στην ανοιχτή σάρωση */
);


void AM_PrintError(
  char *errString /* κείμενο για εκτύπωση */
);


#endif /* AM_H_ */
