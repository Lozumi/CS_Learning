#include <stdio.h>


void BubbleSort(int *arr, int size) // 冒泡排序
{
    for (int i = 1; i < size; i++)
    {
        int flag = 0;
        for (int j = 0; j < size - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                Swap(&arr[j], &arr[j + 1]);
                flag++;
            }
        }
        if (flag == 0)
        {
            break;
        }
    }
}


void InsertSort(int *arr, int size) // 直接插入排序
{
    for (int i = 0; i < size - 1; i++)
    {
        // 单趟插入排序
        // 基本思想：[0,end]区间值为有序
        int end = i;
        int tmp = arr[end + 1];
        while (end >= 0)
        {
            if (tmp < arr[end])
            {
                arr[end + 1] = arr[end];
                end--;
            }
            else
            {
                break; // 在这里break出去再去赋值tmp是为了防止最后一次end = -1进不来赋值
            }
        }
        arr[end + 1] = tmp;
    }
}

void ShellSort(int *arr, int size) // 希尔排序
{
    int gap = size;
    // 多次预排+最后一次直接插入排序
    while (gap > 1)
    {
        gap = gap / 3 + 1; // 控制最后一次进来gap为1进行直接插入排序
        for (int i = 0; i < size - gap; i++)
        {
            int end = i;
            int tmp = arr[end + gap];
            while (end >= 0)
            {
                if (tmp < arr[end])
                {
                    arr[end + gap] = arr[end];
                    end -= gap;
                }
                else
                {
                    break;
                }
            }
            arr[end + gap] = tmp;
        }
    }
}

void SelectSort(int *arr, int size) // 选择排序
{
    for (int i = 0; i < size - 1; i++)
    { // 0~size-2
        int min = i;
        for (int j = i; j < size; j++)
        { // i~size-1
            if (arr[min] > arr[j])
            {
                min = j;
            }
        }
        if (min != i)
        {
            swap(arr[i], arr[min]);
        }
    }
}

void SelectSortPlus(int *arr, int size) // 优化选择排序
{
    int begin = 0;
    int end = size - 1;
    while (begin < end)
    {
        int mini = begin, maxi = begin;
        for (int i = begin + 1; i <= end; i++)
        {
            if (arr[i] < arr[mini])
            {
                mini = i;
            }
            if (arr[i] > arr[maxi])
            {
                maxi = i;
            }
        }
        Swap(&arr[mini], &arr[begin]);
        // 如果maxi = begin，上一步交换了begin和mini的值，会影响maxi指向的值
        if (maxi == begin)
        {
            maxi = mini;
        }
        Swap(&arr[maxi], &arr[end]);
        begin++;
        end--;
    }
}

// 归并算法
void Merge(int sourceArr[],int tempArr[], int startIndex, int midIndex, int endIndex){
    int i = startIndex, j=midIndex+1, k = startIndex;
    while(i!=midIndex+1 && j!=endIndex+1) {
        if(sourceArr[i] > sourceArr[j])
            tempArr[k++] = sourceArr[j++];
        else
            tempArr[k++] = sourceArr[i++];
    }
    while(i != midIndex+1)
        tempArr[k++] = sourceArr[i++];
    while(j != endIndex+1)
        tempArr[k++] = sourceArr[j++];
    for(i=startIndex; i<=endIndex; i++)
        sourceArr[i] = tempArr[i];
}
 
//内部使用递归
void MergeSort(int sourceArr[], int tempArr[], int startIndex, int endIndex) {
    int midIndex;
    if(startIndex < endIndex) {
        midIndex = startIndex + (endIndex-startIndex) / 2;//避免溢出int
        MergeSort(sourceArr, tempArr, startIndex, midIndex);
        MergeSort(sourceArr, tempArr, midIndex+1, endIndex);
        Merge(sourceArr, tempArr, startIndex, midIndex, endIndex);
    }
}

//快速排序法
void QuickSort(int *array, int low, int high)
{
	int i = low;	//第一位
	int j = high;	//最后一位
	int key = array[i]; //将第一个数作为基准值-- 先找到一个基准值
 
	while (i < j)
	{					
		while(i < j && array[j] >= key)
		{
			j--;
		}
		array[i] = array[j];	
 
		while(i < j && array[i] <= key)
		{
			i++;
		}
		array[j] = array[i];
	}
	array[i] = key;
	if (i-1 > low) 
	{
		QuickSort(array, low, i-1);
	}
 
	if (i+1 < high)
	{
		QuickSort(array, i+1, high);
	}
} 
