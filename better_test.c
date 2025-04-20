#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "RBTree.h"

// 访问私有结构体字段的宏 (需要与RBTree.c中定义保持一致)
#define data(tree)   ((tree)->data)
#define left(tree)   ((tree)->left)
#define right(tree)  ((tree)->right)
#define colour(tree) ((tree)->colour)
#define isRed(tree)  ((tree) != NULL && (tree)->colour == 0) // RED=0, BLACK=1

// 层序遍历打印树
void printLevelOrder(Tree t) {
    if (t == NULL) {
        printf("Tree is empty\n");
        return;
    }
    
    printf("Level order:\n");
    
    // 使用数组模拟队列
    Tree queue[100];
    int front = 0, rear = 0;
    int currentLevel = 0;
    int nodesInCurrentLevel = 1;
    int nodesInNextLevel = 0;
    
    // 根节点入队
    queue[rear++] = t;
    
    while (front < rear) {
        Tree node = queue[front++];
        nodesInCurrentLevel--;
        
        // 按照dryrun格式打印节点
        printf(" level %d - %d(%s)\n", 
               currentLevel, 
               data(node), 
               colour(node) == 0 ? "red" : "black"); // RED=0, BLACK=1
        
        // 子节点入队
        if (left(node) != NULL) {
            queue[rear++] = left(node);
            nodesInNextLevel++;
        }
        
        if (right(node) != NULL) {
            queue[rear++] = right(node);
            nodesInNextLevel++;
        }
        
        // 当前层结束，处理下一层
        if (nodesInCurrentLevel == 0) {
            currentLevel++;
            nodesInCurrentLevel = nodesInNextLevel;
            nodesInNextLevel = 0;
        }
    }
}

// 测试函数
void runTest(int testNum, int values[], int numValues) {
    printf("\nTest %d (insertion order:", testNum);
    for (int i = 0; i < numValues; i++) {
        printf(" %d", values[i]);
    }
    printf(")\n");
    
    // 创建树并插入值
    Tree t = newTree();
    for (int i = 0; i < numValues; i++) {
        t = TreeInsert(t, values[i]);
    }
    
    // 按照层序遍历格式打印
    printLevelOrder(t);
    
    // 释放内存
    freeTree(t);
}

int main() {
    // 测试用例1：插入22
    int test1[] = {22};
    runTest(1, test1, 1);
    
    // 测试用例2：插入22, 12
    int test2[] = {22, 12};
    runTest(2, test2, 2);
    
    // 测试用例3：插入22, 12, 8
    int test3[] = {22, 12, 8};
    runTest(3, test3, 3);
    
    // 测试用例4：插入22, 12, 8, 15
    int test4[] = {22, 12, 8, 15};
    runTest(4, test4, 4);
    
    // 测试用例5：插入22, 12, 8, 15, 11
    int test5[] = {22, 12, 8, 15, 11};
    runTest(5, test5, 5);
    
    // 测试用例6：插入22, 12, 8, 15, 11, 19
    int test6[] = {22, 12, 8, 15, 11, 19};
    runTest(6, test6, 6);
    
    // 测试用例7：插入22, 12, 8, 15, 11, 19, 43
    int test7[] = {22, 12, 8, 15, 11, 19, 43};
    runTest(7, test7, 7);
    
    return 0;
} 