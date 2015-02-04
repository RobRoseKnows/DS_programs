#include <stdio.h>
#include <stdlib.h>


typedef struct ternary_node {
   char letter;
   struct ternary_node *l_ptr;      //low val - left node
   struct ternary_node *eq_ptr;     //equal to current node
   struct ternary_node *r_ptr;      //high val - right node
} t_node;

int search(t_node *root, char *str)
{
   if (root == NULL)
      return 0;

   if (*str < root->letter) {
      return search(root->l_ptr, str);
   }
   else if (*str > root->letter) {
      return search(root->r_ptr, str);
   }
   else if (*str == root->letter) {
      if (*(++str) == '\0') 
         return 1;
      else
         return search(root->eq_ptr, ++str);
   }
}


//print all strings and substrings with same prefix
//basic application of ternary trees
void traverse(t_node *root)
{

}

void traverseTSTUtil(t_node *root, char* buffer, int depth)
{
    if (root)
    {
        // First traverse the left subtree
        traverseTSTUtil(root->l_ptr, buffer, depth);
 
        // Store the character of this node
        buffer[depth] = root->letter;
/*
        if (root->isEndOfString)
        {
            buffer[depth+1] = '\0';
            printf( "%s\n", buffer);
        }
*/
         if(root->eq_ptr == NULL)
            printf( "%s\n", buffer);

        // Traverse the subtree using equal pointer (middle subtree)
        traverseTSTUtil(root->eq_ptr, buffer, depth + 1);
 
        // Finally Traverse the right subtree
        traverseTSTUtil(root->r_ptr, buffer, depth);
    }
}

// The main function to traverse a Ternary Search Tree.
// // It mainly uses traverseTSTUtil()
void traverseTST(t_node *root)
{
    char buffer[50];
    traverseTSTUtil(root, buffer, 0);
}

void insert_node(t_node **root, char *str)
{
   if ((*root) == NULL) {
      //create a new root
      (*root) = malloc(sizeof(t_node));
      (*root)->l_ptr = (*root)->r_ptr = (*root)->eq_ptr = NULL;
      (*root)->letter = *str;
   }

   if (*str < (*root)->letter) {
      printf("In left\n");
      insert_node(&((*root)->l_ptr), str);
   } else if (*str > (*root)->letter) {
      printf("In right\n");
      insert_node(&((*root)->r_ptr), str);
   } else {    //if current node equals to current char
      //case 1: make eq as new root and insert next char
      printf("In else %c \n", *(str+1));
      if (*(str+1))
         insert_node(&((*root)->eq_ptr), ++str);
      else if (*(str+1) == '\0')
         //skip
         {}

   }
}


int main()
{
   t_node *root = NULL;
   insert_node(&root, "abc");
   printf("root->letter = %c\n", root->letter);

   insert_node(&root, "at");
   insert_node(&root, "as");
   insert_node(&root, "ass");    //oops
   insert_node(&root, "am");
   insert_node(&root, "act");

/*
 *             a
 *             |
 *             b
 *             | \
 *             c  t
 *               /
 *              s
 *            / |
 *           m  s
 *          /
 *         c
 *          \
 *           t  
 */


   printf("calling search\n");
   printf("Found al = %d\n", search(root, "al") ? 1 : 0);      //0
   printf("Found asm = %d\n", search(root, "asm") ? 1 : 0);    //1
   printf("Found ac = %d\n", search(root, "ac") ? 1 : 0);      //0
   printf("Found ato = %d\n", search(root, "ato") ? 1 : 0);    //0
   printf("Found atc = %d\n", search(root, "atc") ? 1 : 0);    //1
   printf("Found bt = %d\n", search(root, "bt") ? 1 : 0);      //0

   traverseTST(root);

   return 0;
}
