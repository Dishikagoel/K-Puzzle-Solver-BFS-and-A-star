#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int k;

typedef struct node {
	int *array;
    struct node* next;
    struct node* parent;
} Node;

Node *initializeNode(int *arr, Node *parent) {
    Node *myNode = malloc(sizeof(Node));
    myNode->array = arr;
	myNode->next = NULL;
	myNode->parent = parent;
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

Queue *initializeQueue(int capacity){
    
	Queue *newQueue;
    
	newQueue = malloc(sizeof(Queue));
    	newQueue->capacity = capacity;
    	newQueue->size = 0;
    	newQueue->data = malloc(sizeof(Node *)*newQueue->capacity);

    	return newQueue;
}

void doubleCapacity(Queue *myQueue){
    	myQueue->capacity *= 2;
    	myQueue->data = realloc(myQueue->data, sizeof(Node *)*myQueue->capacity);
}

void enque(Queue *myQueue, Node *newNode){

	if(myQueue->size == myQueue->capacity){
		doubleCapacity(myQueue);
	}
	myQueue->data[myQueue->size] = newNode;
	myQueue->size += 1;
}

Node *deque(Queue *myQueue){

	Node *temp=myQueue->data[0];
    
	if(myQueue->size==0){
        	printf("deleteFromHead: Queue size = 0, nothing to delete\n");
    	}	
    
	else{
		for(int i=1; i<myQueue->size; i++){
			myQueue->data[i-1] = myQueue->data[i];
			myQueue->data[i] = NULL;
		}
		myQueue->size -= 1;
	}
	return temp;
}

int *swap(int *arr, int i, int j) {
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
        neighbors[0] = swap(arr, first[1]+first[0]*k, index_0);
    }
    if (i+1 <= k-1) {
        second[0] = i+1;
        second[1] = j;
        neighbors[1] = swap(arr, second[1]+second[0]*k, index_0);
    }
    if (j-1 >= 0) {
        third[0] = i;
        third[1] = j-1;
        neighbors[2] = swap(arr, third[1]+third[0]*k, index_0);
    }
    if (j+1 <= k-1) {
        fourth[0] = i;
        fourth[1] = j+1;
        neighbors[3] = swap(arr,fourth[1]+fourth[0]*k, index_0);
    }
    return neighbors;
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
	Node *firstNode;
	Queue *myQueue;
	OpenHashTable *myHashTable;
	Node *temp;
	int **neighbors;
    Node *myNode;
    int i=0;
    int move=0;


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

	for(int i=0;i<k*k;i++)
		fscanf(fp_in,"%d ",&initial_board[i]);

    int is_solvable = isSolvable(initial_board);
    if (is_solvable != 0) {
        fprintf(fp_out, "#moves\n");
	    fprintf(fp_out, "no solution\n");
        return 0;
    }

	fclose(fp_in);

	myQueue = initializeQueue(2000);
	myHashTable = initializeHashTable(1000);
	firstNode = initializeNode(initial_board, NULL);
    enque(myQueue, firstNode);
    
	while (myQueue->size > 0) {
        //printf("HERE\n");
		temp = deque(myQueue);
        printf("Dequeued:\n");
        for(int i=0; i<k*k; i++) {
        printf("%d", temp->array[i]);
        }
        printf("\n");
		if (arraysMatch(temp->array, solved_board)==0) break;
		insertToHashTable(myHashTable, temp);
		neighbors = gen_neighbors(temp->array);
		for (int i=0; i<4; i++) {
            if (neighbors[i]) {
            printf("printing neighbor[%d]\n", i);
            for (int x=0; x<k*k; x++) {
                printf("%d,", neighbors[i][x]);
            }
            printf("\n");
				if (isMember(myHashTable, neighbors[i])==1) {
					continue;
				}
				else {
                    myNode = initializeNode(neighbors[i], temp);
					enque(myQueue, myNode);
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
