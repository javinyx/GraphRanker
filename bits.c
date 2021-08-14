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

/* Djikstra */
void findShortestPath(int G[MAX][MAX],int n,int startnode)
{
 
    int cost[MAX][MAX], distance[MAX], pred[MAX];
    int visited[MAX], count, mindistance, nextnode, i, j;
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far

    //create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];

    //initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }

    distance[startnode]=0;
    visited[startnode]=1;
    count=1;
    while(count<n-1)
    {
        mindistance=INFINITY;
        //nextnode gives the node at minimum distance
        
        for(i=0;i<n;i++)
            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }
        
        //check if a better path exists through nextnode
        visited[nextnode]=1;
        for(i=0;i<n;i++)
            if(!visited[i])
                if(mindistance+cost[nextnode][i]<distance[i])
                {
                    distance[i]=mindistance+cost[nextnode][i];
                    pred[i]=nextnode;
                }

        count++;
    }
 
    //print the path and distance of each node
    for(i=0;i<n;i++)
        if(i!=startnode)
        {
            printf("\nDistance of node%d=%d",i,distance[i]);
            printf("\nPath=%d",i);
            j=i;
            do
            {
                j=pred[j];
                printf("<-%d",j);
            }while(j!=startnode);
        }
}

// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int graph[V][V], int src)
{
    int dist[V]; // The output array.  dist[i] will hold the shortest
    // distance from src to i
 
    bool sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized
 
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++)
 
            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
 
    // print the constructed distance array
    printSolution(dist);
}

void Dijkstra(int Graph[MAX][MAX], int n, int start);

void Dijkstra(int Graph[MAX][MAX], int n, int start) {
    int cost[MAX][MAX], distance[MAX], pred[MAX];
    int visited[MAX], count, mindistance, nextnode, i, j;

    // Creating cost matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (Graph[i][j] == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = Graph[i][j];

            for (i = 0; i < n; i++) {
                distance[i] = cost[start][i];
                pred[i] = start;
                visited[i] = 0;
            }

            distance[start] = 0;
            visited[start] = 1;
            count = 1;

            while (count < n - 1) {
                mindistance = INFINITY;

                for (i = 0; i < n; i++)
                    if (distance[i] < mindistance && !visited[i]) {
                        mindistance = distance[i];
                        nextnode = i;
                    }

                visited[nextnode] = 1;
                    for (i = 0; i < n; i++)
                        if (!visited[i])
                            if (mindistance + cost[nextnode][i] < distance[i]) {
                                distance[i] = mindistance + cost[nextnode][i];
                                pred[i] = nextnode;
                            }
                    count++;
            }

            // Printing the distance
            for (i = 0; i < n; i++)
                if (i != start) {
                    printf("\nDistance from source to %d: %d", i, distance[i]);
                }
}

/* QUICKSORT */
#include<stdio.h>
#define MAX 100

int insert_array(int a[]) {
    int n, i;
    printf("Quanti elementi?: ");
    scanf("%d", &n);

    for (i=0; i<n; i++) {
        printf("elemento %d: ", i);
        scanf("%d", &a[i]);
    }
    return(n);
}

void stampa_array(int a[], int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return;
}

void quicksort(int a[MAX],int primo,int ultimo){
    int i, j, pivot, temp;
    /*pivot -- inizialmente il pivot è il primo elemento
    primo e ultimo sono le due variabili che servono per scorrere l'array
    */
    if(primo<ultimo){
        pivot=primo;
        i=primo;
        j=ultimo;

        while(i<j){
            while(a[i]<=a[pivot]&&i<ultimo)
                i++;
            while(a[j]>a[pivot])
                j--;
            if(i<j){
                temp=a[i];
                a[i]=a[j];
                a[j]=temp;
            }
        }

        temp=a[pivot];
        a[pivot]=a[j];
        a[j]=temp;
        quicksort(a,primo,j-1);
        quicksort(a,j+1,ultimo);
    }
}

int main(){
    int n, a[MAX],i;
    n = insert_array(a);
    printf("Array iniziale: ");
    stampa_array(a,n);
    quicksort(a,0,n-1);
    printf("Array ordinato con quick-sort: ");
    stampa_array(a,n);
    return 0;
}