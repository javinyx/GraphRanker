#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CMD_SIZE 14
#define TOPK "TopK\n"
#define ADDGRAPH "AggiungiGrafo"
#define ROOT_NODE 0

/* The structure of the graph list */
typedef struct graphList_s
{
    uint *graphWeight;
    size_t occupied;
    size_t totSize;
}
graphList_t;

/* The number of nodes in each graph */
int d = 0;
    
/* The number of graphs in the TopK list */
int k = 0;

/* The list containing all the graphs */
graphList_t *graphList;

/* Function declarations */
void initGraphList(int);
void appendGraphList(uint);
uint findShortestPath(uint [], int [])
uint calculateWeight(uint [][d]);
void addGraph();
void topK();

int main(int argc, char const *argv[])
{
    /* Initial values as string to increase safety by using fgets */
    char init[CMD_SIZE];

    /* The requested command, can be either AggiungiGrafo or TopK */
    char cmd[CMD_SIZE];

    /* Initiate the graph list */
    initGraphList(4);

    /* Ask for d and k and convert them to integers */
    fgets(init, sizeof(init), stdin);
    d = init[0] - '0';
    k = init[3] - '0';

    /* Empty init as it won't be used anymore */
    memset(init, 0, sizeof(init));

    /* Ask for the very first command */
    fgets(cmd, sizeof(cmd), stdin);

    /* If it's TopK ask the user to insert AggiungiGrafo as the first command*/
    while (strcmp(cmd, TOPK) == 0)
    {
        printf("Please use AggiungiGrafo as your first command.\n");
        fgets(cmd, sizeof(cmd), stdin);
    }

    /* If it's AggiungiGrafo, proceed */
    if (strcmp(cmd, ADDGRAPH) == 0)
    {
        addGraph();
    }

    /* Keep asking for input until user quits the program */
    while(1) {
        fgets(cmd, sizeof(cmd), stdin);

        if (strcmp(cmd, ADDGRAPH) == 0)
        {
            addGraph();
        } 
        else if (strcmp(cmd, TOPK) == 0)
        {
            topK();
        }
    }

    return 0;
}

/* Support for creating the graph list */
void initGraphList(int defaultSize)
{
    graphList->graphWeight = malloc(defaultSize * sizeof(uint));
    graphList->occupied = 0;
    graphList->totSize = defaultSize;
}

/* Support for adding graphs to the list */
void appendGraphList(uint newGraphWeight)
{
    if (graphList->occupied == graphList->totSize)
    {
        graphList->totSize = (graphList->totSize * 2);
        graphList->graphWeight = realloc(graphList->graphWeight, graphList->totSize * sizeof(uint));
    }

    graphList->graphWeight[graphList->occupied++] = newGraphWeight;
}

/* Support for finding the shortest path to the current node */
uint findShortestPath(uint path[], int sptSet[])
{
    int min = UINT_MAX, min_index;
 
    for (int v = 0; v < d; v++)
    {
        if (sptSet[v] == 1 && path[v] <= min)
        {
            min = path[v];
            min_index = v;
        }
    }
    
    return min_index;
}

/* Algorithm to calculate the sum of all the shortest paths from node 0 to all the other nodes in the graph, also known as the weight of the graph */
uint calculateWeight(uint graph[d][d])
{
    uint graphWeight = 0;

    uint path[d];
 
    int sptSet[d]; //0 true, 1 false
 
    for (int i = 0; i < d; i++)
    {
        path[i] = UINT_MAX;
        sptSet[i] = 1;
    }
 
    path[ROOT_NODE] = 0;
 
    for (int count = 0; count < d - 1; count++)
    {
        int u = findShortestPath(path, sptSet);
 
        sptSet[u] = 0;
 
        for (int v = 0; v < d; v++)
        {
            if (!sptSet[v] && graph[u][v] && path[u] != UINT_MAX && path[u] + graph[u][v] < path[v])
            {
                path[v] = path[u] + graph[u][v];
            } 
        }
    }

    /* Calculate the sum of all the shortest paths to find the graph weight */
    for (int i = 0; i < d; i++)
    {
        graphWeight = graphWeight + path[i];
    }

    printf("%u", graphWeight);

    return graphWeight;
}

/* Add a new graph to the list of graphs */
void addGraph()
{
    int i, j;
    uint graphWeight;

    /* Create a matrix of unsigned integers for each graph since the max value is 2^32 - 1 */
    uint graph[d][d];

    /* Read each value of the graph from the command line ignoring commas */
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            scanf("%u%*c", &graph[i][j]);
        }
    }

    /* Calculate the weight of the current graph */
    graphWeight = calculateWeight(graph);

    /* Add the weight of the graph to the list containing all the graphs */
    appendGraphList(graphWeight);
    
    return;
}

/* Calculate which graphs have the shortest paths and show the top k graphs in a list */ 
void topK()
{
    int i, j;
    int topKArr[k];


    return;
}