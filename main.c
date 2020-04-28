#include "header.h"

int main()
{
	unsigned option;
	
	account *accountPointer = NULL;
	transaction *transactionPointer = NULL;
	
	int numberOfAccounts = ReadAccounts(&accountPointer);
    int numberOfTransactions = ReadTransaction(&transactionPointer);
	
    while (1)
    {
        printf("\n* * * * * * * Select Action * * * * * *\n");
        printf("1. Display data\n");
        printf("2. Change account balance\n");
        printf("3. Change transaction date\n");
        printf("4. Search by description\n");
        printf("5. Output transaction log for specified account\n");
        printf("6. Display accounts ordered by transaction counts\n");
        printf("0. Exit\n>");
        scanf("%u", &option);
		
        switch (option)
        {
            case 1:
                displayAccounts(accountPointer, numberOfAccounts);
                displayTransactions(accountPointer, transactionPointer, numberOfAccounts, numberOfTransactions);
                break;
            case 2:
                changeBalance(accountPointer, numberOfAccounts);
                break;
            case 3:
                changeDate(transactionPointer, numberOfTransactions);
                break;
            case 4:
                searchByDescription(accountPointer, transactionPointer, numberOfAccounts, numberOfTransactions);
                break;
            case 5:
                displayTransactionLog(accountPointer, transactionPointer, numberOfAccounts, numberOfTransactions);
                break;
            case 6:
				displayAccountsByTransactionCounts(accountPointer, transactionPointer, numberOfAccounts, numberOfTransactions);
				break;            
            case 0:
				freeMem(accountPointer, transactionPointer, numberOfAccounts, numberOfTransactions);
				return EXIT_SUCCESS;
                break;
            default:
                printf("\nOnly selections from 1 to 6 and 0 are accepted\n");
        }
    }
}