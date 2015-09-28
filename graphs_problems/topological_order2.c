/**
 * Another and direct way of finding
 * topological_order
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //for memset


typedef struct graph {
   int v;            //number of vertices
   int **adj;        //2-d array as adjadency matrix
}graph;

graph *g;

int stack[100] = {0};

static int top = -1;

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


/*
 * In file topological_order.c file, we saw finding other method of
 * printing topological order
 *
 * This will also give the order but in slightly different way
 */
void topological_order_util(unsigned int start, graph *g, unsigned int *visited)
{
    int i, j;
    if (visited[start]) return;

    visited[start] = 1;

    for (i=0; i<g->v; i++) {
        j = g->adj[start][i];

        if (j == 1 && !visited[i])
            topological_order_util(i, g, visited);
    }
    stack_push(start);
}

/**
 * g : graph which is to be traversed                           <IN>
 **/
void topological_order(graph *g)
{
   unsigned int *visited;
   int i;
   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));

   for (i = 0; i<g->v; i++) {
        topological_order_util(i, g, visited);
   }
    
}


int main()
{
   int v = 9;

   g = init_graph(g, v);

   printf("\n\n");

   //the graph should be a DAG
   add_edge(0, 1, &g);
   add_edge(1, 2, &g);
   add_edge(3, 1, &g);
   add_edge(3, 4, &g);
   add_edge(2, 5, &g);
   add_edge(4, 5, &g);
   add_edge(6, 7, &g);
   //8 has no edge

   print_adjacency(g, v);

   printf("DFS recursive traversal is : \n");
   topological_order(g);

   while(!stack_empty()) {
       printf("%d, ", stack_pop());
   }     
   printf("\n\n");

   return 0;
}
