#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// 节点结构体
typedef struct Node {
    char character;
    int weight;
    struct Node *left, *right;
} Node;

// 最小堆结构体
typedef struct MinHeap {
    int size;
    Node* array[MAX_SIZE];
} MinHeap;

// 创建一个新节点
Node* newNode(char character, int weight) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->left = temp->right = NULL;
    temp->character = character;
    temp->weight = weight;
    return temp;
}

// 创建一个最小堆
MinHeap* createMinHeap() {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    return minHeap;
}

// 交换两个节点
void swapNode(Node** a, Node** b) {
    Node* t = *a;
    *a = *b;
    *b = t;
}

// 最小堆调整函数
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < minHeap->size && minHeap->array[left]->weight < minHeap->array[smallest]->weight)
        smallest = left;
    
    if (right < minHeap->size && minHeap->array[right]->weight < minHeap->array[smallest]->weight)
        smallest = right;
    
    if (smallest != idx) {
        swapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// 检查大小是否为1
int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// 从最小堆中提取最小值节点
Node* extractMin(MinHeap* minHeap) {
    Node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// 插入一个新节点到最小堆
void insertMinHeap(MinHeap* minHeap, Node* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    
    while (i && node->weight < minHeap->array[(i - 1) / 2]->weight) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

// 构建最小堆
void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i) {
        minHeapify(minHeap, i);
    }
}

// 创建并构建最小堆
MinHeap* createAndBuildMinHeap(char characters[], int weights[], int size) {
    MinHeap* minHeap = createMinHeap();
    
    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = newNode(characters[i], weights[i]);
    }
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// 构建哈夫曼树
Node* buildHuffmanTree(char characters[], int weights[], int size) {
    Node *left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(characters, weights, size);
    
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        
        top = newNode('$', left->weight + right->weight);
        top->left = left;
        top->right = right;
        
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// 生成哈夫曼编码
void generateCodes(Node* root, char* code, int top, char codes[MAX_SIZE][MAX_SIZE]) {
    if (root->left) {
        code[top] = '0';
        generateCodes(root->left, code, top + 1, codes);
    }
    
    if (root->right) {
        code[top] = '1';
        generateCodes(root->right, code, top + 1, codes);
    }
    
    if (!(root->left) && !(root->right)) {
        code[top] = '\0';
        strcpy(codes[(int)root->character], code);
    }
}

// 编码给定的报文
void encode(char* str, char codes[MAX_SIZE][MAX_SIZE], char* encodedStr) {
    int length = strlen(str);
    encodedStr[0] = '\0';
    for (int i = 0; i < length; ++i) {
        strcat(encodedStr, codes[(int)str[i]]);
    }
}

// 解码给定的二进制编码串
void decode(Node* root, char* encodedStr, char* decodedStr) {
    Node* current = root;
    int length = strlen(encodedStr);
    int j = 0;
    
    for (int i = 0; i < length; ++i) {
        if (encodedStr[i] == '0')
            current = current->left;
        else
            current = current->right;
        
        if (!(current->left) && !(current->right)) {
            decodedStr[j++] = current->character;
            current = root;
        }
    }
    decodedStr[j] = '\0';
}

int main() {
    int n;
    scanf("%d", &n);
    
    char characters[MAX_SIZE];
    int weights[MAX_SIZE];
    
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &characters[i]);
    }
    
    for (int i = 0; i < n; ++i) {
        scanf("%d", &weights[i]);
    }
    
    Node* root = buildHuffmanTree(characters, weights, n);
    
    char codes[MAX_SIZE][MAX_SIZE];
    char code[MAX_SIZE];
    generateCodes(root, code, 0, codes);
    
    char message[MAX_SIZE];
    scanf("%s", message);
    
    char encodedStr[MAX_SIZE * 8];
    encode(message, codes, encodedStr);
    
    char decodedStr[MAX_SIZE];
    decode(root, encodedStr, decodedStr);
    
    printf("%s\n", encodedStr);
    printf("%s\n", decodedStr);
    
    return 0;
}
