#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ARRAY_SIZE 1000

enum Operations {
  DIVISION = 47,
  MULTIPLICATION = 42,
  ADDITION = 43,
  SUBTRACTION = 45,
};

enum Type {
  NUM = 0,
  BRACKET = 1,
  SUM = 2,
  COMP = 3,
};

struct List {
  int type;  // 0 - число, 1 - открывающая скобка, 2 - плюс и минус, 3 -
             // умножить, поделить
  long long value;
  struct List* next;
};

void Push(struct List** stack, int type, long long value) {
  struct List* elem = (struct List*)malloc(sizeof(struct List));
  elem->type = type;
  elem->value = value;
  elem->next = *stack;
  *stack = elem;
}

long long Pop(struct List** stack) {
  if (*stack == NULL) {
    return 0;
  }
  struct List* tmp = *stack;
  *stack = tmp->next;
  long long value = tmp->value;
  free(tmp);
  return value;
}

void FreeStack(struct List* stack) {
  while (stack) {
    struct List* tmp = stack;
    stack = stack->next;
    free(tmp);
  }
}

int GetPriority(char symbol) {
  switch (symbol) {
    case '*':
    case '/':
      return COMP;
    case '-':
    case '+':
      return SUM;
    case '(':
      return BRACKET;
    default:
      return NUM;
  }
}

int ConvertToPostfix(const char* infix, const int len, long long* postfix) {
  struct List* operator_stack = NULL;
  int count_cl = 0;
  int count_op = 0;
  int num_flag = 0;
  int postfix_len = 0;

  for (int i = 0; i < len; i++) {
    char current_char = infix[i];
    if (current_char >= '0' && current_char <= '9') {
      long long current_num = 0;
      while (infix[i] >= '0' && infix[i] <= '9') {
        current_num = current_num * 10 + infix[i++] - '0';
      }
      i--;
      postfix[postfix_len++] = current_num;
      num_flag = 1;
    } else if (current_char == '(') {
      Push(&operator_stack, BRACKET, current_char);
      count_op++;
    } else if (current_char == '+' || current_char == '-' ||
               current_char == '*' || current_char == '/') {
      if ((i > 0 && (infix[i - 1] == '+' || infix[i - 1] == '-' ||
                     infix[i - 1] == '*' || infix[i - 1] == '/')) ||
          i == len - 1) {
        printf("syntax error");
        exit(EXIT_SUCCESS);
      }
      while (operator_stack != NULL && operator_stack->type != BRACKET &&
             operator_stack->type >= GetPriority(current_char)) {
        postfix[postfix_len++] = Pop(&operator_stack);
      }
      if (current_char == '+' || current_char == '-') {
        Push(&operator_stack, SUM, current_char);
      } else {
        Push(&operator_stack, COMP, current_char);
      }
    } else if (current_char == ')') {
      while (operator_stack != NULL && operator_stack->value != '(') {
        postfix[postfix_len++] = Pop(&operator_stack);
      }
      if ((i > 0 && infix[i - 1] == '(') || operator_stack == NULL) {
        printf("syntax error");
        exit(EXIT_SUCCESS);
      }
      Pop(&operator_stack);
      count_cl++;
    }
  }

  if (!num_flag) {
    printf("syntax error");
    exit(EXIT_SUCCESS);
  } else {
    if (operator_stack == NULL && (count_cl != count_op) && (count_op != 0)) {
      printf("syntax error");
      exit(EXIT_SUCCESS);
    } else if (operator_stack == NULL && (count_cl == count_op) &&
               (count_op != 0)) {
      postfix[postfix_len] = '\0';
      return postfix_len;
    } else {
      while (operator_stack != NULL) {
        postfix[postfix_len++] = Pop(&operator_stack);
      }
    }
  }
  FreeStack(operator_stack);
  postfix[postfix_len] = '\0';
  return postfix_len;
}

long long Calculate(char* infix, int len) {
  long long postfix[MAX_ARRAY_SIZE];
  int postfix_len = ConvertToPostfix(infix, len, postfix);
  struct List* operand_stack = NULL;
  for (int i = 0; i < postfix_len; i++) {
    if (postfix[i] != ADDITION && postfix[i] != DIVISION &&
        postfix[i] != SUBTRACTION && postfix[i] != MULTIPLICATION) {
      Push(&operand_stack, NUM, postfix[i]);
    } else {
      long long operand2 = operand_stack->value;
      Pop(&operand_stack);
      long long operand1 = operand_stack->value;
      Pop(&operand_stack);
      switch (postfix[i]) {
        case ADDITION:
          Push(&operand_stack, NUM, operand1 + operand2);
          break;
        case SUBTRACTION:
          Push(&operand_stack, NUM, operand1 - operand2);
          break;
        case MULTIPLICATION:
          Push(&operand_stack, NUM, operand1 * operand2);
          break;
        case DIVISION:
          if (operand2 == 0) {
            printf("division by zero\n");
            exit(EXIT_SUCCESS);
          }
          Push(&operand_stack, NUM, operand1 / operand2);
          break;
        default:
          printf("syntax error\n");
          exit(EXIT_SUCCESS);
      }
    }
  }
  if (operand_stack != NULL) {
    long long finalResult = operand_stack->value;
    if (operand_stack->next == NULL) {
      FreeStack(operand_stack);
      return finalResult;
    } else {
      FreeStack(operand_stack);
      printf("syntax error\n");
      exit(EXIT_SUCCESS);
    }
  } else {
    FreeStack(operand_stack);
    printf("syntax error\n");
    exit(EXIT_SUCCESS);
  }
}

int main() {
  char input_string[MAX_ARRAY_SIZE + 2];
  if (fgets(input_string, sizeof(input_string), stdin) == NULL) {
    return 0;
  }

  long long result = Calculate(input_string, strlen(input_string) - 1);
  printf("%lld", result);
  return 0;
}
