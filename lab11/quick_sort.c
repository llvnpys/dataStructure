#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

// 배열을 생성하는 함수 
Array* CreateArray(int size){
    Array *array = (Array *)malloc(sizeof(Array));
    array->size = size;
    array->values = (int *)malloc(sizeof(int) * size);
    return array;
}

// quick sort를 실행하는 함수
void QuickSort(Array* array, int left, int right, int depth){
    // 정렬할 것이 없으면 바로 반환
    if(left >= right) return;
    // Pivot 위치 찾은 후 pivot 양 옆 분할해서 정렬
    int pivotIndex = Partition(array, left, right, depth);
    QuickSort(array, left, pivotIndex - 1, depth + 1);
    QuickSort(array, pivotIndex + 1, right, depth + 1);  
}

// Set the pivot and obtain the proper index of the pivot through the swapping.
int Partition(Array* array, int left, int right, int depth){
    
    int pivotIndex = right;
    int pivotValue = array->values[right];
    // i는 배열의 맨 왼쪽, j는 배열의 맨 오른쪽 index로 선언
    int i = left;
    int j = right;
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivotValue);
    // 정렬 전 현재 상태 출력 
    PrintArray(array, left, right);

    // pivot의 위치를 찾을 때까지 반복
    while(i < j){
        // i index의 value가 pivotValue보다 클 때(swap이 필요한 상황일 때)
        if(array->values[i] > pivotValue){
            // swap할 대상 탐색
            while(i < j && array->values[j] >= pivotValue){
                j--; 
            }
            // swap할 대상을 찾은 경우
            if(i < j){
                swap(&array->values[i], &array->values[j]);
                PrintArray(array, left, right);
            }
        }
        else{
            i++;
        }
    }
    // 탐색한 pivot의 index로 pivot을 이동
    if(array->values[j] > array->values[pivotIndex]){
        swap(&array->values[j], &array->values[pivotIndex]);
        PrintArray(array, left, right);
    }
    // pivotIndex 반환
    return j;
}

// Print all values of the array on the interval [left, right].
void PrintArray(Array* array, int left, int right){
        
    int i ;
    char hyphen = '-';
    // [left, right] 외의 공간엔 - 출력
    for(i = 0; i < left; i++){
        fprintf(fout, "%-3c", hyphen);
    }
    for(i = left; i <= right; i++){
        fprintf(fout, "%-3d", array->values[i]);
    };
    for(i = right; i < array->size - 1; i++){
        fprintf(fout, "%-3c", hyphen);
    }
    fprintf(fout, "\n");
}

// 배열을 제거하는 함수 
void DeleteArray(Array* array){
    free(array->values);
    free(array);
}

// swap 함수 
void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}