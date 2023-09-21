//
//  lab08.c
//  dstructure
//
//  Created by 이승수 on 2022/04/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet{
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);




int main(int argc, char*argv[]){
    srand((unsigned int)time(NULL));
    
    int num;
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    
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
    
    sets->size_maze = num*num;
    sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze+1));
    for (i = 1; i <= sets->size_maze; i++) sets->ptr_arr[i] = i;
    
    maze_print->size_maze = num*num*2;
    maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze+1));
    for (i = 1; i <= maze_print->size_maze; i++) maze_print->ptr_arr[i] = 1;
    maze_print->ptr_arr[maze_print->size_maze-1] = 0;
}

int find(DisjointSets *sets, int i) {
    if (sets == NULL)           //set 없을 때
        return 0;
    
    while(sets->ptr_arr[sets->ptr_arr[i]] != sets->ptr_arr[i]){
        sets->ptr_arr[i] = find(sets, sets->ptr_arr[i]);
    }
    return sets->ptr_arr[i];
}

void Union(DisjointSets *sets, int i, int j) {
    if (sets->ptr_arr == NULL) //union을 사용할 때 j에 들어오는 set이 더 큰 것을 넣는 것으로해서 union은 한가지 경우로 고정
        return;
    sets->ptr_arr[i] = j;
}

void freeMaze(DisjointSets *sets, DisjointSets *maze_print)
{
    free(sets->ptr_arr);
    free(sets);
    free(maze_print->ptr_arr);
    free(maze_print);
}

void printMaze(DisjointSets *maze_print, int num) {
    int row = 2;
    int column = 1;
    
    for (int i = 1; i <= num; i++)
        fprintf(fout, "+---"); //horizontal
    
    fprintf(fout, "+\n "); //오른쪽 끝
    
    for (int k = 1; k <= num; k++) { //column
        for (int i = 1; i <= num; i++) {
            fprintf(fout, "   ");
            
            if (maze_print->ptr_arr[column] == 1) //
                fprintf(fout, "|");
            else
                fprintf(fout, " ");
            column = column + 2;
        }
        
        fprintf(fout, "\n");
        
        for (int i = 1; i <= num; i++) {
            fprintf(fout, "+");
            if (maze_print->ptr_arr[row] == 1)
                fprintf(fout, "---");
            else
                fprintf(fout, "   ");
            
            row = row + 2;
        }
        
        fprintf(fout, "+\n");
        
        if (k != num)
            fprintf(fout, "|");
    }
}


void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num) {
    int* rank = (int*)malloc(sizeof(int) * (sets->size_maze+1));
    for (int i = 1; i <= num; i++) rank[i] = 0;
    
    int Wall;
    int rt1, rt2;
    int Frame1, Frame2;
    while (find(sets, 1) != find(sets, sets->size_maze)) {
        Wall = rand()%(maze_print->size_maze) + 1; //random하게 생성
        if (maze_print->ptr_arr[Wall] == 0)
            continue;
        //메이즈의 틀에 해당하는 부분들은 어떤 경우에도 있어야 하기 때문에
        //그것을 반영해서 메이즈 함수를 만들어야한다
        if (Wall % 2 != 1) { // horizontal edge
            Frame1 = Wall / 2;
            Frame2 = Frame1 + num;
            if (Frame2 > sets->size_maze)
                continue;
        }
        
        else {            //vertical edge
                Frame1 = (Wall+1) / 2;
                Frame2 = Frame1 + 1;
                if (Frame1 % num == 0)
                    continue;
            }
        rt1 = find(sets, Frame1);
        rt2 = find(sets, Frame2);
        
        //일반적 union함수, 실질적으로 미로를 만드는 union
        if (rt1 == rt2) //cycle이 생기는 경우를 없애야함 cycle은 루트가 같은 것들의 union이 일어날 시 발생
            continue;
        maze_print->ptr_arr[Wall] = 0;
        if (rank[rt1] < rank[rt2]) {
            Union(sets, rt1, rt2);
        }
        else {
            if (rank[rt1] == rank[rt2]) {
                rank[rt1]++;
            }
            Union(sets, rt2, rt1);
        }
    }
}


