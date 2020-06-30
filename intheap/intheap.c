#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#define MAX_ELEM	20

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP* heapCreate(int capacity) {
	HEAP* newHeap = (HEAP*)malloc(sizeof(HEAP));
	newHeap->capacity = MAX_ELEM;
	newHeap->heapArr = (int*)malloc(sizeof(int) * MAX_ELEM);
	newHeap->last = 0;
	return newHeap;
}

/* Free memory for heap
*/
void heapDestroy(HEAP* heap) {
	free(heap->heapArr);
	free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert(HEAP* heap, int data);


/*Swap the value between the arguments*/
void swap(int* element1, int* element2);

/*Return the index for the parent node of a argument */
int parent(int index);


/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP* heap, int index) {
	while (index > 0 && heap->heapArr[parent(index)] < heap->heapArr[index]) {  //Swap the value untile the heap is made.
		swap(&(heap->heapArr[index]), &(heap->heapArr[parent(index)]));
		index = parent(index);
	}
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete(HEAP* heap, int* data);

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown(HEAP* heap, int index) {
	int leftIndex = 2 * index + 1;
	int rightIndex = 2 * index + 2;
	int largest = index;                   //Making max-heap
	if (leftIndex <= (heap->last) - 1 &&
		heap->heapArr[leftIndex] > heap->heapArr[index]) {              
		largest = leftIndex;
	} // compare left node
	if (rightIndex <= (heap->last) - 1 &&
		heap->heapArr[rightIndex] > heap->heapArr[largest]) {
		largest = rightIndex;
	}// compare right node
	if (largest != index) {
		swap(&heap->heapArr[index], &heap->heapArr[largest]); // swap
		_reheapDown(heap, largest);   // reculsion, untile last level
	}
}

/* Print heap array */
void heapPrint(HEAP* heap) {
	for (int i = 0; i < heap->last; i++) {
		printf("%d\t", heap->heapArr[i]);
	}
	printf("\n");
}


int main(void)
{
	HEAP *heap;
	int data;
	int i;
	
	heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		heapInsert( heap, data);
		
		heapPrint( heap);
 	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, &data);

		printf( "Deleting %d:	", data);

		heapPrint( heap);
 	}
	
	heapDestroy( heap);
	
	return 0;
}


void swap(int* element1, int* element2) {
	int tmp;
	tmp = *element1;
	*element1 = *element2;
	*element2 = tmp;
}

int parent(int index) {
	if (index % 2 == 0) {
		return (index / 2) - 1;
	}
	else {
		return index / 2;
	}
}

int heapInsert(HEAP* heap, int data) {
	if ((heap->last) == (heap->capacity)) {
		return 0;
	}
	heap->last++;
	heap->heapArr[(heap->last) - 1] = data;
	_reheapUp(heap, (heap->last) - 1);
	return 1;
}

int heapDelete(HEAP* heap, int* data) {
	*data = heap->heapArr[0];
	heap->heapArr[0] = heap->heapArr[(heap->last) - 1];
	heap->last--;
	_reheapDown(heap, 0);
	if (heap->last == 0) {
		heap->last--;
		return 0;
	}
	return 1;
}


