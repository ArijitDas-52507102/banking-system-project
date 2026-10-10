#ifndef GLOBAL_H
#define GLOBAL_H

#define MAX_STR 50
#define MAX_ATMS 5

// --- STRUCTURES (STORED AS NORMAL DELIMITED TEXT) ---
struct CentralHub {
    double nationalVault;
    double totalAllocatedFunds;
    int totalApprovedSubBanks;
};

struct SubBank {
    int bankId;
    char bankName[MAX_STR];
    int isApproved;          
    double vaultLiquidity;   
    int atmCount;
    int hasKuetBranch;       
};

// NEW STRUCTURE: Explicit geographic tracking map for ATM hardware nodes
struct AtmLocation {
    int bankId;
    char bankName[MAX_STR];
    char boothArea[MAX_STR]; // e.g., KUET_Main_Gate, Teligati_Chowrasta
};

struct UserAccount {
    char accountNumber[MAX_STR];  
    int pin;                       
    char linkedBankName[MAX_STR];
    char userName[MAX_STR];
    double balance;
    double activeLoan;
    int loanDueDays;         
    double bondInvestment;
    double totalProfitsEarned;
    int hasCard;             
    char cardNumber[MAX_STR];
    int cardPin;             
};

// --- MASTER FLAT TEXT DATABASE REFERENCES ---
extern const char *CENTRAL_TXT;
extern const char *BANKS_TXT;
extern const char *USERS_TXT;
extern const char *LOCATIONS_TXT; // New Location Database Reference

// --- CORE SYSTEM ROUTINES ---
void clearBuffer(void);
void rootSystemGateway(void);
void initTextDatabases(void);

// --- CENTRAL NATIONAL HUB ---
void centralBankMenu(void);
void verifyAndApproveSubBanks(void);
void injectLiquidityFunding(void);
void executeSystemAuditInvestigation(void);

// --- COMMERCIAL SUB-BANK NODE ROUTINES ---
void subBankRegistryMenu(void);
void registerSubBankNode(void);
void manageAtmBranches(int bankId);
void viewNetworkBankDirectory(void);
void displayAtmsByLocation(int bankId); // Look up helper for locations file

// --- USER OPERATIONS ---
void userPortalGateway(void);
void userRegistrationForm(void);
void userAuthenticationLogin(void);
void customerDashboard(struct UserAccount *activeUser);
int saveUserToTextFile(struct UserAccount *user);

// --- LEDGER & MONETARY ROUTINES ---
void depositLedger(struct UserAccount *user);
void withdrawLedger(struct UserAccount *user);
void loanManagementModule(struct UserAccount *user);
void financialBondEngine(struct UserAccount *user);
void executeInterbankTransfer(struct UserAccount *user);
void processAtmCardLifecycle(struct UserAccount *user);

#endif
