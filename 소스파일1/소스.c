#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h> //���� �߻� �� �����Ҵ�
#include <Windows.h>    //����ð� ����
#include <time.h>   //���� �߻� �õ� ���� ���� time() �Լ�

/*������ ���� ver 2022-05-10*/

//������ ũ�⸦ ���������� ����
int stack_size = 0;

//����Ʈ�� ������ ������ ��� ����
typedef struct Node
{
    int elem;   //����
    struct Node* next;
}Node;

//���� ����
typedef struct Stack
{
    Node* top;  //������ �� �� ��带 ����Ŵ (top)
}Stack;

//�� ��� ���� �� ��� ����
Node* getnode();
void putnode(Node* p);

//���� �⺻ ADT
void InitStack(Stack* pstack);
int IsEmpty(Stack* pstack);
void push(Stack* pstack, int e);
int peek(Stack* pstack);
int pop(Stack* pstack);
void freeStack(Stack* pstack);

//������ �鸸�� push �ϰų� pop
void pushMillion(Stack* elem_stack, Stack* min_stack);
void popMillion(Stack* elem_stack, Stack* min_stack);
//���� �� ���� �� �ּҰ��� ��ȯ
int findMin(Stack* min_stack);


/********************���� �Լ�************************/
int main()
{
    //elem_stack : ���Ҹ� ������ ����
    //min_stack : ���� ������ �����Ǵ� �ּҰ��� ���� ����
    Stack elem_stack, min_stack;
    //e : �Է¹��� ����
    //del_elem : pop�� ����    min_elem : �ּҰ�
    int e, del_elem, min_elem;
    char cmd;   //�Է¹��� ���

    //���� �ʱ�ȭ
    InitStack(&elem_stack);
    InitStack(&min_stack);

    //�õ� �� �ʱ�ȭ
    srand((unsigned int)time(NULL));

    //����ð��� ����� ������ ����
    LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER start, end, diff;

    //Ÿ�̸��� ���ļ��� ������ ����
    QueryPerformanceFrequency(&ticksPerSec);

    while (1)
    {
        scanf("%c", &cmd);  //����� �Է¹���

        //push ȣ��
        if (cmd == 'p')
        {
            scanf("%d", &e);    //push�� ���� e�� �Է¹���
            getchar();

            QueryPerformanceCounter(&start);    //���� ����
            //���Ҹ� ���ÿ� push
            push(&elem_stack, e);

            //���� �ּҰ� ������ ����ְų� ���� �ּҰ��� ���� �Է¹��� ���� �̻��� ���
            //���� �ּҰ��� ����
            if (min_stack.top == NULL || (min_stack.top)->elem >= e)
                push(&min_stack, e);
            QueryPerformanceCounter(&end);      //���� �Ϸ�
            //���������κ��� ����ð� ���
            diff.QuadPart = end.QuadPart - start.QuadPart;

            (stack_size)++;     //������ ũ�⸦ ����
            printf("push %d (%d), ", e, stack_size);
            printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
        }

        //pushMillion ȣ��
        else if (cmd == 'P')
        {
            QueryPerformanceCounter(&start);    //���� ����
            //������ �鸸�� push
            pushMillion(&elem_stack, &min_stack);
            QueryPerformanceCounter(&end);      //���� �Ϸ�
            //���������κ��� ����ð� ���
            diff.QuadPart = end.QuadPart - start.QuadPart;

            stack_size += 100000;      //������ ũ�� ����
            printf("pushMillion (%d), ", stack_size);
            printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
        }

        //pop ȣ��
        else if (cmd == 'o')
        {
            //������ ũ�Ⱑ 0�̸�(������ ���������) ����ó��
            if (stack_size == 0)
                printf("No Element In Stack\n");

            //�ƴϸ� pop ȣ��
            else
            {
                QueryPerformanceCounter(&start);    //���� ����
                //���Ҹ� ���ÿ��� pop
                del_elem = pop(&elem_stack);

                //������ ���Ұ� �����ּҰ��̸�
                //�ּҰ� ���� ���ÿ��� �ּҰ� ����(�� ���� ����, �� ���� �ּҰ��� pop)
                if (del_elem == (min_stack.top)->elem)
                    pop(&min_stack);
                QueryPerformanceCounter(&end);      //���� �Ϸ�
                //���������κ��� ����ð� ���
                diff.QuadPart = end.QuadPart - start.QuadPart;

                (stack_size)--;     //������ ũ�� ����
                printf("pop %d (%d), ", del_elem, stack_size);
                printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
            }
        }

        //popMillion ȣ��
        else if (cmd == 'O')
        {
            //������ ũ�Ⱑ �鸸���� ������ (������ ���Ұ� �鸸���� ����) ����ó��
            if (stack_size < 100000)
                printf("Not Enough Elements In Stack\n");

            //�ƴϸ� popMillion ȣ��
            else
            {
                QueryPerformanceCounter(&start);    //���� ����
                //pop�� �鸸�� ȣ��
                popMillion(&elem_stack, &min_stack);
                QueryPerformanceCounter(&end);      //���� �Ϸ�
                //���������κ��� ����ð� ���
                diff.QuadPart = end.QuadPart - start.QuadPart;

                stack_size -= 100000;  //������ ũ�� ����
                printf("popMillion (%d), ", stack_size);
                printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
            }
        }

        //findMin ȣ��
        else if (cmd == 'f')
        {
            //������ �ּҰ��� ������ ����ó��
            if (min_stack.top == NULL)
                printf("No Element In Stack\n");

            //�ƴϸ� findMin ȣ��
            else
            {
                QueryPerformanceCounter(&start);        //���� ����
                //�ּҰ��� min_elem�� ����
                min_elem = findMin(&min_stack);
                QueryPerformanceCounter(&end);      //���� �Ϸ�
                //���������κ��� ����ð� ���
                diff.QuadPart = end.QuadPart - start.QuadPart;

                printf("min %d (%d), ", min_elem, stack_size);
                printf("cputime = %.12f", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart) * 1000);
            }
        }

        //���α׷� ����
        else if (cmd == 'q')
        {
            //���� �޸� ����
            freeStack(&elem_stack);
            freeStack(&min_stack);
            break;
        }
        printf("\n");
    }

    return 0;
}

//�� ��� ����
Node* getnode()
{
    Node* newnode = (Node*)malloc(sizeof(Node));
    newnode->next = NULL;

    return newnode;
}

//�Ҵ���� ��� �޸� ����
void putnode(Node* p)
{
    free(p);
}

//���� �ʱ�ȭ
void InitStack(Stack* pstack)
{
    pstack->top = NULL;
}

//������ ������� Ȯ��
int IsEmpty(Stack* pstack)
{
    //������ �ʱ� ���¸� (���������) 1 ��ȯ
    if (pstack->top == NULL)
        return 1;
    else return 0;
}

//���ÿ� ���Ҹ� ����
void push(Stack* pstack, int e)
{
    //�� ��带 �Ҵ� �ް�
    //�� ��带 ������ top�� ����Ű�� ��
    Node* newnode = getnode();
    newnode->elem = e;

    newnode->next = pstack->top;
    pstack->top = newnode;
}

//������ top�� ����Ű�� ���� ��ȯ
int peek(Stack* pstack)
{
    return pstack->top->elem;
}

//������ ���Ҹ� ����
int pop(Stack* pstack)
{
    //������ ������� ����ó��
    if (IsEmpty(pstack))
        return;

    //������ top�� ����Ű�� ���� ����
    int del_elem = pstack->top->elem;
    Node* del_node = pstack->top;

    pstack->top = pstack->top->next;
    putnode(del_node);

    //������ ���� ��ȯ
    return del_elem;
}

//���� �޸� ����
void freeStack(Stack* pstack)
{
    Node* cur = pstack->top;

    //cur�� ������ ���� ������ ������
    //cur ���� ��带 �����ϰ� cur ����
    while (cur != NULL)
    {
        Node* rnode = cur;
        cur = cur->next;
        putnode(rnode);
    }
}

//push�� �鸸�� ����
void pushMillion(Stack* elem_stack, Stack* min_stack)
{
    int randNum;

    for (int i = 0; i < 100000; i++)
    {
        //randNum : 1000~9999 ������ ��������
        randNum = rand() % 9000 + 1000;
        push(elem_stack, randNum);

        //���� �ּҰ� ������ ����ְų� ���� �ּҰ��� randNum �̻��� ���
        //���� �ּҰ��� ����
        if (min_stack->top == NULL || (min_stack->top)->elem >= randNum)
            push(min_stack, randNum);
    }
}

//pop�� �鸸�� ����
void popMillion(Stack* elem_stack, Stack* min_stack)
{
    int del_elem;

    for (int i = 0; i < 100000; i++)
    {
        //���Ұ� ����� ������ pop
        del_elem = pop(elem_stack);
        //������ ���Ұ� �����ּҰ��� ���
        //�ּҰ� ���� ���ÿ��� �ּҰ� ����(�� ���� ����, �� ���� �ּҰ��� pop)
        if (del_elem == (min_stack->top)->elem)
            pop(min_stack);
    }
}

//���� �ּҰ� ��ȯ
int findMin(Stack* min_stack)
{
    return min_stack->top->elem;
}