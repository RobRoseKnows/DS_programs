#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //for memset


typedef struct graph {
   int v;            //number of vertices
   int **adj;        //2-d array as adjadency matrix
}graph;

graph *g;

int stack[100] = {0};

int top = -1;

void stack_push(int v)
{
   top++;
   stack[top] = v;
}

int stack_pop()
{
   int v = stack[top];
   top--;
   return v;
}

int stack_empty()
{
   return (top == -1) ? 1 : 0;
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

void dfs_iterative_traversal(int start, graph *g)
{
   unsigned int *visited;
   int vertex;
   int i, j;

   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));


   //push current vertex
   stack_push(start);

   while (!stack_empty()) {
      vertex = stack_pop();
      printf("%d, ", vertex);

      if (!visited[vertex])
         visited[vertex] = 1;

      for(i = 0; i < g->v; i++) {
         j = g->adj[vertex][i];

         if(j == 1 && !visited[i]) {
            stack_push(i);
         }
      }  //end nested for
   } //end while
}


void dfs_recursive(int start, graph *g, unsigned int *visited)
{
   int i, j;

   visited[start] = 1;
   printf("%d, ", start);

   for(i = 0; i < g->v; i++) {
      j = g->adj[start][i];

      if (j == 1 && !visited[i]) {
        dfs_recursive(i, g, visited);
      }
   }
}


/**
 * start : starting point from where traversal has to started  <IN>
 * g : graph which is to be traversed                           <IN>
 **/
void dfs_traversal(int start, graph *g)
{
   unsigned int *visited;

   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));

   dfs_recursive(start, g, visited);
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

   printf("DFS recursive traversal is : \n");
   dfs_traversal(3, g);
   printf("\n\n");

   printf("DFS iterative traversal is : \n");
   dfs_iterative_traversal(3, g);
   //printf("\n\n");
   //print_adjacency(g, v);

   return 0;
}
