#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 50

typedef struct {
    char username[20];
    char password[20];
} Admin;

typedef struct {
    int acc_no;
    char name[50];
    char password[20];
    float balance;
} Account;

Admin adminData;
Account accounts[MAX];
int count = 0;


// ----------------------------------------------------
// Load Admin
// ----------------------------------------------------
int loadAdmin() {
    FILE *fp = fopen("admin.dat", "rb");
    if (fp == NULL) return 0;   // no admin created
    fread(&adminData, sizeof(Admin), 1, fp);
    fclose(fp);
    return 1;
}

// ----------------------------------------------------
// Save Admin
// ----------------------------------------------------
void saveAdmin() {
    FILE *fp = fopen("admin.dat", "wb");
    fwrite(&adminData, sizeof(Admin), 1, fp);
    fclose(fp);
}

// ----------------------------------------------------
// Admin Creation
// ----------------------------------------------------
void createAdmin() {
    printf("\n=== Create Admin Account ===\n");
    printf("Enter Admin Username: ");
    scanf("%s", adminData.username);

    printf("Create Admin Password: ");
    scanf("%s", adminData.password);

    saveAdmin();
    printf("\nAdmin account created successfully!\n");
}


// ----------------------------------------------------
// Load User Accounts
// ----------------------------------------------------
void loadAccounts() {
    FILE *fp = fopen("accounts.dat", "rb");
    if (fp == NULL) return;

    fread(&count, sizeof(int), 1, fp);
    fread(accounts, sizeof(Account), count, fp);
    fclose(fp);
}

// ----------------------------------------------------
// Save User Accounts
// ----------------------------------------------------
void saveAccounts() {
    FILE *fp = fopen("accounts.dat", "wb");
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(accounts, sizeof(Account), count, fp);
    fclose(fp);
}


// ----------------------------------------------------
// Generate Random Unique Account Number
// ----------------------------------------------------
int generateAccountNumber() {
    int num, i, exists;

    do {
        exists = 0;
        num = 10000 + rand() % 90000; // random 5-digit number

        for (i = 0; i < count; i++) {
            if (accounts[i].acc_no == num) {
                exists = 1;
                break;
            }
        }
    } while (exists);

    return num;
}


// ----------------------------------------------------
// Admin: Create User Account
// ----------------------------------------------------
void createUserAccount() {
    Account a;
    int i;

    if (count >= MAX) {
        printf("\nMaximum account limit reached!\n");
        return;
    }

    a.acc_no = generateAccountNumber();
    printf("\nGenerated Account Number: %d\n", a.acc_no);

    printf("Enter User Name: ");
    scanf("%s", a.name);

    printf("Create Password for User: ");
    scanf("%s", a.password);

    printf("Initial Deposit: ");
    scanf("%f", &a.balance);

    accounts[count] = a;
    count++;

    saveAccounts();

    printf("\nUser Account Created Successfully!\n");
}


// ----------------------------------------------------
// Admin: View All User Accounts
// ----------------------------------------------------
void viewAccounts() {
    int i;
    printf("\n=== User Accounts ===\n");

    for (i = 0; i < count; i++) {
        printf("Acc No: %d | Name: %s | Balance: %.2f\n",
               accounts[i].acc_no,
               accounts[i].name,
               accounts[i].balance);
    }
}


// ----------------------------------------------------
// User Deposit
// ----------------------------------------------------
void deposit(Account *a) {
    float amt;
    printf("Enter amount to deposit: ");
    scanf("%f", &amt);

    a->balance += amt;
    saveAccounts();

    printf("Amount Deposited!\n");
}


// ----------------------------------------------------
// User Withdraw
// ----------------------------------------------------
void withdraw(Account *a) {
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);

    if (amt > a->balance) {
        printf("Insufficient Balance!\n");
        return;
    }

    a->balance -= amt;
    saveAccounts();

    printf("Amount Withdrawn!\n");
}


// ----------------------------------------------------
// User Balance Check
// ----------------------------------------------------
void balance(Account *a) {
    printf("Your Balance: %.2f\n", a->balance);
}


// ----------------------------------------------------
// User Menu
// ----------------------------------------------------
void userMenu(Account *a) {
    int ch;

    while (1) {
        printf("\n1. Deposit\n2. Withdraw\n3. Balance\n4. Logout\nChoice: ");
        scanf("%d", &ch);

        if (ch == 1) deposit(a);
        else if (ch == 2) withdraw(a);
        else if (ch == 3) balance(a);
        else if (ch == 4) return;
        else printf("Invalid Choice!\n");
    }
}


// ----------------------------------------------------
// User Login
// ----------------------------------------------------
void userLogin() {
    int acc_no, i;
    char pass[20];

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    printf("Enter Password: ");
    scanf("%s", pass);

    for (i = 0; i < count; i++) {
        if (accounts[i].acc_no == acc_no &&
            strcmp(accounts[i].password, pass) == 0) {

            printf("\nLogin Successful!\n");
            userMenu(&accounts[i]);
            return;
        }
    }

    printf("\nInvalid Login!\n");
}


// ----------------------------------------------------
// Admin Login
// ----------------------------------------------------
void adminLogin() {
    char u[20], p[20];

    printf("Admin Username: ");
    scanf("%s", u);

    printf("Admin Password: ");
    scanf("%s", p);

    if (strcmp(u, adminData.username) == 0 &&
        strcmp(p, adminData.password) == 0) {

        int ch;
        while (1) {
            printf("\n1. Create User Account\n2. View Accounts\n3. Logout\nChoice: ");
            scanf("%d", &ch);

            if (ch == 1) createUserAccount();
            else if (ch == 2) viewAccounts();
            else if (ch == 3) return;
            else printf("Invalid Choice!\n");
        }

    } else {
        printf("Invalid Admin Login!\n");
    }
}


// ----------------------------------------------------
// Main
// ----------------------------------------------------
int main() {
    srand(time(NULL));
    loadAccounts();

    int adminExists = loadAdmin();
    int ch;

    while (1) {
        printf("\n=== Banking System ===\n");

        if (!adminExists)
            printf("1. Create Admin Account\n");
        else
            printf("1. Admin Login\n");

        printf("2. User Login\n3. Exit\nChoice: ");
        scanf("%d", &ch);

        if (ch == 1) {
            if (!adminExists) {
                createAdmin();
                adminExists = 1;
            } else adminLogin();
        }
        else if (ch == 2) userLogin();
        else if (ch == 3) exit(0);
        else printf("Invalid Choice!\n");
    }

    return 0;
}
