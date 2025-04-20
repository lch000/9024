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

Tree rotateRight(Tree);
Tree rotateLeft(Tree);
Tree insertRec(Tree, Item);
Tree balanceTree(Tree);
Tree flipColors(Tree); 

// insert a new item into a tree
Tree TreeInsert(Tree t, Item it) {
   t = insertRec(t, it);
   colour(t) = BLACK;  // 保证根节点是黑色
   return t;
}

// 递归插入节点
Tree insertRec(Tree t, Item it) {
   if (t == NULL)
      return newNode(it);
      
   if (it < data(t))
      left(t) = insertRec(left(t), it);
   else if (it > data(t))
      right(t) = insertRec(right(t), it);
   
   // 插入后进行平衡调整
   return balanceTree(t);
}

// 调整红黑树平衡
Tree balanceTree(Tree t) {
   // 情况3：右子节点为黑色（或NULL），左子节点和左-左子节点都是红色
   if (isRed(left(t)) && isRed(left(left(t))) && !isRed(right(t)))
      t = rotateRight(t);
   
   // 情况4：左子节点为黑色（或NULL），右子节点和右-右子节点都是红色
   if (!isRed(left(t)) && isRed(right(t)) && isRed(right(right(t))))
      t = rotateLeft(t);
   
   // 情况2：左右子节点都是红色
   if (isRed(left(t)) && isRed(right(t)))
      t = flipColors(t);
   
   // 情况5：左子节点和左-右子节点都是红色
   if (isRed(left(t)) && !isRed(left(left(t))) && isRed(left(right(t)))) {
      left(t) = rotateLeft(left(t));
      t = rotateRight(t);
      t = flipColors(t);
   }
   
   // 情况6：右子节点和右-左子节点都是红色
   if (isRed(right(t)) && isRed(right(left(t))) && !isRed(right(right(t)))) {
      right(t) = rotateRight(right(t));
      t = rotateLeft(t);
      t = flipColors(t);
   }
   
   return t;
}

// 翻转节点颜色
Tree flipColors(Tree t) {
   colour(t) = RED;
   colour(left(t)) = BLACK;
   colour(right(t)) = BLACK;
   return t;
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