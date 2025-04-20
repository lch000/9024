// Red-Black Tree ADT implementation ... COMP9024 25T1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "RBTree.h"

#define PRINT_COLOUR_RED   "\x1B[31m"
#define PRINT_COLOUR_RESET "\x1B[0m"

#define data(tree)   ((tree)->data)
#define left(tree)   ((tree)->left)
#define right(tree)  ((tree)->right)
#define colour(tree) ((tree)->colour)
#define isRed(tree)  ((tree) != NULL && (tree)->colour == RED)

typedef enum {RED,BLACK} Colr;

typedef struct Node {
   int  data;
   Colr colour;
   Tree left, right;
} Node;

// create a new empty Tree
Tree newTree() {
   return NULL;
}

// make a new node containing data
Tree newNode(Item it) {
   Tree new = malloc(sizeof(Node));
   assert(new != NULL);
   data(new) = it;
   colour(new) = RED;
   left(new) = right(new) = NULL;
   return new;
}

// 声明辅助函数
Tree rotateRight(Tree);
Tree rotateLeft(Tree);
Tree insertR(Tree, Item);  // 修复函数声明
void flipColors(Tree);  // 只是改变颜色，不改变结构

// 插入新节点到红黑树
Tree TreeInsert(Tree t, Item it) {
   // 执行插入
   t = insertR(t, it);
   
   // 确保根节点为黑色
   colour(t) = BLACK;
   
   return t;
}

// 辅助插入函数
Tree insertR(Tree h, Item it) {
   // 基本情况：到达叶子节点位置，创建新节点
   if (h == NULL) {
      return newNode(it);
   }
   
   // 递归插入
   if (it < data(h)) {
      left(h) = insertR(left(h), it);
   } else if (it > data(h)) {
      right(h) = insertR(right(h), it);
   } else {
      // 键已存在，不做修改
      return h;
   }
   
   // 修复红黑树性质
   
   // 情况1：右子节点为红色，左子节点为黑色
   if (isRed(right(h)) && !isRed(left(h))) {
      h = rotateLeft(h);
   }
   
   // 情况2：左子节点为红色，左-左子节点为红色
   if (isRed(left(h)) && isRed(left(left(h)))) {
      h = rotateRight(h);
   }
   
   // 情况3：左右子节点都为红色
   if (isRed(left(h)) && isRed(right(h))) {
      flipColors(h);
   }
   
   return h;
}

// 颜色翻转：父节点变红，两个子节点变黑
void flipColors(Tree h) {
   colour(h) = RED;
   colour(left(h)) = BLACK;
   colour(right(h)) = BLACK;
}

// check whether a key is in a Tree
bool TreeSearch(Tree t, Item it) {
   if (t == NULL)
      return false;
   else if (it < data(t))
      return TreeSearch(left(t), it);
   else if (it > data(t))
      return TreeSearch(right(t), it);
   else                                 // it == data(t)
      return true;
}

Tree rotateRight(Tree n1) {
   if (n1 == NULL || left(n1) == NULL)
      return n1;
   Tree n2 = left(n1);
   left(n1) = right(n2);
   right(n2) = n1;
   return n2;
}

Tree rotateLeft(Tree n2) {
   if (n2 == NULL || right(n2) == NULL)
      return n2;
   Tree n1 = right(n2);
   right(n2) = left(n1);
   left(n1) = n2;
   return n1;
}

// free memory associated with Tree
void freeTree(Tree t) {
   if (t != NULL) {
      freeTree(left(t));
      freeTree(right(t));
      free(t);
   }
}

// display Tree sideways
void showTreeR(Tree t, int depth) {
   if (t != NULL) {
      showTreeR(right(t), depth+1);
      int i;
      for (i = 0; i < depth; i++)
         putchar('\t');            // TAB character
      if (isRed(t))
         printf("%s%d%s\n", PRINT_COLOUR_RED, data(t), PRINT_COLOUR_RESET);
      else
         printf("%d\n", data(t));
      showTreeR(left(t), depth+1);
   }
}

void showTree(Tree t) {
   showTreeR(t, 0);
}