#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //for memset


typedef struct graph {
   int v;            //number of vertices
   int **adj;        //2-d array as adjadency matrix
}graph;

graph *g;


int queue[100];
int rear = -1, front = -1;


/*
 * =============
 * Queue functions
 * =============
 */
int q_empty()
{
   if (front > rear)
      return 1;
  
   return 0;
}

void q_enqueue(int elem)
{
   ++rear;
   queue[rear] = elem;
}

int q_dequeue()
{
   int res;
   if (front == -1)
      ++front;
   res = queue[front];
   front++;
   return res;

}

/* ==============
 * Graph functions
 * ==============
 */
void print_adjacency(graph *g, int v)
{
   int i, j;

   for (i = 0; i < v; i++) {
      for (j = 0; j < v; j++) {
         printf("%d, ",g->adj[i][j]);
      }
      printf("\n");
   }
}

void fill_zeros(graph *g, int v)
{
   int i, j;

   for (i = 0; i < v; i++)
      for (j = 0; j < v; j++)
         g->adj[i][j] = 0;

}

graph *init_graph(graph *g, int v)
{

   int i;
   g = malloc(sizeof(graph));
   
   //initialize number of vertices
   g->v = v;
   
   //allocate 2-d array
   
   //a. allocate array of pointers
   g->adj = malloc((sizeof(int *)) * v);

   //b. allocate array i
   for (i = 0; i < v; i++) {
      g->adj[i] = malloc(sizeof(int) * v);
   }

   fill_zeros(g, v);
   print_adjacency(g, v);
   
   return g;
}

/**
 * src : src vertex           <IN>
 * dest : destination vertex  <IN>
 * g : resulting graph        <OUT>
 **/
void add_edge(int src, int dest, graph **g)
{
   //this denotes there is an edge exist from
   //src to dest
   (*g)->adj[src][dest] = 1;
}

/**
 * start : starting point from where traversal has to started  <IN>
 * g : graphwhich is to be traversed                           <IN>
 **/
void bfs_traversal(int start, graph *g)
{
   unsigned int *visited;
   int vertex;
   int i, j;
   /*
    * allocate visited array of size=number of vertices.
    * This helps in avoiding cycles during traversals
    */
   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));

   //mark current node as visited and enqueue it
   visited[start] = 1;
   q_enqueue(start);

   while (!q_empty()) {
      vertex = q_dequeue();

      printf("%d ", vertex);
      
      //add to queue, all vertices adjacent to v
      //before enqueuing if adjacent is marked visited,
      //mark it visited
      for (i = 0; i < g->v; i++) {
         j = g->adj[vertex][i];

         //if there exist an edge between vertex and i [ i.e j==1]
         //and
         //i is not yet visited
         if (j == 1 && !visited[i]) {
            visited[i] = 1;
            //printf("adding %d ...\n", i);
            q_enqueue(i);
         }
      } //end nested for
   } //end while
}

int main()
{
   int v = 4;

   g = init_graph(g, v);

   printf("\n\n");
   add_edge(0, 1, &g);
   add_edge(1, 2, &g);
   add_edge(2, 3, &g);
   add_edge(3, 0, &g);
   add_edge(0, 2, &g);
   add_edge(3, 1, &g);

   print_adjacency(g, v);

   printf("BFS traversal is : \n");
   bfs_traversal(3, g); 
   printf("\n");

   return 0;
}
