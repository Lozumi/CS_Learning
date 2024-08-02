// Exp-EC-2 Q1
#include <stdio.h>
#define MAXSIZE 100

void mergeSortedArrays(int arr1[], int m, int arr2[], int n)
{
    int i=0,j=0,k=0;
    int mergedArray[MAXSIZE];
    // 遍历比较插入
    while (i<m&&j<n)
    {
        if (arr1[i]<arr2[j])
            mergedArray[k++]=arr1[i++];
        else
            mergedArray[k++]=arr2[j++];
    }
    
    //插入剩余数字
    while (i<m)
    {
        mergedArray[k++]=arr1[i++];
    }
    while (j<n)
    {
        mergedArray[k++]=arr2[j++];
    }

    //打印合并数组
    for (int i=0;i<m+n;i++)
        printf("%d\n",mergedArray[i]);
}

int main()
{
    int m=0,n=0;
    int arr1[MAXSIZE];
    int arr2[MAXSIZE];
    scanf("%d",&m);
    for (int i=0;i<m;i++)
        scanf("%d",&arr1[i]);
    scanf("%d",&n);
    for (int j=0;j<n;j++)
        scanf("%d",&arr2[j]);

    mergeSortedArrays(arr1,m,arr2,n);

    return 0;
}