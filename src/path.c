#include <stdlib.h>

#include "path.h"

Path path_Init()
{ /* Initialise path structure */
    Path path;
    path.size = 10;                  // Max 10
    path.p = malloc(10*sizeof(int)); // Realloc if needed
    path.depth = -1;                 // Initialised at no depth
    return path;
};

int path_Update(Path *path, int b)
{ /* Include block id to the path and realloc structure memory if needed */
    (path->depth)++;
    if (path->depth == path->size-1) {
        path->size *= 2;
        if ((path->p = realloc(path->p, sizeof(int)*path->size)) < 0) {
            return -1;
        }
    }
    path->p[path->depth] = b;
}

int path_isEmpty(Path p)
{ /* Return 1 if path is empty */
    return (p.depth == -1);
}

int path_Pop(Path *path)
{ /* Pop the last block id in the path and return it */
    if (path_isEmpty(*path))
        return -1;
    int a = path->p[path->depth];
    (path->depth)--;
    return a;
}

void path_Destroy(Path path)
{ /* Deallocate structure */
    free(path.p);
}
