#include<stdio.h>
#include<stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		if(feof(fin))
			break;
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(maxHeap, key);
				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

// heapsize만큼 메모리를 동적할당해서 생성하는 함수
Heap* CreateHeap(int heapSize){
	Heap *heap = (Heap *) malloc(sizeof(Heap));
	heap->Capacity = heapSize;
	heap->Size = 0;
	// element를 list에 저장하기 위해 heap size만큼 메모리 할당
	heap->Elements = (int *) malloc(sizeof(int) * (heapSize + 1));
	// root index를 1로 시작하기 위해 0에 INF 삽입
	heap->Elements[0] = INF;
	return heap;
}

// heap에 value를 삽입하는 함수
void Insert(Heap *heap, int value){
	// heap이 full이면 오류 메세지 출력 후 return 
	if(IsFull(heap)) {
		fprintf(fout, "insert error : heap is full\n");
		return;
	}

	// heap에 해당 value가 이미 있으면 오류 메세지 출력 후 return
	if(Find(heap, value)) {
		fprintf(fout, "insert error : %d is already in the heap\n", value);
		return;
	}
	
	// percolate up
	int idx = heap->Size + 1;
	while(idx > 0) {
		// swap이 필요없는 경우, 반복문 탈출
		if(heap->Elements[idx/2] > value) {
			break;
		}
		// swap이 필요한 경우 parent node와 swap
		else {
			heap->Elements[idx] = heap->Elements[idx/2];
			idx = idx/2;
		}
	}
	// 최종 위치에 insert 후, size++
	heap->Elements[idx] = value;
	heap->Size++;
	fprintf(fout, "insert %d\n", value);
}

// tree에 value가 있으면 1, 없으면 0을 return하는 함수
int Find(Heap* heap, int value){
	// list의 처음부터 탐색
	for(int i = 1; i < heap->Size+1; i++) {
		if(heap->Elements[i] == value) {
			return 1;
		}
	}
	return 0;
}

// tree의 root node를 제거 후 return하는 함수
int DeleteMax(Heap* heap){
	// 빈 tree인 경우 
	if(heap->Size == 0) {
		fprintf(fout, "delete error : heap is empty\n");
		return -INF;
	}
	// percolate down과 return에 필요한 변수
	int max_element = heap->Elements[1];
	// tree의 제일 마지막 node
	int temp = heap->Elements[heap->Size--];
	int idx = 1;

	// percolate down
	while(1) {
		// child node가 둘 다 있을 경우
		if(2*idx+1 < heap->Size) {
			// left child가 제일 클 경우, swap 필요
			if(heap->Elements[2*idx] > heap->Elements[2*idx + 1] && heap->Elements[2*idx] > temp) {
				heap->Elements[idx] = heap->Elements[2*idx];
				idx = 2*idx;
			}
			// right child가 제일 클 경우, swap 필요
			else if(heap->Elements[2*idx + 1] > heap->Elements[2*idx] && heap->Elements[2*idx + 1] > temp) {
				heap->Elements[idx] = heap->Elements[2*idx + 1];
				idx = 2*idx + 1;
			}
			// temp가 제일 클 경우
			else {
				heap->Elements[idx] = temp;
				break;
			}
		}
		// child node가 왼쪽만 있을 경우
		else if(2*idx == heap-> Size) {
			// left child가 제일 클 경우, swap 필요   
			if(heap->Elements[2*idx] > temp) {
				heap->Elements[idx] = heap->Elements[2*idx];
				idx = 2*idx;
			}
			else {
				heap->Elements[idx] = temp;
				break;
			}
		}
		// child node가 없을 경우 
		else {
			heap->Elements[idx] = temp;
			break;
		}
	}
	return max_element;
}

// heap의 node를 levelorder로 출력하는 함수
void PrintHeap(Heap* heap){
	if(heap->Size == 0) {
		fprintf(fout, "print error : heap is empty\n");
		return;
	}
	// list에 들어간 순서로 출력함
	for(int i = 1; i < heap->Size+1; i++) {
		fprintf(fout, "%d ", heap->Elements[i]);
	}
	fprintf(fout, "\n");
}

// heap이 full인지 확인하는 함수
int IsFull(Heap* heap){ 
	if(heap->Size == heap->Capacity) return 1;
	else return 0;
}
