#ifndef PATH_H_
#define PATH_H_

/* Structure to hold the path of index blocks to a data block during search */
typedef struct {
    int *p;      // Path of index blocks to data block
    int depth;   // Depth of the search
    int size;    // Max size (can increase with realloc)
} Path;

Path path_Init();
void path_Destroy(Path path);
int path_Update(Path *path, int b);
int path_Pop(Path *path);
int path_isEmpty(Path p);

#endif /* PATH_H_ */
