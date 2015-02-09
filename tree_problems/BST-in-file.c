#include <stdio.h>
#include <stdlib.h>		//for malloc

//for open
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

//for string ops
#include <string.h>

#define NUM_OPS 5



//data structure of BST
typedef struct BST_node {
	int key;
	struct BST_node *left, *right;
} bst_node;

bst_node *root; //root of BST

bst_node *new_root;

int count_chars = 0;		//count chars in buffer,used to write in file	

/* ******************
 * Functions
 * ******************
 */
bst_node *insert(int key, bst_node *node); //function prototype

print_preorder_BST(bst_node *node)
{
	if (node != NULL) {
		printf("%d ", node->key);
		print_preorder_BST(node->left);
		print_preorder_BST(node->right);
	}
}

/*
 * De-serialization : process of reading the data from file and 
 * reconstruct the data structure in memory
 */
void deserialize()
{
	int fd;
	char *buffer;
	char delim[2] = ",";
	char *token;
	int key;

	fd = open("file.txt",O_RDONLY);

	buffer = malloc(sizeof(char) * 100);

	if (buffer == NULL)
		goto close_file;
			
	memset(buffer, 0, 100);

	read(fd, buffer, 100);

	token = strtok(buffer, delim);

	while (token != NULL) {
		sscanf(token, "%d", &key);
		printf("Now inserting %d\n", key);
		new_root = insert(key, new_root);
		token = strtok(NULL, delim);
	}
	
	printf("tree is reconstructed as :\n");
	print_preorder_BST(new_root);

	root = new_root;

close_file:
	free(buffer);
	close(fd);	

}

//helper function to serialize the tree
void serialize_helper_write(bst_node *root, char *buffer, int fd)
{
	char buf[10];
	if (root != NULL) {
		sprintf(buf, "%d", root->key);
		
		//if buffer can accomadate next node, then only add in buffer
		if (strlen(buffer) < 100 && ((strlen(buffer) + strlen(buf) + 1 /*1 for comma*/) <=100)) {
			count_chars += strlen(buf);
			strcat(buffer, buf);
			strcat(buffer, ",");
			count_chars++;
		}
		else {
			buffer[count_chars + 1] = '\0';
			//write all that is in buffer
			write(fd, buffer, strlen(buffer));

			//wipe out the contents from buffer
			memset(buffer, 0, 100);

			//re-initialize count_chars
			count_chars = 0;
		
			//for next filling, 
			strcat(buffer, ",");
			count_chars++;
		}

		serialize_helper_write(root->left, buffer, fd);
		serialize_helper_write(root->right, buffer, fd);
	}
}

/*
 * Serialization of BST - storing the tree in a file
 */
void serialize_bst(bst_node *root)
{
	int fd;
	char *buffer;

	//open a file and store the tree in preorder fashion
	fd = open("file.txt",O_CREAT|O_RDWR);

	//allocate a buffer of some fixed size and write those 
	//many nodes of tree at once(rather than 1 node at a time) 
	//in the file to avoid performance deterioration
	buffer = malloc(sizeof(char) * 100);

	if (buffer == NULL)
		goto close_file;
			
	memset(buffer, 0, 100);
	serialize_helper_write(root, buffer, fd);

	//what if tree is so small that buffer remain half filled even after
	//containing all node. In that case dont forget to store the buffer in file
	buffer[count_chars + 1] = '\0';
	//write all that is in buffer
	write(fd, buffer, strlen(buffer));


	// TODO :once every thing is written in file. delete the in-memro data structure
	// as there is no facility to freeing nodes is implemented in the program,
	// it leads to memory leaks
	
close_file:
	free(buffer);
	close(fd);
}

void generate_node(int key, bst_node **node)
{
	(*node) = malloc(sizeof(bst_node));
	(*node)->key = key;
	(*node)->left = (*node)->right = NULL;
}


bst_node *insert(int key, bst_node *node)
{
	if (node == NULL) {
		generate_node(key, &node);
	} else if (key < node->key) {
		node->left = insert(key, node->left);
	} else if (key > node->key) {
		node->right = insert(key, node->right);
	}
		
	return node;
}

void perform_action(int op)
{
    int x;
    switch (op) {
        case 1:                                         //insert a node
            printf("Enter element :");
            scanf("%d", &x);
            root = insert(x, root);
            printf("key = %d\n", root->key);
            break;
        case 2:                                         //TODO: delete a node
            break;
        case 3:                                         //print BST
            print_preorder_BST(root);
            printf("\n");
            break;
		case 4:											//serialize BST
			serialize_bst(root);
			break;
		case 5:											//de-serialize BST
			deserialize();
			break;

        default :
            ; //nothing
    }
}

int main()
{
    int op;
    do {
        printf("Operations : \n");
        printf("1. Insert an element into BST\n");
        printf("2. Delete an element from BST\n");
        printf("3. Print BST\n");
        printf("4. Serialize BST\n");
        printf("5. De-Serialize BST\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);
}
