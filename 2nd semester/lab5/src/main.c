#include <stdio.h>
#include "huffman.h"

int main() {
    FILE* in = fopen("in.txt", "rb+");
    FILE* out = fopen("out.txt", "wb+");

    char mode;
    if (fread(&mode, sizeof(char), 1, in) != 1) {
        fclose(in);
        fclose(out);
        return 0;
    }
    switch(mode){
        case('c'):
            Encode(in, out);
            break;
        case ('d'):
            Decode(in, out);
            break;
        default:
            break;
    }
    fclose(in);
    fclose(out);
    return 0;
}
