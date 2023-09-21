//
//  lab06.c
//  dstructure
//
//  Created by 이승수 on 2022/04/07.
//

#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
    int value;
    struct BST* left;
    struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
Tree FindMin(Tree root);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);


void main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    
    char cv;
    int key;
    
    Tree root = NULL;
    
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                fprintf(fout, "insert %d\n", key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(findNode(root, key))
                    fprintf(fout, "%d is in the tree\n", key);
                else
                    fprintf(fout, "finding error: %d is not in the tree\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if(findNode(root, key)){
                    root = deleteNode(root, key);
                    fprintf(fout, "delete %d\n", key);
                }
                else{
                    fprintf(fout, "deletion error: %d is not in the tree\n", key);
                }
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if(cv == 'i'){
                    if (root == NULL)
                        fprintf(fout, "tree is empty");
                    else
                        printInorder(root);
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);
}


Tree insertNode(Tree root, int key){
	if(findNode(root, key)){ //이미 tree 안에 존재 할 때 
	 fprintf(fout, "insertion error: %d is already in the tree\n", key);
	 return root;
	 }
    if(root == NULL){                   //재귀적으로 돌았을 때 가리키는 노드가 null이라면 이제 내가 새롭게 insert하고싶은 노드가 위치할 공간이므로 새 node를 만들어서 연결해준다.
        root = (Tree)malloc(sizeof(struct BST));
        if(root == NULL){ // node가 할당되지 않았을 때
            fprintf(fout, "Out of space");
            return NULL;
        }
        root->value = key;
        root->left = NULL;
        root->right = NULL;
    }
    
    else if(key < root->value){  //BST는 parent node의 key 값보다 작으면 왼쪽 sub tree로 크면 오른쪽 sub tree로 들어가므로 두 경우를 나누어준다.
        root->left = insertNode(root->left, key);
    }
    else if(key > root->value){
        root->right = insertNode(root->right, key);
    }
    return root;
}

Tree deleteNode(Tree root, int key){
    Tree tmp;
    if(root == NULL){
        return NULL;
    }
    else if(key < root->value){ //찾아야할 키 값이 노드의 value보다 작으면 왼쪽에서 delete 시행
        root->left = deleteNode(root->left, key);
    }
    else if(key > root->value){ // 크다면 오른쪽으로
        root->right = deleteNode(root->right, key);
    }
    else if(root->left && root->right){ //삭제해야할 노드를 발견했을 때 오른쪽에서 가장 작은 값을 삭제할 노드의 위치로 옮겨주면 BST가 유지되므로 바꿔준다.
        tmp = FindMin(root->right);
        root->value = tmp->value;
        root->right = deleteNode(root->right, root->value);
    }
    else{ //삭제할 노드의 자식 노드가 1개 또는 0개일 때는 위에처럼 연결해줄 필요가 없다.
        tmp = root;
        if(root->left == NULL){
            root = root->right;
        }
        else{
            root = root->left;
        }
        free(tmp);
    }
    return root;
}

int findNode(Tree root, int key){
    if(root == NULL)
        return 0;
    
    else if(key == root->value) //찾으려는 값과 같을 때
        return 1;
    
    else if(key < root->value){ //찾으려는 값보다 작다는 것은 그 값이 현재 노드의 왼쪽에 있다는 것이므로
        root = root->left;
        return findNode(root, key);
    }
    
    else{ //크다면 오른쪽에 있다는 것이므로
        root = root->right;
        return findNode(root, key);
    }
}

void printInorder(Tree root){ //inorder 순서로 프린트 해준다.
    if(root){
        printInorder(root->left);
        fprintf(fout, "%d ", root->value);
        printInorder(root->right);
    }
}

Tree FindMin(Tree root){ //삭제할 때 삭제할 노드의 오른쪽에서 가장 작은 노드를 찾아서 바꿔주어야하기 때문에 작은 노드를 찾는 함수가 필요.
    if(root == NULL){
        return NULL;
    }
    if(root->left == NULL){
        return root;
    }
    return FindMin(root->left);
}

void deleteTree(Tree root){
    if(root){
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}
