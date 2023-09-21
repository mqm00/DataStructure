
//
//  Created by 이승수 on 2022/03/17.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node
{
    ElementType element;
    Position next;
};

List MakeEmpty( List L );
int IsEmpty( List L );
int IsLast( Position P, List L );
void Insert( ElementType X, List L, Position P );
void PrintList( List L );
void Delete( ElementType X, List L );
Position Find( ElementType X, List L );
Position FindPrevious( ElementType X, List L );
void DeleteList ( List L );

FILE *input;
FILE *output;

int main(int argc, char *argv[]) {
    char command;
    int key1, key2;
    //FILE *input;
    Position header = NULL;
    Position tmp = NULL;
    if (argc == 1) {
        input = fopen("input.txt", "r");
        output = fopen("output.txt", "w");
    }
    else {
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
    }
    header = MakeEmpty(header);
    while (1) {
        command = fgetc(input);
        if (feof(input)) break;
        switch (command) {
                
            case 'i':
                fscanf(input, "%d %d", &key1, &key2);
                //duplication check
                tmp = Find(key1,header);
                if (tmp != NULL) {
                    fprintf(output, "insertion(%d) failed : the key already exists in the list\n", key1);
                    break;
                }
                tmp = Find(key2, header);
                if (tmp == NULL) {
                    fprintf(output, "insertion(%d) failed : can not find the location to be inserted\n", key1);
                    break;
                }
                Insert(key1, header, tmp);
                break;
                
            case 'd' :
                fscanf(input, "%d", &key1);
                Delete(key1, header);
                break;
                
            case 'f':
                fscanf(input, "%d", &key1);
                tmp = FindPrevious(key1, header);
                if (IsLast(tmp, header))
                    fprintf(output, "finding(%d) failed : element %d is not in the list\n", key1, key1);
                else {
                    if (tmp->element > 0)
                        fprintf(output, "key of the previous node of %d is %d\n", key1, tmp->element);
                    else
                        fprintf(output, "key of the previous node of %d is head\n", key1);
                }
                break;
                
            case 'p':
                PrintList(header);
                break;
                
            default:
                break;
        }
    }
    DeleteList(header);
    fclose(input);
    fclose(output);
    return 0;
}

List MakeEmpty( List L){
    L = (Position)malloc(sizeof(struct Node)); //header로 하는 노드를 만들어준다.
    L->element = -1; //임의의 element를 저장해준다. 뒤의 노드와 헷갈리지않게 -1을 저장하였다.
    L->next = NULL; // header가 가리키는 주소는 없기에 NULL값으로 설정해준다.
    return L;
}

int IsEmpty( List L){
    if(L->next == NULL) //header로 설정한 L의 next가 NULL이면 비어있는 list이므로 1을 return하고 그 반대는 0을 return한다.
        return 1;
    else
        return 0;
}

int IsLast(Position P, List L){
    if(P->next == NULL || P == NULL) //마지막 노드라면 가리키는 곳이 NULL이다. 혹은 그 노드 자체가 NULL이면 마지막 노드로 볼 수 있다.
        return 1;
    else
        return 0;
}

void Insert( ElementType X, List L, Position P){
    Position Insert_node = (Position)malloc(sizeof(struct Node)); //insert 할 노드를 만든다
    Insert_node->element = X; // insert할 노드의 element를 설정한다.
    Insert_node->next = P->next; // insert할 노드는 삽입되는 부분 기준 앞에 노드가 가리키는 노드를 가리켜야한다
    P->next = Insert_node; // 그후 insert하는 노드를 그 앞의 노드가 가리키게 하여 linked list를 만든다.
    
}

void PrintList(List L){
    Position p = L->next; //헤더를 제외한 노드의 element를 출력해야하므로 다음 노드의 값을 저장해준다.
    if( p == NULL){
        fprintf(output, "empty list!\n"); //헤더가 가리키는 노드인 p가 NULL이라면 비어있는 List이므로 예외처리
        return;
    }
    while(p != NULL){
        fprintf(output, "key:%d ", p->element);//하나씩 print해주고 다음으로 넘어간다.
        p = p->next;
    }
    fprintf(output, "\n");
}

void Delete( ElementType X, List L){
    Position tmp1 = FindPrevious(X, L); //삭제하고자 하는 노드의 앞의 노드를 찾는다
    if(tmp1->next == NULL) //그 앞의 노드가 가리키는 것이 NULL이라면 list에 찾고자 하는 값이 없는 것이므로 예외처리 해준다.
        fprintf(output, "deletion(%d) failed : element %d is not in the list\n", X, X);
    else{
        Position tmp2 = tmp1->next; // NULL값이 아니라면 삭제하고자하는 element가 list에 있는 것이다. 그렇다면 삭제할 노드의 주소값을 저장하고 삭제할 노드의 앞 뒤를 연결해준후 미리 저장했던 삭제할 노드의 주소를 free를 통해 없애준다.
        tmp1->next = tmp2->next;
        free(tmp2);
    }
}

Position Find( ElementType X, List L ){
    Position p = L; //header의 주소값을 넘겨준다.
    if(X == -1) // 인자로 받은 element가 header에 해당하는 곳일 때의 처리
        return L;
    while( p != NULL && p->element != X){ //하나씩 비교해가며 넘어간다.
        p = p->next;
    }
    return p;
}

Position FindPrevious( ElementType X, List L){
    Position p = L;
    while(p->next != NULL && p->next->element !=X) {
        p = p->next; //어떤 노드의 element가 X와 같을 때까지 하나하나 넘어가며 비교. 그리고 이 함수는 찾는 X의 전의 노드의 주소를 찾는 것이기 때문에 p->next->element로 비교한다.
        
    }
    return p;
}

void DeleteList(List L){
    Position p1 = L;
    while(p1 != NULL){ //NULL이면 마지막 노드이기 때문
        Position p2 = p1->next; //그냥 p1을 삭제하면 p1이 가리키는 노드의 주소값을 잃어버리므로 다른 곳에 저장한 후 p1을 free
        free(p1);
        p1 = p2;
    }
}
