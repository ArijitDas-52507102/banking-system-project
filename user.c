#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "global.h"
#include <time.h>

void userPortalGateway(void) {
    int choice;
    while (1) {
        printf("\n \n");
        printf("  RETAIL BANKING OPERATIONS GATEWAY CLIENT PORTAL  \n");
        printf("\n");
        printf("1. Create New Consumer Bank Account\n");
        printf("2. Log In to Account Portal\n");
        printf("3. Return to Main Dashboard Network Router\n");
        printf("\n");
        printf("Enter Option: ");
        scanf("%d", &choice);
        clearBuffer();

        if (choice == 3) return;
        switch (choice) {
            case 1: userRegistrationForm(); break;
            case 2: userAuthenticationLogin(); break;
            default: printf("Invalid vector map index.\n");
        }
    }
}


int accountNumberExists(const char *accountNumber)
{
    FILE *f = fopen(USERS_TXT, "r");
    if (f == NULL)
        return 0;

    char existing[MAX_STR];
    int ch;

    while (fscanf(f, "%49s", existing) == 1) {
        if (strcmp(existing, accountNumber) == 0) {
            fclose(f);
            return 1;
        }

        while ((ch = fgetc(f)) != '\n' && ch != EOF) {
        }
    }

    fclose(f);
    return 0;
}

void generateAccountNumber(char *accountNumber)
{
    static int seeded = 0;

    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    do {
        unsigned int number =
            1000000000U + (unsigned int)(rand() % 900000000);

        snprintf(accountNumber, MAX_STR, "%010u", number);

    } while (accountNumberExists(accountNumber));
}


void userRegistrationForm(void) {
    FILE *bf = fopen(BANKS_TXT, "r");

    if (!bf) {
        printf("Error: Cannot open bank database.\n");
        return;
    }

    struct SubBank b;
    int availableBanks = 0;

    printf("\nAvailable Approved Banks:\n");

    while (fscanf(bf, "%*d %49s %d %*f %*d %*d",
                  b.bankName, &b.isApproved) == 2) {
        if (b.isApproved == 1) {
            printf("Bank: %s\n", b.bankName);
            availableBanks++;
        }
    }

    fclose(bf);

    if (availableBanks == 0) {
        printf("No approved banks are available.\n");
        return;
    }

    struct UserAccount u = {0};

    printf("\nEnter Bank Name: ");
    scanf("%49s", u.linkedBankName);
    clearBuffer();

    bf = fopen(BANKS_TXT, "r");

    if (!bf) {
        printf("Error: Cannot verify bank.\n");
        return;
    }

    int valid = 0;

    while (fscanf(bf, "%*d %49s %d %*f %*d %*d",
                  b.bankName, &b.isApproved) == 2) {
        if (b.isApproved == 1 &&
            strcmp(b.bankName, u.linkedBankName) == 0) {
            valid = 1;
            break;
        }
    }

    fclose(bf);

    if (!valid) {
        printf("Invalid or unapproved bank.\n");
        return;
    }


    generateAccountNumber(u.accountNumber);

    printf("\nBank-generated Account Number: %s\n",
           u.accountNumber);

    printf("Create a 5-digit PIN: ");
    scanf("%d", &u.pin);
    clearBuffer();

    if (u.pin < 10000 || u.pin > 99999) {
        printf("Invalid PIN. Use exactly 5 digits.\n");
        return;
    }

    printf("Enter Full Name (no spaces): ");
    scanf("%49s", u.userName);
    clearBuffer();


    u.balance = 0.0;
    u.activeLoan = 0.0;
    u.loanDueDays = 0;
    u.bondInvestment = 0.0;
    u.totalProfitsEarned = 0.0;
    u.hasCard = 0;
    strcpy(u.cardNumber, "None");
    u.cardPin = 0;

    FILE *uf = fopen(USERS_TXT, "a");

    if (!uf) {
        printf("Error: Could not save the new account.\n");
        return;
    }

    fprintf(uf,
        "%s %d %s %s %.2f %.2f %d %.2f %.2f %d %s %d\n",
        u.accountNumber,
        u.pin,
        u.linkedBankName,
        u.userName,
        u.balance,
        u.activeLoan,
        u.loanDueDays,
        u.bondInvestment,
        u.totalProfitsEarned,
        u.hasCard,
        u.cardNumber,
        u.cardPin
    );

    fclose(uf);

    printf("\n");
    printf("     ACCOUNT OPENED SUCCESSFULLY   \n");
    printf("\n");
    printf("Account Holder : %s\n", u.userName);
    printf("Bank Name      : %s\n", u.linkedBankName);
    printf("Account Number : %s\n", u.accountNumber);
    printf("Initial Balance: $%.2f\n", u.balance);
    printf("Please save your account number.\n");
    printf("\n");
}

void userAuthenticationLogin(void) {
    char inputAcc[MAX_STR];
    int inputPin;
    printf("\n   CLIENT SECURE TEXT AUTHENTICATION HANDSHAKE PORTAL   \n");
    printf("Account Reference ID String: ");
    scanf("%49s", inputAcc);
    printf("Security Password (5-Digit PIN): ");
    scanf("%d", &inputPin);
    clearBuffer();

    FILE *f = fopen(USERS_TXT, "r");
    if (!f) return;

    struct UserAccount u;
    int authenticated = 0;
    while (fscanf(f, "%s %d %s %s %lf %lf %d %lf %lf %d %s %d\n",
           u.accountNumber, &u.pin, u.linkedBankName, u.userName, &u.balance,
           &u.activeLoan, &u.loanDueDays, &u.bondInvestment, &u.totalProfitsEarned,
           &u.hasCard, u.cardNumber, &u.cardPin) == 12) {
        if (strcmp(u.accountNumber, inputAcc) == 0 && u.pin == inputPin) {
            authenticated = 1; break;
        }
    }
    fclose(f);

    if (authenticated) {
        printf("Access granted. Initializing transaction matrix channels.\n");
        customerDashboard(&u);
    } else {
        printf("Authentication Denied: Verification code mismatch.\n");
    }
}

void customerDashboard(struct UserAccount *activeUser) {
    int choice;
    while (1) {
        FILE *f = fopen(USERS_TXT, "r");
        if (f) {
            struct UserAccount t;
            while (fscanf(f, "%s %d %s %s %lf %lf %d %lf %lf %d %s %d\n",
                   t.accountNumber, &t.pin, t.linkedBankName, t.userName, &t.balance,
                   &t.activeLoan, &t.loanDueDays, &t.bondInvestment, &t.totalProfitsEarned,
                   &t.hasCard, t.cardNumber, &t.cardPin) == 12) {
                if (strcmp(t.accountNumber, activeUser->accountNumber) == 0) { *activeUser = t; break; }
            }
            fclose(f);
        }

        printf("\n\n");
        printf("  USER ACCOUNT CENTRAL TERMINAL CORE INTERFACE  \n");
        printf("\n");
        printf("Holder: %-15s | Node Institution: %s\n", activeUser->userName, activeUser->linkedBankName);
        printf("Liquid Bal: $%-11.2f | Outstanding Loans: $%-10.2f\n", activeUser->balance, activeUser->activeLoan);
        printf("Locked Bonds: $%-9.2f | Profit Accumulator: $%-8.2f\n", activeUser->bondInvestment, activeUser->totalProfitsEarned);
        printf("\n");
        printf("1. Deposit Cash Liquidity\n");
        printf("2. Execute Conditional Cash Withdrawal\n");
        printf("3. Open Debt/Loan Management System (Risk Modules & Penalties)\n");
        printf("4. Allocate Capital to Long Term Bonds & Financial Profit Instruments\n");
        printf("5. Direct Cross-Node Clearing House Bank Transfer Routing\n");
        printf("6. Open Dedicated ATM Token Card Lifecycle Management Panel\n");
        printf("7. Terminate Authentication Session (Log Out)\n");
        printf("\n");
        printf("Enter System Call Selection: ");
        scanf("%d", &choice);
        clearBuffer();

        if (choice == 7) return;
        switch (choice) {
            case 1: depositLedger(activeUser); break;
            case 2: withdrawLedger(activeUser); break;
            case 3: loanManagementModule(activeUser); break;
            case 4: financialBondEngine(activeUser); break;
            case 5: executeInterbankTransfer(activeUser); break;
            case 6: processAtmCardLifecycle(activeUser); break;
            default: printf("System parameter matching missing.\n");
        }
    }
}

int saveUserToTextFile(struct UserAccount *user) {
    FILE *f = fopen(USERS_TXT, "r");
    if (!f) return 0;

    struct UserAccount list[500];
    int count = 0, found = -1;

    while (fscanf(f, "%s %d %s %s %lf %lf %d %lf %lf %d %s %d\n",
           list[count].accountNumber, &list[count].pin, list[count].linkedBankName, list[count].userName, &list[count].balance,
           &list[count].activeLoan, &list[count].loanDueDays, &list[count].bondInvestment, &list[count].totalProfitsEarned,
           &list[count].hasCard, list[count].cardNumber, &list[count].cardPin) == 12) {
        if (strcmp(list[count].accountNumber, user->accountNumber) == 0) {
            found = count;
        }
        count++;
    }
    fclose(f);

    if (found != -1) {
        list[found] = *user;
        f = fopen(USERS_TXT, "w");
        for (int i = 0; i < count; i++) {
            fprintf(f, "%s %d %s %s %.2f %.2f %d %.2f %.2f %d %s %d\n",
                    list[i].accountNumber, list[i].pin, list[i].linkedBankName, list[i].userName, list[i].balance,
                    list[i].activeLoan, list[i].loanDueDays, list[i].bondInvestment, list[i].totalProfitsEarned,
                    list[i].hasCard, list[i].cardNumber, list[i].cardPin);
        }
        fclose(f); return 1;
    }
    return 0;
}

void depositLedger(struct UserAccount *user) {
    double amount;
    printf("\nEnter deposit amount: $");
    scanf("%lf", &amount);
    if (amount <= 0) return;

    user->balance += amount;
    saveUserToTextFile(user);
    printf("Text Ledger entries synchronized. Balance credited.\n");
}

void withdrawLedger(struct UserAccount *user) {
    double amount;
    printf("\nEnter cash withdrawal threshold boundaries: $");
    scanf("%lf", &amount);
    if (amount <= 0 || amount > user->balance) { printf("Invalid boundaries or low cash liquidity depth reserves.\n"); return; }

    user->balance -= amount;
    saveUserToTextFile(user);
    printf("Cash dispatched. Storage values rewritten cleanly.\n");
}

void loanManagementModule(struct UserAccount *user) {
    int choice;
    printf("\n--- LOAN, LIABILITY AND DEADLINE FINE MATRIX ENGINE ---\n");
    printf("1. Request Asset Credit Line Advance\n");
    printf("2. Liquidate Outstanding Loans\n");
    printf("Enter selection: ");
    scanf("%d", &choice);

    if (choice == 1) {
        if (user->activeLoan > 0) { printf("Blocked: Settle pending $%.2f balance logs first.\n", user->activeLoan); return; }
        double req; printf("Enter requested loan volume: $"); scanf("%lf", &req);
    if (req <= 0) return;
user->activeLoan = req;
user->balance += req;
user->loanDueDays = 0;
saveUserToTextFile(user);
printf("Loan processed. Capital injected into balance profile.\n");
} else if (choice == 2) {
if (user->activeLoan <= 0) { printf("No liabilities flagged.\n"); return; }
printf("Input calculated days past timeline maturity date: ");
scanf("%d", &user->loanDueDays);
double fineValue = 0.0;
if (user->loanDueDays > 30) {
fineValue = (user->activeLoan * 0.15);
printf("CRITICAL TRACKING WARNING: Maturity exceeded by %d days! Fine applied: $%.2f\n", user->loanDueDays, fineValue);
}
double totalPaybackValue = user->activeLoan + fineValue;
printf("Total deduction required: $%.2f. Authorize execution? (1=Yes, 0=No): ", totalPaybackValue);
int auth; scanf("%d", &auth);
if (auth == 1) {
if (user->balance < totalPaybackValue) { printf("Transaction dropped: Insufficient funds to reconcile ledger balance.\n"); return; }
user->balance -= totalPaybackValue;
user->activeLoan = 0.0;
user->loanDueDays = 0;
saveUserToTextFile(user);
printf("Debt records expunged. Clean profile structural status synchronized.\n");
}
}
}
void financialBondEngine(struct UserAccount *user) {
int choice;
printf("\n--- NATIONAL SAVINGS BONDS INVESTMENT MATRIX ---\n");
printf("1. Allocate cash to fixed maturity government yield bonds\n");
printf("2. Compute and release accrued compounding cash distributions\n");
scanf("%d", &choice);
if (choice == 1) {
double amt; printf("Enter capital volume to lock down: $"); scanf("%lf", &amt);
if (amt <= 0 || amt > user->balance) return;
user->balance -= amt;
user->bondInvestment += amt;
saveUserToTextFile(user);
printf("Capital safely transitioned into text bonds registry values.\n");
} else if (choice == 2) {
if (user->bondInvestment <= 0) return;
double profitFactor = user->bondInvestment * 0.075;
user->totalProfitsEarned += profitFactor;
user->balance += profitFactor;
saveUserToTextFile(user);
printf("Profit distributed! Balance credited: $%.2f\n", profitFactor);
}
}
void executeInterbankTransfer(struct UserAccount *user) {
char targetAcc[MAX_STR];
double sendAmount;
printf("\n--- NETWORK CENTRAL TEXT ROAMING ROUTING SERVICE ---\n");
printf("Enter Destination Bank Clearing Account String: ");
scanf("%49s", targetAcc);
printf("Enter capitalization volume parameters to dispatch: $");
scanf("%lf", &sendAmount);
if (sendAmount <= 0 || user->balance < sendAmount) { printf("Routing Aborted: Invalid parameters or balance check dropped.\n"); return; }
FILE *f = fopen(USERS_TXT, "r");
struct UserAccount list[500];
int count = 0, targetIndex = -1, sourceIndex = -1;
while (fscanf(f, "%s %d %s %s %lf %lf %d %lf %lf %d %s %d\n",
list[count].accountNumber, &list[count].pin, list[count].linkedBankName, list[count].userName, &list[count].balance,
&list[count].activeLoan, &list[count].loanDueDays, &list[count].bondInvestment, &list[count].totalProfitsEarned,
&list[count].hasCard, list[count].cardNumber, &list[count].cardPin) == 12) {
if (strcmp(list[count].accountNumber, user->accountNumber) == 0) sourceIndex = count;
if (strcmp(list[count].accountNumber, targetAcc) == 0) targetIndex = count;
count++;
}
fclose(f);
if (sourceIndex != -1 && targetIndex != -1) {
list[sourceIndex].balance -= sendAmount;
list[targetIndex].balance += sendAmount;
f = fopen(USERS_TXT, "w");
for (int i = 0; i < count; i++) {
fprintf(f, "%s %d %s %s %.2f %.2f %d %.2f %.2f %d %s %d\n",
list[i].accountNumber, list[i].pin, list[i].linkedBankName, list[i].userName, list[i].balance,
list[i].activeLoan, list[i].loanDueDays, list[i].bondInvestment, list[i].totalProfitsEarned,
list[i].hasCard, list[i].cardNumber, list[i].cardPin);
}
fclose(f);
printf("Wire Clearance Finalized! Funds processed between %s and %s.\n",
list[sourceIndex].linkedBankName, list[targetIndex].linkedBankName);
} else {
printf("Routing Terminal Fault: Beneficiary mapping index not located.\n");
}
}
void processAtmCardLifecycle(struct UserAccount *user) {
printf("\n--- CARD ENVIRONMENT SERVICES TERMINAL PANELS ---\n");
if (user->hasCard == 0) {
printf("Request brand new physical smart card asset? (1=Yes, 0=No): ");
int choice; scanf("%d", &choice);
if (choice == 1) {
user->hasCard = 1;
sprintf(user->cardNumber, "CRD%05d", rand() % 100000);
user->cardPin = 4321;
saveUserToTextFile(user);
printf("Card issued through text database configuration map! Code: %s | Default PIN: 4321\n", user->cardNumber);
}
} else {
printf("Active Card Registered: %s | Pin State Configuration: %d\n", user->cardNumber, user->cardPin);
printf("1. Report Hardware Asset as Lost/Damaged (Reset mapping configuration loops)\n");
printf("2. Modify 4-Digit Security ATM PIN Value\n");
int c; scanf("%d", &c);
if (c == 1) {
user->hasCard = 0;
strcpy(user->cardNumber, "None");
user->cardPin = 0;
saveUserToTextFile(user);
printf("Asset records voided out inside users_ledger.txt logs.\n");
} else if (c == 2) {
printf("Define replacement 4-digit secure value mapping: ");
int np; scanf("%d", &np);
if (np >= 1000 && np <= 9999) {
user->cardPin = np;
saveUserToTextFile(user);
printf("ATM operational properties adjusted on disk records.\n");
}
}
}
}
