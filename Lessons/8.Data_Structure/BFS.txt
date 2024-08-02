#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// 图的结构体定义，使用邻接矩阵表示图
typedef struct {
    int adj[MAX][MAX]; // 邻接矩阵
    int n; // 图的节点数
} Graph;

// 初始化图，设置节点数并将邻接矩阵的所有边初始化为0
void initGraph(Graph *graph, int n) {
    graph->n = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph->adj[i][j] = 0; // 无边的情况下，邻接矩阵值为0
        }
    }
}

// 添加边，将节点x和节点y之间的有向边加入图中
void addEdge(Graph *graph, int x, int y) {
    graph->adj[x][y] = 1; // 表示从x到y有一条有向边
}

// 广度优先搜索（BFS）算法，从起始节点start开始遍历图
int bfs(Graph *graph, int start) {
    int queue[MAX], front = 0, rear = 0; // 队列实现BFS，front为队首，rear为队尾
    int visited[MAX] = {0}; // 访问标记数组，初始化为0，表示所有节点均未访问
    int distance[MAX] = {0}; // 记录从起始节点到每个节点的距离
    int maxDistance = 0; // 记录最大距离

    queue[rear++] = start; // 将起始节点加入队列
    visited[start] = 1; // 标记起始节点为已访问

    // 当队列不为空时，继续遍历
    while (front < rear) {
        int current = queue[front++]; // 取出队首节点
        // 遍历所有节点，寻找与当前节点相连且未访问的节点
        for (int i = 0; i < graph->n; i++) {
            if (graph->adj[current][i] && !visited[i]) {
                queue[rear++] = i; // 将找到的相连节点加入队列
                visited[i] = 1; // 标记该节点为已访问
                distance[i] = distance[current] + 1; // 更新该节点到起始节点的距离
                // 更新最大距离
                if (distance[i] > maxDistance) {
                    maxDistance = distance[i];
                }
            }
        }
    }

    return maxDistance; // 返回从起始节点到最远节点的距离
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m); // 读取节点数和边数

    Graph graph;
    initGraph(&graph, n); // 初始化图

    // 读取所有边的信息，并添加到图中
    for (int i = 0; i < m; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        addEdge(&graph, x, y); // 添加边到图
    }

    int result = bfs(&graph, 0); // 从节点0开始进行BFS遍历，计算最远距离
    printf("%d\n", result); // 输出最远距离

    return 0;
}
