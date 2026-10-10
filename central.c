#include <stdio.h>
#include <stdlib.h>
#include "global.h"

void centralBankMenu(void) {
    int choice;
    while (1) {
        FILE *f = fopen(CENTRAL_TXT, "r");
        struct CentralHub hub = {0};
        if (f) {
            fscanf(f, "%lf %lf %d", &hub.nationalVault, &hub.totalAllocatedFunds, &hub.totalApprovedSubBanks);
            fclose(f);
        }

        printf("\n-----------------------------------------------------\n");
        printf("   CENTRAL NATIONAL BANK HUB MANAGEMENT LAYER   \n");
        printf("-----------------------------------------------------\n");
        printf("Current National Vault Stockpile: $%.2f\n", hub.nationalVault);
        printf("Sub-Bank Allocated Liquidity:    $%.2f\n", hub.totalAllocatedFunds);
        printf("Registered Network Members:       %d Banks\n", hub.totalApprovedSubBanks);
        printf("1. Audit & Approve Pending Sub-Bank Applications\n");
        printf("2. Allocate Capital Funding Injections\n");
        printf("3. Trigger Network-Wide Forensic Investigation & Audit Records\n");
        printf("4. Return to Core Framework Matrix\n");
        printf("-----------------------------------------------------\n");
        printf("Enter Vector: ");
        scanf("%d", &choice);
        clearBuffer();

        if (choice == 4) return;
        switch (choice) {
            case 1: verifyAndApproveSubBanks(); break;
            case 2: injectLiquidityFunding(); break;
            case 3: executeSystemAuditInvestigation(); break;
            default: printf("Command parameter index missing.\n");
        }
    }
}

void verifyAndApproveSubBanks(void) {
    FILE *f = fopen(BANKS_TXT, "r");
    if (!f) return;

    struct SubBank list[100];
    int count = 0;
    
    while (fscanf(f, "%d %s %d %lf %d %d\n", 
           &list[count].bankId, list[count].bankName, &list[count].isApproved, 
           &list[count].vaultLiquidity, &list[count].atmCount, &list[count].hasKuetBranch) == 6) {
        count++;
    }
    fclose(f);

    int modified = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].isApproved == 0) {
            printf("\nPending Network Entity ID: %d | Name: %s | KUET Campus Branch: %s\n", 
                   list[i].bankId, list[i].bankName, list[i].hasKuetBranch ? "YES" : "NO");
            printf("Approve this sub-bank to clear for operations? (1=Yes, 0=No): ");
            int app;
            scanf("%d", &app);
            clearBuffer();

            if (app == 1) {
                list[i].isApproved = 1;
                modified = 1;

                FILE *cf = fopen(CENTRAL_TXT, "r+");
                if (cf) {
                    struct CentralHub hub;
                    fscanf(cf, "%lf %lf %d", &hub.nationalVault, &hub.totalAllocatedFunds, &hub.totalApprovedSubBanks);
                    hub.totalApprovedSubBanks++;
                    rewind(cf);
                    fprintf(cf, "%.2f %.2f %d\n", hub.nationalVault, hub.totalAllocatedFunds, hub.totalApprovedSubBanks);
                    fclose(cf);
                }
                printf("Sub-bank network authentication complete.\n");
            }
        }
    }

    if (modified) {
        f = fopen(BANKS_TXT, "w");
        for (int i = 0; i < count; i++) {
            fprintf(f, "%d %s %d %.2f %d %d\n", 
                    list[i].bankId, list[i].bankName, list[i].isApproved, 
                    list[i].vaultLiquidity, list[i].atmCount, list[i].hasKuetBranch);
        }
        fclose(f);
    } else {
        printf("No pending sub-bank nodes waiting authorization.\n");
    }
}

void injectLiquidityFunding(void) {
    int targetId;
    double fundAmount;
    printf("Enter Target Sub-Bank ID for Capital Injection: ");
    scanf("%d", &targetId);
    printf("Enter Fund Value to Release from National Stockpile: $");
    scanf("%lf", &fundAmount);

    FILE *cf = fopen(CENTRAL_TXT, "r+");
    struct CentralHub hub;
    fscanf(cf, "%lf %lf %d", &hub.nationalVault, &hub.totalAllocatedFunds, &hub.totalApprovedSubBanks);

    if (fundAmount <= 0 || fundAmount > hub.nationalVault) {
        printf("Transaction blocked: Insufficient Vault Core reserves.\n");
        fclose(cf); return;
    }

    FILE *f = fopen(BANKS_TXT, "r");
    struct SubBank list[100];
    int count = 0, found = -1;

    while (fscanf(f, "%d %s %d %lf %d %d\n", 
           &list[count].bankId, list[count].bankName, &list[count].isApproved, 
           &list[count].vaultLiquidity, &list[count].atmCount, &list[count].hasKuetBranch) == 6) {
        if (list[count].bankId == targetId && list[count].isApproved == 1) {
            found = count;
        }
        count++;
    }
    fclose(f);

    if (found != -1) {
        hub.nationalVault -= fundAmount;
        hub.totalAllocatedFunds += fundAmount;
        rewind(cf);
        fprintf(cf, "%.2f %.2f %d\n", hub.nationalVault, hub.totalAllocatedFunds, hub.totalApprovedSubBanks);

        list[found].vaultLiquidity += fundAmount;

        f = fopen(BANKS_TXT, "w");
        for (int i = 0; i < count; i++) {
            fprintf(f, "%d %s %d %.2f %d %d\n", 
                    list[i].bankId, list[i].bankName, list[i].isApproved, 
                    list[i].vaultLiquidity, list[i].atmCount, list[i].hasKuetBranch);
        }
        fclose(f);
        printf("Liquidity dispatched successfully. Text records synced on disk.\n");
    } else {
        printf("Target institution node not found or unauthorized.\n");
    }
    fclose(cf);
}

void executeSystemAuditInvestigation(void) {
    printf("\n--- NATIONAL SECURITY SYSTEM FORENSIC AUDIT DATA DUMP ---\n");
    FILE *f = fopen(USERS_TXT, "r");
    if (!f) { printf("No active data entries found.\n"); return; }

    struct UserAccount u;
    printf("%-12s | %-15s | %-12s | %-10s | %-8s\n", "ACC_NUM", "HOLDER", "BANK NODE", "BALANCE", "LOAN");
    printf("----------------------------------------------------------------------\n");
    
    while (fscanf(f, "%s %d %s %s %lf %lf %d %lf %lf %d %s %d\n", 
           u.accountNumber, &u.pin, u.linkedBankName, u.userName, &u.balance, 
           &u.activeLoan, &u.loanDueDays, &u.bondInvestment, &u.totalProfitsEarned, 
           &u.hasCard, u.cardNumber, &u.cardPin) == 12) {
        printf("%-12s | %-15s | %-12s | $%-9.2f | $%-7.2f\n", 
               u.accountNumber, u.userName, u.linkedBankName, u.balance, u.activeLoan);
    }
    fclose(f);
}
