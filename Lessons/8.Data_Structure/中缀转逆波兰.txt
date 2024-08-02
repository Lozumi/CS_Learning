#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义运算符的优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 将中缀表达式转换为逆波兰表达式
void infixToPostfix(char *infix, char *postfix) {
    int i, j;
    char stack[100];
    int top = -1;

    for (i = 0, j = 0; infix[i] != '\0'; i++) {
        if (infix[i] >= 'a' && infix[i] <= 'z') {
            postfix[j++] = infix[i]; // 变量直接输出
        } else if (infix[i] == '(') {
            stack[++top] = '(';
        } else if (infix[i] == ')') {
            while (top >= 0 && stack[top] != '(') {
                postfix[j++] = stack[top--];
            }
            top--; // 弹出左括号
        } else {
            while (top >= 0 && precedence(stack[top]) >= precedence(infix[i])) {
                postfix[j++] = stack[top--];
            }
            stack[++top] = infix[i]; // 入栈运算符
        }
    }

    while (top >= 0) {
        postfix[j++] = stack[top--];
    }
    postfix[j] = '\0';
}

int main() {
    char infix[100], postfix[100];

    // printf("输入中缀表达式：");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    printf("%s\n", postfix);

    return 0;
}