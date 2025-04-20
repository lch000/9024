#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "RBTree.h"

// 测试用例
void runTest(int testNum, int values[], int numValues) {
    printf("\nTest %d (insertion order:", testNum);
    for (int i = 0; i < numValues; i++) {
        printf(" %d", values[i]);
    }
    printf(")\n");
    
    // 创建并插入值到红黑树
    Tree t = newTree();
    for (int i = 0; i < numValues; i++) {
        t = TreeInsert(t, values[i]);
    }
    
    // 打印树结构
    printf("Tree structure:\n");
    showTree(t);
    
    // 验证搜索功能
    for (int i = 0; i < numValues; i++) {
        if (!TreeSearch(t, values[i])) {
            printf("Error: Value %d not found in tree!\n", values[i]);
        }
    }
    
    // 释放树的内存
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