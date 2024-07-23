#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH_SIZE 43

enum Color { RED = 1, BLACK = 0 };

typedef struct Node {
  int value;
  enum Color color;
  struct Node* left;
  struct Node* right;
} Node;

Node* InitializeNewNode(Node* arr, int index, int value) {
  arr[index].value = value;
  arr[index].color = RED;
  arr[index].left = arr[index].right = NULL;
  return &arr[index];
}

Node* RotateLeft(Node* root, Node* new_node, int node_depth, Node** path) {
  Node* parent = NULL;
  Node* tmp = new_node->right;
  new_node->right = tmp->left;
  tmp->left = new_node;
  if (node_depth > 0) {
    parent = path[node_depth - 1];
  } else {
    return tmp;
  }
  if (parent->left == new_node) {
    parent->left = tmp;
  } else {
    parent->right = tmp;
  }
  return root;
}

Node* RotateRight(Node* root, Node* new_node, int node_depth, Node** path) {
  Node* parent = NULL;
  Node* tmp = new_node->left;
  new_node->left = tmp->right;
  tmp->right = new_node;

  if (node_depth > 0) {
    parent = path[node_depth - 1];
  } else {
    return tmp;
  }
  if (parent->right == new_node) {
    parent->right = tmp;
  } else {
    parent->left = tmp;
  }
  return root;
}

Node* FindUncle(Node* parent, Node* grand_parent) {
  if (grand_parent == NULL) {
    return NULL;
  }
  if (parent == grand_parent->left) {
    return grand_parent->right;
  } else {
    return grand_parent->left;
  }
}

int GetBlackHeight(Node* root) {
  if (root == NULL) {
    return 1;
  } else {
    int currentHeight = (root->color == BLACK) ? 1 : 0;
    return GetBlackHeight(root->left) + currentHeight;
  }
}

Node* BalanceTree(Node* root, Node* new_node, int node_depth, Node** path) {
  if (root == NULL || new_node == NULL || *path == NULL) {
    return root;
  }
  Node* new_root = root;
  if (node_depth > 0) {
    Node* parent = path[node_depth - 1];
    if (parent == NULL) {
      return new_root;
    }
    if (parent->color == BLACK) {
      return new_root;
    } else {
      Node* grand_parent = path[node_depth - 2];
      if (grand_parent == NULL) {
        return new_root;
      }
      Node* uncle = FindUncle(parent, grand_parent);
      if (uncle != NULL && uncle->color == RED) {
        parent->color = BLACK;
        uncle->color = BLACK;
        grand_parent->color = RED;
        if (node_depth > 2) {
          new_root = BalanceTree(root, grand_parent, node_depth - 2, path);
        }
      } else {
        if (grand_parent->left == parent) {
          if (parent->right == new_node) {
            path[node_depth] = parent;
            root = RotateLeft(root, parent, node_depth - 1, path);
            path[node_depth - 1] = path[node_depth - 2];
            parent = path[node_depth - 1];
            grand_parent = path[node_depth - 2];
          }
          parent->color = BLACK;
          grand_parent->color = RED;
          new_root = RotateRight(root, grand_parent, node_depth - 2, path);
        } else {
          if (parent->left == new_node) {
            path[node_depth] = parent;
            root = RotateRight(root, parent, node_depth - 1, path);
            path[node_depth - 1] = path[node_depth - 2];
            parent = path[node_depth - 1];
            grand_parent = path[node_depth - 2];
          }
          parent->color = BLACK;
          grand_parent->color = RED;
          new_root = RotateLeft(root, grand_parent, node_depth - 2, path);
        }
      }
    }
  }
  return new_root;
}

Node* Insert(Node* root, Node* new_node) {
  if (root == NULL) {
    return new_node;
  }
  int node_depth = 0;
  Node* path[MAX_PATH_SIZE];
  Node* current = root;
  Node* last_node = current;
  while (current != NULL) {
    last_node = current;
    path[node_depth++] = current;
    if (new_node->value <= current->value) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  if (new_node->value < last_node->value) {
    last_node->left = new_node;
  } else {
    last_node->right = new_node;
  }
  return BalanceTree(root, new_node, node_depth, path);
}

int main() {
  int n;
  if (scanf("%d", &n) != 1) {
    return 0;
  }
  if (!n) {
    printf("0");
    return 0;
  }
  Node* root = NULL;
  Node* nodes = (struct Node*)malloc(n * sizeof(Node));
  for (int i = 0; i < n; i++) {
    int value;
    if (scanf("%d", &value) != 1) {
      free(nodes);
      return 0;
    }
    Node* new_node = InitializeNewNode(nodes, i, value);
    root = Insert(root, new_node);
    root->color = BLACK;
  }
  printf("%d\n", GetBlackHeight(root));
  free(nodes);
  return 0;
}
