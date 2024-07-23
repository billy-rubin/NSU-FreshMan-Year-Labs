#ifndef HUFFMAN_H
#define HUFFMAN_H

#define ALPHABET_SIZE 256
#define BITS_IN_BYTE 8

typedef unsigned char uchar;

enum ExitError {
    SUCCESS,
    EMPTY_FILE,
    OUT_OF_MEMORY,
    WRONG_MESSAGE_SIZE,
    TREE_STRUCTURE_ERROR,
};

typedef struct TreeNode {
    uchar symbol;
    int frequency;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct CharacterHistogram {
    uchar symbol;
    int frequency;
} CharacterHistogram;

typedef struct Queue {
    TreeNode* value;
    struct Queue* next;
} Queue;

enum ExitError Encode(FILE* in, FILE* out);
enum ExitError Decode(FILE* in, FILE* out);

#endif // HUFFMAN_H
