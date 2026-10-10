#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include <windows.h>

// Define plain text tracking databases
const char *CENTRAL_TXT = "central_vault.txt";
const char *BANKS_TXT = "banks_network.txt";
const char *USERS_TXT = "users_ledger.txt";
const char *LOCATIONS_TXT = "locations_db.txt"; // Normal Text Location File

int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    printf("■ NATIONAL INTERCONNECT PLAIN TEXT MATRIX NETWORKING ENGINE ■\n");
    initTextDatabases();
    rootSystemGateway();
    return 0;
}

void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



void rootSystemGateway(void) {
    int choice;
    while (1) {
        printf("=============================================================\n");
        printf("  NATIONAL INTERCONNECT PLAIN TEXT MATRIX NETWORKING ENGINE  \n");
        printf("=============================================================\n");
        printf("1. Access Central National Bank Module\n");
        printf("2. Access Sub-Bank Management Nodes\n");
        printf("3. Access User Portal Gateway Interface\n");
        printf("4. Shutdown Micro-Network Terminal Grid\n");
        printf("-------------------------------------------------------------\n");
        printf("Enter Routing Choice (1-4): ");
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            printf("System Error: Text entries rejected at terminal gateway.\n");
            continue;
        }
        clearBuffer();

        switch (choice) {
            case 1: centralBankMenu(); break;
            case 2: subBankRegistryMenu(); break;
            case 3: userPortalGateway(); break;
            case 4: 
                printf("\nSevering link matrix nodes. All text databases closed verified.\n");
                exit(0);
            default: printf("Routing coordinate match not found.\n");
        }
    }
}
// Add this declaration at the top of main.c if not already in global.h
void populateMockCampusLocations(void);

void initTextDatabases(void) {
    FILE *f = fopen(CENTRAL_TXT, "r");
    if (!f) {
        f = fopen(CENTRAL_TXT, "w");
        if (f) {
            fprintf(f, "100000000.00 0.00 0\n");
            fclose(f);
        }
    } else {
        fclose(f);
    }
    
    f = fopen(BANKS_TXT, "a"); if (f) fclose(f);
    f = fopen(USERS_TXT, "a"); if (f) fclose(f);
    f = fopen(LOCATIONS_TXT, "a"); if (f) fclose(f); 
    
    // Trigger the automated campus data population script
    populateMockCampusLocations(); 
}

