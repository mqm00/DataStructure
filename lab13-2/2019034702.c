//
//  lab13-2.c
//  DataStructure
//
//  Created by 이승수 on 2022/06/01.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node{
    int vertex;
    int dist;
    int prev;
}Node;

typedef struct Graph{
    int size;
    int** vertices;
    Node* nodes;
}Graph;

typedef struct Heap{
    int Capacity;
    int Size;
    Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);
void dijkstra(Graph* g, Node* SP);
int isEmpty(Heap* heap);

int main(int argc, char *argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    Graph* g;
    int size;
    fscanf(fin, "%d\n", &size);
    g = createGraph(size+1);
    
    char tmp = 0;
    while(tmp != '\n' && tmp != EOF){
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->vertices[node1][node2] = weight;
        tmp = fgetc(fin);
    }
    printShortestPath(g);
    deleteGraph(g);
    return 0;
}


Graph* createGraph(int size){
    Graph* g = (Graph*)malloc(sizeof(struct Graph)); //그래프 만들기
    g->nodes = (Node*)malloc(sizeof(struct Node)*size);
    g->size = size;
    g->vertices = (int**)malloc(sizeof(int*)*size); //2차원 배열은 동적할당 각각 해주어야함. free할 때 유의
    for(int i=0; i<size; i++){
        g->vertices[i] = (int*)malloc(sizeof(int)*size);
    }
    for(int i=0; i<size; i++){ //일단 distance를 모두 INF로 설정해준다.
        for(int k=0; k<size; k++){
            g->vertices[i][k] = INF;
        }
    }
    g->vertices[1][1] = 0; //1번은 start이므로 0
    for(int i=1; i<size; i++){ //main에서 size+1만큼 들어온다.
        g->nodes[i].vertex = i; //node도 마찬가지로 vertex는 각각 설정해주되 distance는 일단 iNF로 한다.
        g->nodes[i].dist = INF;
        g->nodes[i].prev = -1;
    }
    return g;
}

void printShortestPath(Graph* g){
    Node* SP = (Node*)malloc(sizeof(struct Node)*(g->size-1)); //Short path를 받아줄 node 배열 , 최종적으로 이곳에 저장할 것
    for(int i=0; i<(g->size-1); i++){ //일단 모두 INF로 초기화
        SP[i].dist = INF;
    }
    dijkstra(g, SP); //dijkstra 정렬
    for(int i=1; i<g->size-1; i++){
        if(SP[i].dist == INF){ //Inf면 연결되는 path가 없는 것
            fprintf(fout, "can not reach to node %d\n", SP[i].vertex);
            continue;
        }
        while(1){
            int order[100]; //path의 흔적을 남겨줄 배열을 만든다.
            int k = 0;
            Node tmp = SP[i];
            while(tmp.vertex != 1){ //prev 값을 이용해 path의 경로를 담아준다
                order[k] = tmp.vertex;
                tmp = g->nodes[tmp.prev];
                k++;
            }
            fprintf(fout, "1"); //출력 형식
            k--;
            for(;k>=0; k--){
                fprintf(fout, "->%d", order[k]);
            }
            fprintf(fout, " (cost : %d)\n", SP[i].dist);
            break;
        }
    }
}

Heap* createMinHeap(int heapSize){ //heap 생성
    Heap* heap = (Heap*)malloc(sizeof(struct Heap));
    heap->Element = (Node*)malloc(sizeof(struct Node)*(heapSize)); //heap의 node 배열
    heap->Capacity = heapSize; //heap의 크기
    heap->Size = heapSize; // heap의 현재크기, 나는 처음부터 모든 node를 heap에 넣어주고 시작할 것이기 때문에 heapsize로 설정했다.
    heap->Element[0].dist = INF; //0번째는 안쓴다.
    for(int i=1; i<heapSize; i++){
        heap->Element[i].vertex = i;
        heap->Element[i].dist = INF;
        heap->Element[i].prev = 1;
    }
    return heap;
}

void dijkstra(Graph* g, Node* SP){
    //weight에 대한 정보는 matrix에 있다.
    //heap 만든다.
    Heap* heap = createMinHeap(g->size);
    //시작값 뽑는다.
    insertToMinHeap(heap, 1, 0); //heap의 1번 index에 0을 넣어준다. 자기자신
    Node source = deleteMin(heap); //1번을 꺼내준다.
    SP[0] = source; //SP배열 첫번째에 start-start가 저장된다.
    int start = source.vertex; //첫시작 노드
    //source값에 인접한 노드들을 찾고, 그 노드들의 distance를 업데이트 해준다.
    for(int i=1; i<g->size; i++){
        if(g->vertices[start][i] != INF){
            if(start == 1 && i == 1) continue;//2차원 배열에 정보가 있으니까 2차원 배열로 판단. INF가 아니라면 edge가 있는 것
            insertToMinHeap(heap, i, g->vertices[start][i]);
            g->nodes[i].prev = 1;
        }
    }
    int cnt = 1;
    while(!isEmpty(heap)){
        Node u = deleteMin(heap);
        int present = u.vertex;
        SP[cnt++] = u; //Delete됐다는 건 가장 작은 Path로 설정되어 있다는 것이니까 SP에 넣는다.
        for(int i=1; i<g->size; i++){ //그리고 그 노드에서 최단거리인 것들을 찾는다.
            if(g->vertices[present][i] != INF){ //edge가 있다면
                if(g->vertices[1][present] + g->vertices[present][i] < g->vertices[1][i]){ //1번에서 목적지까지 가는 것과 1번에서 현재 노드로 가고 그 노드에서 목적지로 가는 것의 길이를 비교한다.
                    int j;
                    for(j=0; j<g->size; j++){ //그리고 그렇게 path가 바뀐다면 그 목적지 노드의 Prev를 현재 노드로 바꿔준다.
                        if(heap->Element[j].vertex == i){
                            heap->Element[j].prev = present;
                            break;
                        }
                    }
                    insertToMinHeap(heap, i, g->vertices[1][present] + g->vertices[present][i]); //그리고 그 path를 heap에 넣어준다.
                    g->nodes[i].prev = present; //node에서의 prev도 바꿔준다.
                    g->vertices[1][i] = g->vertices[1][present] + g->vertices[present][i]; //2차원 배열에서 1번에서 목적지까지의 값을 update 해준다./
                }
            }
        }
    }
    deleteMin(heap); // free heap
}

void insertToMinHeap(Heap* minHeap, int vertex, int distance){ //insert함수를 heap에 있는 해당 vertex의 Distance를 들어온 distance로 바꿔주는 함수라고 해석했다.
    for(int k=0; k<minHeap->Size; k++){ //해당 vertex값을 갖는 heap의 node를 찾아준다.
        if(minHeap->Element[k].vertex == vertex){
            minHeap->Element[k].dist = distance; //그 노드의 distance를 바꿔준다.
            Node tmp = minHeap->Element[k];
            int i;
            for(i = k; minHeap->Element[i/2].dist > distance && i/2 != 0; i/=2){ //그리고 heap정렬을 해준다
                minHeap->Element[i] = minHeap->Element[i/2];
            }
            minHeap->Element[i] = tmp;
            break;
        }
    }
}

Node deleteMin(Heap* minHeap){
    //distance를 기준으로 pop
    int i, child;
    Node min_element = minHeap->Element[1]; //일반적인 MinHeap의 delete
    Node last_element = minHeap->Element[minHeap->Size-1];
    for(i=1; i*2<=minHeap->Size; i = child){
        child = i*2; //child 노드를 작은 값을 선택하여 부모 노드로 올린다. 다만 lastelement를 주의해야한다.
        if(child < (minHeap->Size) && minHeap->Element[child+1].dist < minHeap->Element[child].dist) //
            child++;
        if(minHeap->Element[child].dist < last_element.dist)
            minHeap->Element[i] = minHeap->Element[child];
        else
            break;
    }
    minHeap->Element[i] = last_element;
    minHeap->Element[minHeap->Size-1].dist = INF; //LastElement가 남아있어서 마지막 loop가 되었을 때 Min값이 중복되는 경우가 생겼다. 그래서 lastelement는 어차피 위로 가니까 lastElemen의 dist를 INF로 바꿔주었다.
    minHeap->Size--;
    return min_element;
}

void deleteMinHeap(Heap* minHeap){
    free(minHeap->Element);
    free(minHeap);
}
void deleteGraph(Graph* g){
    for(int i=0; i<g->size; i++){
        free(g->vertices[i]);
    }
    free(g->vertices);
    free(g->nodes);
    free(g);
}

int isEmpty(Heap* heap){
    if(heap->Size == 1) return 1;
    else return 0;
}
