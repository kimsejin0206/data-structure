#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h> //난수 발생 및 동적할당
#include <Windows.h>    //실행시간 측정
#include <time.h>   //난수 발생 시드 값의 인자 time() 함수

/*수정된 파일 ver 2022-05-10*/

//스택의 크기를 전역변수로 선언
int stack_size = 0;

//리스트로 구현할 스택의 노드 정의
typedef struct Node
{
    int elem;   //원소
    struct Node* next;
}Node;

//스택 정의
typedef struct Stack
{
    Node* top;  //스택의 맨 앞 노드를 가리킴 (top)
}Stack;

//새 노드 생성 및 노드 해제
Node* getnode();
void putnode(Node* p);

//스택 기본 ADT
void InitStack(Stack* pstack);
int IsEmpty(Stack* pstack);
void push(Stack* pstack, int e);
int peek(Stack* pstack);
int pop(Stack* pstack);
void freeStack(Stack* pstack);

//스택을 백만번 push 하거나 pop
void pushMillion(Stack* elem_stack, Stack* min_stack);
void popMillion(Stack* elem_stack, Stack* min_stack);
//스택 내 원소 중 최소값을 반환
int findMin(Stack* min_stack);


/********************메인 함수************************/
int main()
{
    //elem_stack : 원소를 저장할 스택
    //min_stack : 스택 내부의 누적되는 최소값을 담을 스택
    Stack elem_stack, min_stack;
    //e : 입력받을 원소
    //del_elem : pop한 원소    min_elem : 최소값
    int e, del_elem, min_elem;
    char cmd;   //입력받을 명령

    //스택 초기화
    InitStack(&elem_stack);
    InitStack(&min_stack);

    //시드 값 초기화
    srand((unsigned int)time(NULL));

    //실행시간을 계산할 변수들 선언
    LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER start, end, diff;

    //타이머의 주파수를 변수에 저장
    QueryPerformanceFrequency(&ticksPerSec);

    while (1)
    {
        scanf("%c", &cmd);  //명령을 입력받음

        //push 호출
        if (cmd == 'p')
        {
            scanf("%d", &e);    //push할 원소 e를 입력받음
            getchar();

            QueryPerformanceCounter(&start);    //측정 시작
            //원소를 스택에 push
            push(&elem_stack, e);

            //누적 최소값 스택이 비어있거나 현재 최소값이 새로 입력받은 원소 이상일 경우
            //현재 최소값을 갱신
            if (min_stack.top == NULL || (min_stack.top)->elem >= e)
                push(&min_stack, e);
            QueryPerformanceCounter(&end);      //측정 완료
            //측정값으로부터 실행시간 계산
            diff.QuadPart = end.QuadPart - start.QuadPart;

            (stack_size)++;     //스택의 크기를 증가
            printf("push %d (%d), ", e, stack_size);
            printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
        }

        //pushMillion 호출
        else if (cmd == 'P')
        {
            QueryPerformanceCounter(&start);    //측정 시작
            //스택을 백만번 push
            pushMillion(&elem_stack, &min_stack);
            QueryPerformanceCounter(&end);      //측정 완료
            //측정값으로부터 실행시간 계산
            diff.QuadPart = end.QuadPart - start.QuadPart;

            stack_size += 100000;      //스택의 크기 갱신
            printf("pushMillion (%d), ", stack_size);
            printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
        }

        //pop 호출
        else if (cmd == 'o')
        {
            //스택의 크기가 0이면(스택이 비어있으면) 예외처리
            if (stack_size == 0)
                printf("No Element In Stack\n");

            //아니면 pop 호출
            else
            {
                QueryPerformanceCounter(&start);    //측정 시작
                //원소를 스택에서 pop
                del_elem = pop(&elem_stack);

                //삭제된 원소가 현재최소값이면
                //최소값 누적 스택에서 최소값 갱신(맨 위의 원소, 즉 현재 최소값을 pop)
                if (del_elem == (min_stack.top)->elem)
                    pop(&min_stack);
                QueryPerformanceCounter(&end);      //측정 완료
                //측정값으로부터 실행시간 계산
                diff.QuadPart = end.QuadPart - start.QuadPart;

                (stack_size)--;     //스택의 크기 갱신
                printf("pop %d (%d), ", del_elem, stack_size);
                printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
            }
        }

        //popMillion 호출
        else if (cmd == 'O')
        {
            //스택의 크기가 백만보다 작으면 (삭제할 원소가 백만보다 적음) 예외처리
            if (stack_size < 100000)
                printf("Not Enough Elements In Stack\n");

            //아니면 popMillion 호출
            else
            {
                QueryPerformanceCounter(&start);    //측정 시작
                //pop을 백만번 호출
                popMillion(&elem_stack, &min_stack);
                QueryPerformanceCounter(&end);      //측정 완료
                //측정값으로부터 실행시간 계산
                diff.QuadPart = end.QuadPart - start.QuadPart;

                stack_size -= 100000;  //스택의 크기 갱신
                printf("popMillion (%d), ", stack_size);
                printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
            }
        }

        //findMin 호출
        else if (cmd == 'f')
        {
            //스택의 최소값이 없으면 예외처리
            if (min_stack.top == NULL)
                printf("No Element In Stack\n");

            //아니면 findMin 호출
            else
            {
                QueryPerformanceCounter(&start);        //측정 시작
                //최소값을 min_elem에 저장
                min_elem = findMin(&min_stack);
                QueryPerformanceCounter(&end);      //측정 완료
                //측정값으로부터 실행시간 계산
                diff.QuadPart = end.QuadPart - start.QuadPart;

                printf("min %d (%d), ", min_elem, stack_size);
                printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
            }
        }

        //프로그램 종료
        else if (cmd == 'q')
        {
            //스택 메모리 해제
            freeStack(&elem_stack);
            freeStack(&min_stack);
            break;
        }
        printf("\n");
    }

    return 0;
}

//새 노드 생성
Node* getnode()
{
    Node* newnode = (Node*)malloc(sizeof(Node));
    newnode->next = NULL;

    return newnode;
}

//할당받은 노드 메모리 해제
void putnode(Node* p)
{
    free(p);
}

//스택 초기화
void InitStack(Stack* pstack)
{
    pstack->top = NULL;
}

//스택이 비었는지 확인
int IsEmpty(Stack* pstack)
{
    //스택이 초기 상태면 (비어있으면) 1 반환
    if (pstack->top == NULL)
        return 1;
    else return 0;
}

//스택에 원소를 저장
void push(Stack* pstack, int e)
{
    //새 노드를 할당 받고
    //그 노드를 스택의 top가 가리키게 함
    Node* newnode = getnode();
    newnode->elem = e;

    newnode->next = pstack->top;
    pstack->top = newnode;
}

//스택의 top이 가리키는 원소 반환
int peek(Stack* pstack)
{
    return pstack->top->elem;
}

//스택의 원소를 삭제
int pop(Stack* pstack)
{
    //스택이 비었으면 예외처리
    if (IsEmpty(pstack))
        return;

    //스택의 top이 가리키는 원소 삭제
    int del_elem = pstack->top->elem;
    Node* del_node = pstack->top;

    pstack->top = pstack->top->next;
    putnode(del_node);

    //삭제된 원소 반환
    return del_elem;
}

//스택 메모리 해제
void freeStack(Stack* pstack)
{
    Node* cur = pstack->top;

    //cur이 스택의 끝에 도달할 때까지
    //cur 이전 노드를 삭제하고 cur 갱신
    while (cur != NULL)
    {
        Node* rnode = cur;
        cur = cur->next;
        putnode(rnode);
    }
}

//push를 백만번 실행
void pushMillion(Stack* elem_stack, Stack* min_stack)
{
    int randNum;

    for (int i = 0; i < 100000; i++)
    {
        //randNum : 1000~9999 사이의 랜덤변수
        randNum = rand() % 9000 + 1000;
        push(elem_stack, randNum);

        //누적 최소값 스택이 비어있거나 현재 최소값이 randNum 이상일 경우
        //현재 최소값을 갱신
        if (min_stack->top == NULL || (min_stack->top)->elem >= randNum)
            push(min_stack, randNum);
    }
}

//pop을 백만번 실행
void popMillion(Stack* elem_stack, Stack* min_stack)
{
    int del_elem;

    for (int i = 0; i < 100000; i++)
    {
        //원소가 저장된 스택을 pop
        del_elem = pop(elem_stack);
        //삭제된 원소가 현재최소값일 경우
        //최소값 누적 스택에서 최소값 갱신(맨 위의 원소, 즉 현재 최소값을 pop)
        if (del_elem == (min_stack->top)->elem)
            pop(min_stack);
    }
}

//현재 최소값 반환
int findMin(Stack* min_stack)
{
    return min_stack->top->elem;
}