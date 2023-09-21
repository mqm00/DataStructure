//
//  lab07.c
//  dstructure
//
//  Created by 이승수 on 2022/04/14.
//

#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
    ElementType element;
    AVLTree left, right;
    int height;
}AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
Position Find(ElementType X, AVLTree T);
Position FindMin(AVLTree T);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);
int Height(AVLTree T);
int Max(int height1, int height2);

int main(int argc, char *argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    
    AVLTree Tree = NULL;
    char cv;
    int key;
    
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d\n", &key);
                Tree = Insert(key, Tree);
                break;
            case 'd':
                fscanf(fin, "%d\n", &key);
                Tree = Delete(key, Tree);
                break;
        }
        PrintInorder(Tree);
        fprintf(fout, "\n");
    }
    
    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);
    
    return 0;
}


AVLTree Insert(ElementType X, AVLTree T){
    
    if(T == NULL){          //삽입될 node 새로 만들기
        T = (AVLTree)malloc(sizeof(struct AVLNode));
        T->left = NULL;
        T->right = NULL;
        T->element = X;
        T->height = 0;
        return T;
    }
    
    if(Find(X,T)){  //같은 element를 가진 노드가 이미 있을 때
        fprintf(fout, "insertion error : %d is already in the tree!\n", X);
    }
        
    else{
        if(T->element < X){ //삽입될 노드의 element가 root의 element보다 클 때 오른쪽으로 삽입
            T->right = Insert(X , T->right); //오른쪽 노드를 root로 해서 재귀적으로 타고 들어간다.
            if(Height(T->right)-Height(T->left) >= 2){  //AVL트리 이므로 높이 차가 2 이상이면 로테이트 시켜줘서 높이차를 1이하로 만든다.
                if(X > T->right->element){ //X가 T의 오른쪽 노드보다 크면 T->right의 right로 들어가기때문에 right rotate 시행해준다
                    T = SingleRotateWithRight(T);
                }
                else{ //T->right의 left로 들어가게되면 왼쪽싱글 한번 오른쪽 싱글한번을 통해 balancing해야되기 때문에 doublerotate 사용
                    T = DoubleRotateWithRight(T);
                }
            }
        }
        else if(T->element > X){ // 작을 때 왼쪽으로 삽입
            T->left = Insert(X, T->left);
            if(Height(T->left) - Height(T->right) >= 2){
                if(X < T->left->element){ //왼쪽의 왼쪽으로 삽입되니까 T의 왼쪽노드를 left rotate로 올려준다.
                    T = SingleRotateWithLeft(T);
                }
                else{ //T의 왼쪽의 오른쪽으로 삽입된다면 오른쪽으로 돌려주고 왼쪽으로 돌려준다.
                    T = DoubleRotateWithLeft(T);
                }
            }
        }
    }
    T->height = Max(Height(T->left), Height(T->right)) + 1; //balancing 이후 높이 다시 설정해준다. 양쪽 자식 노드의 높이 중 큰 것을 받았고 부모 노드이니까 +1 해주어야한다.
    return T;
}

AVLTree Delete(ElementType X, AVLTree T){
    Position tmp;
    if(!Find(X, T)){ //삭제하려한 element를 가진 노드가 없을 때
        fprintf(fout, "deletion error : %d is not in the tree!\n", X);
        return T;
    }
    else{
        if(X < T-> element){ //먼저 삭제할 element를 찾아야하는데 BST이므로 현재 노드의 값과 비교해서 작으면 왼쪽으로 노드를 옮겨서 delete함수 시행해주고 크다면 오른쪽으로 옮겨서 시행해준다.
            T->left = Delete(X, T->left);
        }
        else if(X > T->element){
            T->right = Delete(X, T->right);
        }
        else if(T->left && T->right){ //삭제하려는 노드를 찾았을 때
            tmp = FindMin(T->right); //노드를 삭제하게 되면 그 밑의 자식노드들을 다시 BST 특성에 맞게 재정렬해주어야한다. 자식 노드중 가장 작은 노드와 바꿔주고 삭제하면 BST를 망가뜨리지않고 원하는 노드를 삭제할 수 있다.
            T->element = tmp->element;
            T->right = Delete(T->element, T->right);
        }
        else{
            tmp = T; //밑에 딸린 자식 노드가 하나밖에 없을 때 그냥 그 다음 노드를 이어주면 된다.
            if(T->left == NULL){
                T = T->right;
            }
            else{
                T = T->left;
            }
            free(tmp);
        }
    }
    return T;
}

Position SingleRotateWithLeft(Position node){ //왼쪽에 있는 노드를 돌릴 때
    AVLTree tmp;
        tmp = node->left; //노드의 왼쪽 노드를 저장해주고
        node->left = tmp->right; // 노드의 왼쪽에 노드 자리에 위치할 노드의 오른쪽 노드를 이어준다.
        tmp->right = node; //자리를 바꿔준다.
    
    node->height = Max(Height(node->left), Height(node->right)) + 1;
    tmp->height = Max(Height(tmp->left), node->height)+1;
    
    return tmp; //그리고 바꾼 노드를 return해준다.
    }

Position SingleRotateWithRight(Position node){ //rotatewithleft와 반대로 시행해준다.
        AVLTree tmp;
        tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        
        node->height = Max(Height(node->left), Height(node->right)) + 1;
        tmp->height = Max(Height(tmp->left), node->height)+1;
        
        return tmp;
    }

Position DoubleRotateWithLeft(Position node){ //left로 double rotate는 오른쪽 subtree에 왼쪽 child로 새로운 노드가 들어간거니까 왼쪽 child를 right single rotate로 돌려주고 이후에 left single rotate로 돌려서 완성한다.
     
    node->left = SingleRotateWithRight(node->left);
    return SingleRotateWithLeft(node);
}

Position DoubleRotateWithRight(Position node){ //left와 반대로 해준다.
    node->right = SingleRotateWithLeft(node->right);
    return SingleRotateWithRight(node);
}

void PrintInorder(AVLTree T){ //inorder traversal를 이용해 print한다.
    if(T){
        PrintInorder(T->left);
        fprintf(fout, "%d(%d) ", T->element, T->height);
        PrintInorder(T->right);
    }
}

void DeleteTree(AVLTree T){ //모든 tree를 delete 해준다. 가장 왼쪽부터 차례차례 free
    if(!T)
        return;
    DeleteTree(T->left);
    DeleteTree(T->right);
    
    free(T);
}

Position Find(ElementType X, AVLTree T){ //노드에 찾고자하는 element를 찾는다.
    if(T == NULL){
        return NULL;
    }
        
    else if(T->element == X)
        return T;
    
    else if(T->element > X) //BST이니까 element가 작다면 왼쪽 subtree로
        return Find(X, T->left);
    
    else if(T->element < X) // 크다면 오른쪽 subtree로 간다.
         return Find(X, T->right);
    
    return NULL;
}

Position FindMin(AVLTree T){ //delete를 실행할 때 삭제할 노드의 자식노드중 가장 작은 노드를 찾아 올려주어햐하는데 그때 쓰이는 함수.
    if(T == NULL)
        return NULL;
    if(T->left == NULL)
        return T;
    return FindMin(T->left); //가장 왼쪽 아래로 내려가서 return 해준다.
}

int Height(AVLTree T){ //height가 -1인 경우도 있는데 노드의 height를 초기화하는 것으로 -1을 표현할 수 없다. 때문에 따로 함수를 만들어서 계산해준다.
    if(T == NULL){
        return -1;
}
    return T->height;
}

int Max(int height1, int height2){ //높이를 계산할 때 양쪽 subtree중 긴 것이 height이기 때문에 더 높은 쪽 찾는다.
    if(height1 > height2)
        return height1;
    else if(height1 < height2)
        return height2;
    else
        return height1;
}
