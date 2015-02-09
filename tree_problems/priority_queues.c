/*
 * Implement priority queues
 * =========================
 *
 * Q/ What is it ?
 * A/ 
 * Should follow structure and heap property 
 *
 * Q/ Different ways to implement and their time complexities ?
 * A/
 *
 * Q/ The best way. Describe data structure
 * A/	TODO: array to represent tree
 *
 * Q/ Applications ?
 * A/
 */


//Implementing MinHeaps

#include <stdio.h>

#define NUM_OPS 5

//An array representing min-heap. Assuming only 100 elements can be present at time in priority queue.
//Keeping heap[0] blank, just to match (e,p) pair in array, where e is the element and p is its priority.
//So heap[i] contains element with priority i.

#define MAX_SIZE 101
int heap[MAX_SIZE];

//Maintaining the current size of the heap
unsigned int current_size = 0;

//TODO: display in tree fashion
void display_heap()
{
	int i;
	
	printf("\n");
	for(i = 1; i <= current_size; i++)
		printf(" %d", heap[i]);
	
	printf("\n");
}

/*
 * Algo :
 * a) Insert an elemnt in heap at empty slot such that it meets structure property
 * b) Percolate up, that element until it meets heap property
 *
 * Time Complexity : O(log n) as the deciding factor is divided by 2 at each iteration 
 */
void insert_elem(unsigned int x)
{
	unsigned int hole;
	
	//check if heap is already full
	if(current_size == MAX_SIZE) {
		printf("Heap is already full. Cannot insert an element\n");
		return;
	}

	//make slot for new element
	hole = ++current_size;

	//insert an element in hole
	heap[hole] = x;

	//percolate up the heap to settle new element in heap
	for(; hole > 1 && x < heap[hole/2]; hole /= 2) {
		//swapping parent and child
		heap[hole] = heap[hole/2];
		heap[hole/2] = x;
	}
	
	//printf("heap is \n");
	//display_heap();
}


//element at "indx" has to be percolated down
void percolate_down(int indx)
{	
	int tmp, child;
	//printf("new case......\n");
	while(indx < current_size) {
		child = indx * 2; 	//left child
		//printf("indx %d,%d and child.. %d, %d\n", indx, heap[indx], child, heap[child]);
		
		//for leaf nodes
		if(child > current_size)
			break;

		/*
 		 * second cond is for cases :
 		 * lets say currently the heap is	 
 		 *
 		 * 					16
 		 * 				   /  \
 		 * 				  9    1 
 		 *
 		 * without 2nd condition : the heap would become
 		 * 					9
 		 * 				   / \
 		 * 				  16  1
 		 *
 		 * but this does not satisfy heap property and not result in min-heap
 		 *
 		 * Now with 2nd condition, the heap results in min-heap
 		 * 					1
 		 * 				   / \
 		 * 				  9  16
 		 */
		if(child+1 <= current_size && heap[child] > heap[child+1])
			child++;

		if (heap[indx] > heap[child]) {
			//swap
			tmp = heap[indx];
			heap[indx] = heap[child];
			heap[child] = tmp;
		} else
			break;

		//display_heap();	
		indx = child;	
	} //end of while
}

#if 0

//from pdf
void percolate_down(int indx)
{
	int child;
	int tmp = heap[indx];

	for(; indx * 2 <= current_size; indx = child) {
	
		child = indx * 2;
		if(indx != current_size && child+1 <= current_size && heap[child+1] < heap[child])	
			child++;

		if(heap[child] < tmp)
			heap[indx] = heap[child];
		else
			break;
	}
	heap[indx] = tmp;

		display_heap();	
}
#endif

/*
 * Algo :
 * As if we have to remove an element, we have to remove it based on its priority
 * and only elemnt which has highest priority will be removed , So
 * to remove root(as highest priority element is always on top of heap)
 *
 * a) remove root and replace it with last elemnt in min-heap
 * b) Now percolate down this new root, until heap property is met
 *
 * Time Complexity : O(log n) as the deciding factor is multiplied by 2 at each iteration 
 * 
 */
void delete_elem()
{
	int min_element;
	if(current_size == 0) {
		printf("Heap is empty\n");
		return;
	}

	min_element = heap[1];
	heap[1] = heap[current_size];
	current_size--;
	percolate_down(1);			

	printf(" the element %d is removed\n", min_element);
	printf(" Heap after removalas follows:");	
	display_heap();
}

/*
 * ======= Build heap ======
 * What if elements to be inserted in the priority queue are already known.
 * In that case, the need to arrange the elements based on their priorities
 * so that the application can perform better.
 *
 * Two ways to build heap(priority queue)
 * 1. Slower method : insert N times into heap (Runtime complexity: O(N log N)
 * 2. Faster method : 
 * a) add elemnts into queue in random order, meeting structural property
 * b) Now percolate down the non-leaf(internal elemnts of heap) elements, until they satisfy heap property
 */


//Method 1
void build_heap()
{
	//int list[] = {24, 26, 16, 10, 6, 9, 1};
	int list[] = {8, 9, 10, 11, 12, 13, 14, 2, 3, 5, 4, 7, 6, 1};    //TODO: not working with this
	int i;
	int list_len = sizeof(list)/sizeof(list[0]);

	//cleaning heap
	current_size = 0;
	
	//printf("len is %d\n", list_len);
	for(i = 0; i < list_len; i++)
		insert_elem(list[i]);
	
	printf("heap as follows:");
	display_heap();
}

//Method 2
void build_heap_faster()
{
	//int list[] = {24, 26, 16, 10, 6, 9, 1};
	int list[] = {8, 9, 10, 11, 12, 13, 14, 2, 3, 5, 4, 7, 6, 1};    //TODO: not working with this
	int i;
	int list_len = sizeof(list)/sizeof(list[0]);
	
	//cleaning heap
	current_size = 0;

	//printf("len is %d\n", list_len);
	for(i = 0; i < list_len; i++) {
		++current_size;
		heap[current_size] = list[i];
	}

	//printf("curr size of heap %d\n", current_size);
	
	//now rearrange internal elements of heap
	//start with lowest rightmost internal node
	for(i = current_size/2; i > 0; i--)
		percolate_down(i);		

	printf("heap as follows:");
	display_heap();

}

void perform_action(unsigned int op)
{
	int x;
	switch(op) {
		case 1: 
				printf("Enter element :");
				scanf("%d", &x);
				insert_elem(x);
				break;
		case 2:
				delete_elem();
				break;
		case 3:
				display_heap();
				break;
		case 4:
				build_heap();
				break;
		case 5:
				build_heap_faster();
				break;
	}
}

int main(void)
{
	int op;

	do {
		printf("Operations : \n");
		printf("1. Insert an element\n");
		printf("2. delete an element with lowest priority\n");
		printf("3. display min-heap\n");
		printf("4. Build heap(Slower method)\n");
		printf("5. Build heap(Faster method)\n");
		
		//TODO: other ops
		
		printf("Enter option :");
		scanf("%d", &op);

		perform_action(op);
	} while(op > 0 && op <= NUM_OPS);
	
	return 0;
}
