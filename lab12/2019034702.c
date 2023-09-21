//
//  main.c
//  lab12
//
//  Created by 이승수 on 2022/05/19.
//

#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size; //node에 있는 key값의 갯수
    BNodePtr *child;
    int *key;
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);
void Insert(BNodePtr *root, int key);
int Find(BNodePtr root, int key);
void PrintTree(BNodePtr root);
void DeleteTree(BNodePtr root);
void sorting(BNodePtr root);
void basicInsert(BNodePtr* level, int key);
void split(BNodePtr* parent, int key);

int main(int argc, const char * argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    
    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);
    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
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
                if(root->size == 0)
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

BNodePtr CreateTree(int order){ //노드 만들기
    BNodePtr Node = (BNodePtr)malloc(sizeof(struct BNode)); //자식 노드의 배열 따로 키 값의 배열 따로인 점을 헷갈리지 말아야한다.
    Node->child = (BNodePtr*)malloc(sizeof(struct BNode)*(order+1)); //자식 노드를 가리키는 포인터는 order보다 하나 많게 만들어서 split할 때 직관적으로 포인터를 옮길 것이다.
    Node->key = (int*)malloc(sizeof(int)*(order)); // split을 할 때에 일단 key값을 집어넣고 split을 진행할 예정이기 때문에 키 배열을 order만큼 만든다.
    Node->order = order;
    Node->size = 0; //들어간 key값의 갯수
    Node->is_leaf = 1;
    for(int i=0; i<Node->order; i++){ //일단 0으로 할당해준다.
        Node->key[i] = 0;
    } 
    return Node;
    //제출 전 할당 안됐을 때 처리 해주기
}

void Insert(BNodePtr *root, int key){
    
    //인서트는 리프일 때와 리프가 아닐 때를 구분해서 진행해야한다.
    //리프일 때는 인서트를 하면 되지만 리프가 아닐 때에는 key값에 맞는 노드를 찾는 것이 우선이 되어야하기 때문이다.
    if((*root)->is_leaf == 1){
        if((*root)->size < (*root)->order-1)
            basicInsert(&(*root), key); //basicinsert는 기본적인 인서트이다. key의 갯수가 order-1을 넘어가지 않는다면 그냥 넣고 sorting을 진행하는 함수이다.
        else
            split(&(*root), key); //split은 key의 갯수가 order-1을 넘었을 때 진행한다.
    }
    
    else{                           //리프가 아닐 때에 크기를 비교하며 들어갈 노드를 찾는다
        int childIndex = 0;
        for(int i=0; i<(*root)->order; i++){    //키값보다 작다면 한칸씩 올려야된다. && 처음에 키 배열을 모두 0으로 초기화 했기 때문에 그에 대한 처리도 해준다.
            if((*root)->key[i] < key && (*root)->key[i] != 0)
                childIndex++;
            else{                               // 키값이 들어갈 위치를 찾았다면 그 인덱스를 가지고 insert를 다시 진행한다.
                Insert(&(*root)->child[childIndex], key);
                break;
            }
        }                                       //insert후에 split이 진행되었다면 가운데 값을 가지고 있는 노드가 생길텐데 그 노드의 사이즈는 1이 된다. insert를 진행했던 루트의 사이즈가 1이 되었다면 루트는 위에 말한 가운데 값을 가진 노드가 된 것이다. 그렇다면 그 노드를 그것의 부모 노드에 붙여야한다.
            if((*root)->child[childIndex]->size == 1 ){
                int tmp = (*root)->child[childIndex]->key[0];
                if((*root)->key[0] == tmp) return;
                basicInsert(&(*root), tmp); //일단 키 값을 부모노드에 집어넣는다.
                BNodePtr idtmp1, idtmp2;
                idtmp2 = (*root)->child[childIndex]; //그리고나서 자식노드들을 부모 노드의 알맞은 인덱스로 넣어준다.
                (*root)->child[childIndex] = (*root)->child[childIndex]->child[0]; //split된 왼쪽 노드 삽입
                idtmp1 = (*root)->child[childIndex+1]; //부모 노드의 원래 자식 노드를 담아준다.
                (*root)->child[childIndex+1] = idtmp2->child[1]; // split된 오른쪽 노드를 그 위치에 넣어준다.
                for(int i=childIndex+2; i<=(*root)->order; i++){ //그리고 하나씩 밀면서 자식 노드 배열을 다시 설정해준다.
                    idtmp2 = (*root)->child[i];
                    (*root)->child[i] = idtmp1;
                    idtmp1 = idtmp2;
                }
                if((*root)->size == (*root)->order) //만약 이 작업 후에 부모 노드의 키값이 order와 같아진다면 다시 한번 split을 진행해야한다. 이런식으로 split하고 확인하고 split을 한번 더 해야하면 다시 부모 노드를 대상으로 split을 한번 더 한다.
                    split(&(*root), tmp);
            }
        }
    }

void basicInsert(BNodePtr *root, int key){
    ///insert와 split에서 겹치는 경우가 있어 예외처리를 해준다
    for(int i=0; i<(*root)->order; i++){
        if((*root)->key[i] ==  key){
            return;
        }
    }
    for(int i=0; i<(*root)->order; i++){ //빈자리에 insert하고 sorting
        if((*root)->key[i] == 0){
            (*root)->key[i] = key;
            (*root)->size++;
            sorting((*root));
            break;
        }
    }
}

void split(BNodePtr *parent, int key){
    basicInsert(parent, key);//일단 넣고 정렬
    int median = (*parent)->order/2;
    int tmp = (*parent)->key[median]; //중간지점의 key값
    
    BNodePtr leftNode = CreateTree((*parent)->order); //중간지점을 기준으로 분리하기
    BNodePtr rightNode = CreateTree((*parent)->order);
    leftNode->is_leaf = (*parent)->is_leaf; //leaf값은 leaf에서 split이 날 수도 있고 그 위에서 날 수도 있기 때문에 parent의 leaf값을 따른다.
    rightNode->is_leaf = (*parent)->is_leaf;
    
    for(int i=0; i<median; i++){ //키값 복사. 둘로 나눠진 노드에 중간값을 기준으로 왼쪽 노드와 오른쪽 노드에 복사해준다
        leftNode->key[i] = (*parent)->key[i];
        (*parent)->key[i] = 0;
        leftNode->size++;
        rightNode->key[i] = (*parent)->key[median+i+1];
        (*parent)->key[median+i+1] = 0;
        rightNode->size++;
    }
    (*parent)->key[0] = tmp; //그리고 split하고 중간값을 담고 있는 부모 노드에 첫번 째 값을 중간값으로 바꿔주고 원래 자리는 다시 0으로 바꾼다.
    (*parent)->key[median] = 0;

    //리프가 아닌 곳에서 split이 난 거라면 그에 딸려 있는 자식 노드도 연결해주어야 하기 때문에 이것도 중간값을 기준으로 둘로 나눠서 각각의 자리에 넣어준다.
    if(!(*parent)->is_leaf){
        for(int i=0; i<=median; i++){
            leftNode->child[i] = (*parent)->child[i];
            (*parent)->child[i] = NULL;
            rightNode->child[i] = (*parent)->child[i+median+1];
            (*parent)->child[i+median+1] = NULL;
        }
    }
    //split되어 올라간 노드를 그에 맞게 초기화해준다.
    (*parent)->child[0] = leftNode;
    (*parent)->child[1] = rightNode;
    (*parent)->size = 1;
    (*parent)->is_leaf = 0;
    
}


void sorting(BNodePtr node){ //키 넣었을 때 sorting 삽입할 때마다 sorting이 진행되기 때문에 마지막으로 삽입된 값만 앞의 것과 비교하며 바꿔주면 된다.
    for(int i=node->size-1; i>=0; i--){
        if(node->key[i] < node->key[i-1] && node->key[i] != 0){
            int tmp = node->key[i-1];
            node->key[i-1] = node->key[i];
            node->key[i] = tmp;
        }
        else if(node->key[i] == 0)
            continue;
        else
            break;
    }
}


void PrintTree(BNodePtr root){ //inoder로 프린트하기
    for(int i=0; i<=root->size; i++){ //자식 노드가 있다면 그 노드로 들어가서 다시 print 진행
        if(root->child[i])
            PrintTree(root->child[i]);
        if(root->key[i]==0) //0이면 어떤 값도 없는 것이므로 return
            return;
        fprintf(fout, "%d ", root->key[i]);
    }
}

int Find(BNodePtr root, int key){ //print와 비슷한 맥락으로 find를 진행한다.
    if(root){
        int i;
        for(i=0; i<root->size; i++){
            if(root->key[i] == key)
                return 1;
            if(key < root->key[i]) //더 작다면 그 아래 자식 노드로 들어가서 다시 find
                return Find(root->child[i], key);
        }
        return Find(root->child[i], key);
    }
    return 0;
}

void DeleteTree(BNodePtr root){
    //free 해야할 것: node, child* 배열, int* 배열
    if(root == NULL)
        return;
    for(int i=0; i<root->order; i++){
        DeleteTree(root->child[i]);
    }
    free(root->child);
    free(root->key);
    free(root);
}
