#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define CMD_SIZE 14
#define TOPK 'T'
#define ADDGRAPH 'A'
#define ROOT_NODE 0

/* The structure of the topK element */
typedef struct topKList_s
{
    /* The weight of the current graph */
    uint graphWeight;
    /* The index of the current graph */
    int graphIndex;
}
topKList_t;

/* The number of nodes in each graph */
int d;

/* The number of graphs in the TopK list */
int k;

/* Global graph index for the next added graph */
int gId = 0;

/* The list containing all the topK graphs */
topKList_t* topKList;

/* Max graph weight in topK list */
topKList_t* maxGraphWeight;

/* Counter keeping track of how many new graphs have been inserted in the topK list */
int topKCounter = 0;

/* Function declarations */
void cmdAddGraph();
void cmdTopK();

void checkIfTopK(uint, int);
void fillTopK();
void quickSort(int, int);

uint getArchWeight();
uint calculateWeight(uint[][d]);
int findShortestPath(uint[], bool[]);

int main(int argc, char const *argv[])
{
    /* The requested command, can be either AggiungiGrafo or TopK */
    char cmd[CMD_SIZE];

    /* Ask for d and k and convert them to integers */
    d = (int)getArchWeight();
    k = (int)getArchWeight();

    /* The list containing all the topK graphs */
    topKList = malloc(k * sizeof(topKList_t));

    /* Initiate maxWeight */
    maxGraphWeight = malloc(sizeof(topKList_t));
    maxGraphWeight->graphWeight = 0;
    maxGraphWeight->graphIndex = -1;

    /* Fill the array with default values */
    fillTopK();

    /* Keep asking for input until user quits the program */
    while(fgets(cmd, sizeof(cmd), stdin) != NULL) {

        if (cmd[0] == ADDGRAPH)
        {
            cmdAddGraph();
        }
        else if (cmd[0] == TOPK)
        {
            cmdTopK();
        }
    }

    return 0;
}

/* Support function for filling in the topK list */
void fillTopK()
{
    for (int i = 0; i < k; i++)
    {
        topKList[i].graphWeight = UINT_MAX;
        topKList[i].graphIndex = -1;
    }
}

/* Support for finding the index of the shortest path to the current node */
int findShortestPath(uint shortestPath[], bool isShortest[])
{
    /* The shortest path */
    uint shortest;
    /* The index of the shortest path */
    int shortestId = 0;
    int i;

    /* Set the shortest initial path to be the maximum number */
    shortest = UINT_MAX;

    for (i = 0; i < d; i++)
    {
        /* If the path is shorter than the minimum, and it's part of the shortest path array, then replace it */
        if (shortestPath[i] <= shortest && isShortest[i] == false)
        {
            shortest = shortestPath[i];
            shortestId = i;
        }
    }

    return shortestId;
}

/* Algorithm to calculate the sum of all the shortest paths from node 0 to all the other nodes in the graph, also known as the weight of the graph */
uint calculateWeight(uint graph[d][d])
{
    /* Initial graphWeight set to 0 */
    uint graphWeight = 0;
    /* shortestPath array containing the shortest paths to each node */
    uint shortestPath[d];
    /* isShortest array containing booleans which indicate if the path is the shortest to that node */
    bool isShortest[d];
    int i;

    /* Initiate all nodes as longest paths possible */
    for (i = 0; i < d; i++)
    {
        shortestPath[i] = UINT_MAX;
        isShortest[i] = false;
    }

    /* Set the distance of node 0 to node 0 as 0 */
    shortestPath[ROOT_NODE] = 0;

    for (int ctr = 0; ctr < d - 1; ctr++)
    {
        /* Find the shortest path for the current node */
        int curr = findShortestPath(shortestPath, isShortest);
        isShortest[curr] = true;

        for (int j = 0; j < d; j++)
        {
            /* Update the shortest paths following requirements */
            if (graph[curr][j] && !isShortest[j] && shortestPath[curr] != UINT_MAX && shortestPath[curr] + graph[curr][j] < shortestPath[j])
            {
                shortestPath[j] = shortestPath[curr] + graph[curr][j];
            }
        }
    }

    /* Calculate the sum of all the shortest paths to find the graph weight */
    for (i = 0; i < d; i++)
    {
        if (shortestPath[i] != UINT_MAX)
        {
            graphWeight = graphWeight + shortestPath[i];
        }
    }

    return graphWeight;
}

/* Support function for reading weights of arches */
uint getArchWeight()
{
    /* The character being read */
    int c = 0;
    /* The weight to save */
    uint weight = 0;

    /* Read the first character from stdin */
    c = getchar_unlocked();

    /* When , is read, move on to the next number */
    if (c == ',' || c == '\n' || c == ' ')
    {
        c = getchar_unlocked();
    }

    /* When reading numbers, keep counting */
    while (c >= '0' && c <= '9')
    {
        weight = (weight * 10) + (c - '0');
        c = getchar_unlocked();
    }

    return weight;
}

/* Add a new graph to the list of graphs */
void cmdAddGraph()
{
    /* The weight of the added graph */
    uint graphWeight;
    /* Create a matrix of unsigned integers for each graph since the max value is 2^32 - 1 */
    uint graph[d][d];
    int i, j;

    /* Read each value of the graph from the command line ignoring commas */
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            graph[i][j] = getArchWeight();
        }
    }

    /* Calculate the weight of the current graph */
    graphWeight = calculateWeight(graph);

    /* Check if the graph is smaller than the other graphs in the topK list */
    checkIfTopK(graphWeight, gId);

    /* Update the global index */
    gId++;
}

/* Check if the graph is part of the ones with the smallest weight, if so, add it to topK list */
void checkIfTopK(uint newGraphWeight, int newGraphIndex)
{
    /* If the topK list isn't full, then replace one of the default values to the new graph weight */
    if (topKCounter < k)
    {
        topKList[topKCounter].graphWeight = newGraphWeight;
        topKList[topKCounter].graphIndex = newGraphIndex;

        if (newGraphWeight > maxGraphWeight->graphWeight)
        {
            maxGraphWeight = &topKList[topKCounter];
        }

        topKCounter++;
    }

    /* Else, if the topK list is full, replace the highest weight present in topK with the new weight, if the new weight is smaller */
    else if (newGraphWeight < maxGraphWeight->graphWeight)
    {
        maxGraphWeight->graphWeight = newGraphWeight;
        maxGraphWeight->graphIndex = newGraphIndex;

        quickSort(0, k - 1);

        maxGraphWeight = &topKList[k - 1];
    }
}

/* Support function to sort the array in ascending order */
void quickSort(int first, int last)
{
    int i, j, pivot;
    topKList_t tmp;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (topKList[i].graphWeight <= topKList[pivot].graphWeight && i < last)
            {
                i++;
            }
            while (topKList[j].graphWeight > topKList[pivot].graphWeight)
            {
                j--;
            }
            if (i < j)
            {
                tmp.graphWeight = topKList[i].graphWeight;
                tmp.graphIndex = topKList[i].graphIndex;

                topKList[i].graphWeight = topKList[j].graphWeight;
                topKList[i].graphIndex = topKList[j].graphIndex;

                topKList[j].graphWeight = tmp.graphWeight;
                topKList[j].graphIndex = tmp.graphIndex;
            }
        }

        tmp.graphWeight = topKList[pivot].graphWeight;
        tmp.graphIndex = topKList[pivot].graphIndex;

        topKList[pivot].graphWeight = topKList[j].graphWeight;
        topKList[pivot].graphIndex = topKList[j].graphIndex;

        topKList[j].graphWeight = tmp.graphWeight;
        topKList[j].graphIndex = tmp.graphIndex;

        quickSort(first, j - 1);
        quickSort(j + 1, last);
    }
}

/* Print the indexes of the graphs have the shortest paths from the topK list */
void cmdTopK()
{
    int i;

    for (i = 0; i < topKCounter - 1; i++)
    {
        if (topKList[i].graphIndex != UINT_MAX)
        {
            printf("%d ", topKList[i].graphIndex);
        }
    }
    if (topKList[i].graphIndex != UINT_MAX)
    {
        printf("%d", topKList[i].graphIndex);
    }

    printf("\n");
}