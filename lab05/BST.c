#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

// printInorder함수에서 last node 출력 후 공백을 없애기 위한 변수
int lastNode = -2147483647;

// BST에 value가 key인 node를 넣는 함수
Tree insertNode(Tree root, int key){
	// root가 NULL이면 새로운 노드 동적할당
	if(root == NULL){
		root = (Tree)malloc(sizeof(BST));
		root -> value = key;
		root -> left = NULL;
		root -> right = NULL;
		fprintf(fout, "insert %d\n", key);

		// tree의 마지막 node 최신화
		if (key > lastNode) lastNode = key;
	}
	// root의 key와 비교하여 한 단계씩 내려감
	else if(root -> value > key) root -> left = insertNode(root -> left, key);
	else if(root -> value < key) root -> right = insertNode(root -> right, key);

	// 이미 key가 tree에 존재하는 경우 에러 메세지 출력
	else fprintf(fout, "insertion error: %d is already in the tree\n", key);

	return root;
}

// BST에서 value가 key인 node를 제거하는 함수
Tree deleteNode(Tree root, int key){
	// root가 NULL이면 leaf node까지 탐색했는데 key가 tree에 없다는 의미. 에러 메세지 출력
	if(root == NULL){
		fprintf(fout, "deletion error: %d is not in the tree\n", key);
		return root;
	}
	// root의 key와 비교하여 한 단계씩 내려감
	if(root -> value > key) root -> left = deleteNode(root -> left, key);
	else if(root -> value< key) root -> right = deleteNode(root -> right, key);

	// key를 발견했을 경우 child node 유무를 판단
	else{
		// child node가 오른쪽에만 있거나 둘 다 없는 경우
		if(root -> left == NULL){
			Tree temp = root -> right;
			free(root);
			return temp;
		}
		// child node가 왼쪽에만 있는 경우
		else if(root -> right == NULL){
			Tree temp = root -> left;
			free(root);
			return temp;
		}
		// 둘 다 있는 경우, right child를 parent로 하는 node중 제일 왼쪽 node가 delete된 node 자리에 옴(중복)
		// 이후 중복된 node를 지우기 위해 subtree에서 deleteNode 수행
		else{
			Tree temp = root -> right;
			while(temp -> left != NULL) temp = temp -> left;
			root -> value = temp -> value;
			root -> right = deleteNode(root -> right, temp -> value);
		}
	}

	// delete하려는 value가 tree의 마지막 node인 경우, max값 수정
	if(lastNode == key){
		Tree temp = root -> right;
		if (temp == NULL) lastNode = root -> value;
		else{
			while(temp -> left != NULL || temp -> right != NULL){
			if(temp -> right == NULL) temp = temp -> left;
			else temp = temp -> right;
		}
			lastNode = temp -> value;
		}
	}
	return root;
}

// BST에서 value가 key인 node가 존재하는지 확인하는 함수
int findNode(Tree root, int key){
	// key가 없는 경우
	if(root == NULL) return 0;
	// key가 존재하는 경우
	if(root-> value == key) return 1;
	// root의 key와 비교하여 한 단계씩 내려감
	if(root -> value > key) return findNode(root->left, key);
		
	else return findNode(root->right, key);
}

// tree에 있는 node value를 inorder traversal 방식으로 출력하는 함수
void printInorder(Tree root){
	if(root == NULL) return;
	// left child - current node - right child 순으로 순회하며 출력
	printInorder(root -> left);
	// 마지막 node 출력 후 공백을 없애기 위한 코드
	if (root -> value == lastNode) fprintf(fout, "%d", root -> value);
	else fprintf(fout, "%d ", root -> value);

  printInorder(root -> right);
}

// tree 전체를 제거하는 함수
void deleteTree(Tree root){
	if(root == NULL) return;
	deleteTree(root -> left);
	deleteTree(root -> right);
	free(root);
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		if(feof(fin))
			break;
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}