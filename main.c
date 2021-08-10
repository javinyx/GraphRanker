#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

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
int d;

/* The number of graphs in the TopK list */
int k;

/* The list containing all the graphs */
graphList_t graphList;

/* Function declarations */
void initGraphList(size_t);
void appendGraphList(uint);
int findShortestPath(uint [], bool []);
uint calculateWeight(uint [][d]);
void addGraph();
void checkIfTopK(uint);
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
void initGraphList(size_t defaultSize)
{
    graphList_t *tmp = &graphList;

    tmp->graphWeight = malloc(defaultSize * sizeof(uint));
    tmp->occupied = 0;
    tmp->totSize = defaultSize;
}

/* Support for adding graphs to the list */
void appendGraphList(uint newGraphWeight)
{
    graphList_t *tmp = &graphList;

    if (tmp->occupied == tmp->totSize)
    {
        tmp->totSize = (tmp->totSize * 2);
        tmp->graphWeight = realloc(tmp->graphWeight, tmp->totSize * sizeof(uint));
    }

    tmp->graphWeight[tmp->occupied++] = newGraphWeight;
}

/* Support for finding the shortest path to the current node */
int findShortestPath(uint shortestPath[], bool isShortest[])
{
    int min, minId;
    int i;

    min = UINT_MAX;

    for (i = 0; i < d; i++)
    {
        if (isShortest[i] == false && shortestPath[i] <= min)
        {
            min = shortestPath[i];
            minId = i;
        }
    }

    return minId;
}

/* Algorithm to calculate the sum of all the shortest paths from node 0 to all the other nodes in the graph, also known as the weight of the graph */
uint calculateWeight(uint graph[d][d])
{
    int i;
    uint graphWeight = 0;
    uint shortestPath[d];
    bool isShortest[d];

    for (i = 0; i < d; i++)
    {
        shortestPath[i] = UINT_MAX;
        isShortest[i] = false;
    }

    shortestPath[ROOT_NODE] = 0;

    for (int ctr = 0; ctr < d - 1; ctr++)
    {
        int curr = findShortestPath(shortestPath, isShortest);

        isShortest[curr] = true;

        for (int j = 0; j < d; j++)
        {
            if (graph[curr][j] && !isShortest[j] && shortestPath[curr] != UINT_MAX && shortestPath[curr] + graph[curr][j] < shortestPath[j])
            {
                shortestPath[j] = shortestPath[curr] + graph[curr][j];
            }
        }
    }

    /* Calculate the sum of all the shortest paths to find the graph weight */
    for (i = 0; i < d; i++)
    {
        graphWeight = graphWeight + shortestPath[i];
    }

    printf("GRAPH WEIGHT: %u\n", graphWeight);

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

    /* Check if the graph is smaller than the other graphs in the topK array */
    checkIfTopK(graphWeight);

    /* Add the weight of the graph to the list containing all the graphs */
    appendGraphList(graphWeight);

    return;
}

/* Check if the graph is part of the ones with the smallest weight, if so, add it to topK array */
void checkIfTopK(uint graphWeight) {

}

/* Calculate which graphs have the shortest paths and show the top k graphs in a list */
void topK()
{
    int i, j;
    int topKArr[k];


    return;
}