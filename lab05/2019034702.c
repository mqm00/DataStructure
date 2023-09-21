//
//  lab05.c
//  dstructure
//
//  Created by 이승수 on 2022/03/31.
//

#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree{
    int left_thread;
    ThreadedPtr left_child;
    ElementType data;
    ThreadedPtr right_child;
    int right_thread;
}ThreadedTree;

ThreadedPtr CreateTree();
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx);
void printInorder(ThreadedPtr root);
void DeleteTree(ThreadedPtr root);

int main(int argc, char *argv[]){
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w");
    
    ThreadedPtr root = CreateTree();
    
    int NUM_NODES;
    fscanf(fin, "%d", &NUM_NODES);
    
    int root_idx=0, idx=0;
    
    while(++idx <= NUM_NODES){
        ElementType data;
        fscanf(fin, "%d", &data);
        
        if(Insert(root, root_idx, data, idx) == 0){
            return 0;
            
        }
    
    }
    
    printInorder(root);
    DeleteTree(root);
    
    
    fclose(fin);
    fclose(fout);
    
    return 0;
}

ThreadedPtr CreateTree(){
    ThreadedPtr tree = NULL;
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    
    if(tree==NULL){
        fprintf(fout, "Out of Space!\n");
        return NULL;
    }
    
    tree->data = -1;
    tree->left_thread = 1;
    tree->left_child = tree;
    tree->right_thread = 1;
    tree->right_child = tree;
    return tree;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
    ThreadedPtr child = (ThreadedPtr)malloc(sizeof(ThreadedTree)); //새로 삽입될 노드 생성
    if(child==NULL){ //child가 할당되지 않았을 때
        fprintf(fout, "Out of Space!\n");
        return 0;
    }
    child->data = data;
    child->left_thread = 1;
    child->right_thread = 1;
    int dataForRoot[7]; //주어진 문제에서 최대 node 수는 100개 이하이므로 node를 연결하는 Edge는 7개 이하다. 따라서 edge를 저장할 Int 형 배열을 만든다.
    
    if(idx == 1){ //idx가 1일 때 먼저 설정해준다
        root->right_child = child;
        root->right_thread = 0;
        root->left_thread = 0;
        child->right_child = root;
        child->left_child = root;
    }
    
    else{
    while(1){ //인덱스를 2로 나눴을 때 나머지가 0과 1인 경우를 기준으로 트리를 만들 것
        dataForRoot[root_idx] = idx%2;
        idx = idx/2;
        root_idx++;
        if(idx == 0) break;
    }
        root_idx --;
    
    for(int i=root_idx; i>0; i--){ //배열의 뒤에서부터 거꾸로 읽으면서 그에 맞게 트리를 내려간다.
        switch(dataForRoot[i]){
            case 0: //0일때는 왼쪽 트리로
                root = root->left_child;
                break;
            case 1: // 1일때는 오른쪽으로
                root = root -> right_child;
                break;
            }
    }
}
    if(root->left_thread == 1 ){ //부모가 되는 노드의 left child가 thread라면 그것을 자식 노드로 이어주고 자식 노드의 thread를 다시 설정해준다.
        child->left_child = root->left_child; //자식 노드의 thread 연결
        child->left_thread = 1;
        child->right_child = root; //자식 노드의 thread
        child->right_thread = 1;
        root->left_child = child; //진짜 child 설정
        root->left_thread = 0; //표시
    }
    else if(root->right_thread ==1){ //오른쪽일때도 마찬가지. 한가지 다른 점은 이 경우엔 자식 노드의 left thread가 자신의 부모노드를 가리키는 것이다.
        child->right_child = root->right_child; //자식 노드의 thread 연결
        child->right_thread = 1;
        child->left_child = root;
        child->left_thread = 1;
        root->right_child = child;
        root->right_thread = 0;
    }
    return 1;
}

void printInorder(ThreadedPtr root) {
    ThreadedPtr tmp = root->right_child; //프린트하면서 나올 노드를 가리킬 tmp를 만든다.
    if(tmp==NULL){ //tmp가 할당되지 않았을 때
        fprintf(fout, "Out of Space!\n");
        exit(0);
    }
    fprintf(fout, "inorder traversal :");
    while(!tmp->left_thread){       //그 노드를 가장 마지막 제일 왼쪽 노드를 가리키게 만든다.
        tmp = tmp->left_child;
    }
    
    while(1){
        if(tmp == root)
            break;          //tmp가 root 노드를 가리키면 프린트가 끝난 것이기에 break;
        fprintf(fout, "% d", tmp->data);
        
        if(!tmp->right_thread){     //tmp의 right_thread가 f이면 이어지는 자식 노드가 있으므로 그 노드를 가리키게 만든다
            tmp = tmp -> right_child;
            while(!tmp->left_thread)    //tmp의 left_thread가 f이면 이어지는 자식 노드가 있는 것이므로 inorder 순서에 맞게 자식 노드로 옮긴다.
                tmp = tmp ->left_child;
        }
        else{           //tmp 의 right_thread가 T이면 이어지는 자식 노드가 없으므로 그 부모 노드를 가리키게 만든다.
            tmp = tmp->right_child;
        }
    }
    
}

void DeleteTree(ThreadedPtr root){ //inorder로 각 노드로 접근해 그 주소값을 배열에 저장하고 한꺼번에 free를 할 것
    ThreadedPtr rootArr[100]; //노드의 주소값을 저장할 배열
    ThreadedPtr tmp = root->right_child;
    if(tmp==NULL){
        fprintf(fout, "Out of Space!\n");
        exit(0);
    }
    int deleteIdx = 0;
    
    while(!tmp->left_thread){ //print와 마찬가지로 제일 아래 왼쪽 노드로 접근
        tmp = tmp->left_child;
    }
    
    while(1){
        
        if(tmp == root){ //tmp가 root 노드를 가리키면 각 주소가 다 담긴 것
            rootArr[deleteIdx] = root; // 마지막으로 root 주소 저장하고
            for(int i = --deleteIdx; i>=0; i--){ // 첫번째로 저장된 주소가 free될 때까지 계속 free
                free(rootArr[i]);
            }
            break;
        }
        //print와 같게 inorder로 탐색하며 포인팅하는 노드를 옮긴다.
        rootArr[deleteIdx] = tmp;
        
        if(!tmp->right_thread){
            tmp = tmp -> right_child;
            while(!tmp->left_thread)
                tmp = tmp ->left_child;
        }
        else{
            tmp = tmp->right_child;
        }
        deleteIdx++;
    }
}
