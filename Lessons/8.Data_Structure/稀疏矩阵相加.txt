#include <stdio.h>
#define MAX 1000

typedef struct{
    int row,col,value;
} Triple;

typedef struct {
    Triple data[MAX];
    int m,n,lens;
} TSMatrix;

TSMatrix PlusMatrix(TSMatrix A,TSMatrix B){
    int i=0,j=0,k=0;
    TSMatrix C;
    C.lens=0;
    C.m=A.m;
    C.n=A.n;

    //判断情况分三种，大、小、等
    while (i<=A.lens&&j<=B.lens&&(!(i==A.lens&&j==B.lens))){
        if (A.data[i].row<B.data[j].row || (A.data[i].row==B.data[j].row && A.data[i].col<B.data[j].col)){
            C.data[k++]=A.data[i++];
            C.lens++;
        }
        else if (A.data[i].row>B.data[j].row || (A.data[i].row==B.data[j].row && A.data[i].col>B.data[j].col)){
            C.data[k++]=B.data[j++];
            C.lens++;
        }
        else{
            if (A.data[i].value+B.data[i].value!=0){
                C.data[k]=A.data[i]; //注意初始化节点，得到行列属性
                C.data[k++].value=A.data[i++].value+B.data[j++].value;
                C.lens++;
            }
        }  
    }
    return C;
}

int main(){
    int m,n,lens1,lens2;
    scanf("%d %d %d %d",&m,&n,&lens1,&lens2);
    TSMatrix A,B,C;
    A.m=B.m=m;
    A.n=B.n=n;
    A.lens=lens1;
    B.lens=lens2;

    for (int i=0;i<lens1;i++)
        scanf("%d %d %d",&A.data[i].row,&A.data[i].col,&A.data[i].value);
    for (int i=0;i<lens2;i++)
        scanf("%d %d %d",&B.data[i].row,&B.data[i].col,&B.data[i].value);
    
    C = PlusMatrix(A,B);

    for (int i=0;i<C.lens;i++)
        printf("%d %d %d\n",C.data[i].row,C.data[i].col,C.data[i].value);

    return 0;    
    
}