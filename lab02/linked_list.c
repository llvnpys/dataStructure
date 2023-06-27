#include <stdio.h> 
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct Node *PtrToNode; 
typedef PtrToNode List;
typedef PtrToNode Position; 
typedef int ElementType;

FILE *fin;
FILE *fout;

struct Node {
  ElementType element;
  Position next; 
};

// 빈 linked list를 생성하는 function
List MakeEmpty(List L){
  L = malloc(sizeof(struct Node));
  L -> next = NULL;
  // option "f"를 위해 header의 element값을 -1로 설정
  L -> element = -1;
  return L;
}

// linked list가 비어있는지 확인하는 function
// header의 next가 NULL이면 True / 아니면 False
int IsEmpty(List L){
  return (L -> next == NULL);
}

// 현재 node가 마지막인지 확인하는 function
// 현재 node의 next가 NULL이면 True / 아니면 False
int IsLast(Position P, List L){
  return (P -> next == NULL);
}

// P node 뒤에 X값을 가진 node를 삽입하는 function
void Insert(ElementType X, List L, Position P){
  // new node에 메모리 할당 후 값 삽입
  Position tmp;
  tmp = malloc(sizeof(struct Node));
  tmp -> element = X;
  // P node 뒤에 new node를 넣는 코드
  tmp -> next = P -> next;
  P -> next = tmp;
} 

// linked list의 element들을 하나씩 출력하는 function
void PrintList(List L){
  // 첫 번째 node 지정
  Position P = L -> next;
  if(IsEmpty(L)){
    fprintf(fout, "empty list!");
  }
  else{
    while(P != NULL){
    fprintf(fout, "key:%d ", P->element);
    // 다음 node로 이동
    P = P -> next;
    }
  }
  fprintf(fout, "\n");
}

// 이전 node를 찾는 function
Position FindPrevious (ElementType X, List L){
  Position P = L;
  // P가 last node이거나 P의 next noded의 element가 X면 break
  while(P -> next != NULL && P -> next -> element != X){
    P = P -> next;
  }
  return P;
}

// 현재 node를 찾는 function
Position Find(ElementType X, List L){
  Position P = L;
  // P가 last node이거나 P의 element가 X면 break
  while(P != NULL && P -> element != X){
    P = P -> next;
  }
  return P;
}

// element X를 포함한 node를 linked list에서 삭제하는 function
void Delete(ElementType X, List L){
  // 삭제할 node 탐색
  Position P = Find(X, L);
  // 없으면
  if (P==NULL){
    fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", X, X);
  }
  else{
    // 이전 node 탐색해서 P의 next node와 연결 후 P 메모리 해제
    Position tmp = FindPrevious(P -> element, L);
    tmp -> next = P-> next;
    free(P);
  }
}

// linked list의 모든 node를 삭제하는 function
void DeleteList(List L){
  Position tmp;
  // header부터 하나씩 할당 해제
  while(L != NULL){
    tmp = L -> next;
    free(L);
    L = tmp;
  }
}


int main(int argc, char **argv){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	char x;

	Position header=NULL, tmp=NULL;
	header = MakeEmpty(header);
	while(fscanf(fin,"%c",&x)!=EOF){
		if(x=='i'){
			int a,b;	fscanf(fin,"%d%d",&a,&b);
			tmp = Find(a, header);
			if(tmp!=NULL){
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if(tmp==NULL){
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
		}
		else if(x=='d'){
			int a;	fscanf(fin,"%d",&a);
			Delete(a, header);
		}
		else if(x=='f'){
			int a;	fscanf(fin,"%d",&a);
			tmp = FindPrevious(a, header);
			if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
			else{
				if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if(x=='p') PrintList(header);
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);

	return 0;
}