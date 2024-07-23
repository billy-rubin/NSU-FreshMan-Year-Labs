#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8;

typedef struct Node {
  int value;
  uint8 height;
  struct Node* left;
  struct Node* right;
} Node;

int GetHeight(Node* tree) {
  if (tree == NULL)
    return 0;
  return tree->height;
}

int Max(int a, int b) {
  return (a > b) ? a : b;
}

int FixHeight(Node* tree) {
  int height_left = GetHeight(tree->left);
  int height_right = GetHeight(tree->right);
  tree->height = Max(height_left, height_right) + 1;
  return tree->height;
}

Node* RotateLeft(Node* root) {
  Node* right = root->right;
  root->right = right->left;
  right->left = root;
  root->height = FixHeight(root);
  right->height = FixHeight(right);
  return right;
}

Node* RotateRight(Node* root) {
  Node* left = root->left;
  root->left = left->right;
  left->right = root;
  root->height = FixHeight(root);
  left->height = FixHeight(left);
  return left;
}

int GetBalanceFactor(Node* tree) {
  if (tree == NULL)
    return 0;
  return GetHeight(tree->right) - GetHeight(tree->left);
}

Node* BalanceTree(Node* tree) {
  FixHeight(tree);
  int balance_factor_of_tree = GetBalanceFactor(tree);
  int balance_of_left_subtree = GetBalanceFactor(tree->left);
  int balance_of_right_subtree = GetBalanceFactor(tree->right);
  if (balance_factor_of_tree == 2) {
    if (balance_of_right_subtree < 0) {
      tree->right = RotateRight(tree->right);
    }
    return RotateLeft(tree);
  } else if (balance_factor_of_tree == -2) {
    if (balance_of_left_subtree > 0) {
      tree->left = RotateLeft(tree->left);
    }
    return RotateRight(tree);
  }
  return tree;
}

Node* CreateNewNode(Node* arr, int value, int counter) {
  arr[counter].value = value;
  arr[counter].height = 1;
  arr[counter].left = arr[counter].right = 0;
  return &arr[counter];
}

Node* Insert(Node* tree, int new_value, Node* nodes) {
  if (tree == NULL)
    return CreateNewNode(nodes, new_value, 0);
  if (new_value < tree->value) {
    tree->left = Insert(tree->left, new_value, nodes);
  } else {
    tree->right = Insert(tree->right, new_value, nodes);
  }
  tree->height = FixHeight(tree);
  return BalanceTree(tree);
}

int main() {
  int n;
  if (scanf("%d", &n) != 1)
    return 0;

  if (!n) {
    printf("0");
    return 0;
  }

  Node* root = NULL;
  Node* nodes = (struct Node*)malloc(n * sizeof(Node));
  for (int i = 0; i < n; i++) {
    CreateNewNode(nodes, 0, i);
    int value;
    if (scanf("%d", &value) != 1) {
      free(nodes);
      return 0;
    }
    root = Insert(root, value, &nodes[i]);
  }
  printf("%d", (int)root->height);
  free(nodes);
  return 0;
}
