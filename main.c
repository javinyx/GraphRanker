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
typedef struct topKGraph_s
{
    /* The weight of the current graph */
    uint graphWeight;

    /* The index of the current graph */
    int graphIndex;
}
topKGraph_t;

/* The number of nodes in each graph */
int d;

/* The number of graphs in the TopK list */
int k;

/* Global graph index for the next added graph */
int gId = 0;

/* The list containing all the topK graphs */
topKGraph_t* topKList;

/* Max graph weight in topK list */
topKGraph_t* maxGraphWeight;

/* Temporary max weight for new insertions in topK list */
topKGraph_t* tmpMaxGraphWeight;

/* Initial position (and values) of tmpMaxGraphWeight */
topKGraph_t* resetTmpMaxWeight;

/* Support index to keep track of second-highest previous weight in topK list */
int tracker;

/* Counter keeping track of how many new graphs have been inserted in the topK list */
int topKCounter = 0;

/* Function declarations */
void cmdAddGraph();
void cmdTopK();

void checkIfTopK(uint, int);
void mergeSort(int, int);
void mergeArrays(int, int, int);

uint getArchWeight();
uint calculateWeight(uint[][d]);
int findShortestPath(const uint*, const bool*);

int main(int argc, char const *argv[])
{
    /* The requested command, can be either AggiungiGrafo or TopK */
    char cmd[CMD_SIZE];

    /* Ask for d and k and convert them to integers */
    d = (int)getArchWeight();
    k = (int)getArchWeight();

    /* The list containing all the topK graphs */
    topKList = malloc(k * sizeof(topKGraph_t));

    /* Initiate maxWeight */
    maxGraphWeight = malloc(sizeof(topKGraph_t));
    maxGraphWeight->graphWeight = 0;
    maxGraphWeight->graphIndex = -1;

    /* Initiate tmpMaxWeight */
    tmpMaxGraphWeight = malloc(sizeof(topKGraph_t));
    tmpMaxGraphWeight->graphWeight = 0;
    tmpMaxGraphWeight->graphIndex = -1;

    /* Save the initial values of tmpMaxWeight */
    resetTmpMaxWeight = tmpMaxGraphWeight;

    /* Tracker saving the spot of the (k - n) highest weight */
    tracker = k - 2;

    /* Keep reading the input until the program stops receiving it */
    while (fgets(cmd, sizeof(cmd), stdin) != NULL)
    {
        /* AggiungiGrafo */
        if (cmd[0] == ADDGRAPH)
        {
            cmdAddGraph();
        }

        /* TopK */
        else if (cmd[0] == TOPK)
        {
            cmdTopK();
        }
    }

    return 0;
}

/* Support for finding the index of the shortest path to the current node */
int findShortestPath(const uint* shortestPath, const bool* isShortest)
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

    /* Initialize all nodes as longest paths possible */
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

        /* Once found, set it as the shortest */
        isShortest[curr] = true;

        for (int j = 0; j < d; j++)
        {
            /* Update the shortest paths following requirements */
            if (graph[curr][j] && !isShortest[j] && (shortestPath[curr] != UINT_MAX) && ((shortestPath[curr] + graph[curr][j]) < shortestPath[j]))
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

    /* When one of the following is read, move on to the next number */
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
    /* If the topK list isn't full, then add the new graph weight */
    if (topKCounter < k)
    {
        topKList[topKCounter].graphWeight = newGraphWeight;
        topKList[topKCounter].graphIndex = newGraphIndex;

        if (newGraphWeight > maxGraphWeight->graphWeight)
        {
            maxGraphWeight = &topKList[topKCounter];
        }

        topKCounter++;

        if (topKCounter == k)
        {
            mergeSort(0, k - 1);
            maxGraphWeight = &topKList[k - 1];
        }
    }

    /* Else, replace the highest weight present in topK with the new weight, if the new weight is smaller than either max*/
    else if ((newGraphWeight < maxGraphWeight->graphWeight) || (newGraphWeight < tmpMaxGraphWeight->graphWeight))
    {
        if (maxGraphWeight->graphWeight > tmpMaxGraphWeight->graphWeight)
        {
            /* Replace the max weight with the new weight */
            maxGraphWeight->graphWeight = newGraphWeight;
            maxGraphWeight->graphIndex = newGraphIndex;

            /* If the new weight is bigger than the temp weight, set it as the temporary max */
            if (newGraphWeight >= tmpMaxGraphWeight->graphWeight)
            {
                tmpMaxGraphWeight = maxGraphWeight;
            }

            /* If the new weight is smaller than the tracker (k - n), set the new max as the tracker */
            if (newGraphWeight < topKList[tracker].graphWeight)
            {
                maxGraphWeight = &topKList[tracker];
            }

            /* If the start of the list is reached, reset tmpMaxWeight and tracker */
            if (tracker < 0)
            {
                maxGraphWeight = &topKList[k - 1];
                tmpMaxGraphWeight = resetTmpMaxWeight;
                tracker = k - 1;
            }

            tracker--;
        }

        else
        {
            /* Replace the temp max with the new graph */
            tmpMaxGraphWeight->graphWeight = newGraphWeight;
            tmpMaxGraphWeight->graphIndex = newGraphIndex;

            /* Sort the topKList and reset pointers and tracker */
            mergeSort(0, k - 1);

            maxGraphWeight = &topKList[k - 1];
            tmpMaxGraphWeight = resetTmpMaxWeight;
            tracker = k - 2;
        }
    }
}

/* Support function to merge more arrays into one */
void mergeArrays(int start, int middle, int end)
{
    /* Calculate the sizes of the new arrays */
    int dim1 = middle - start + 1;
    int dim2 = end - middle;

    /* Initiate the support arrays */
    topKGraph_t *tmp1, *tmp2;
    tmp1 = malloc(dim1 * sizeof(topKGraph_t));
    tmp2 = malloc(dim2 * sizeof(topKGraph_t));

    /* Copy the contents of the list */
    for (int i = 0; i < dim1; i++)
    {
        tmp1[i].graphWeight = topKList[start + i].graphWeight;
        tmp1[i].graphIndex = topKList[start + i].graphIndex;
    }

    for (int j = 0; j < dim2; j++)
    {
        tmp2[j].graphWeight = topKList[middle + 1 + j].graphWeight;
        tmp2[j].graphIndex = topKList[middle + 1 + j].graphIndex;
    }

    /* Reset indexes of support arrays */
    int i = 0;
    int j = 0;
    int l = start;

    /* Put elements of the support arrays into the main array, in order */
    while (i < dim1 && j < dim2)
    {
        if (tmp1[i].graphWeight <= tmp2[j].graphWeight)
        {
            topKList[l] = tmp1[i];
            i++;
        }
        else
        {
            topKList[l] = tmp2[j];
            j++;
        }
        l++;
    }

    /* Copy the remaining elements of the support arrays into the main array */
    while (i < dim1)
    {
        topKList[l] = tmp1[i];
        i++;
        l++;
    }

    while (j < dim2)
    {
        topKList[l] = tmp2[j];
        j++;
        l++;
    }
}

/* Algorithm to order the list in ascending order */
void mergeSort(int start, int end)
{
    if (start < end)
    {
        /* Find the center of the list */
        int middle = start + (end - start) / 2;

        /* Split the list in 2 and sort */
        mergeSort(start, middle);
        mergeSort(middle + 1, end);

        /* Merge the arrays */
        mergeArrays(start, middle, end);
    }
}

/* Print the indexes of the graphs have the shortest paths from the topK list */
void cmdTopK()
{
    int i;

    for (i = 0; i < topKCounter; i++)
    {
        /* If it's the last character, remove the space */
        if (i == topKCounter - 1)
        {
            printf("%d", topKList[i].graphIndex);
        }
        else
        {
            printf("%d ", topKList[i].graphIndex);
        }
    }

    printf("\n");
}