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
Tree insertR(Tree, Item);

// 插入新节点到红黑树
Tree TreeInsert(Tree t, Item it) {
   t = insertR(t, it);
   colour(t) = BLACK;  // 确保根节点为黑色
   return t;
}

// 辅助插入函数 - 按照课程伪代码精确实现
Tree insertR(Tree h, Item it) {
   // 基本情况：到达叶子节点位置，创建新的红色节点
   if (h == NULL) {
      return newNode(it);
   }
   
   // 递归插入
   if (it < data(h)) {
      left(h) = insertR(left(h), it);
   } else if (it > data(h)) {
      right(h) = insertR(right(h), it);
   } else {
      return h;  // 键已存在
   }
   
   // 情况1：修复右倾红色链接
   if (isRed(right(h)) && !isRed(left(h))) {
      h = rotateLeft(h);
   }
   
   // 情况2：修复连续的左倾红色链接
   if (isRed(left(h)) && left(left(h)) != NULL && isRed(left(left(h)))) {
      h = rotateRight(h);
   }
   
   // 情况3：如果左右子节点都是红色，执行颜色翻转
   if (isRed(left(h)) && isRed(right(h))) {
      colour(h) = RED;
      colour(left(h)) = BLACK;
      colour(right(h)) = BLACK;
   }
   
   return h;
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
   
   // 维持颜色: 新的根节点(n2)继承原根节点(n1)的颜色，原根节点变为红色
   colour(n2) = colour(n1);
   colour(n1) = RED;
   
   return n2;
}

Tree rotateLeft(Tree n2) {
   if (n2 == NULL || right(n2) == NULL)
      return n2;
   Tree n1 = right(n2);
   right(n2) = left(n1);
   left(n1) = n2;
   
   // 维持颜色: 新的根节点(n1)继承原根节点(n2)的颜色，原根节点变为红色
   colour(n1) = colour(n2);
   colour(n2) = RED;
   
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