#include <stdlib.h>
#include <string.h>
#include "bitstream.h"

BitStream* CreateBitStream(FILE* file) {
    BitStream* stream = malloc(sizeof(BitStream));
    stream->file = file;
    stream->buffer = 0;
    stream->bit_count = 0;
    return stream;
}

void WriteBit(BitStream* stream, int bit) {
    if (stream->bit_count == BITS_IN_BYTE) {
        fwrite(&(stream->buffer), sizeof(uchar), 1, stream->file);
        stream->buffer = 0;
        stream->bit_count = 0;
    }
    stream->buffer = (stream->buffer << 1) | bit;
    stream->bit_count++;
}

void WriteHuffmanTree(BitStream* stream, TreeNode* tree) {
    if (tree->left == NULL && tree->right == NULL) {
        WriteBit(stream, 0);
        for (int i = 0; i < BITS_IN_BYTE; i++) {
            WriteBit(stream, (tree->symbol >> (BITS_IN_BYTE - 1 - i)) & 1);
        }
    } else {
        WriteBit(stream, 1);
        WriteHuffmanTree(stream, tree->left);
        WriteHuffmanTree(stream, tree->right);
    }
}

void WriteSymbol(BitStream* stream, char* code_table[], uchar symbol){
    for (unsigned int i = 0; i < strlen(code_table[symbol]); i++) {
        WriteBit(stream, (code_table[symbol][i] == '1') ? 1 : 0);
    }
}

void ReleaseBitStream(BitStream* stream) {
    if (stream->bit_count > 0) {
        stream->buffer <<= (BITS_IN_BYTE - stream->bit_count);
        fwrite(&stream->buffer, sizeof(uchar), 1, stream->file);
    }
    free(stream);
}

int ReadBit(BitStream* stream) {
    if (stream->bit_count == 0) {
        stream->buffer = fgetc(stream->file);
        stream->bit_count = BITS_IN_BYTE;
    }
    int bit = ((stream->buffer & (1 << (stream->bit_count - 1))) != 0);
    stream->bit_count--;
    return bit;
}

uchar ReadSymbol(BitStream* stream) {
    uchar symbol = 0;
    for (int i = 0; i < BITS_IN_BYTE; i++) {
        if (ReadBit(stream)) {
            symbol |= (1 << (BITS_IN_BYTE - 1 - i));
        }
    }
    return symbol;
}

TreeNode* RestoreHuffmanTree(BitStream* stream) {
    if (ReadBit(stream) == 0) {
        TreeNode* leaf = (TreeNode*)malloc(sizeof(TreeNode));
        leaf->left = leaf->right = NULL;
        leaf->symbol = ReadSymbol(stream);
        return leaf;
    } else {
        TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
        node->left = RestoreHuffmanTree(stream);
        node->right = RestoreHuffmanTree(stream);
        return node;
    }
}

enum ExitError DecodeText(TreeNode* current_node, BitStream* stream, int* index, int encoded_message_size, FILE* out) {
    if (current_node == NULL) {
        return TREE_STRUCTURE_ERROR;
    }
    if (current_node->left == NULL && current_node->right == NULL) {
        fwrite(&current_node->symbol, sizeof(uchar), 1, out);
        (*index)++;
    } else {
        if (*index == encoded_message_size) {
            return SUCCESS;
        }
        int bit = ReadBit(stream);
        if (bit) {
            DecodeText(current_node->right, stream, index, encoded_message_size, out);
        } else {
            DecodeText(current_node->left, stream, index, encoded_message_size, out);
        }
    }
    return SUCCESS;
}
