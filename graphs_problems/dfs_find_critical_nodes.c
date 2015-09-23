/**
 * Problem statement :
 * Critical node: If a node reaches another node only through one node.
 * Eg: A-C-B and A-E-B , the A and B are critical nodes. (A reach B through one node which is C or E)
 * If A reaches B through more than one node, then they are not critical nodes.
 * 1) A-C-B
 * A-D-E-B (A reach B thro c which might lead to critical node but A has another path to B thro D and E, so they are not critical nodes).
 * 2) X-Y-Z
 * X-A-Z (X and Z are critical nodes)
 * Now find all critical nodes
 *
 * Algo:
 * For this we have to maintain one more 2-d array which will maintain
 * critical nodes pairs
 *
 * example :
 * for following DIRECTED graph
 *
 *  Edge(0,1)
 *  Edge(0,2)
 *  Edge(1,3)
 *  Edge(1,4)
 *  Edge(3,4)
 *  Edge(2,4)
 *
 *   0  1  2  3  4
 *   -------------
 * 0|0  0  0  1  1         //represents that {0,3} and {0, 4} are critical nodes pair
 * 1|0  0  0  0  0       
 * 2|0  0  0  0  0
 * 3|0  0  0  0  0
 * 4|0  0  0  0  0
 * 
 * if u traverse 1 via 3, {1,4} are critical nodes but 1,4 are directly connected also, 
 * so they cannot be. we need conditions to rule out such corner cases
 *
 *
 * Usecase : finding friends of friends on facebook
 *
 * But if we need to find friends of friends of friends, the algo will become complex using DFS
 *
 * BFS should be used instead.
 * For friends of friends - print 2 level of graph    { assuming root at level 0 }
 * For freinds of friends of friends - print 3 level of graph
 * and so on...
 *
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
void print_adjacency(graph *g)
{
   int i, j;

   int v = g->v;
   for (i = 0; i < v; i++) {
      for (j = 0; j < v; j++) {
         printf("%d, ",g->adj[i][j]);
      }
      printf("\n");
   }
}

void fill_zeros(graph *g)
{
   int i, j;
   int v = g->v;
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

   fill_zeros(g);
   print_adjacency(g);

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

int count_visited = 0; //after 2 recursions, mark critical node in matrix
int parent_of_parent; //denotes u in (u,v) critical node pair
unsigned int critical[10][10] = {0};


//the return value will not be used by 1st caller. Just for recursion, its added
int dfs_recursive(int start, graph *g, unsigned int *visited)
{
   int i, j;
   int critical_node;

#if 0
   if (visited[start])    //if starting node is already visited
      return; 
#endif
   if (count_visited == 0)
      parent_of_parent = start;

   if (count_visited == 2) {
         printf("Critical node : %d\n", start);
        critical[parent_of_parent][start] = 1;
         return ;
   }

   count_visited++;
#if 0
   visited[start] = 1;
#endif

   for(i = 0; i < g->v; i++) {
      j = g->adj[start][i];
      printf("for %d, %d = %d\n...", start, i, j);

      //if (j == 1 && !visited[i]) {
      if (j == 1) {
        dfs_recursive(i, g, visited);
      }
   }
  
   count_visited = 1;
   //return start;
}


/**
 * start : starting point from where traversal has to started  <IN>
 * g : graph which is to be traversed                           <IN>
 **/
void dfs_traverse_all_node(graph *g)
{
   int u, v;
   unsigned int *visited;
   unsigned int critical_node;

   int num_vertices = g->v;
   visited = malloc(sizeof(unsigned int) * (g->v));
   memset(visited, 0, sizeof(visited));

   //create 2-d array to maintain critical node pair
   //TODO: dynamic allocation

   for (v = 0; v < num_vertices; v++) {
      count_visited = 0;
      printf("visiting %d-----\n", v);
      critical_node = dfs_recursive(v, g, visited);
   }

   //print_critical()
   for (u = 0; u < num_vertices; u++) {
      for (v = 0; v < num_vertices; v++) {
         printf("%d, ",critical[u][v]);
      }
      printf("\n");
   }
   
   //now check corner cases
   //case 1: if adj[u][v] == 1, then critical[u][v] should not be 1.
   //        if it is, make it 0.

   for (u = 0; u < num_vertices; u++) {
      printf("critical node %d --> ", u);
      for (v = 0; v < num_vertices; v++) {
         if (g->adj[u][v] == 1 && critical[u][v] == 1)
            critical[u][v] = 0;

         if (critical[u][v] == 1)
            printf("%d, ", v);

      } //inner for
      
      printf("\n");
   } //outer for
      
}


int main()
{
   int v = 6;  //num of vertices

   g = init_graph(g, v);

   printf("\n\n");
//example 1
/*
   add_edge(0, 1, &g);
   add_edge(1, 3, &g);
   add_edge(0, 2, &g);
   add_edge(3, 4, &g);
   add_edge(1, 4, &g);
   add_edge(2, 4, &g);
*/

//example 2 
/* 
   add_edge(0, 1, &g);
   add_edge(0, 2, &g);
   add_edge(0, 3, &g);
   add_edge(1, 3, &g);
   add_edge(3, 4, &g);
   add_edge(4, 5, &g);
   add_edge(4, 6, &g);
*/

   add_edge(0, 1, &g);
   add_edge(1, 2, &g);
   add_edge(2, 3, &g);
   add_edge(3, 5, &g);
   add_edge(4, 5, &g);
   add_edge(4, 1, &g);


   print_adjacency(g);

   printf("DFS recursive traversal is : \n");
   dfs_traverse_all_node(g);
   printf("\n\n");


   return 0;
}
