#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stdio.h>
#include "huffman.h"

typedef struct BitStream {
    FILE* file;
    uchar buffer;
    int bit_count;
} BitStream;

BitStream* CreateBitStream(FILE* file);
void WriteBit(BitStream* stream, int bit);
void WriteHuffmanTree(BitStream* stream, TreeNode* tree);
void WriteSymbol(BitStream* stream, char* code_table[], uchar symbol);
void ReleaseBitStream(BitStream* stream);
int ReadBit(BitStream* stream);
uchar ReadSymbol(BitStream* stream);
TreeNode* RestoreHuffmanTree(BitStream* stream);
enum ExitError DecodeText(TreeNode* tree, BitStream* stream, int* index,  int encoded_message_size, FILE* out);

#endif // BITSTREAM_H
