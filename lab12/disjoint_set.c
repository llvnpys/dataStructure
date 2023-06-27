#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *maze_print, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);

void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int main(int argc, char* agrv[]) {
    srand((unsigned int)time(NULL));

    int num, i;
    fin = fopen(agrv[1], "r");
    fout = fopen(agrv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num) {
	int i;

	sets->size_maze = num * num;
	sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze + 1));
	for (i = 1; i <= sets->size_maze; i++) sets->ptr_arr[i] = i;

	maze_print->size_maze = num * num * 2;
	maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze + 1));
	for (i = 1; i <= maze_print->size_maze; i++) maze_print->ptr_arr[i] = 1;
	//maze_print->ptr_arr[(x-1)*2+1 ~ x*2] : two directions(right, down) walls of the number x

	//start and end position have not to have the wall
	maze_print->ptr_arr[maze_print->size_maze - 1] = 0;
}

// i와 j의 root가 서로 다르면 하나의 set으로 합쳐주는 함수
void Union(DisjointSets *sets, int i, int j){
    
    int root_i = find(sets, i);
    int root_j = find(sets, j);

    if(root_i != root_j){
        // 값이 더 작은 것을 root로 유지
        if(root_i < root_j){
            sets->ptr_arr[root_j] = root_i;
        }
        else{
            sets->ptr_arr[root_i] = root_j;
        }
    }
}

// root를 찾아서 반환하는 함수
int find(DisjointSets *sets, int i){
    // root node는 parent pointer로 자기 자신 값을 가짐
    // root가 아닌 경우, parent node에 대한 find 수행
    if(sets->ptr_arr[i] != i){
        sets->ptr_arr[i] = find(sets, sets->ptr_arr[i]);
    }
    return sets->ptr_arr[i];
}

void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num){
    // cell : 벽을 뚫을 위치, direction : 위치에서 벽울 뚫을 방향
    int cell, direction;

    // start point와 end point가 같은 set이 될 때까지 반복, cycle이 생기면 안 됨
    while(find(sets, 1) != find(sets, num*num)){
        cell = rand() % (num * num) + 1;
        direction = rand() % 2; // 0 : right, 1 : down
        
        // 뚫으면 안 되는 벽
        // Cell % num == 0 && direction == 0 : 맨 우측 벽
        // cell > (num*num - num) && direction == 1 : 맨 아래 벽

        // direction : 0 (맨 우측 벽 제외)
        if(direction == 0 && cell % num != 0){
            // cycle이 생기지 않는지 확인 (root가 같은지)
            if(find(sets, cell+1) != find(sets, cell)){
                Union(sets, cell, cell+1);
                maze_print->ptr_arr[(cell-1)*2+1] = 0;
            }

        } 
        // direction : 1 (맨 하단 벽 제외)
        else if(direction == 1 && cell <= (num*num - num)){
            // cycle이 생기지 않는지 확인 (root가 같은지)
            if(find(sets, cell+num) != find(sets, cell)){
                Union(sets, cell, cell+num);
                maze_print->ptr_arr[cell*2] = 0;
            }
        }
    }
}

// 미로를 print하는 함수
void printMaze(DisjointSets *maze_print, int num){
    int i, j;

    // 상단 벽 출력
    for(i = 0; i < num; i++) {
        fprintf(fout, "+---");
    }
    fprintf(fout, "+\n");

    // 미로 내용 출력
    for(i = 0; i < num; i++){

        // start point
        if(i == 0){
            fprintf(fout, " ");
        }
        else{
            fprintf(fout, "|");
        }

        // 세로 벽
        for(j = 1; j < num*2; j = j+2){
            if(maze_print->ptr_arr[2*num*i + j] == 0){
                fprintf(fout, "    ");
            }
            else{
                fprintf(fout, "   |");
            }
        }
        fprintf(fout, "\n");
        fprintf(fout, "+");

        // 가로 벽
        for(j = 2; j < num*2 + 1; j = j+2){
            if(maze_print->ptr_arr[2*num*i + j] ==0){
                fprintf(fout, "   +");
            }
            else{
                fprintf(fout, "---+");
            }
        }
        fprintf(fout, "\n");
    } 
}

// 메모리 해제
void freeMaze(DisjointSets *sets, DisjointSets *maze_print){
    free(sets->ptr_arr);
    free(maze_print->ptr_arr);
    free(sets);
    free(maze_print);
}
