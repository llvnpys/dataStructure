#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);
void Insert(BNodePtr *root, int key);
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int pos);
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos);
BNodePtr Createnode(int order);
int Find(BNodePtr root, int key);
void PrintTree(BNodePtr root);
void DeleteTree(BNodePtr root);
void free_node(BNodePtr node);



int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        if(feof(fin))
			break;
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 1)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }

    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/* 새로운 트리를 생성하는 함수 */
BNodePtr CreateTree(int order){
    BNodePtr root = (BNodePtr)malloc(sizeof(BNode));
    root->order = order;
    root->size = 1;
    root->child = (BNodePtr *)malloc(sizeof(BNodePtr) * (order + 1));
    for(int i = 0; i < order; i++){
        root->child[i] = NULL;
    }
    root->key = (int *)malloc(sizeof(int)*(order));
    root->is_leaf = 1;
    return root;
}

/* insert key */
void Insert(BNodePtr *root, int key){ // 루트를 함수 내에서 변경시키기 위해 이중 포인터로 선언
    *root = Insert_sub(*root, *root, key, 0); // 리턴값이 있는 재귀함수 활용, 처음에 root 노드는 현재 노드이자 parent이므로 같이 parent, node 똑같이 매개변수 전달
}

// 서브트리에서 노드를 삽입하는 역할
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int p_pos){
    int pos;

    // node를 삽입할 position을 찾는 과정
    for(pos = 0; pos < node->size - 1; pos++){
        if(node->key[pos] > key) break;
    }

    // 현재 node가 leaf가 아닌 경우
    if(!node->is_leaf){
        // subtree에 삽입
        node = Insert_sub(node, node->child[pos], key, pos);
        // node가 꽉 찬 경우, node split
        if(node->size > node->order){
            parent = split_node(parent, node, p_pos);
        }
    }
    // 현재 node가 leaf인 경우
    else{
        // 삽입을 위해 pos부터 key를 한 칸씩 밈
        for(int i = node->size - 1; i > pos; i--){
            node->key[i] = node->key[i-1];
        }
        // key값 삽입, size증가
        node->key[pos] = key;
        node->size++;
        // node가 꽉 찬 경우, node split
        if(node->size > node->order){
            parent = split_node(parent, node, p_pos);
        }
    }
    return parent;
}

// 노드가 가득차있다면, 노드를 두 개의 노드로 분리하는 역할
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos){
    int i;
    int mid = (node->order - 1) / 2;
    BNodePtr rightChild = Createnode(node->order);
    
    // Createnode에서 초기화 하지 않은 값 설정
    rightChild->is_leaf = node->is_leaf;
    rightChild->size = node->size - mid - 1;
    // node size 수정 이후 left child가 됨
    node->size = node->size - rightChild->size;

    // right child로 key 이동
    for(i = 0; i < rightChild->size - 1; i++){
        rightChild->key[i] = node->key[i + mid + 1];
    }

    // leaf node가 아닌 경우, child update
    if(!node->is_leaf){
        for(i = 0; i < rightChild->size; i++){
            rightChild->child[i] = node->child[i + mid + 1];
        }
    }

    // 중앙값 올리기
    // parent != node인 경우
    if(parent != node){
        // parent node에 node의 mid값을 삽입하기 위해 key, child 한 칸씩 밀어줌
        for(i = parent->size - 1; i > pos; i--){
            parent->key[i] = parent->key[i-1];
            parent->child[i+1] = parent->child[i];
        }
        // mid값 삽입
        parent->key[pos] = node->key[mid];
        // parent child 연결
        parent->child[pos] = node;
        parent->child[pos+1] = rightChild;
        parent->size++;
        return parent;
    }
    // parent == node인 경우
    else{
        // parent node가 될 새 node 생성
        BNodePtr newRoot = Createnode(node->order);
        newRoot->is_leaf = 0;
        // key는 mid값 1개이므로 size는 2
        newRoot->size = 2;
        newRoot->key[0] = node->key[mid];
        newRoot->child[0] = node;
        newRoot->child[1] = rightChild;
        return newRoot;
    }
}

/* 노드 하나를 동적할당하는 함수, size, is_leaf는 노드마다 다르므로 초기화하지 않고 공통되는 부분만 초기화 */
BNodePtr Createnode(int order){ 
    BNodePtr node = (BNodePtr)malloc(sizeof(BNode));
    node->order = order;
    node->child = (BNodePtr *)malloc(sizeof(BNodePtr) * (order + 1));
    for(int i=0; i<order; i++){
        node->child[i] = NULL;
    }
    node->key = (int *)malloc(sizeof(int)*(order));
    return node;
}

/* find node that has key */
int Find(BNodePtr root, int key){
    // node의 왼쪽부터 탐색
    int pos;
    for(pos = 0; pos < root->size - 1; pos++){
        if(root->key[pos] >= key) break;
    }

    // 탐색 종료 후 찾았으면 1, 찾지 못했고 현재가 leaf node면 0을 반환 / 둘 다 아니면 현재 탐색한 position의 child node 재탐색
    if(root->key[pos] == key) return 1;
    else if(root->is_leaf) return 0;
    else return Find(root->child[pos], key);
}

/* print Tree, inorder traversal */
void PrintTree(BNodePtr root){
    if (root == NULL) return;
    int i;
    // tree의 맨 좌측으로 이동, leaf node면 key 출력
    for (i = 0; i < root->size - 1; i++) {
        if (!root->is_leaf) PrintTree(root->child[i]);
        fprintf(fout, "%d ", root->key[i]);
    }

    // subtree의 most left node를 제외한 나머지 node 차례로 방문
    if (!root->is_leaf)
        PrintTree(root->child[i]);
}

/* depth first search -> free memory */
void DeleteTree(BNodePtr root){
    // dual free를 방지하기 위한 code
    if (root == NULL) return;
    if (root->is_leaf) {
        free_node(root);
        return;
    }
    // child node를 순회하며 메모리 해제
    for (int i = 0; i < root->size; i++) {
        DeleteTree(root->child[i]);
    }
    free_node(root);
}

/* 노드의 메모리를 해제해주는 helper function */
void free_node(BNodePtr node){
    if (node == NULL) return;
    // 동적 할당한 메모리 전부 해제
    free(node->child);
    free(node->key);
    free(node);
}

