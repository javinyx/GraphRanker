#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_SIZE 14
#define TOPK "TopK\n"
#define ADDGRAPH "AggiungiGrafo"

/* The structure of the graph list */
typedef struct graph_s
{
    /* The data of the current graph */
    uint *graphData;
    /* The pointer to the next graph in the list */
    struct graph_s* next;
}
graph_t;

/* The number of nodes in each graph */
int d;
    
/* The number of graphs in the TopK list */
int k;

/* The list containing all the graphs */
graph_t* glHead = NULL;

/* Function declarations */
void addGraph();
void append(uint*);
void topK();

int main(int argc, char const *argv[])
{
    /* Initial values as string to increase safety by using fgets */
    char init[CMD_SIZE];

    /* The requested command, can be either AggiungiGrafo or TopK */
    char cmd[CMD_SIZE];

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

    // TEMP PRINT
    int i, j;
    for(graph_t* t=glHead; t!=NULL; t = t->next) {
            for (i = 0; i < d; i++)
            {
                for (j = 0; j < d; j++)
                {
                    printf("%u ", (t->graphData)[i*d + j]);
                }
                printf("\n");
            }
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

        // TEMP PRINT
        for(graph_t* t=glHead; t!=NULL; t = t->next) {
            printf("\n");
            for (i = 0; i < d; i++)
            {
                for (j = 0; j < d; j++)
                {
                    printf("%u ", (t->graphData)[i*d + j]);
                }
                printf("\n");
            }
        }
    }

    return 0;
}

/* Add a new graph to the list of graphs */
void addGraph()
{
    int i, j;
    uint *graph;

    /* Create a matrix of unsigned integers for each graph since the max value is 2^32 - 1 */
    graph = (uint *)malloc(d * d * sizeof(uint));

    /* Read each value of the graph from the command line ignoring commas */
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            scanf("%u%*c", &graph[i*d + j]);
        }
    }

    append(graph);
    
    return;
}

/* Support for adding graphs to the linked list */
void append(uint* newGraphData)
{
    graph_t* newGraph = (graph_t*) malloc(sizeof(graph_t));
    graph_t* curr = glHead;
  
    /* Create the new graph */
    newGraph->graphData = newGraphData;
    newGraph->next = NULL;
 
    /* If the graph list is empty make newGraph the head */
    if (glHead == NULL)
    {
       glHead = newGraph;
       return;
    }
    
    /* Cycle through the list up to the last element */
    while (curr->next != NULL)
    {
        curr = curr->next;
    }

    /* Set the last element's next value as the new graph */
    curr->next = newGraph;
    
    return;
}

/* Calculate which graphs have the shortest paths and show the top k graphs in a list */ 
void topK()
{
    printf(TOPK);
    return;
}