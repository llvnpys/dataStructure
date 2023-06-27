#include<stdio.h>
#include<stdlib.h>
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);

Position selectRotation(Position node);
int setHeight(Position node);

// AVL tree에 X값을 넣는 함수
AVLTree Insert(ElementType X, AVLTree T){
	// 넣을 위치에 올 때 
	if(T == NULL){
		T = (AVLTree)malloc(sizeof(AVLNode));
		T->element = X;
		T->left = NULL;
		T->right = NULL;
		T->height = 0;
	}
	// 현재 node보다 작을 때 
	else if(T->element > X){
		T->left = Insert(X, T->left);
		T->height = setHeight(T);
		T = selectRotation(T);	
	}
	// 현재 node보다 클 때
	else if(T->element < X){
		T->right = Insert(X, T->right);
		T->height = setHeight(T);
		T = selectRotation(T);
	}
	// 이미 X가 존재할 때
	else{
		fprintf(fout, "insertion error : %d is already in the tree!\n", X);
	}
	return T;
};

// AVL tree에서 X값을 제거하는 함수
AVLTree Delete(ElementType X, AVLTree T){
	// tree에 X가 없을 떄
	if(T==NULL){
		fprintf(fout, "deletion error : %d is not in the tree!\n", X);
		return T;
	}
	// 현재 node보다 작을 때 
	else if(T->element > X) T->left = Delete(X, T->left);
	// 현재 node보다 클 때
	else if(T->element < X) T->right = Delete(X, T->right);
	
	// 찾았을 때
	else{
		Position temp;
		// child node가 오른쪽에만 있거나 둘 다 없는 경우
		if(T->left ==NULL){
			temp = T->right;
			free(T);
			return temp;
		}
		// child node가 왼쪽에만 있는 경우
		else if(T->right ==NULL){
			temp = T->left;
			free(T);
			return temp;
		}
		// 둘 다 있는 경우,
		else{
			temp = T->right;
			while(temp->left != NULL) temp = temp->left;
			T->element = temp->element;
			T->right = Delete(temp->element, T->right);
		}
	} 
	// Tree 높이 설정 및 회전
	T->height = setHeight(T);
	T = selectRotation(T);
	return T;
	};

// Case : LL
// node를 root로 뒀을 때 root -> root right로 / root left -> root로 / root left right -> root right로 이동
Position SingleRotateWithLeft(Position node){
	Position temp = node -> left;
	node->left = temp->right;
	temp->right = node;
	node->height = setHeight(node);
  temp->height = setHeight(temp);
	return temp;
};

// Case : RR
// node를 root로 뒀을 때 root -> root left로 / root right -> root로 / root right left -> root right로 이동
Position SingleRotateWithRight(Position node){
	Position temp = node -> right;
	node->right = temp->left;
	temp->left = node;
	node->height = setHeight(node);
  temp->height = setHeight(temp);
	return temp;
};

// Case : LR
// node를 root로 뒀을 때 root left에 대해 SingleRotateWithRight 진행, 이후 root에서 SingleRotateWithLeft
Position DoubleRotateWithLeft(Position node){
	node -> left = SingleRotateWithRight(node->left);
	return SingleRotateWithLeft(node);
};

// Case : RL
// node를 root로 뒀을 때 root right에 대해 SingleRotateWithLeft 진행, 이후 root에서 SingleRotateWithRight
Position DoubleRotateWithRight(Position node){
	node->right =SingleRotateWithLeft(node->right);
	return SingleRotateWithRight(node);
};

// insert, delete, rotate 이후 달라진 높이를 재설정해주는 함수
int setHeight(Position node) {
	// child가 2개일 때, 더 깊은 쪽 선택
	if(node->left != NULL && node->right != NULL){
		return max(node->left->height, node->right->height) + 1;
	}
	// left만 있을 때
	else if(node->left != NULL){
		return node->left->height + 1;
	}
	// right만 있을 때
	else if(node->right != NULL){
		return node->right->height + 1;
	}
	// 둘 다 없으면 leaf node
	else{
		return 0;
	}
};

// 어떤 Rotation 함수를 실행할지 선택해주는 함수
Position selectRotation(Position node){
	// child가 없을 때
	if(node->left == NULL && node->right == NULL) return node;
	// right child가 없을 때, left depth가 2 이상이면,
	else if(node->right == NULL){
		if(node->height >= 2) node = SingleRotateWithLeft(node);
	}
	// left child가 없을 때, right depth가 2 이상이면,
	else if(node->left == NULL){
		if(node->height >= 2) node = SingleRotateWithRight(node);
	}
	// 양쪽 child가 다 있을 때 
	else{
		// depth 차이가 2 이상이면 
		if(max(node->left->height, node->right->height)- min(node->left->height, node->right->height) >= 2){
			Position temp2;
			// // left가 더 깊을 때
			if(node->left->height > node->right->height){
				temp2 = node->left;
				// rotation case 판단
				// left subtree에서 right가 left보다 2이상 깊으면 double
				if((temp2->left == NULL && temp2->right->height == 1) || 
				(temp2->left != NULL && temp2->right->height - temp2->left->height == 2)) node = DoubleRotateWithLeft(node);
				// 아닐 경우 single
				else node = SingleRotateWithLeft(node);
			}
			// // right가 더 깊을 때 
			else{
				temp2 = node->right;
				// rotation case 판단
				if((temp2->right == NULL && temp2->left->height == 1) || 
				(temp2->right != NULL && temp2->left->height - temp2->right->height == 2)) node = DoubleRotateWithRight(node);
				else node = SingleRotateWithRight(node);
			}
		}
	}
	return node;
};

// Inorder순으로 출력해주는 함수
void PrintInorder(AVLTree T){
	if(T == NULL) return;
	PrintInorder(T -> left);
	fprintf(fout, "%d(%d) ", T -> element, T->height);
  PrintInorder(T -> right);
};

// Tree 전체를 제거해주는 함수
void DeleteTree(AVLTree T){
	if(T == NULL) return;
	DeleteTree(T -> left);
	DeleteTree(T -> right);
	free(T);
};

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}

