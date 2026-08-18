#include <stdio.h>

int main() {
    char op;
    double a, b, result;

    printf("=== Simple Calculator ===\n");
    printf("Type 'q' as operator to quit.\n\n");

    while (1) {
        printf("Enter operator (+, -, *, /): ");
        scanf(" %c", &op);

        if (op == 'q') {
            printf("Goodbye!\n");
            break;
        }

        if (op != '+' && op != '-' && op != '*' && op != '/') {
            printf("Invalid operator!\n\n");
            continue;
        }

        printf("Enter first number: ");
        scanf("%lf", &a);
        printf("Enter second number: ");
        scanf("%lf", &b);

        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Error: Division by zero!\n\n");
                    continue;
                }
                result = a / b;
                break;
        }

        printf("Result: %.2lf\n\n", result);
    }

    return 0;
}

