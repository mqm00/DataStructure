//
//  main.c
//  lab09
//
//  Created by 이승수 on 2022/05/12.
//

#include <stdio.h>
#include <stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
    int Capacity;
    int Size;
    int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap *heap);
int IsFull(Heap *heap);


int main(int argc, const char * argv[]) {
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    
    char cv;
    Heap* maxHeap;
    int heapSize, key, max_element;
    
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'n':
                fscanf(fin, "%d", &heapSize);
                maxHeap = CreateHeap(heapSize);
                break;
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(maxHeap, key);
                break;
            case 'd':
                max_element = DeleteMax(maxHeap);
                if(max_element != -INF){
                    fprintf(fout, "max element : %d deleted\n", max_element);
                }
                break;
            case 'p':
                PrintHeap(maxHeap);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
                break;
        }
    }
    return 0;
}


Heap* CreateHeap(int heapSize){ //heap 할당
    Heap *heap = (Heap*)malloc(sizeof(struct HeapStruct));
    if(heap == NULL){
        fprintf(fout, "allocation error : Out of space !\n");
        return NULL;
    }
    heap->Size = heapSize;
    heap->Capacity = 0;
    heap->Elements = (int*)malloc(sizeof(int)*(heapSize+1)); //heap에서 배열로 쓰일 array 할당
    if(heap->Elements == NULL){
        fprintf(fout, "allocation error : Out of space !\n");
        return NULL;
    }
    heap->Elements[0] = -INF; //계산 편의를 위해 [0]은 임의로 초기화하고 index는 1부터 사용
    return heap;
}

void Insert(Heap* heap, int value){
    if(IsFull(heap)){   //heap이 다 찼을 때
        fprintf(fout, "insert error : heap is full\n");
        return;
    }
    
    for(int i=1; i<heap->Size; i++){ //heap에 이미 존재하는 element를 insert 했을 때
        if(heap->Elements[i] == value){
            fprintf(fout, "insert error : %d is already in the heap\n", value);
            return;
        }
    }
    int i;
    // insert
    // insert할 때 capacity가 1씩 증가한다고 했을 때
    // i가 삽입할 index가 되고 그 index의 root(i/2)가 value보다 작고 0번째 index가 아니라면 index i 와 index i/2를 바꾼다.
    for(i=++heap->Capacity; heap->Elements[i/2] < value && i/2 != 0; i/=2){
        heap->Elements[i] = heap->Elements[i/2];
        }
    heap->Elements[i] = value;
    
    fprintf(fout, "insert %d\n", value);
        }

int Find(Heap* heap, int value){ //array에서 element 찾는 일반적인 방법으로 find
    for(int i=1; i<heap->Size; i++){
        if(heap->Elements[i] == value){
            return 1;
        }
    }
    fprintf(fout, "finding error : %d is not in the heap\n", value);
    return 0;
}

void PrintHeap(Heap *heap){
    if(heap->Capacity == 0 || heap == NULL){ //heap에 element가 하나도 없거나 heap이 생성이 되지 않으면 에러
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    
    for(int i=1; i<=heap->Capacity; i++){
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
}
int IsFull(Heap *heap){
    if(heap->Capacity >= heap->Size){
        return 1;
    }
    return 0;
}

int DeleteMax(Heap* heap){
    //delete 할 element가 없을 때
    if(heap->Capacity < 1){
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }
    int i, child;
    int max_element = heap->Elements[1];
    int last_element = heap->Elements[heap->Capacity];
    
    //index=1, i*2는 자식 노드, 그것이 capacity보다 작거나 같다는 조건이면 처음부터 마지막 element까지 돌 수 있다
    for(i=1; i*2<=heap->Capacity; i=child){
        int child = i*2;
        //자식 element중에 더 큰 것 고르기
        // child가 heap의 마지막 노드가 아니고, child의 sibling이 child보다 더 크다면 child를 sibling으로 바꿔준다.
        if(child < heap->Capacity && heap->Elements[child+1] > heap->Elements[child])
            child++;
        //child가 last element보다 크다면 그냥 부모의 자리를 채우면 되고 작다면 last element를 올려주어야한다.
        if(heap->Elements[child] > last_element)
            heap->Elements[i] = heap->Elements[child];
        else
            break;
}
    heap->Elements[i] = last_element;
    heap->Capacity--;
    return max_element;
}
