#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"

// Helper function to convert strings to lowercase for case-insensitive matching
void convertToLowercase(char *dest, const char *src) {
    int i = 0;
    while (src[i]) {
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

// NEW FUNCTION: Automatically populates locations_db.txt with mock university data
void populateMockCampusLocations(void) {
    FILE *lf = fopen(LOCATIONS_TXT, "r");
    if (lf) {
        // Check if file already has content by trying to read the first character
        int ch = fgetc(lf);
        fclose(lf);
        if (ch != EOF) {
            return; // File is not empty, do not overwrite user data
        }
    }

    // Write 10 diverse engineering university campus nodes
    lf = fopen(LOCATIONS_TXT, "w");
    if (lf) {
        fprintf(lf, "1 Sonali_Bank KUET_Main_Gate\n");
        fprintf(lf, "1 Sonali_Bank KUET_Auditorium_Loop\n");
        fprintf(lf, "1 Sonali_Bank BUET_Bakshibazar_Gate\n");
        fprintf(lf, "2 Dutch_Bangla KUET_Teligati_Chowrasta\n");
        fprintf(lf, "2 Dutch_Bangla BUET_Cafeteria_Booth\n");
        fprintf(lf, "2 Dutch_Bangla RUET_Kazla_Gate\n");
        fprintf(lf, "3 Rupali_Bank CUET_Main_Campus\n");
        fprintf(lf, "3 Rupali_Bank CUET_Chittagong_Highway\n");
        fprintf(lf, "4 Agrani_Bank RUET_Admin_Building\n");
        fprintf(lf, "4 Agrani_Bank BUET_Palashi_Intersection\n");
        fclose(lf);
        printf("Empty locations_db.txt detected. 10 University ATM nodes auto-generated.\n");
    }
}

void searchAtmByCampusArea(void) {
    FILE *lf = fopen(LOCATIONS_TXT, "r");
    if (!lf) {
        printf("Error: Location Registry file (locations_db.txt) not found.\n");
        return;
    }

    char searchToken[MAX_STR];
    char lowerSearchToken[MAX_STR];
    
    printf("\n--- REGIONAL CAMPUS ATM LOCATOR SERVICES ---\n");
    printf("Enter target region name query (e.g., kuet, buet, cuet, ruet): ");
    scanf("%49s", searchToken);
    clearBuffer();

    convertToLowercase(lowerSearchToken, searchToken);

    struct AtmLocation loc;
    int matchesFound = 0;

    printf("\n==================== SEARCH RESULTS FOR: %s ====================\n", searchToken);
    printf("%-10s | %-20s | %-30s\n", "BANK ID", "BANK NAME", "ATM BOOTH SITE AREA");
    printf("----------------------------------------------------------------------\n");

    char lowerBoothArea[MAX_STR];
    
    while (fscanf(lf, "%d %s %s\n", &loc.bankId, loc.bankName, loc.boothArea) == 3) {
        convertToLowercase(lowerBoothArea, loc.boothArea);

        // Substring matching logic to catch partial matches
        if (strstr(lowerBoothArea, lowerSearchToken) != NULL) {
            printf("%-10d | %-20s | %-30s\n", loc.bankId, loc.bankName, loc.boothArea);
            matchesFound++;
        }
    }
    fclose(lf);

    if (matchesFound == 0) {
        printf("\nNo ATM booth endpoints currently registered around the '%s' campus network area.\n", searchToken);
    } else {
        printf("\nTotal active ATM clusters discovered: %d\n", matchesFound);
    }
}
