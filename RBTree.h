// Red-Black Tree ADT interface ... COMP9024 25T1

#include <stdbool.h>

typedef int Item;      // item is just a key

// 颜色枚举
typedef enum {RED,BLACK} Colr;

// Node结构体定义
typedef struct Node {
   int  data;
   Colr colour;
   struct Node *left, *right;
} Node;

typedef struct Node *Tree;

// 访问Node结构体字段的宏
#define data(tree)   ((tree)->data)
#define left(tree)   ((tree)->left)
#define right(tree)  ((tree)->right)
#define colour(tree) ((tree)->colour)
#define isRed(tree)  ((tree) != NULL && (tree)->colour == RED)

Tree newTree();        // create an empty Tree
void freeTree(Tree);   // free memory associated with Tree
void showTree(Tree);   // display a Tree (sideways)

bool TreeSearch(Tree, Item);   // check whether an item is in a Tree
Tree TreeInsert(Tree, Item);   // insert a new item into a Tree