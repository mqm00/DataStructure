//
//  lab13.c
//  DataStructure
//
//  Created by 이승수 on 2022/05/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue{
    int* key;
    int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph{
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
int isEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
void sort(int* arr, int n);
int check_degree(Graph* G, int* arr);

int main(int argc, char * argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    
    char numbers[100], inputs[150];
    fgets(numbers, 100, fin);
    int n = 0;
    countInput(&n, numbers);
    int* nodes = (int*)malloc(sizeof(int)*n);
    parseInput(nodes, numbers, n);
    
    Graph* G = CreateGraph(nodes, n);
    
    fgets(inputs, 100, fin);
    int len = strlen(inputs);
    int i, j;
    for(i=0; i<len; i+=4){
        int a = inputs[i] - '0', b = inputs[i+2] - '0';
        InsertEdge(G, a, b);
    }
    
    PrintGraph(G);
    Topsort(G);
    DeleteGraph(G);
    
    fclose(fin);
    fclose(fout);
    
    return 0;
}

void countInput(int* n, char* str){
    int len = strlen(str), i;
    for(i=0; i<len; i++)
        if(0 <= str[i] - '0' && str[i] - '0' < 10)
            (*n)++;
}

void parseInput(int* arr, char* str, int n){
    int len = strlen(str), i;
    int cnt = 0;
    for(i=0; i<len; i++){
        if(0 <= str[i] - '0' && str[i] - '0' < 10)
            arr[cnt++] = str[i] - '0';
    }
}

Graph* CreateGraph(int* nodes, int n){
    Graph* G = (Graph*)malloc(sizeof(struct _Graph));
    G->node = (int*)malloc(sizeof(int)*n);
    G->size = n;
    for(int i=0; i<n; i++){
        G->node[i] = nodes[i];
    }
    sort(G->node, G->size); //그래프에 입력들 정렬
    
    G->matrix = (int**)malloc(sizeof(int*)*(n+1)); //이중포인터로 만드는 2차원 배열이므로 할당을 각각 해주어야한다
    for(int i=0; i<n+1; i++){
        G->matrix[i] = (int*)malloc(sizeof(int)*(n+1));
    }
    for(int i=0; i<n+1; i++){ //전부 0으로 만들어준다
        for(int k=0; k<n+2; k++){
            G->matrix[i][k] = 0;
        }
    }
    for(int i=1; i<n+1; i++){ //그리고 각각 첫번 째 행과 첫번 째 열에는 입력된 인자들을 놓는다.
            G->matrix[i][0]=G->node[i-1];
            G->matrix[0][i]=G->node[i-1];
    }
    return G;
}

void InsertEdge(Graph* G, int a, int b){ //edge를 추가하는 함수
    int row = 0;
    int col = 0;
    for(int i=1; i< G->size+1; i++) //a가 outgoing이고 b가 ingoing인 edge이다.
        if(G->matrix[i][0] == a) //먼저 a가 나타내는 인자를 2차원 배열에서 찾고 그 인덱스를 저장한다.
            row = i;
    for(int i=1; i<G->size+1; i++) //b가 나타내는 인자를 2차원 배열에서 찾고 그 인덱스를 저장한다.
        if(G->matrix[0][i] == b)
            col = i;
    G->matrix[row][col]++; //그리고 각각의 인덱스를 가지고 edge로 표현할 2차원 배열의 index를 특정한 다음, 1을 올려 edge가 있음을 표현한다.
}

void PrintGraph(Graph* G){ //2차원 배열을 프린트하면 된다.
    fprintf(fout, "   ");
    for(int i=1; i<G->size+1; i++){
        fprintf(fout, "%d  ", G->matrix[0][i]);
    }
    fprintf(fout, "\n");
    for(int i=1; i<G->size+1; i++){
        for(int k=0; k<G->size+1; k++){
            fprintf(fout, "%d  ", G->matrix[i][k]);
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}

void DeleteGraph(Graph* G){ //2차원 배열을 이중포인터로 동적할당 했기 때문에 free할 때 각각 해주어야한다.
    free(G->node);
    for(int i=0; i<G->size; i++){
        free(G->matrix[i]);
    }
    free(G->matrix);
}

void Topsort(Graph* G){ //위상정렬
    Queue* Q = MakeNewQueue(G->size); //정렬을 나타낼 큐를 만든다.
    int outputArr[Q->max_queue_size]; //output될 노드를 담는 배열
    for(int k=0; k<Q->max_queue_size; k++){ //max_queue_size는 graph에 입력된 인자의 갯수와 같기 때문에 그만큼 반복하여 정렬을 시행해준다.
        int in_degrees[G->size]; //각각 노드의 indegree를 저장할 배열
        int checking = check_degree(G, in_degrees); //indgree가 0인 것들을 찾아 배열에 그 인덱스를 넣어주는 함수이다.
        for(int i=0; i<checking; i++){ //그리고 그 배열에 넣어진 갯수를 checking에 넣어주고 넣어진 만큼 큐에 넣어준다.
            Enqueue(Q, in_degrees[i]);
        }
        
        int outNode = Dequeue(Q); //가장 먼저 들어간 인자부터 해서 dequeue를 해준다.
        if(outNode == -1){
            fprintf(fout, "sorting error : cycle!\n");
            return;
        }
        for(int i=1; i<G->size+1; i++){ //그리고 dequeue를 했다는 건 그래프에서 그 노드가 없어진 것이기 때문에 그 노드에서 나오는 edge를 다 없애주어야한다.
            if(G->matrix[i][0] == outNode){
                for(int k=1; k<G->size+1; k++){
                    G->matrix[i][k] = 0;
                }
                break;
            }
        }
        outputArr[k] = outNode;
//        printf("%d ", outNode);
    }
    for(int i=0; i<Q->max_queue_size; i++)
        fprintf(fout, "%d ", outputArr[i]);
    DeleteQueue(Q);
}

Queue* MakeNewQueue(int X){
    Queue* Q = (Queue*)malloc(sizeof(struct _Queue)); //큐 할당
    Q->key = (int*)malloc(sizeof(int)*X);
    Q->first = 0;
    Q->rear = -1;
    Q->max_queue_size = X;
    Q->qsize = 0;
    return Q;
}


int isEmpty(Queue* Q){
    if(Q->first > Q->rear && Q->rear >= 0)
        return 1;
    else if(Q->qsize == 0)
        return 1;
    else
        return 0;
}

int Dequeue(Queue* Q){
    if(isEmpty(Q)){
//        printf("Queue is empty\n");
        return -1;
    }
    return Q->key[Q->first++];
}

void Enqueue(Queue* Q, int X){
    if(Q->rear == Q->max_queue_size){
        fprintf(fout, "Queue is full\n");
        exit(0);
    }
    
    for(int i=0; i<Q->qsize; i++){ //2차원 배열을 중복해서 사용하다보니 Topsort를 반복할 수록 queue에 같은 값이 점점 더 들어가게 되어서 같은 값이 들어가려할 때는 return으로 끝내주는 조건을 달았다.
        if(Q->key[i] == X)
            return;
    }
    Q->key[++Q->rear] = X;
    Q->qsize++;
}

void DeleteQueue(Queue* Q){
    free(Q->key);
    free(Q);
}

void sort(int* arr, int n){  //위상정렬 시 그리고 처음 노드를 입력 받을 시에 필요한 sort, bubble sort로 구현했다.
    for(int i=0; i<n; i++){ // 위상 정렬할 때 동시에 indegree가 0인 노드가 존재할 때 작은 값부터 나오는 것을 의도했다.
        for(int j=0; j<n-1; j++){
            if(arr[j]>arr[j+1]){
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

int check_degree(Graph* G, int* arr){  //indegree가 0임을 확인하는 함수
    int indegree = 0;
    for(int i=1; i<G->size+1; i++){ //행을 기준으로
        int checkNum = 0;
        for(int k=1; k<G->size+1; k++){ //열을 본다
            if(G->matrix[k][i] != 0){ //0이 아닌 값, 즉 edge가 있을 때 체킹넘버를 올려서 이를 나타낸다.
                checkNum++;
                break;
                
            }
        }
        if(checkNum == 0){ //체킹넘버가 0이라면 indegree가 0인 것이고 그것을 indegree 배열에 넣어준다.
            arr[indegree] = G->matrix[i][0];
            indegree++;
        }
    }
    sort(arr, indegree);
    return indegree;
}
