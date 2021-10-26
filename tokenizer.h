#include <stdlib.h>
#include <string.h>

#define Operator int
#define MUL 1
#define DIV 2
#define ADD 3
#define SUB 4
#define L_P 5
#define R_P 6
#define NEG 7
#define NOOP 8

struct Token {
    double number;
    Operator operator;
};

Operator convertToOperator(char op, int isExpressionExpected);


int getOperatorPrecedence(Operator op) {
    switch (op) {
        case NEG:
            return 3;
        case MUL:
        case DIV:
            return 2;
        case SUB:
        case ADD:
            return 1;
        default:
            printf("Illegal state: Precedence for Operator \"%d\" is not defined", op);
            abort();
    }
}

double toDouble(const char *s, int start, int stop) {
    double ret = 0;
    int pointEncountered = 0;
    int digitsAfterPoint = 0;
    for (int i = start; i <= stop; ++i) {
        if (s[i] != '.') {
            ret = ret * 10 + (s[i] - '0');
            if (pointEncountered == 1) {
                digitsAfterPoint++;
            }
        } else {
            pointEncountered = 1;
        }
    }
    if (pointEncountered == 1) {
        for (int i = 0; i < digitsAfterPoint; ++i) {
            ret /= 10;
        }
    }
    return ret;
}

struct TokenArray{
    struct Token * tokens;
    int size;
};

struct TokenArray tokenize(const char *expression_string, unsigned int sizeOfStr) {
    int t = 0;
    struct Token *tokens = malloc(sizeOfStr * sizeof(struct Token));
    int numStart = -1;
    int numEnd;
    int isExpressionExpected = 1;
    for (int i = 0; i < sizeOfStr; ++i) {
        char currentChar = expression_string[i];
        char nextChar = expression_string[i + 1];
        if (currentChar == ' ') continue;
        if (currentChar == '.' || (currentChar <= '9' && currentChar >= '0')) {
            if (numStart == -1) {
                numStart = i;
            }
            if (nextChar != '.' && !(nextChar <= '9' && nextChar >= '0')) {
                numEnd = i;
                tokens[t].number = toDouble(expression_string, numStart, numEnd);
                tokens[t].operator = NOOP;
                numStart = -1;
                isExpressionExpected = 0;
                t++;

            }
        } else {
            tokens[t].operator = convertToOperator(currentChar, isExpressionExpected);
            if (tokens[t].operator == NOOP || tokens[t].operator == R_P) {
                isExpressionExpected = 0;
            } else isExpressionExpected = 1;
            t++;
        }
    }
    struct TokenArray result;
    result.tokens = tokens;
    result.size = t;
    return result;
}

Operator convertToOperator(char op, int isExpressionExpected) {
    switch (op) {
        case '*':
            return MUL;
        case '/':
            return DIV;
        case '+':
            return ADD;
        case '-':
            if (isExpressionExpected == 1) {
                return NEG;
            } else return SUB;
        case '(':
            return L_P;
        case ')':
            return R_P;
        default:
            return NOOP;
    }
}