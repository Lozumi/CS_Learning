#include <stdio.h>
#define MAX 1000

// 定义三元组
typedef struct {
    int row, col, value;
} Triple;

typedef struct {
    Triple data[MAX];
    int m,n,lens;
} TSMatrix;

void Transpose(TSMatrix A, TSMatrix *B, int lens)
{
    int index, i, j;

    // 设定矩阵大小
    B->m=A.n;
    B->n=A.m;

    if(lens){
        index=0;
        for (i=0;i<A.n;i++){ // 遍历A的每一列
            for (j=0;j<lens;j++){
                if(A.data[j].col=i){ //交换行列并赋值给转置矩阵
                    B->data[index].row=A.data[j].col;
                    B->data[index].col=A.data[j].row;
                    B->data[index].value=A.data[j].value;
                    index++;
                }
            }
        }
    }
}

int main(){
    int m,n,lens=0;
    TSMatrix A,B;

    scanf("%d %d",&m,&n);
    A.m=m;
    A.n=n;
    while(1){
        int row,col,value;
        scanf("%d %d %d",&row,&col,&value);
        if(row==0&&col==0&&value==0)
            break;
        A.data[lens].row=row;
        A.data[lens].col=col;
        A.data[lens].value=value;
        lens++;
    }

    Transpose(A,&B,lens);

    for (int i=0;i<lens;i++)
        printf("%d %d %d\n",B.data[i].row,B.data[i].col,B.data[i].value);
    
    return 0;
}