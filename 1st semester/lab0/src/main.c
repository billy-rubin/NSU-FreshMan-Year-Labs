#include <stdio.h>

int IsValid(char c, int b1) {
    if (c >= '0' && c <= '9') {
        return (c - '0' < b1) ? 1 : 0;
    }
    if (c >= 'A' && c <= 'F') {
        return (c - 'A' + 10 < b1) ? 1 : 0;
    }
    if (c >= 'a' && c <= 'f'){
        return (c - 'a' - 22 < b1) ? 1 : 0;
    }
    return 0;
}

int StrIsValid(int b1, char *str) {
    int point = 0;
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == '.') {
            if (point) {
                return 0;
            }
            point = 1;
        } else if (!IsValid(str[i], b1)) {
            return 0;
        }
        i++;
    }

    return 1;
}

int ConvertToInt(char symb) {
    int num = -1;
    if (symb >= '0' && symb <= '9') {
        num = (int)symb - '0';
    } else if (symb >= 'A' && symb <= 'F') {
        num = (int)symb - 'A' + 10;
    } else if (symb >= 'a' && symb <= 'f') {
        num = (int)symb - 'a' + 10;
    }
    return num;
}

char ConvertToChar(int num){
    char symb;
    if (num < 10) {
        symb = (char)(num + 48);
    } else {
        symb = (char)(num + 87);
    }
    return symb;
}

int main() {

    int b1, b2;
    const int kNumLim = 14;
    const int kFrLim = 12;

    if (scanf("%d %d", &b1, &b2) != 2) {
        return 0;
    }
    if ((b1 < 2 || b1 > 16) || (b2 < 2 || b2 > 16)) {
        printf("bad input");
        return 0;
    }

    char str[kNumLim];

    if (scanf("%13s", str) != 1) {
        return 0;
    }

    int length = 0;
    int i;
    char mas[kNumLim+1];

    for (i = 0; str[i] != '\0'; ++i) {
        mas[i] = str[i];
    }
    mas[i] = '\0';

    if (!StrIsValid(b1, mas)) {
        printf("bad input");
        return 0;
    }

    while (mas[length] != '\0') {
        length++;
    }

    int dot_index = -1;

    for (i = 0; mas[i] != '\0'; i++) {
        if (mas[i] == '.') {
            dot_index = i;
            break;
        } else {
            dot_index = -1;
        }
    }

    if (dot_index == 0 || mas[dot_index+1] == '\0'){
        printf("bad input");
        return 0;
    }

    int num, length_int = 0;
    long long sum_int = 0, b1_degree = b1;
    double sum_fr = 0;
    char integer_part[100];
    char fractional_part[100];

    if (dot_index != -1) {
        for (i = 0; i < dot_index; i++) {
            integer_part[i] = mas[i];
            num = ConvertToInt(integer_part[i]);
            sum_int *= b1;
            sum_int += num;
        }
        integer_part[dot_index] = '\0';

        int j = 0;
        for (i = dot_index + 1; mas[i] != '\0'; i++) {
            fractional_part[j] = mas[i];
            num = ConvertToInt(fractional_part[j]);
            sum_fr += (double)num / b1_degree;
            b1_degree *= b1;
            j++;
        }
        fractional_part[j] = '\0';
    } else {
        i = 0;
        while (i < length){
            integer_part[i] = mas[i];
            num = ConvertToInt(integer_part[i]);
            sum_int *= b1;
            sum_int += num;
            ++i;
        }
        integer_part[i] = '\0';
    }

    while (integer_part[length_int] != '\0') {
        length_int++;
    }

    if (sum_int == 0)
        printf("0");

    int int_i = 0;
    while (sum_int != 0) {
        integer_part[int_i] = ConvertToChar(sum_int % b2);
        sum_int /= b2;
        int_i++;
    }
    while (int_i > 0) {
        printf("%c", integer_part[int_i - 1]);
        int_i--;
    }
    if (dot_index != -1){
        printf(".");
        int fr_i = 0, nonzero_i = 0;
        while (fr_i < kFrLim) {
            sum_fr *= b2;
            fractional_part[fr_i] = ConvertToChar((int)sum_fr);
            sum_fr -= (int)sum_fr;
            if (fractional_part[fr_i] != '0') {
                nonzero_i = fr_i;
            }
            fr_i++;
        }
        for (i = 0; i <= nonzero_i; i++) {
            printf("%c", fractional_part[i]);
        }
    }
    return 0;
}
