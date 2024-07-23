#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "bitstream.h"

static enum ExitError CalculateFrequencies(FILE* in, CharacterHistogram table[]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        table[i].frequency = 0;
        table[i].symbol = (uchar)i;
    }
    fseek(in, 1, SEEK_SET);
    uchar symbol;
    int symbols_count = 0;
    while (fread(&symbol, sizeof(uchar), 1, in) == 1) {
        table[symbol].frequency++;
        symbols_count++;
    }
    if (symbols_count == 0) {
        return EMPTY_FILE;
    }
    return SUCCESS;
}

static TreeNode* Pop(Queue** head) {
    if (*head == NULL) {
        return NULL;
    }
    Queue* tmp = *head;
    (*head) = tmp->next;
    TreeNode* value = tmp->value;
    free(tmp);
    return value;
}

static Queue* CreatePriorityQueue(CharacterHistogram table[]) {
    Queue* head = NULL;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (table[i].frequency <= 0) {
            continue;
        }
        Queue* element = (Queue*)malloc(sizeof(Queue));
        element->value = (TreeNode*)malloc(sizeof(TreeNode));
        element->value->left = element->value->right = NULL;
        element->value->frequency = table[i].frequency;
        element->value->symbol = table[i].symbol;
        element->next = NULL;

        if (head == NULL || head->value->frequency > element->value->frequency) {
            element->next = head;
            head = element;
        } else {
            Queue* current = head;
            while (current->next != NULL && current->next->value->frequency <= element->value->frequency) {
                current = current->next;
            }
            element->next = current->next;
            current->next = element;
        }
    }
    return head;
}

static Queue* MergeNodes(TreeNode* node1, TreeNode* node2) {
    Queue* new_node = (Queue*)malloc(sizeof(Queue));
    new_node->value = (TreeNode*)malloc(sizeof(TreeNode));
    new_node->value->frequency = node1->frequency + node2->frequency;
    new_node->value->left = node1;
    new_node->value->right = node2;
    new_node->value->symbol = 0;
    new_node->next = NULL;
    return new_node;
}

static Queue* InsertTreeNode(Queue* head, Queue* new_node) {
    if (!head || new_node->value->frequency < head->value->frequency) {
        new_node->next = head;
        return new_node;
    }
    Queue* current = head;
    while (current->next && current->next->value->frequency <= new_node->value->frequency) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
    return head;
}

static TreeNode* CreateHuffmanTree(CharacterHistogram table[]) {
    Queue* head = CreatePriorityQueue(table);
    if (head && !head->next) {
        TreeNode* root = (TreeNode*)malloc(sizeof(TreeNode));
        root->symbol = head->value->symbol;
        root->frequency = head->value->frequency;
        root->left = root->right = NULL;
        free(head->value);
        free(head);
        return root;
    }
    while (head && head->next) {
        TreeNode* node1 = Pop(&head);
        TreeNode* node2 = Pop(&head);
        Queue* new_node = MergeNodes(node1, node2);
        head = InsertTreeNode(head, new_node);
    }
    TreeNode* root = Pop(&head);
    return root;
}

static void CreateSymbolCodes(TreeNode* node, char* code, char** table) {
    const int code_length = (int)strlen(code);
    if (!node->left && !node->right) {
        table[node->symbol] = (char*)malloc((code_length + 1) * sizeof(char));
        strcpy(table[node->symbol], code);
    } else {
        if (node->left) {
            code[code_length] = '0';
            code[code_length + 1] = '\0';
            CreateSymbolCodes(node->left, code, table);
        }
        if (node->right) {
            code[code_length] = '1';
            code[code_length + 1] = '\0';
            CreateSymbolCodes(node->right, code, table);
        }
    }
}

static void FreeHuffmanTree(TreeNode* tree) {
    if (tree->left) {
        FreeHuffmanTree(tree->left);
    }
    if (tree->right) {
        FreeHuffmanTree(tree->right);
    }
    free(tree);
}

static void FreeCodeTable(char** code_table) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (code_table[i] != NULL) {
            free(code_table[i]);
        }
    }
    free(code_table);
}

enum ExitError Encode(FILE* in, FILE* out) {
    CharacterHistogram* table = (CharacterHistogram*)calloc(ALPHABET_SIZE, sizeof(CharacterHistogram));
    if (CalculateFrequencies(in, table) == EMPTY_FILE) {
        free(table);
        return EMPTY_FILE;
    }

    TreeNode* root = CreateHuffmanTree(table);
    if (root == NULL) {
        free(table);
        return OUT_OF_MEMORY;
    }

    char** code_table = (char**)calloc(ALPHABET_SIZE, sizeof(char*));
    char* code = malloc(ALPHABET_SIZE * sizeof(uchar));
    code[0] = '\0';

    CreateSymbolCodes(root, code, code_table);
    BitStream* stream = CreateBitStream(out);
    fwrite(&(root->frequency), sizeof(int), 1, stream->file);

    WriteHuffmanTree(stream, root);
    fseek(in, 1, SEEK_SET);
    uchar symbol;
    while (fread(&symbol, sizeof(uchar), 1, in) == 1) {
        WriteSymbol(stream, code_table, symbol);
    }

    ReleaseBitStream(stream);
    FreeHuffmanTree(root);
    FreeCodeTable(code_table);
    free(table);
    free(code);
    return SUCCESS;
}

enum ExitError Decode(FILE* in, FILE* out) {
    fseek(in, 1, SEEK_SET);
    int encoded_message_size = 0;
    if (fread(&encoded_message_size, sizeof(int), 1, in) != 1) {
        return WRONG_MESSAGE_SIZE;
    }
    BitStream* stream = CreateBitStream(in);
    TreeNode* tree = RestoreHuffmanTree(stream);
    int index = 0;
    enum ExitError result = SUCCESS;
    while (index < encoded_message_size) {
        result = DecodeText(tree, stream, &index, encoded_message_size, out);
    }
    free(stream);
    FreeHuffmanTree(tree);
    return result;
}
