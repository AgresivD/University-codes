/*
    * File: adjancency.c
    * 
    * The program accept a neighbors matrix of a tree with N nodes
    * And then uses the path function that returns TRUE value if there is a directed arc from the u node to the v node.
    * In other words if u parent or ancestor of v in the tree.
        
        * @Task: Maman12
        * @Author: Dan Elimelech
*/
#include <stdio.h>

/* Definitions */
#define N 11
#define INVALID_VALUE -1
#define ZERO_VALUE 0
#define ONE_VALUE 1

/* Macros */ 
#define InBounds(i) (i >= 0 && i <= N - 1) 
#define InMatrixBounds(u, v) (InBounds(u) && InBounds(v))
#define InvalidPathValues(u, v) (u == INVALID_VALUE && v == INVALID_VALUE)

/* Boolean type definition */
typedef enum {
    FALSE = 0,
    TRUE = 1
} Boolean;

/* Neighbers matrix definition. (NxN) */
typedef int Adjmat[N][N];

/* Signatures. */
Boolean path(Adjmat adj, int u, int v);
Boolean pathRec(Adjmat adj, int u, int v, int r_index, int i);

Boolean path(Adjmat adj, int u, int v)
{
    if(!InMatrixBounds(u, v))
        return FALSE;
    if(u == v || adj[u][v] == 1)
        return TRUE;
    return pathRec(adj, u, v, v, 0);
}

Boolean pathRec(Adjmat adj, int u, int v, int r_index, int i)
{
    if(!InMatrixBounds(r_index, i))
        return FALSE;
    if(adj[i][r_index] != 1)
        return pathRec(adj, u, v, r_index, i + 1);
    else
    {
        if(adj[u][i] == 1)
            return TRUE;
        else
            return pathRec(adj, u, v, i, 0);
    }
}

int main()
{
    Adjmat adj;
    int i, j, u, v, element = ZERO_VALUE; 
    printf("Program Adjmat defined to be (%dx%d).\n", N, N);
    printf("Enter adjmat elements: \n");
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            printf("Enter element [%d,%d] : ", i, j);
            scanf("%d", &element);
            if(element != ZERO_VALUE && element != ONE_VALUE)
            {
                printf("[ERROR]: Adjmat elements can be 0 or 1.\n");
                j--;
            }
            else
                adj[i][j] = element;
        }
    }
    printf("Adjmat created successfully!\n\n");
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
            printf("%d\t", adj[i][j]);
        printf("\n");
    }
    printf("\n\nNow you can use path function.\n");
    printf("path accept indexes of nodes u and v\n");
    printf("and returns true if there is a directed route from u to v node in the neighborhood matrix you created.\n");
    printf("If you want to stop the program enter u and v values to be -1.\n");
    printf("\033[0;32mUSAGE: \033[0mpath: [u] [v] ** Without square brackets **\n\n");
    u = v = ZERO_VALUE;
    do
    {
        printf("path: ");
        scanf("%d %d", &u, &v);
        if(!InvalidPathValues(u, v))
            printf("path(adjmat, %d, %d): %s\n", u, v, path(adj, u, v) == TRUE ? "TRUE" : "FALSE");
        else
            printf("Exit successfully!");
    } while (!InvalidPathValues(u, v));
    return 0;
}
