#define MAXOPENFILES 20
#define MAXSCANS 20

typedef struct {
    char filename[25];
    int filedesc;

} index_info;

typedef struct {
    int filedesc;
    int bfdesc;             /*bf level filedesc*/
    int op;                 /*operator*/
    int v1_length;           /*key-value length*/
    int v2_length;          /*value2 length*/
    char v_type;             /*value type*/
    void *value;            /*value to be compared*/
    int lastblock;          /*the last block id*/
    int lastrecordoffset;   /*byte of the last record read*/

} scan_info;

index_info *open_inds[MAXOPENFILES];
scan_info *scans[MAXSCANS];

int open_indes;
int open_scans;
int AM_errno;
