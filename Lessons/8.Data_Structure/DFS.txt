#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// 深度优先搜索函数
void DFS(int graph[MAX][MAX], int visited[MAX], int v, int n) {
    visited[v] = 1; // 标记当前节点为已访问
    for (int i = 0; i < n; i++) {
        if (graph[v][i] == 1 && !visited[i]) {
            DFS(graph, visited, i, n); // 递归访问所有连通的节点
        }
    }
}

// 计算与v0连通的节点个数
int countConnectedNodes(int graph[MAX][MAX], int n) {
    int visited[MAX] = {0}; // 初始化访问标记数组
    DFS(graph, visited, 0, n); // 从节点v0开始进行DFS遍历

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            count++;
        }
    }
    return count;
}

int main() {
    int n;
    scanf("%d", &n); // 读取节点数

    int graph[MAX][MAX];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]); // 读取邻接矩阵
        }
    }

    int connectedNodesCount = countConnectedNodes(graph, n);
    printf("%d\n", connectedNodesCount-1); // 输出与v0连通的节点个数，去除自身

    return 0;
}
