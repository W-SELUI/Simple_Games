#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int accNo;
    char name[50];
    int pin;
    float balance;
} Account;

void createAccount() {
    Account acc;
    printf("Enter account number: ");
    scanf("%d", &acc.accNo);
    printf("Enter name: ");
    scanf("%s", acc.name);
    printf("Set PIN (4 digits): ");
    scanf("%d", &acc.pin);
    acc.balance = 0;

    FILE* fp = fopen("accounts.dat", "ab");
    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

Account* findAccount(int accNo, int pin) {
    static Account acc;
    FILE* fp = fopen("accounts.dat", "rb");
    if (!fp) return NULL;

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNo == accNo && acc.pin == pin) {
            fclose(fp);
            return &acc;
        }
    }
    fclose(fp);
    return NULL;
}

void updateAccount(Account* acc) {
    FILE* fp = fopen("accounts.dat", "rb+");
    Account temp;
    long pos;

    while (fread(&temp, sizeof(Account), 1, fp)) {
        if (temp.accNo == acc->accNo) {
            pos = ftell(fp) - sizeof(Account);
            fseek(fp, pos, SEEK_SET);
            fwrite(acc, sizeof(Account), 1, fp);
            break;
        }
    }
    fclose(fp);
}

void deposit() {
    int accNo, pin;
    float amount;
    printf("Account number: ");
    scanf("%d", &accNo);
    printf("PIN: ");
    scanf("%d", &pin);

    Account* acc = findAccount(accNo, pin);
    if (!acc) {
        printf("Invalid account or PIN!\n");
        return;
    }

    printf("Amount to deposit: $");
    scanf("%f", &amount);
    acc->balance += amount;
    updateAccount(acc);
    printf("Deposited $%.2f. New balance: $%.2f\n", amount, acc->balance);
}

void withdraw() {
    int accNo, pin;
    float amount;
    printf("Account number: ");
    scanf("%d", &accNo);
    printf("PIN: ");
    scanf("%d", &pin);

    Account* acc = findAccount(accNo, pin);
    if (!acc) {
        printf("Invalid account or PIN!\n");
        return;
    }

    printf("Amount to withdraw: $");
    scanf("%f", &amount);
    if (amount > acc->balance) {
        printf("Insufficient balance!\n");
        return;
    }
    acc->balance -= amount;
    updateAccount(acc);
    printf("Withdrew $%.2f. New balance: $%.2f\n", amount, acc->balance);
}

void checkBalance() {
    int accNo, pin;
    printf("Account number: ");
    scanf("%d", &accNo);
    printf("PIN: ");
    scanf("%d", &pin);

    Account* acc = findAccount(accNo, pin);
    if (!acc) {
        printf("Invalid account or PIN!\n");
        return;
    }
    printf("Balance: $%.2f\n", acc->balance);
}

int main() {
    int choice;

    printf("=== Bank Account Management ===\n");

    while (1) {
        printf("\n1. Create Account\n2. Deposit\n3. Withdraw\n4. Check Balance\n5. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: checkBalance(); break;
            case 5: printf("Goodbye!\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}

