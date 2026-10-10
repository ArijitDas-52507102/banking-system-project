#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

void subBankRegistryMenu(void) {
    int choice;
    while (1) {
        printf("\n-----------------------------------------------------\n");
        printf(" SUB-BANK GEOGRAPHIC CONFIGURATION GRID DIRECTORY\n");
        printf("-----------------------------------------------------\n");
        printf("1. Register/Apply for New Sub-Bank Entity Creation\n");
        printf("2. Manage & Deploy ATM Booths (To Location File)\n");
        printf("3. Print Active Certified Interbank Directory Map\n");
        printf("4. Return to System Gateway Terminal\n");
        printf("-----------------------------------------------------\n");
        printf("Enter Selection: ");
        scanf("%d", &choice);
        clearBuffer();

        if (choice == 4) return;
        switch (choice) {
            case 1: registerSubBankNode(); break;
            case 2: {
                int bid;
                printf("Enter structural Sub-Bank ID reference code: ");
                scanf("%d", &bid);
                manageAtmBranches(bid);
                break;
            }
            case 3: viewNetworkBankDirectory(); break;
            default: printf("Command lookup failed.\n");
        }
    }
}

void registerSubBankNode(void) {
    struct SubBank b = {0};
    printf("Enter System Numeric ID for New Sub-Bank: ");
    scanf("%d", &b.bankId);
    clearBuffer();

    FILE *f = fopen(BANKS_TXT, "r");
    struct SubBank temp;
    if (f) {
        while (fscanf(f, "%d %s %*d %*f %*d %*d\n", &temp.bankId, temp.bankName) == 2) {
            if (temp.bankId == b.bankId) {
                printf("Error: Footprint ID conflicts with an existing allocation!\n");
                fclose(f); return;
            }
        }
        fclose(f);
    }

    printf("Enter Institutional Commercial Bank Name (No spaces): ");
    scanf("%49s", b.bankName);
    clearBuffer();

    printf("Confirm Deployment of branch at KUET Campus? (1=Confirm, 0=Abort): ");
    scanf("%d", &b.hasKuetBranch);
    clearBuffer();

    if (b.hasKuetBranch != 1) {
        printf("Registry Denied: Operational guidelines require establishing a branch in KUET campus.\n");
        return;
    }

    b.isApproved = 0; 
    b.vaultLiquidity = 0.0;
    b.atmCount = 0;

    f = fopen(BANKS_TXT, "a");
    if (f) {
        fprintf(f, "%d %s %d %.2f %d %d\n", b.bankId, b.bankName, b.isApproved, b.vaultLiquidity, b.atmCount, b.hasKuetBranch);
        fclose(f);
        printf("Application filed into text logs. Awaiting central bank review.\n");
    }
}

// Helper: Dynamically looks up and parses ATM booths from locations_db.txt
void displayAtmsByLocation(int bankId) {
    FILE *lf = fopen(LOCATIONS_TXT, "r");
    if (!lf) return;
    
    struct AtmLocation loc;
    int count = 1;
    printf("Active ATM Booth Footprints (Loaded from locations_db.txt):\n");
    while (fscanf(lf, "%d %s %s\n", &loc.bankId, loc.bankName, loc.boothArea) == 3) {
        if (loc.bankId == bankId) {
            printf(" %d. Location Endpoint Area: %s\n", count++, loc.boothArea);
        }
    }
    fclose(lf);
    if (count == 1) printf("No remote ATM deployment tokens registered for this branch.\n");
}

void manageAtmBranches(int bankId) {
    FILE *f = fopen(BANKS_TXT, "r");
    if (!f) return;

    struct SubBank list[100];
    int count = 0, found = -1;

    while (fscanf(f, "%d %s %d %lf %d %d\n", 
           &list[count].bankId, list[count].bankName, &list[count].isApproved, 
           &list[count].vaultLiquidity, &list[count].atmCount, &list[count].hasKuetBranch) == 6) {
        if (list[count].bankId == bankId) {
            found = count;
        }
        count++;
    }
    fclose(f);

    if (found == -1) { printf("Node matching configuration was not found.\n"); return; }
    if (list[found].isApproved == 0) { printf("Access Blocked: Node pending national approval authorization.\n"); return; }

    printf("\nBank Matrix: %s | Vault Assets: $%.2f\n", list[found].bankName, list[found].vaultLiquidity);
    
    // Look up and render from locations text database file
    displayAtmsByLocation(bankId);

    if (list[found].atmCount >= MAX_ATMS) {
        printf("Hardware Ceiling Limit Hit: Cannot deploy further ATMs.\n");
        return;
    }

    printf("\nDeploy New Remote ATM Booth Location Token String (e.g., KUET_Main_Gate, Teligati_Bazar):\n>> ");
    char newLoc[MAX_STR];
    scanf("%49s", newLoc);
    clearBuffer();

    // 1. Append directly to the dedicated spatial locations file text layer
    FILE *lf = fopen(LOCATIONS_TXT, "a");
    if (lf) {
        fprintf(lf, "%d %s %s\n", list[found].bankId, list[found].bankName, newLoc);
        fclose(lf);
    }

    // 2. Increment the internal counter in bank structural network table
    list[found].atmCount++;

    f = fopen(BANKS_TXT, "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d %s %d %.2f %d %d\n", 
                list[i].bankId, list[i].bankName, list[i].isApproved, 
                list[i].vaultLiquidity, list[i].atmCount, list[i].hasKuetBranch);
    }
    fclose(f);
    printf("Success! ATM booth logged into locations_db.txt and linked to system.\n");
}

void viewNetworkBankDirectory(void) {
    FILE *f = fopen(BANKS_TXT, "r");
    if (!f) { printf("Network directory maps offline.\n"); return; }

    struct SubBank b;
    printf("\n==================== CERTIFIED INTERBANK MAP DIRECTORY ====================\n");
    while (fscanf(f, "%d %s %d %lf %d %d\n", &b.bankId, b.bankName, &b.isApproved, &b.vaultLiquidity, &b.atmCount, &b.hasKuetBranch) == 6) {
        printf("Bank %d: %-12s | Certified: %s | Capital: $%-9.2f | Core ATMs Registered: %d\n", 
               b.bankId, b.bankName, b.isApproved ? "YES" : "NO", 
               b.vaultLiquidity, b.atmCount);
    }
    fclose(f);
}
