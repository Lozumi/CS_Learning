#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int top,S[1000];
void push(int x){//元素入栈
    top++;
    S[top] = x;
    }
int pop(){//元素出栈
    top--;
    return S[top+1];
    }
int main()
{
    int a,b,i=0;
    top = 0;
    char s[100];
    while(scanf("%s",s)!= EOF){//输入数据以空格相断 如1 2 * 3 4而不是12*34因为scanf("%s",s)读取不了空格后面的，但是while并没有结束，还可以接着输入
        if (s[0] == '+'){
            a = pop();
            b = pop();
            push(a+b);
        }
        else if(s[0] == '-'){
            a = pop();
            b = pop();
            push(b - a);
        }
        else if(s[0] == '*'){
            a = pop();
            b = pop();
            push(a*b);
        }
        else{

            push(atoi(s));
            printf("%d\n",atoi(s));//把字符串变为数字，传入的字符串首地址
        }

    }
    printf("%d",pop());
    return 0;
}

