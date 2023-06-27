#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;


Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b,result, error_flag=0;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i]!='#'){
		// 숫자 입력 시
		if(input_str[i] >= '1' && input_str[i] <= '9'){
			// 스택이 full 상태면 error_flag 설정 후 break
			if(IsFull(stack)){
				error_flag = 1;
				break;
			}
			// 배열 자료형에 맞게 입력
			else{
				Push(stack, input_str[i] - '0');
			}
    }
		// 연산자 입력 시
    else{
			// 숫자 2개가 스택에 있는지 확인, 없으면 error_flag 설정 후 break
			if(IsEmpty(stack)){
				error_flag = 2;
				break;
			}
			else{
				b = Pop(stack);
				if(IsEmpty(stack)){
					error_flag = 2;
				}
				else{
					a = Pop(stack);
					// 연산자에 맞는 연산을 위해 switch-case 이용
					switch(input_str[i]){
						case '+' : 
							a += b;
							Push(stack, a);
							break;
						case '-' :
							a -= b;
							Push(stack, a);
							break;
						case '*' :
							a *= b;
							Push(stack, a);
							break;
						case '/' :
							// Divide by zero error를 위한 code 
							if(b == 0){
								error_flag = 3;
							}
							else{
								a /= b;
							Push(stack, a);
							}
							break;
						case '%' :
							a %= b;
							Push(stack, a);
							break;
							// 이외의 입력에 대한 오류 메세지 명세가 없으므로 따로 설정하지 않음
						default : break;
      		}
				}  
			}
  	}
		if(error_flag) break;

		int t = Top(stack);
		fprintf(fout, "%d ",t);
		result=t;
		i++;
	}

	if(error_flag == ISFULL){
		fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
	}
	else if(error_flag == ISEMPTY){
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
	}
	else if(error_flag == DIVIDEZERO){
		fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
	}

	else{
		if(stack->top+1 > 1){
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
		}
		else{
			fprintf(fout, "\nevaluation result : %d\n",result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

// 스택 생성 메모리 할당 및 max-size, top index 설정
Stack* CreateStack(int max){
    Stack* S = (Stack*)malloc(sizeof(struct Stack));
    S->key = (int*)malloc(sizeof(int)*max);
    S->max_stack_size = max;
    S->top = -1;
    return S;
}

// 스택 제거, 메모리 해제
void DeleteStack(Stack* S){
    free(S->key);
    free(S);
}

// 스택에 값 삽입, top index ++ 후 X 삽입
void Push(Stack* S, int X){
  S->key[++S->top] = X;
}

// 스택의 맨 위 값 제거, X 스택에서 제거 후 top index --
int Pop(Stack* S){
  return S->key[S->top--];
}

// 스택의 맨 위 값 Return
int Top(Stack* S){
  return S->key[S->top];
}

// top index가 19를 넘어가면 True
int IsFull(Stack* S){
  return S->top == S->max_stack_size -1;
}

// top index가 -1이면 True
int IsEmpty(Stack* S){
  return S->top == -1;
}
