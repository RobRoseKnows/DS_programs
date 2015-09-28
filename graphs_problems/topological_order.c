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

void dfs_iterative_traversal(int start, graph *g, unsigned int *visited)
{
   //unsigned int *visited;
   int vertex;
   int i, j;

   //visited = malloc(sizeof(unsigned int) * (g->v));
   //memset(visited, 0, sizeof(visited));

   if (visited[start]) return;

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

   if (visited[start]) return;

   visited[start] = 1;
   printf("%d, ", start);  //<-------this is already printing it in topological order

   for(i = 0; i < g->v; i++) {
      j = g->adj[start][i];

      if (j == 1 && !visited[i]) {
        dfs_recursive(i, g, visited);
      }
   }
}

//prints bottom to top (based on descending visited times)
//Reverse of the output would be topological order
void dfs_recursive_modified(int start, graph *g, unsigned int *visited)
{
   int i, j;

   if (visited[start]) return;

   visited[start] = 1;
   //put_to_stack
   stack_push(start);

   for(i = 0; i < g->v; i++) {
      j = g->adj[start][i];

      if (j == 1 && !visited[i]) {
        dfs_recursive_modified(i, g, visited);
      }
   }
}

/**
 * start : starting point from where traversal has to started  <IN>
 * g : graph which is to be traversed                           <IN>
 **/
void dfs_traversal(graph *g)
{
   unsigned int *visited;
   int i;
   int arr[100];
   int cnt = 0;
   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));

   printf("\nDFS Recursive \n");
   for (i = 0; i < g->v; i++)
     dfs_recursive(i, g, visited);

   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));

#if 0
   printf("Iterative\n");
   for (i = 0; i < g->v; i++)
     dfs_iterative_traversal(i, g, visited);

   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));
#endif
   printf("\n\nDFS Recursive modified. ");
   for (i = 0; i < g->v; i++)
     dfs_recursive_modified(i, g, visited);
 
   printf("Print DFS bottom-up\n"); 
   while (!stack_empty()) {
        i = stack_pop();
        arr[cnt++] = i;
        printf("%d, ", i);
   }

   printf("\n\nTopological order\n");
   do {
      printf("%u, ", arr[--cnt]);
   } while (cnt != 0);

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

   dfs_traversal(g);
   printf("\n\n");

   return 0;
}
