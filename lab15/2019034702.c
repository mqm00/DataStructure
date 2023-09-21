//
//  lab15.c
//  DataStructure
//
//  Created by 이승수 on 2022/06/09.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl{
    int TableSize;
    List *TheLists;
}HashTbl;

HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);

int main(int argc, char *argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    
    char solution_str[20];
    int solution, TableSize;
    
    fscanf(fin, "%s", solution_str);
    if(!strcmp(solution_str, "linear"))
        solution = 1;
    else if(!strcmp(solution_str, "quadratic"))
        solution = 2;
    else{
        fprintf(fout, "error: invalid hashing soulution!");
        return 0;
    }
    
    fscanf(fin, "%d", &TableSize);
    HashTable H = createTable(TableSize);
    
    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(H, key, solution);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                int result = Find(H, key, solution);
                if(result != -1)
                    fprintf(fout, "%d is in the table\n", key);
                else if(result == -1)
                    fprintf(fout, "%d is not in the table\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                Delete(H, key, solution);
                break;
            case 'p':
                printTable(H);
                break;
        }
    }
    
    deleteTable(H);
    
    return 0;
}

HashTable createTable(int TableSize){ //hashtable 할당
    HashTable h = (HashTbl*)malloc(sizeof(struct HashTbl));
    h->TableSize = TableSize;
    h->TheLists = (List*)malloc(sizeof(List)*TableSize);
    for(int i=0; i<TableSize; i++){
        h->TheLists[i] = 0;
    }
    return h;
}

void Insert(HashTable h, ElementType key, int solution){
    int i;
    int find_idx = Find(h, key, solution); //Find에서 인덱스
    if(find_idx != -1){
        fprintf(fout, "insertion error : %d already exists at address %d\n", key, find_idx);
        return;
    }
    
    int hfunction = key%(h->TableSize);
    if(h->TheLists[hfunction] == 0){ //비어있다면
        h->TheLists[hfunction] = key;
        fprintf(fout, "insert %d into address %d\n", key, hfunction);
        return;
    }
    // collision이 발생했을 때
    if(solution == 1){ //resolve 방법이 linear인 경우
            for(i=0; i<h->TableSize; i++){ //F(i) = i 로 resolve
                hfunction = (key+i)%(h->TableSize);
                if(h->TheLists[hfunction] == 0){ // 비어있다면
                    h->TheLists[hfunction] = key;
                    fprintf(fout, "insert %d into address %d\n", key, hfunction);
                    return;
                }
                else{               //테이블을 끝까지 돌았을 때
                    if(i==h->TableSize-1){
                        fprintf(fout, "insertion error : table is full\n");
                        return;
                    }
                    continue;
                }
            }
        }
    if(solution == 2){               //resolve 방법이 quadratic인 경우
        for(i=0; i<h->TableSize; i++){
            hfunction = ((key+(i*i))%h->TableSize); //F(i) = i^2;
            if(h->TheLists[hfunction] == 0){
                h->TheLists[hfunction] = key;
                fprintf(fout, "insert %d into address %d\n", key, hfunction);
                return;
            }
            else{
                if(i==h->TableSize-1){ //테이블을 끝까지 돌았을 때
                    fprintf(fout, "insertion error : table is full\n");
                    return;
                }
                continue;
            }
        }
    }
}

void Delete(HashTable H, ElementType Key, int solution){
    if(Find(H, Key, solution) == -1){
        fprintf(fout, "deletion error : %d is not in the table\n", Key);
        return;
    }
    
    int location = Find(H, Key, solution);
    H->TheLists[location] = 0;
    fprintf(fout, "delete %d\n", Key);
    return;
    
}

int Find(HashTable H, ElementType Key, int solution){
    int hfunction;
    if(solution == 1){ //linear 방식으로 find;
        for(int i=0; i<H->TableSize; i++){
            hfunction = (Key+i)%H->TableSize; //F(i) = i를 이용해 index를 찾는다
            if(H->TheLists[hfunction] == Key) //해당 index의 키값이 key값과 같다면 그 index를 리턴
                return hfunction;
        }
    }
    if(solution == 2){ //quadratic 방식으로 find
        for(int i=0; i<H->TableSize; i++){ //F(i) = i^2을 이용해 index를 찾는다.
            hfunction = (Key+(i*i))%H->TableSize;
            if(H->TheLists[hfunction] == Key)
                return hfunction;
        }
    } //찾지못했다면 -1 return
    return -1;
}

void printTable(HashTable H){ //순차적으로 프린트
    for(int i=0; i<H->TableSize; i++){
        fprintf(fout, "%d ", H->TheLists[i]);
    }
    fprintf(fout, "\n");
}

void deleteTable(HashTable H){
    free(H->TheLists);
    free(H);
}
