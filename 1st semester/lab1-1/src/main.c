#include <stdio.h>
#include <string.h>
#define EXIT_SUCCESS 0
#define MAX_PATTERN_SIZE 18
#define MAX_SIZE 100
#define NEW_STR_SYMBOL 1

typedef unsigned long long ull;
typedef unsigned char uchar;

const int kDegreeOfThree[16] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907};

size_t CalcHashChar(char c, int mult) {
  return ((ull)c % 3) * mult;
}

size_t UpdateHash(size_t old_hash, char old_char, int new_char, int length) {
  return (old_hash - (uchar)old_char % 3) / 3 +
         CalcHashChar(new_char, kDegreeOfThree[length - 1]);
}

int CalcHashStr(char* str, int start, int length) {
  int hash = 0;
  for (int i = start; i < length; i++) {
    hash += CalcHashChar((ull)str[i], kDegreeOfThree[i - start]);
  }
  return hash;
}

void FindSubstring(char* pattern, char* text, int pattern_len, int text_len, int pattern_hash, int count) {
  int text_hash = CalcHashStr(text, 0, pattern_len);
  for (int index = 0; index <= text_len - pattern_len; index++) {
    if (pattern_hash == text_hash) {
      int i = 0;
      while (i < pattern_len && i + index < text_len && (ull)pattern[i] == (ull)text[i + index]) {
        printf("%d ", index + count + 1 + i++);
      }
      if (i < pattern_len)
        printf("%d ", index + count + 1 + i);
    }
    text_hash = UpdateHash(text_hash, text[index], text[index + pattern_len], pattern_len);
  }
}

int main() {
  char pattern[MAX_PATTERN_SIZE];
  char text[MAX_SIZE];

  if (fgets(pattern, MAX_PATTERN_SIZE, stdin) == NULL) {
    return 1;
  }

  int pattern_len = strlen(pattern) - 1;
  int pattern_hash = CalcHashStr(pattern, 0, pattern_len);
  printf("%d\n", pattern_hash);

  ull count = 0;
  while (fgets(text, MAX_SIZE, stdin)) {
    int text_len = strlen(text);
    if (text[text_len - 1] == '\n')
      text_len--;

    FindSubstring(pattern, text, pattern_len, text_len, pattern_hash, count);
    count = count + text_len + NEW_STR_SYMBOL;
  }
  return EXIT_SUCCESS;
}
