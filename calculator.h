#ifndef CALCULATOR
#define CALCULATOR
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

double calculateOperator(double left, double right, Operator operator);

void processOperator(struct Token **operators_stack, int * index_of_operators, double *output_stack, int * index_of_output) {
    if (operators_stack[(*index_of_operators)]->operator == NEG) {
        output_stack[*index_of_output] *= -1;
        (*index_of_operators)--;
    } else {
        double num = calculateOperator(output_stack[*index_of_output - 1],
                                       output_stack[*index_of_output],
                                       operators_stack[(*index_of_operators)--]->operator);
        (*index_of_output)--;
        output_stack[*index_of_output] = num;
    }
}

double calculateExpressionTokens(struct TokenArray tokenArray) {
    struct Token *tokens = tokenArray.tokens;
    const int size = tokenArray.size;
    double *output_stack = malloc(size * sizeof(double));
    struct Token **operators_stack = malloc(size * sizeof(struct Token));
    int index_of_output = -1;
    int index_of_operators = -1;
    for (int i = 0; i < size; ++i) {
        if (tokens[i].operator == NOOP) {
            output_stack[++index_of_output] = tokens[i].number;
        } else if (tokens[i].operator == L_P) {
            operators_stack[++index_of_operators] = &tokens[i];
        } else if (tokens[i].operator == R_P) {
            while (operators_stack[index_of_operators]->operator != L_P) {
                if (index_of_operators < 0) {
                    printf("ERROR: there are mismatched parentheses");
                    abort();
                }
                processOperator(operators_stack, &index_of_operators, output_stack, &index_of_output);
                if (operators_stack[index_of_operators]->operator == L_P) {
                    index_of_operators--;
                    break;
                }
            }
        } else {
            while (index_of_operators != -1
                   && operators_stack[index_of_operators]->operator != L_P
                   && getOperatorPrecedence(tokens[i].operator) <=
                      getOperatorPrecedence(operators_stack[index_of_operators]->operator)) {
                processOperator(operators_stack, &index_of_operators, output_stack, &index_of_output);
            }
            operators_stack[++index_of_operators] = &tokens[i];
        }
    }
    while (index_of_operators >= 0) {
        if (operators_stack[index_of_operators]->operator == L_P
            || operators_stack[index_of_operators]->operator == R_P) {
            printf("ERROR: there are mismatched parenthesis");
            abort();
        }
        processOperator(operators_stack, &index_of_operators, output_stack, &index_of_output);
    }
    double result = index_of_output == -1 ? 0 : output_stack[0];
    free(operators_stack);
    free(output_stack);
    return result;
}

double calculateOperator(double left, double right, Operator operator) {
    switch (operator) {
        case MUL:
            return left * right;
        case DIV:
            return left / right;
        case ADD:
            return left + right;
        case SUB:
            return left - right;
        default:
            printf("Illegal state: Operator is not defined");
            abort();
    }
}
#endif