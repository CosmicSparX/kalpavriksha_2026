#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define STACK_SIZE 100

typedef enum {
    NO_ERROR = 0,
    ERROR_DIV_BY_ZERO,
    ERROR_INVALID_EXPRESSION,
    ERROR_STACK_OVERFLOW
} ErrorCode;

static void apply_op(int* value_top, int values[], int* op_top, char ops[], ErrorCode* error_code) {
    if (*value_top < 1 || *op_top < 0) {
        *error_code = ERROR_INVALID_EXPRESSION;
        return;
    }
    int val2 = values[(*value_top)--];
    int val1 = values[(*value_top)--];
    char op = ops[(*op_top)--];
    int result;

    switch (op) {
        case '+': result = val1 + val2; break;
        case '-': result = val1 - val2; break;
        case '*': result = val1 * val2; break;
        case '/':
            if (val2 == 0) {
                *error_code = ERROR_DIV_BY_ZERO;
                return;
            }
            result = val1 / val2;
            break;
        default:
            *error_code = ERROR_INVALID_EXPRESSION;
            return;
    }
    
    if (*value_top >= STACK_SIZE - 1) {
        *error_code = ERROR_STACK_OVERFLOW;
        return;
    }
    values[++(*value_top)] = result;
}

int main() {
    char expression[256];
    int values[STACK_SIZE];
    char ops[STACK_SIZE];
    int value_top = -1;
    int op_top = -1;

    if (fgets(expression, sizeof(expression), stdin) != NULL) {
        size_t len = strlen(expression);
        if (len > 0 && expression[len - 1] == '\n') {
            expression[len - 1] = '\0';
        }

        char* p = expression;
        ErrorCode error_code = NO_ERROR;
        int expect_operand = 1;

        while (*p && error_code == NO_ERROR) {
            if (isspace((unsigned char)*p)) {
                p++;
                continue;
            }

            if (isdigit((unsigned char)*p)) {
                if (!expect_operand) {
                    error_code = ERROR_INVALID_EXPRESSION;
                    continue;
                }
                char* end_ptr;
                long val = strtol(p, &end_ptr, 10);
                
                if (value_top >= STACK_SIZE - 1) {
                    error_code = ERROR_STACK_OVERFLOW;
                    continue;
                }
                values[++value_top] = (int)val;
                p = end_ptr;
                expect_operand = 0;
            } else if (strchr("+-*/", *p)) {
                if (expect_operand) {
                    error_code = ERROR_INVALID_EXPRESSION;
                    continue;
                }

                int current_op_prec = (*p == '*' || *p == '/') ? 2 : 1;
                while (op_top > -1) {
                    int top_op_prec = (ops[op_top] == '*' || ops[op_top] == '/') ? 2 : 1;
                    if (top_op_prec < current_op_prec) break;
                    
                    apply_op(&value_top, values, &op_top, ops, &error_code);
                    if (error_code != NO_ERROR) break;
                }
                if (error_code != NO_ERROR) continue;

                if (op_top >= STACK_SIZE - 1) {
                    error_code = ERROR_STACK_OVERFLOW;
                    continue;
                }
                ops[++op_top] = *p;
                p++;
                expect_operand = 1;
            } else {
                error_code = ERROR_INVALID_EXPRESSION;
            }
        }

        if (error_code == NO_ERROR && expect_operand && strlen(expression) > 0) {
            error_code = ERROR_INVALID_EXPRESSION;
        }

        while (error_code == NO_ERROR && op_top > -1) {
            apply_op(&value_top, values, &op_top, ops, &error_code);
        }

        if (error_code == NO_ERROR && value_top != 0) {
            error_code = ERROR_INVALID_EXPRESSION;
        }

        switch (error_code) {
            case NO_ERROR:
                printf("%d\n", values[value_top--]);
                break;
            case ERROR_DIV_BY_ZERO:
                printf("Error: Division by zero.\n");
                break;
            case ERROR_INVALID_EXPRESSION:
                printf("Error: Invalid expression.\n");
                break;
            case ERROR_STACK_OVERFLOW:
                printf("Error: Expression too complex (stack overflow).\n");
                break;
        }
    }
    return 0;
}
