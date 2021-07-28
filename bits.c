/* The structure of a graph */
typedef struct graph_s
{
    int graphId;
}
graph_t;


/* The structure of each node inside a graph */
typedef struct node_s
{
    int graphId;
    struct node_s* next;
    struct node_s* prev;
}
node_t;