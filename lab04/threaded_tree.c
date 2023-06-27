#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread
}ThreadedTree;

ThreadedPtr CreateTree(){
	ThreadedPtr tree = NULL;
	tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if(tree==NULL){
		fprintf(fout, "Out of Space!\n");
		return NULL;
	}
	tree->data = -1;
	tree->left_thread = 1;
	tree->left_child = tree;
	tree->right_thread = 1;
	tree->right_child = tree;
	return tree;
}

// 새로운 노드를 삽입하는 함수
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){

  // 삽입할 노드 생성
  ThreadedPtr newNode = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if(newNode == NULL){
		fprintf(fout, "Out of Space!\n");
  // 오류 발생 시 리턴 
		return 0;
	}

  // newNode data 입력 후 초기화
	newNode->data = data;
	newNode->left_thread = 1;
	newNode->left_child = root;
	newNode->right_thread = 1;
	newNode->right_child = root;

  // depth가 1일 때
  if(root -> right_thread == 1){
    root -> right_thread = 0;
    root -> right_child = newNode;
  }
  else{
    ThreadedPtr prevNode = root;
    ThreadedPtr currentNode = root -> right_child;

    int dep = 1;
    int temp = idx;
    // newNode가 최후에 왼쪽으로 추가되는지 오른쪽으로 추가되는지 판단하는 변수 
    int flag;

    // newNode가 어느 depth에 추가될지 판단하는 코드
    while(temp / 2 != 1 && temp / 2 != 0){
      temp = temp / 2;
      dep++;
    }

    // 이진탐색으로 newNode의 위치를 찾아가는 코드
    while(dep != 0){
      int div = 1 << dep;
      if((idx % div) < (div / 2)){
        //left
        prevNode = currentNode;
        currentNode = currentNode -> left_child;
        flag = 0;
      }
      else{
        //right
        prevNode = currentNode;
        currentNode = currentNode -> right_child;
        flag = 1;
      }
      dep--;
    }

    if(flag ==0){
      // left, newNode의 left_child엔 부모 node의 left_child / newNode의 right_child엔 부모 node를 넣어줌
      newNode -> left_child = prevNode -> left_child;
      newNode -> right_child = prevNode;
      prevNode -> left_thread = 0;
      prevNode -> left_child = newNode; 
    }
    else if (flag == 1){
      // right, newNode의 left_child엔 부모 node / newNode의 right_child엔 부모 node의 right_child를 넣어줌
      newNode -> left_child = prevNode;
      newNode -> right_child = prevNode -> right_child;
      prevNode -> right_thread = 0;
      prevNode -> right_child = newNode;
    }
  }
  return 1;
}

// 노드를 inorder traversal로 출력하는 함수
void printInorder(ThreadedPtr root){
  fprintf(fout, "inorder traversal : ");
  ThreadedPtr currentNode = root->right_child;
  while (currentNode != root) {
    // 가장 왼쪽에 있는 노드로 이동
    while (currentNode->left_thread == 0) {
      currentNode = currentNode->left_child;
    }
    fprintf(fout, "%d ", currentNode->data);

    // 중위 순회를 돌기 위한 조건
    while (currentNode->right_thread == 1 && currentNode->right_child != root) {
      currentNode = currentNode->right_child;
      fprintf(fout, "%d ", currentNode->data);
    }
    currentNode = currentNode->right_child;
  }
}

// 동적할당한 메모리를 해제하는 함수
void DeleteTree(ThreadedPtr root){
  if (root == NULL) return;
  else{
    if (root -> right_thread == 0){
      DeleteTree(root->right_child);
    }
    if (root -> left_thread == 0){
      DeleteTree(root->left_child);
    }
  }
  // 자식 노드들이 전부 해제되면 루트 노드 메모리 해제
  free(root);
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		
		if(Insert(root, root_idx, data, idx) == 0){
			return 0;
		}
	}

	printInorder(root);
	DeleteTree(root);
	
	fclose(fin);
	fclose(fout);

	return 0;
}
