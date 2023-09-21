//
//  lab04.c
//  dstructure
//
//  Created by 이승수 on 2022/03/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
    int* key;
    int top;
    int max_stack_size;
}Stack;

Stack* CreatStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]){
    fin=fopen("input.txt", "r");
    fout=fopen("output.txt", "w");
    
    Stack* stack;
    char input_str[101];
    int max=20, i=0, a, b, result, error_flag=0;
    
    fgets(input_str, 101, fin);
    stack = CreatStack(max);
    
    fprintf(fout, "top numbers : ");
    
    while (input_str[i]!='#') {     //#이 나오면 종료
        
        if(0<input_str[i]-'0' && input_str[i]-'0' < 10){ //input_str 배열이 char이기 때문에 숫자로 바꿔서 비교
            
            if(IsFull(stack)){ //stack이 전부 찼을 때
                error_flag = ISFULL;
                break;
            }
            
            Push(stack, input_str[i]-'0'); //stack에 push
        }
        
        else if(input_str[i] == '+'){
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            b = Pop(stack);
            
            if(IsEmpty(stack)){ // 하나 pop한 후 비어있을지 모르기 때문에 다시 확인
                error_flag = ISEMPTY;
                break;
            }
            
            a = Pop(stack);
            Push(stack, a+b); //연산 후 다시 push, 연산이 실행되었다면 스택에 쌓인 두 숫자가 하나가 되었을 것이기에 스택이 꽉 찼는지 확인할 필요는 없다.
        }
        
        else if(input_str[i] == '-'){
            
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            b = Pop(stack);
            
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            a = Pop(stack);
            Push(stack, a-b);
        }
        
        else if(input_str[i] == '/'){
            
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            b = Pop(stack);
            
            if(b == 0){ //0으로 나눌시 에러
                error_flag = DIVIDEZERO;
                break;
            }
            
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            a = Pop(stack);
            
            Push(stack, a/b);
            
        }
        
        else if(input_str[i] == '%'){
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            b = Pop(stack);
            
            if(b == 0){
                error_flag = DIVIDEZERO;
                break;
            }
            
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            a = Pop(stack);
            
            Push(stack, a%b);
        }
        
        else if (input_str[i] == '*'){
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            b = Pop(stack);
            
            if(IsEmpty(stack)){ //연산 전 stack 확인
                error_flag = ISEMPTY;
                break;
            }
            
            a = Pop(stack);
            
            Push(stack, a*b);
        }
            
        
        int t = Top(stack);
        fprintf(fout, "%d ", t);
        result = t;
        i++;
    }
    
    if(error_flag == ISFULL){
        fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
    }
    
    else if(error_flag == ISEMPTY){
        fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
    }
    
    else if(error_flag == DIVIDEZERO){
        fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
    }
    
    else{
        if(stack->top+1 > 1){
            fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
        }
        
        else{
            fprintf(fout, "\nevaluation result : %d\n", result);
        }
    }
    
    fclose(fin);
    fclose(fout);
    DeleteStack(stack);
}

Stack* CreatStack(int max){
    Stack* s;
    
    s=(Stack*)malloc(sizeof(struct Stack)); //Stack의 헤드 할당
    s->key = (int*)malloc(sizeof(int)*max); //Stack의 배열 max size로 만들기
    s->top = -1;
    s->max_stack_size = max;
    return s;
}

void Push(Stack* S, int X){
    S->key[++(S->top)]=X; //Stack의 top을 하나 올린 인덱스에 해당 X 대입
}

int Pop(Stack* S){
    return S->key[(S->top)--]; //top에 해당하는 인자 return후 top -1
}

int Top(Stack* S){
    if(IsEmpty(S))
        return 0;
    else
        return S->key[S->top];
}

int IsEmpty(Stack* S){
    if(S->top < 0)
        return 1;
    else
        return 0;
}

int IsFull(Stack* S){
    if(S->top > 19)
        return 1;
    else
        return 0;
}

void DeleteStack(Stack* S){
    free(S->key);
}


