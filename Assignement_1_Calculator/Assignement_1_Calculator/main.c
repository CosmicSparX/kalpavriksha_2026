#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define STACK_SIZE 100

int main() {
    char expression[256];
    int values[STACK_SIZE];
    char ops[STACK_SIZE];
    int value_top = -1;
    int op_top = -1;

    if (fgets(expression, sizeof(expression), stdin) != NULL) {
        int len = strlen(expression);
        if (len > 0 && expression[len - 1] == '\n') {
            expression[len - 1] = '\0';
        }

        char* p = expression;
        int error_code = 0;
        int expect_operand = 1;

        while (*p && !error_code) {
            if (isspace(*p)) {
                p++;
                continue;
            }

            if (isdigit(*p)) {
                if (!expect_operand) {
                    error_code = 2;
                    continue;
                }
                char* end_ptr;
                long val = strtol(p, &end_ptr, 10);
                if (value_top < STACK_SIZE - 1) {
                    values[++value_top] = (int)val;
                }
                p = end_ptr;
                expect_operand = 0;
            } else if (strchr("+-*/", *p)) {
                if (expect_operand) {
                    error_code = 2;
                    continue;
                }

                int current_op_prec = (*p == '*' || *p == '/') ? 2 : 1;

                while (op_top > -1) {
                    char top_op = ops[op_top];
                    int top_op_prec = (top_op == '*' || top_op == '/') ? 2 : 1;
                    if (top_op_prec < current_op_prec) break;

                    if (value_top < 1) {
                        error_code = 2;
                        break;
                    }
                    int val2 = values[value_top--];
                    int val1 = values[value_top--];
                    char op = ops[op_top--];
                    int result;
                    switch (op) {
                        case '+':
                            result = val1 + val2; 
                            break;
                        case '-':
                            result = val1 - val2; 
                            break;
                        case '*':
                            result = val1 * val2; 
                            break;
                        case '/':
                            if (val2 == 0) {
                                error_code = 1;
                            } else {
                                result = val1 / val2;
                            }
                            break;
                        default: 
                            result = 0; 
                            break;
                    }
                    if (error_code) break;
                    if (value_top < STACK_SIZE - 1) {
                        values[++value_top] = result;
                    }
                }
                if (error_code) break;

                if (op_top < STACK_SIZE - 1) {
                    ops[++op_top] = *p;
                }
                p++;
                expect_operand = 1;
            } else {
                error_code = 2;
            }
        }

        if (expect_operand && op_top > -1 && strlen(expression) > 0) {
            error_code = 2;
        }

        while (!error_code && op_top > -1) {
            if (value_top < 1) {
                error_code = 2;
                break;
            }
            int val2 = values[value_top--];
            int val1 = values[value_top--];
            char op = ops[op_top--];
            int result;
            switch (op) {
                case '+': 
                    result = val1 + val2; 
                    break;
                case '-': 
                    result = val1 - val2;  
                    break;
                case '*':  
                    result = val1 * val2; 
                    break;
                case '/':
                    if (val2 == 0) {
                        error_code = 1;
                    } else {
                        result = val1 / val2;
                    }
                    break;
                default: 
                    result = 0; 
                    break;
            }
            if (error_code) break;
            if (value_top < STACK_SIZE - 1) {
                values[++value_top] = result;
            }
        }

        if (error_code == 1) {
            printf("Error: Division by zero.\n");
        } else if (error_code == 2 || value_top != 0) {
            printf("Error: Invalid expression.\n");
        } else {
            printf("%d\n", values[value_top--]);
        }
    }
    return 0;
}
