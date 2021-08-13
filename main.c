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

/* The structure of the topK list */
typedef struct topKList_s
{
    /* The weight of the current graph */
    uint graphWeight;
    /* The index of the current graph */
    int graphIndex;
    /* The pointer to the next graph in the list */
    struct topKList_s* next;
}
topKList_t;

/* The number of nodes in each graph */
int d;

/* The number of graphs in the TopK list */
int k;

/* Global graph index for the next added graph */
int gId = 0;

/* Max graph weight in topK list */
uint maxGraphWeight = 0;

/* Boolean indicating whether the topK list is full or not */
bool isTopKFull = false;

/* Counter keeping track of how many new graphs have been inserted in the topK list */
int topKCounter = 0;

/* The list containing all the graphs */
topKList_t* topKHead = NULL;

/* Function declarations */
void cmdAddGraph();
void cmdTopK();

void appendTopK(uint, int);
void checkIfTopK(uint, int);
void fillTopK();
//bool isAlreadyTopK(uint);

void replaceHighestGraph(uint, int);
void findNewHighestGraph();
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

    /* Initiate the topK list */
    fillTopK();

    /* Ask for the very first command */
    if (fgets(cmd, sizeof(cmd), stdin) == NULL){}

    /* If it's TopK ask the user to insert AggiungiGrafo as the first command */
    while (strcmp(cmd, TOPK) == 0)
    {
        printf("Please use AggiungiGrafo as your first command.\n");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL){}
    }

    /* If it's AggiungiGrafo, proceed */
    if (strcmp(cmd, ADDGRAPH) == 0)
    {
        cmdAddGraph();
    }

    /* Keep asking for input until user quits the program */
    while(stdin != NULL) {
        if (fgets(cmd, sizeof(cmd), stdin) == NULL){}

        if (strcmp(cmd, ADDGRAPH) == 0)
        {
            cmdAddGraph();
        }
        else if (strcmp(cmd, TOPK) == 0)
        {
            cmdTopK();
        }
    }

    return 0;
}

/* Support function for filling in the topK list */
void fillTopK()
{
    for(int i = 0; i < k; i++)
    {
        appendTopK(UINT_MAX, -1);
    }
}

/* Support function for appending new graphs to topK */
void appendTopK(uint newGraphWeight, int newGraphIndex)
{
    /* Allocate memory for the new graph and use a pointer to move from the head */
    topKList_t* newGraph = (topKList_t*)malloc(sizeof(topKList_t));
    topKList_t* curr = topKHead;

    /* Create the new graph */
    newGraph->graphWeight = newGraphWeight;
    newGraph->graphIndex = newGraphIndex;
    newGraph->next = NULL;

    /* If the graph list is empty make the new graph the head */
    if (topKHead == NULL)
    {
        topKHead = newGraph;
        return;
    }

    /* Cycle through the list up to the last element */
    while (curr->next != NULL)
    {
        curr = curr->next;
    }

    /* Set the last element's next value as the new graph */
    curr->next = newGraph;
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

    //printf("\nGRAPH WEIGHT: %u - ID: %d\n", graphWeight, gId);

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
            //printf("\n%u\n", graph[i][j]);
        }
    }

    /* Calculate the weight of the current graph */
    graphWeight = calculateWeight(graph);

    /* Check if the graph is smaller than the other graphs in the topK list */
    checkIfTopK(graphWeight, gId);

    /* Update the global index */
    gId++;

    //TEMP PRINT TOPK
    /*for (topKList_t* curr = topKHead; curr != NULL; curr = curr->next)
    {
        printf("INDEX: %d - WEIGHT: %u\n", curr->graphIndex, curr->graphWeight);
    }*/
}

/* Check if the graph is part of the ones with the smallest weight, if so, add it to topK list */
void checkIfTopK(uint newGraphWeight, int newGraphIndex)
{
    /* Set the topK list as full if the elements have been filled */
    if (topKCounter == k)
    {
        isTopKFull = true;
    }

    for (topKList_t* curr = topKHead; curr != NULL; curr = curr->next)
    {
        /* If the topK list isn't full, then replace one of the default values to the new graph weight */
        if (!isTopKFull)
        {
            if ((curr->graphWeight) == UINT_MAX)
            {
                curr->graphWeight = newGraphWeight;
                curr->graphIndex = newGraphIndex;

                if (newGraphWeight > maxGraphWeight)
                {
                    maxGraphWeight = newGraphWeight;
                }

                topKCounter++;
                break;
            }
        }

        /* Else, if the topK list is full, replace the highest number present in topK with the new number, if the new number is smaller and already present */
        else if (newGraphWeight < (curr->graphWeight)) /* && !isAlreadyTopK(newGraphWeight) */
        {
            replaceHighestGraph(newGraphWeight, newGraphIndex);
            findNewHighestGraph();
            break;
        }
    }
}

/* Support function for finding if the newGraphWeight is already present in the topK list */
/*bool isAlreadyTopK(uint newGraphWeight)
{
    for (topKList_t* curr = topKHead; curr != NULL; curr = curr->next)
    {
        if (newGraphWeight == curr->graphWeight)
        {
            return true;
        }
    }

    return false;
}*/

/* Support function for replacing the highest graphWeight on the topKList with the newGraphWeight */
void replaceHighestGraph(uint newGraphWeight, int newGraphIndex)
{
    for (topKList_t* curr = topKHead; curr != NULL; curr = curr->next)
    {
        if (curr->graphWeight == maxGraphWeight)
        {
            curr->graphWeight = newGraphWeight;
            curr->graphIndex = newGraphIndex;
        }
    }
}

/* Support function for finding the new highest graphWeight in the topK list */
void findNewHighestGraph()
{
    maxGraphWeight = 0;

    for (topKList_t* curr = topKHead; curr != NULL; curr = curr->next)
    {
        if (curr->graphWeight > maxGraphWeight)
        {
            maxGraphWeight = curr->graphWeight;
        }
    }

}

/* Print the indexes of the graphs have the shortest paths from the topK list */
void cmdTopK()
{
    for (topKList_t* curr = topKHead; curr != NULL; curr = curr->next)
    {
        if (curr->graphWeight != UINT_MAX)
        {
            printf("%d ", curr->graphIndex);
        }
    }
    printf("\n");
}