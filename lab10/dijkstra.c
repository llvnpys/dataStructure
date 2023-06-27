#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);


void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size + 1);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	printShortestPath(g);

	deleteGraph(g);
} 

// 그래프를 생성하는 함수
Graph* createGraph(int size){
	int i;
	// 메모리 할당
	Graph *g = (Graph*)malloc(sizeof(Graph));
	g->vertices = (int**)malloc(sizeof(int*)*(size));
	g->nodes = (Node*)malloc(sizeof(Node)*(size));
	// 값 설정
	for(i = 0; i < size; i++){
		// vertices 2차원 배열을 할당해서 edge의 weight를 저장할 수 있게 함
		g->vertices[i] = (int*)malloc(sizeof(int)*(size));
		// 음수로 초기화
		memset(g->vertices[i], -1, sizeof(int) * (size));

		g->nodes[i].vertex = i;
		// vertex의 distance는 0으로 초기화
		g->nodes[i].dist = INF;
		g->nodes[i].prev = 0;
	}
	// 시작 노드의 distance는 0으로 변경
	g->nodes[1].dist = 0;
	g->size = size; 
	return g;
}

// 그래프를 제거하는 함수
void deleteGraph(Graph* g){
	// 할당해준 메모리 해제
	for(int i = 0; i < g->size; i++){
		free(g->vertices[i]);
	}
	free(g->vertices);
	free(g->nodes);
	free(g);
}

// 시작 노드에서 각 노드로 이동하는 최단 거리를 찾아서 출력하는 함수
void printShortestPath(Graph* g){
	// 힙 생성
	Heap *heap = createMinHeap(g->size);
	Node minNode;
	Node temp;
	int i;
	// 처음 노드 힙에 삽입
	insertToMinHeap(heap, 1, 0);
	// 힙이 비어있을 때까지 반복
	while(heap->Size > 1){
		// minNode 힙에서 제거
		minNode = deleteMin(heap);
		// minNode와 연결된 노드가 있는지 순회
		for(i = 1; i < g->size; i++){
			// 현재 노드의 distance보다 지금 이동중인 path의 distance가 더 짧은 경우
			if(g->vertices[minNode.vertex][i] > -1 && g->nodes[i].dist > minNode.dist + g->vertices[minNode.vertex][i]){
				// 노드의 distacne와 prev 변경 후 힙에 삽입
				g->nodes[i].dist = minNode.dist + g->vertices[minNode.vertex][i];
				g->nodes[i].prev = minNode.vertex;
				insertToMinHeap(heap, i, g->nodes[i].dist);
			}
		}
	}

	// 최단 거리 출력
	// 시작 노드를 제외한 노드를 순회
	for(int j = 2; j < g->size; j++){
		// 연결된 노드가 없으면
		if(g->nodes[j].prev == 0){
			fprintf(fout, "can not reach to node %d\n", j);
		}
		// 연결된 노드가 있으면
		else{
			// 이동한 노드를 확인하는 작업
			temp = g->nodes[j];
			// 이동한 노드를 저장하는 배열 할당
			int *path = (int*)malloc(sizeof(int) * g->size);
			int cnt = 0;
			path[cnt++] = temp.vertex;
			// 이전 노드가 시작 노드가 아니라면
			while(temp.prev != 0){
				path[cnt++] = temp.prev;
				temp = g->nodes[temp.prev];
			}
			cnt--;
			// 출력
			while(cnt > 0){
				fprintf(fout, "%d->", path[cnt]);
				cnt--;
			}
			fprintf(fout, "%d (cost : %d)\n", path[0], g->nodes[j].dist);
			free(path); 
		}
	}
	// 힙 메모리 해제
	free(heap->Element);
	free(heap);
}

// 힙을 생성하는 함수
Heap* createMinHeap(int heapSize){
	Heap *heap = (Heap*)malloc(sizeof(Heap));
	heap->Capacity = heapSize;
	heap->Size = 1;
	heap->Element = (Node*)malloc(sizeof(Node)*(heapSize));
	heap->Element[0].dist = -INF;
	return heap;
}

// 힙을 제거하는 함수
void deleteMinHeap(Heap* minHeap){
	free(minHeap->Element);
	free(minHeap);
}

// 힙에 노드를 삽입하는 함수
// project7 이용
void insertToMinHeap(Heap* minHeap, int vertex, int distance){	
	// percolate up
	int idx = minHeap->Size;
	while(idx > 0) {
		// swap이 필요없는 경우, 반복문 탈출
		if(minHeap->Element[idx/2].dist <= distance) {
			break;
		}
		// swap이 필요한 경우 parent node와 swap
		else {
			minHeap->Element[idx] = minHeap->Element[idx/2];
			idx = idx/2;
		}
	}
	// 최종 위치에 insert 후, size++
	minHeap->Element[idx].vertex = vertex;
	minHeap->Element[idx].dist = distance;
	minHeap->Size++;
}

// 힙에 최상위 노드를 제거하는 함수
// project7 이용
Node deleteMin(Heap* minHeap){
	Node minNode = minHeap->Element[1];
	Node temp = minHeap->Element[minHeap->Size--];
	int idx = 1;

	// percolate down
	while(1) {
		// child node가 둘 다 있을 경우
		if(2*idx+1 < minHeap->Size) {
			// left child가 제일 작을 경우, swap 필요
			if(minHeap->Element[2*idx].dist < minHeap->Element[2*idx + 1].dist && minHeap->Element[2*idx].dist < temp.dist) {
				minHeap->Element[idx] = minHeap->Element[2*idx];
				idx = 2*idx;
			}
			// right child가 제일 작을 경우, swap 필요
			else if(minHeap->Element[2*idx + 1].dist < minHeap->Element[2*idx].dist && minHeap->Element[2*idx + 1].dist < temp.dist) {
				minHeap->Element[idx] = minHeap->Element[2*idx + 1];
				idx = 2*idx + 1;
			}
			// temp가 제일 작을 경우
			else {
				minHeap->Element[idx] = temp;
				break;
			}
		}
		// child node가 왼쪽만 있을 경우
		else if(2*idx == minHeap->Size) {
			// left child가 제일 작을 경우, swap 필요   
			if(minHeap->Element[2*idx].dist < temp.dist) {
				minHeap->Element[idx] = minHeap->Element[2*idx];
				idx = 2*idx;
			}
			else {
				minHeap->Element[idx] = temp;
				break;
			}
		}
		// child node가 없을 경우 
		else {
			minHeap->Element[idx] = temp;
			break;
		}
	}
	return minNode;
}