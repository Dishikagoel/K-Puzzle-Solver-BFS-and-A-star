#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int k;
int numOfMoves=0;
int *solvedArrayPtr;

typedef struct node {
	int *array;
    struct node* next;
    struct node* parent;
    int manhattan;
    int moves;
    int priority;
	
} Node;

Node *initializeNode(int *arr, Node *parent, int manhattan) {
    Node *myNode = malloc(sizeof(Node));
    myNode->array = arr;
	myNode->next = NULL;
	myNode->parent = parent;
    myNode->moves = numOfMoves;
    myNode->manhattan = manhattan;
    myNode->priority = myNode->moves+manhattan;
    return myNode;
}

typedef struct openHashTable {
    int size;
    Node **table;
} OpenHashTable;

int arraysMatch(int *array1, int *array2) {
	for (int i=0; i<k*k; i++) {
		if (array1[i] != array2[i]) {
			return 1;
		}
	}
	return 0;
}

OpenHashTable *initializeHashTable(int size){

    OpenHashTable *hashTable = malloc(sizeof(OpenHashTable));
    hashTable->size = size;
    hashTable->table = malloc(sizeof(Node *)*size);

    for(int i=0;i<size;i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

int hash(int *array,int size) {
    unsigned long total=0;
    for(int i=0; i<k*k; i++)
    {
        total = total + array[i]*(i+1);
        total = total * 101;
    }
    return (total % size);
}

void insertToHashTable(OpenHashTable *hashTable, Node *newNode){

    int position = hash(newNode->array, hashTable->size);

    Node *ptr = hashTable->table[position];
    while(ptr!=NULL){
        if(arraysMatch(newNode->array, ptr->array) == 0) {
            return;
        }
        else {
            ptr=ptr->next;
        }
    }
    newNode->next = hashTable->table[position];
    hashTable->table[position] = newNode;
}

int isMember(OpenHashTable *hashTable, int *array){

    int position = hash(array, hashTable->size);

    Node *ptr = hashTable->table[position];
    while(ptr!=NULL){
        if(arraysMatch(array, ptr->array) == 0)
            return 1;
        else
            ptr=ptr->next;
    }
    return 0;
}

typedef struct queue 
{
	int capacity;
	int size;
	Node **data;
} Queue;


int *swapInt(int *arr, int i, int j) {
    int *temp_arr=malloc(sizeof(int)*(k*k));
    int temp;

    for (int i=0; i<k*k; i++) {
        temp_arr[i] = arr[i];
    }

    temp = arr[i];
    temp_arr[i] = temp_arr[j];
    temp_arr[j] = temp;
    return temp_arr;
}

typedef struct heap {
    int capacity;
    int size;
    Node **array;
} Heap;

Heap *initializeHeap(int capacity)
{
    Heap *myHeap = malloc(sizeof(Heap));

    myHeap->capacity = capacity;
    myHeap->size=0;
    myHeap->array = malloc(sizeof(Node *)*capacity);
    return myHeap;
}


void swapNode(Node **heapArray, int index, int parentIndex) {

    Node *temp;

    temp = heapArray[index];
    heapArray[index] = heapArray[parentIndex];
    heapArray[parentIndex] = temp;
}

void doubleCapacity(Heap *myHeap){
    	myHeap->capacity *= 2;
    	myHeap->array = realloc(myHeap->array, sizeof(Node *)*myHeap->capacity);
}

int parent(Heap *myHeap, int index)
{
    int parentIndex = (index-1)/2;
    return parentIndex;
}

void upheap(Heap *myHeap, int index)
{
    if(index == 0)
        return; //current node is the root
    int parentIndex = parent(myHeap, index);
    Node *parentValue = myHeap->array[parentIndex];
    if (parentValue->priority <= myHeap->array[index]->priority)
        return; 
    //current node’s value is larger than its parent’s
    //else, we need to upheap current value by swapping with the parent
    swapNode(myHeap->array,index,parentIndex);
    upheap(myHeap, parentIndex);
}

void insertToHeap(Heap *myHeap, Node *node)
{
    if(myHeap->size == myHeap->capacity){
		doubleCapacity(myHeap);
	}
    myHeap->array[myHeap->size]=node;
    upheap(myHeap,myHeap->size);
    myHeap->size++;
}

void printHeap(Heap *myHeap) {
    printf("My Heap-\n");
    for (int i =0; i< myHeap->size; i++) {
        printf("priority: %d\n", myHeap->array[i]->priority);
    }
}


int minChildIndex(Heap *myHeap, int index)
{
    if(myHeap->size-1 >= (index+1)*2)
    {
        if(myHeap->array[(index+1)*2]->priority>=myHeap->array[(index+1)*2-1]->priority)
            return (index+1)*2-1;
        else
            return (index+1)*2;
    }
        else if(myHeap->size-1 == (index+1)*2-1)
            return (index+1)*2-1;
        else
        return -1;
}

void downHeap(Heap *myHeap, int index)
{
    if(myHeap->size-1 < (index+1)*2-1) //size-1 is due to last indexed value
        return; //children do not exist
    int minChild = minChildIndex(myHeap,index);
    if(myHeap->array[index]->priority <= myHeap->array[minChild]->priority)
        return; //parent smaller than children
        //else, we need to downheap current value by swapping with the min child
    swapNode(myHeap->array,index,minChild);
    downHeap(myHeap, minChild);
}

Node *deleteMin(Heap *myHeap)
{
    Node *temp=myHeap->array[0];

    swapNode(myHeap->array,0,myHeap->size-1);
    myHeap->size--;
    downHeap(myHeap,0);
    return temp;
}

int **gen_neighbors (int *arr) {
    int index_0, i, j;
    int first[2] = {-1};
    int second[2] = {-1};
    int third[2] = {-1};
    int fourth[2] = {-1};
    int **neighbors = malloc(sizeof(int *)*4);
    neighbors[0] = NULL;
    neighbors[1] = NULL;
    neighbors[2] = NULL;
    neighbors[3] = NULL;


    for (int x=0; x<k*k; x++) {
        if (arr[x] == 0) {
            index_0 = x;
            break;
        }
    }
    
    i = index_0/k;
    j = index_0%k;
    if (i-1 >= 0) {
        first[0] = i-1;
        first[1] = j;
        neighbors[0] = swapInt(arr, first[1]+first[0]*k, index_0);
    }
    if (i+1 <= k-1) {
        second[0] = i+1;
        second[1] = j;
        neighbors[1] = swapInt(arr, second[1]+second[0]*k, index_0);
    }
    if (j-1 >= 0) {
        third[0] = i;
        third[1] = j-1;
        neighbors[2] = swapInt(arr, third[1]+third[0]*k, index_0);
    }
    if (j+1 <= k-1) {
        fourth[0] = i;
        fourth[1] = j+1;
        neighbors[3] = swapInt(arr,fourth[1]+fourth[0]*k, index_0);
    }
    return neighbors;
}

int manhattan(int *arr) {
    int x=0, y=0;
    for (int i=0; i<k*k; i++) {
        x=0, y=0; 
        if (arr[i] != solvedArrayPtr[i]) {
            for (int j=0; j<k*k; j++ ) {
                if (arr[i] != solvedArrayPtr[j]) {
                    x = (i/k)+(j/k);
                    y = (i%k)+(j%k);
                }
            }
        }
    }
    return x+y;
}

void printBoard(int *initial_board) {
    for(int x=0; x < k*k; x++) {
        printf("%d", initial_board[x]);
    }
    printf("\n");
}

int findMove(int *arr1, int *arr2) {
    for (int i=0; i<k*k; i++) {
        if (arr1[i] != arr2[i]) {
            if (arr1[i] != 0) {
                return arr1[i];
            }
            else {
                return arr2[i];
            }
        }
    }
}

int find0Position(int puzzle[])
{
    int i=0;
    for (int x=0; x<k*k; x++) {
        if (puzzle[x] == 0) {
            i = k - x/k;
            break;
        }
    }
    return i;
        
}

int isSolvable(int arr[])
{
    int inv_count=0;
    int zero_pos = find0Position(arr);
    for (int i=0; i<k*k-1; i++)
    {
        for (int j=i+1; j<k*k; j++)
        {
            if (arr[j]!=0 && arr[i]!=0) {
                if (arr[i]>arr[j]) {
                    inv_count++;
                }
            }
        }
    }

    if (k%2 == 1) {
        if (inv_count%2 == 0) {
            return 0;
        }
    }
 
    else     // grid is even
    {
        if (zero_pos%2==0 && inv_count%2 == 1) {
            return 0;
        }
        if (zero_pos%2==1 && inv_count%2 == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{
	FILE *fp_in=NULL,*fp_out=NULL;
	char *line = NULL;
	size_t lineBuffSize = 0;
	ssize_t lineSize;//get kxk memory to hold the initial board
	Heap *myHeap;
	OpenHashTable *myHashTable;
    Node *firstNode;
	Node *temp;
    Node *myNode;
	int **neighbors;
    int i=0;
    int move=0;
    int manhattan_dist=0;


	fp_in = fopen(argv[1], "r");
	if (fp_in == NULL){
		printf("Could not open a file.\n");
		return -1;
	}
	
	fp_out = fopen(argv[2], "w");
	if (fp_out == NULL){
		printf("Could not open a file.\n");
		return -1;
	}
	
	getline(&line,&lineBuffSize,fp_in);//ignore the first line in file, which is a comment
	fscanf(fp_in,"%d\n",&k);//read size of the board
	
	getline(&line,&lineBuffSize,fp_in);//ignore the second line in file, which is a comment

    int initial_board[k*k];
    int solved_board[k*k];

    solved_board[k*k-1] = 0;
    for (int i=0; i<k*k-1; i++) {
        solved_board[i] = i+1;
    }

    solvedArrayPtr = solved_board;

	for(int i=0;i<k*k;i++)
		fscanf(fp_in,"%d ",&initial_board[i]);

    int is_solvable = isSolvable(initial_board);
    if (is_solvable != 0) {
        fprintf(fp_out, "#moves\n");
	    fprintf(fp_out, "no solution\n");
        return 0;
    }

	myHeap = initializeHeap(100);
	myHashTable = initializeHashTable(1000);
    manhattan_dist = manhattan(&initial_board[0]);
	firstNode = initializeNode(initial_board, NULL, manhattan_dist);
    insertToHeap(myHeap, firstNode);
    
	while (myHeap->size > 0) {
		temp = deleteMin(myHeap);
        //printf("Extracted min:\n");
        for(int i=0; i<k*k; i++) {
        }
        numOfMoves++;
		if (arraysMatch(temp->array, solved_board)==0) break;
		insertToHashTable(myHashTable, temp);
		neighbors = gen_neighbors(temp->array);
		for (int i=0; i<4; i++) {
            if (neighbors[i]) {
				if (isMember(myHashTable, neighbors[i])==1) {
					continue;
				}
				else {
                    manhattan_dist = manhattan(neighbors[i]);
                    myNode = initializeNode(neighbors[i], temp, manhattan_dist);
					insertToHeap(myHeap, myNode);
				}
			}
		}
	}
    
    while (temp->parent != NULL) {
        temp->parent->next = temp;
        temp = temp->parent;
    }

    fprintf(fp_out, "#moves\n");

    while (temp->next != NULL) {
        move = findMove(temp->array, temp->next->array);
        fprintf(fp_out, "%d ",move);
        temp = temp->next;
    }
    fprintf(fp_out, "\n");
}

