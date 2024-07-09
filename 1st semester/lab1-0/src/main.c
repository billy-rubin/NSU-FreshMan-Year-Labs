#include <stdio.h>
#include <string.h>

#define SHIFT_CHARS 256
#define MAX_PATTERN_SIZE 16
#define MAX_BUFFER_SIZE 4096

typedef unsigned char uchar;

void ShiftingTable(uchar* pattern, int size, uchar* stop_shift) {
  for (int i = 0; i < SHIFT_CHARS; i++) {
    stop_shift[i] = size;
  }
  for (int i = 0; i < size - 1; i++) {
    stop_shift[(int)pattern[i]] -= i + 1;
  }
}

void BoyerMoor(uchar* buf, uchar* pattern, int pattern_len, uchar* chars_shift, long long* global_shift) {
  int buf_len = (int)strlen((char*)buf);
  long long local_shift = 0;
  while (local_shift <= (buf_len - pattern_len)) {
    int count_mismatch = pattern_len - 1;
    while (count_mismatch != -1 &&
           pattern[count_mismatch] == buf[local_shift + count_mismatch]) {
      printf("%lld ", *global_shift + local_shift + count_mismatch + 1);
      count_mismatch--;
    }
    if (count_mismatch == -1) {
      local_shift += (local_shift + pattern_len < buf_len) ? pattern_len : 1;
    } else {
      printf("%lld ", *global_shift + local_shift + count_mismatch + 1);
      if (chars_shift[(int)buf[local_shift + pattern_len - 1]] > 1) {
        local_shift += chars_shift[(int)buf[local_shift + pattern_len - 1]];
      } else {
        local_shift++;
      }
    }
  }
  *global_shift += local_shift;
}

int main() {
  uchar pattern[MAX_PATTERN_SIZE + 1], buf[MAX_BUFFER_SIZE + 1],
      chars_shift[SHIFT_CHARS];
  FILE* input = fopen("in.txt", "r");

  int symb, i = 0;
  while ((symb = fgetc(input)) != EOF && symb != '\n') {
    pattern[i++] = symb;
  }
  pattern[i] = '\0';
  int pattern_len = (int)strlen((char*)pattern);
  ShiftingTable(pattern, pattern_len, chars_shift);

  int buffer_len = (int)fread(buf, sizeof(char), MAX_BUFFER_SIZE, input);

  if (buffer_len == 0) {
    fclose(input);
    return 0;
  }

  long long global_shift = 0;
  while (buffer_len == MAX_BUFFER_SIZE) {
    buf[MAX_BUFFER_SIZE] = '\0';
    BoyerMoor(buf, pattern, pattern_len, chars_shift, &global_shift);
    memset(buf, '\0', sizeof(buf));
    buffer_len = (int)fread(buf, sizeof(char), MAX_BUFFER_SIZE, input);
  }
  buf[buffer_len] = '\0';

  BoyerMoor(buf, pattern, pattern_len, chars_shift, &global_shift);

  fclose(input);
  return 0;
}
