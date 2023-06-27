#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
void checkIndegree(Graph* G, int* idg);
void sort_node(int* arr, int n);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
	Topsort(G);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

// Graph 생성 함수
Graph* CreateGraph(int* nodes, int n){
	Graph* G = (Graph*)malloc(sizeof(Graph));
	// node 순서대로 정렬
  sort_node(nodes, n);
	G->size = n;
	G->node = nodes;
	G->matrix = (int**)malloc(sizeof(int*) * n);

	// matrix 동적할당 후 element 0으로 초기화
	for (int i = 0; i < n; i++) {
		G->matrix[i] = (int*)malloc(sizeof(int) * n);
		memset(G->matrix[i], 0, sizeof(int) * n);
	}
	return G;
}

// edge를 matrix에 적용하는 함수
void InsertEdge(Graph* G, int a, int b){
	int idx1, idx2;
	// 입력 받은 node의 index를 탐색
	for(int i = 0; i< G->size; i++){
		if(G->node[i] == a){
			idx1 = i;
		}
		else if(G->node[i] == b){
			idx2 = i;
		}
	}
	// matrix에 입력
	G->matrix[idx1][idx2] = 1;
}

// matrix를 출력하는 함수
void PrintGraph(Graph* G){
	fprintf(fout, "   ");
	// column element 출력
	for (int i = 0; i < G->size; i++) {
		fprintf(fout, "%d  ", G->node[i]);
	}
	fprintf(fout, "\n");
	
	// row element 및 matrix 출력
	for (int i = 0; i < G->size; i++) {
		fprintf(fout, "%d  ", G->node[i]);
		for (int j = 0; j < G->size; j++) {
			fprintf(fout, "%d  ", G->matrix[i][j]);
		} 
		fprintf(fout, "\n");
	}
}

// graph를 제거하고 할당된 메모리를 해제하는 함수
void DeleteGraph(Graph* G){
	// matrix의 row를 돌며 해제 
	for (int i = 0; i < G->size; i++) {
		free(G->matrix[i]);
	}
	free(G->matrix);
	free(G);
}

// graph의 node를 topological sorting 후 출력하는 함수
void Topsort(Graph* G){
	// topological sort를 위해 node의 indegree counting
	int* indegree = (int*)malloc(sizeof(int) * G->size);
	checkIndegree(G, indegree);

	// 결과 출력 및 cycle 판단을 위한 변수
	int* result = (int*)malloc(sizeof(int) * G->size);
	int idx = 0;

	// queue 생성 후, indegree가 0인 node부터 queue에 삽입
	Queue* Q = MakeNewQueue(G->size);
	for (int i = 0; i < G->size; i++) {
		if (indegree[i] == 0) {
			Enqueue(Q, i);
		}
	}
	fprintf(fout, "\n");

	// queue가 빌 때 까지 반복 
	while(!IsEmpty(Q)){
		// queue의 first node dequeue
		int node = Dequeue(Q);
		result[idx] = G->node[node];
		idx++;

    // 제거한 node를 기준으로 indegree 수정
		for(int i = 0; i< G->size; i++){
			if(G->matrix[node][i] == 1){
				G->matrix[node][i] = 0;
				indegree[i]--;
				// indegree 수정 후 0이면 enqueue
        if(indegree[i] == 0){
					Enqueue(Q, i);
				}
			}
		}
	}
	// result에 node가 모두 들어오지 않았다는 건 cycle이 존재한다는 의미
	if(idx < G->size){
		fprintf(fout, "sorting error : cycle!\n");
	}
	// cycle이 없을 경우 
	else{
		for(int i = 0; i < G->size; i++){
			fprintf(fout, "%d ", result[i]);
		}
		fprintf(fout, "\n");
	}

	DeleteQueue(Q);
	free(indegree);
	free(result);
}

// queue를 생성하고 메모리를 동적 할당해주는 함수
Queue* MakeNewQueue(int X){
	Queue* Q = (Queue*)malloc(sizeof(Queue));
	Q->key = (int*)malloc(sizeof(int) * X);
	Q->first = 0;
	Q->rear = 0;
	Q->qsize = 0;
	Q->max_queue_size = X;
	return Q;
}

// queue가 비어있는지 확인하는 함수
int IsEmpty(Queue* Q){
	if(Q->qsize) return 0;
	else return 1;
}

// queue의 first value를 queue에서 제거하는 함수
int Dequeue(Queue* Q){
	int value = Q->key[Q->first];
	Q->first = (Q->first + 1) % Q->max_queue_size;
	Q->qsize--;
	return value;
}

// queue의 rear에 value를 삽입하는 함수
void Enqueue(Queue* Q, int X){
	Q->key[Q->rear] = X;
	Q->rear = (Q->rear + 1) % Q->max_queue_size;
	Q->qsize++;
}

// queue를 제거하고 메모리를 해제해주는 함수
void DeleteQueue(Queue* Q){
	free(Q->key);
	free(Q);
}

// node의 indegree를 count하는 함수
void checkIndegree(Graph* G, int* idg){
	for(int i = 0; i < G->size; i++){
		idg[i] = 0;
		for(int j = 0; j< G->size; j++){
			if(G->matrix[j][i] == 1) idg[i]++;
		}
	}
}

// node를 정렬하기 위한 함수
void sort_node(int* arr, int n){
  for (int i = 0; i < n - 1; i++){
    for (int j = 0; j < n - i - 1; j++){
      if (arr[j] > arr[j + 1]){
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}