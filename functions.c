#include "header.h"

int ReadAccounts(account **AP)
{
	FILE *fi = fopen(accountsFile, "r");
	FileError(fi, accountsFile);

	account *A = NULL;

	account *tempA = NULL;

	char buffer1[MAX_BUFFER_LENGTH];
	char buffer2[MAX_BUFFER_LENGTH];
	float balance;
	int i = 0;
	
	while(fscanf(fi, "%[^,],%[^,],%f\n", buffer1, buffer2, &balance) == 3)
	{
		tempA = realloc(A, sizeof(account) * (i + 1));
		AllocationError(tempA, "tempA");

		A = tempA;
		
		(A + i)->accountNumber = malloc((strlen(buffer1) + 1) * sizeof(char));
		AllocationError((A + i)->accountNumber, "accountNumber");
		
		(A + i)->name = malloc((strlen(buffer2) + 1) * sizeof(char));
		AllocationError((A + i)->name, "name");
		
		strcpy((A + i)->accountNumber, buffer1);
		strcpy((A + i)->name, buffer2);
		(A + i)->balance = balance;



		i++;
	}

	*AP = A;

	fclose(fi);
	return i;
}

int ReadTransaction(transaction **TP)
{
	FILE *fi = fopen(transactionsFile, "r");
	FileError(fi, transactionsFile);

	transaction *T = NULL;

	transaction *tempT = NULL;

	char buffer1[MAX_BUFFER_LENGTH];
	char buffer2[MAX_BUFFER_LENGTH];
	char buffer3[MAX_BUFFER_LENGTH];
	int d, m, y, i = 0;
	float balance;
	
	while(fscanf(fi, "%[^,],%[^,],%d/%d/%d,%f,%[^\n]\n", buffer1, buffer2, &d, &m, &y, &balance, buffer3) == 7)
	{
		tempT = realloc(T, sizeof(transaction) * (i + 1));
		AllocationError(tempT, "tempT");
		
		T = tempT;
		
		(T + i)->transactionID = (char *) malloc((strlen(buffer1) + 1) * sizeof(char));
		AllocationError((T + i)->transactionID, "transactionID");
		
		(T + i)->accountNumber = (char *) malloc((strlen(buffer2) + 1) * sizeof(char));
		AllocationError((T + i)->accountNumber, "accountNumber");
			
		(T + i)->description = (char *) malloc((strlen(buffer3) + 1) * sizeof(char));
		AllocationError((T + i)->description, "description");
			
		strcpy((T + i)->transactionID, buffer1);
		strcpy((T + i)->accountNumber, buffer2);
		(T + i)->date.day = d;
		(T + i)->date.month = m;
		(T + i)->date.year = y;
		(T + i)->balanceChange = balance;
		strcpy((T + i)->description, buffer3);

		i++;
	}

	*TP = T;

	fclose(fi);
	return i;
}


void displayAccounts(account *A, int length)
{
	printf("\n    %s\n", accountsFile);

	for(int i = 0; i < length; i++)
	{
		printf("%s,%s,%.2f\n", (A + i)->accountNumber, (A + i)->name, (A + i)->balance);
	}
}

void displayTransactions(account *A, transaction *T, int ALength, int TLength)
{
	printf("\n    %s\n", transactionsFile);

	for(int i = 0; i < ALength; i++)
	{
		int counter = 0;

		printf("\n%s\n", (A + i)->accountNumber);

		for(int j = 0; j < TLength; j++)
		{
			if(!strcmp((A + i)->accountNumber, (T + j)->accountNumber))
			{
				printf("	%s,%.2d/%.2d/%.2d,%.2f,%s\n", (T + j)->transactionID, (T + j)->date.day,
					(T + j)->date.month, (T + j)->date.year, (T + j)->balanceChange, (T + j)->description);
				counter++;
			}
		}
		if(!counter)
		{
			printf("	No transactions done...\n");
		}
	}
}

void displayTransactionLog(account *AP, transaction *TP, int ALen, int TLen)
{
	char accSearch[MAX_BUFFER_LENGTH];
	unsigned appearance = 0, transactionsDone = 0;
	float currentBalance = 0;

	
	transaction *copyOfTransactions = NULL;
	
	printf("Enter account number\n>");
	scanf("%s", accSearch);
	
	for(int i = 0; i < ALen; i++)
	{
		if(!strcmp(accSearch, (AP + i)->accountNumber))
		{
			currentBalance = (AP + i)->balance;
			appearance++;
		}
	}
	
	for(int i = 0; i < TLen; i++)
	{
		if(!strcmp(accSearch, (TP + i)->accountNumber))
		{
			transactionsDone++;
		}
	}
	
	if(appearance && transactionsDone)
	{
		copyOfTransactions = (transaction *) malloc(transactionsDone * sizeof(transaction));
		AllocationError(copyOfTransactions, "copyOfTransactions");
		
		for(int i = 0; i < transactionsDone; )
		{
			(copyOfTransactions + i)->accountNumber = (char *) malloc((strlen(accSearch) + 1) * sizeof(char));
			AllocationError((copyOfTransactions + i)->accountNumber, "(copyOfTransactions + i)->accountNumber");
			strcpy((copyOfTransactions + i)->accountNumber, accSearch);
		
			for(int j = 0; j < TLen; j++)
			{
				if(!strcmp(accSearch, (TP + j)->accountNumber))
				{
					(copyOfTransactions + i)->transactionID = (char *) malloc((strlen((TP + j)->transactionID) + 1) * sizeof(char));
					AllocationError((copyOfTransactions + i)->transactionID, "(copyOfTransactions + i)->transactionID");
					strcpy((copyOfTransactions + i)->transactionID, (TP + j)->transactionID);
					
					(copyOfTransactions + i)->description = (char *) malloc((strlen((TP + j)->description) + 1) * sizeof(char));
					AllocationError((copyOfTransactions + i)->description, "(copyOfTransactions + i)->description");
					strcpy((copyOfTransactions + i)->description, (TP + j)->description);
					
					(copyOfTransactions + i)->date.day = (TP + j)->date.day;
					(copyOfTransactions + i)->date.month = (TP + j)->date.month;
					(copyOfTransactions + i)->date.year = (TP + j)->date.year;
					(copyOfTransactions + i)->balanceChange = (TP + j)->balanceChange;
					
					i++;
				}
			}
		}
		
		qsort(copyOfTransactions, (size_t)transactionsDone, sizeof(transaction), ComparFunc);
		
		displayTransactionsDoneDuringTimePeriod(copyOfTransactions, transactionsDone, currentBalance, accSearch);
	}
	else if(appearance && !transactionsDone)
	{
		printf("No transactions from this account\n");
	}
	else if(!appearance && transactionsDone)
	{
		printf("Transactions done from account, not mentioned in account list\n");
	}
	else
	{
		printf("No information about this account\n");
	}
}


int ComparFunc(const void *x, const void *y)
{
	if(((transaction *)x)->date.year < ((transaction *)y)->date.year)
		return 1;
	else if(((transaction *)x)->date.year == ((transaction *)y)->date.year &&
			((transaction *)x)->date.month < ((transaction *)y)->date.month)
		return 1;
	else if(((transaction *)x)->date.year == ((transaction *)y)->date.year &&
			((transaction *)x)->date.month == ((transaction *)y)->date.month &&
			((transaction *)x)->date.day < ((transaction *)y)->date.day)
		return 1;
	else
		return 0;
}


void displayTransactionsDoneDuringTimePeriod(transaction *TP, int TLen, float currentBalance, char *accName)
{
	time_t now = time(NULL);
	struct tm tm = *localtime(&now);
	
	date current;
	
	current.day = tm.tm_mday;
	current.month = tm.tm_mon + 1;
	current.year = tm.tm_year + 1900;
	
	int period = 0;
	
	while(!period)
	{
		printf("Today's date: %02d/%02d/%d\nShow transactions for\n1. Last month\n2. Last 6 month\n3. All time\n>",
				current.day, current.month, current.year);
		scanf("%d", &period);
		
		switch(period)
		{
			case 1:
				period = 1;
				break;
			case 2:
				period = 6;
				break;
			case 3:				
				for(int i = 0; i < TLen; i++)
				{
					printf("\n    \t%s\n    \t%.2f\n    \t%s\n        Date: %02d/%02d/%d\n        Current balance: %.2f\n", 
							(TP + i)->transactionID, (TP + i)->balanceChange, (TP + i)->description, (TP + i)->date.day,
							(TP + i)->date.month, (TP + i)->date.year, currentBalance);
					
					currentBalance -= (TP + i)->balanceChange;
				}
				break;
			default:
				printf("\nOnly selections from 1 to 3s are accepted\n");
				period = 0;
		}
	}
	if(period != 3)
	{
		if(current.month >= period)
		{
			for(int i = 0; i < TLen; i++)
			{
				if(((TP + i)->date.month > current.month - period &&
					(TP + i)->date.year == current.year) ||
				   ((TP + i)->date.month >= current.month - period &&
				    (TP + i)->date.day >= current.day && 
				    (TP + i)->date.year == current.year))
				{					
					printf("\n    \t%s\n    \t%.2f\n    \t%s\n        Date: %02d/%02d/%d\n        Current balance: %.2f\n", 
							(TP + i)->transactionID, (TP + i)->balanceChange, (TP + i)->description, (TP + i)->date.day,
							(TP + i)->date.month, (TP + i)->date.year, currentBalance);
					
					currentBalance -= (TP + i)->balanceChange;
				}
			}
		}
		else
		{
			for(int i = 0; i < TLen; i++)
			{
				if(((TP + i)->date.month > current.month - (period - 12) &&
				  (TP + i)->date.year >= current.year - 1) ||
				  ((TP + i)->date.month == current.month - (period - 12) &&
				  (TP + i)->date.year >= current.year - 1 &&
				  (TP + i)->date.day >= current.day))
				{
					printf("\n    \t%s\n    \t%.2f\n    \t%s\n        Date: %02d/%02d/%d\n        Current balance: %.2f\n", 
							(TP + i)->transactionID, (TP + i)->balanceChange, (TP + i)->description, (TP + i)->date.day,
							(TP + i)->date.month, (TP + i)->date.year, currentBalance);
				}
			}
		}
	}
}


void displayAccountsByTransactionCounts(account *AP, transaction *TP, int ALen, int TLen)
{
	for(int i = 0; i < ALen; i++)
	{
		(AP + i)->transactionCount = 0;
		
		for(int j = 0; j < TLen; j++)
		{
			if(!strcmp((AP + i)->accountNumber, (TP + j)->accountNumber))
			{
				((AP + i)->transactionCount)++;
			}
		}
	}
	
	printf("Number of transactions\n");
	
	for(int i = TLen; i >=0; i--)
	{
		int temp = 0;
		
		for(int j = 0; j < ALen; j++)
		{
			if((AP + j)->transactionCount == i)
			{
				if(!temp)
				{
					printf("\n%d\n", i);\
					temp++;
				}
				
				printf("- %s (%s)\n", (AP + j)->name, (AP + j)->accountNumber);
			}
		}
	}
}

void changeBalance(account *A, int length)
{
	FILE *fo = fopen(accountsFile, "w");
	FileError(fo, accountsFile);

	char buffer[MAX_BUFFER_LENGTH];

	printf("Enter account number\n>");
	scanf("%s", buffer);

	for(int i = 0; i < length; i++)
	{
		if(!strcmp(buffer, (A + i)->accountNumber))
		{
			printf("Current balance %.2f\nEnter new balace\n>", (A + i)->balance);
			scanf("%f", &(A + i)->balance);
		}

		fprintf(fo, "%s,%s,%.2f\n", (A + i)->accountNumber, (A + i)->name, (A + i)->balance);
	}

	fclose(fo);
}

void changeDate(transaction *T, int TLength)
{
	FILE *fo = fopen(transactionsFile, "w");
	FileError(fo, transactionsFile);

	char buffer[MAX_BUFFER_LENGTH];
	int d, m, y;

	printf("Enter transaction ID\n>");
	scanf("%s", buffer);

	for(int i = 0; i < TLength; i++)
	{
		if(!strcmp(buffer, (T + i)->transactionID))
		{
			printf("Old date %.2d/%.2d/%.2d\nEnter new date [ dd/mm/yyyy ]\n>",
				(T + i)->date.day, (T + i)->date.month, (T + i)->date.year);
			scanf("%d/%d/%d", &d, &m, &y);

			while(!isDateCorrect(d, m, y))
			{
				printf("Wrong date! Try again...\n>");
				scanf("%d/%d/%d", &d, &m, &y);
			}
			(T + i)->date.day = d;
			(T + i)->date.month = m;
			(T + i)->date.year = y;
		}
		
		fprintf(fo, "%s,%s,%.2d/%.2d/%.2d,%.2f,%s\n", (T + i)->transactionID, (T + i)->accountNumber,
				 (T + i)->date.day, (T + i)->date.month, (T + i)->date.year, (T + i)->balanceChange, (T + i)->description);
	}

	fclose(fo);
}

void searchByDescription(account *A, transaction *T, int ALength, int TLength)
{
	int found = 0;
	int j = 0;
	char buffer[MAX_BUFFER_LENGTH];

	printf("Enter description\n>");
	scanf(" %[^\n]", buffer);

	while(strlen(buffer) < 4)
	{
		printf("Enter at least 4 characters...\n");
		printf("Enter description\n>");
		scanf(" %[^\n]", buffer);
	}

	for(j = 0; j < TLength; j++)
	{
		if(strstr((T + j)->description, buffer))
		{
			found++;
			break;
		}
	}

	if(!found)
	{
		printf("No such description found...\n");
		return;
	}

	for(int i = 0; i < ALength; i++)
	{
		if(!strcmp((T + j)->accountNumber, (A + i)->accountNumber))
		{
			printf("%s with account No %s has made transaction\n", (A + i)->name, (A + i)->accountNumber);

			printf("	%s,%.2d/%.2d/%.2d,%.2f,%s\n", (T + j)->transactionID, (T + j)->date.day,
					(T + j)->date.month, (T + j)->date.year, (T + j)->balanceChange, (T + j)->description);
		}
	}
}

void FileError(FILE *f, char* name)
{
	if(f == NULL)
	{
		printf("Cannot open \"%s\"...\n", name);
		exit(1);
	}
}

void AllocationError(void *p, char *name)
{
	if(p == NULL)
	{
		printf("Cannot allocate memory for \"%s\"...\n", name);
		exit(1);
	}
}

int isDateCorrect(int day, int month, int year)
{
	int valid = 0;

	int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if(month >= 1 && month <= 12)
	{
		if(day >= 1 && day <= days[month - 1])
		{
			valid = 1;
		}
	}

	if(month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
	{
		if(day >= 1 && day <= 29)
		{
			valid = 1;
		}
	}

	return valid;
}

void freeMem(account *A, transaction *T, int ALength, int TLength)
{
	for(int i = 0; i < ALength; i++)
	{
		free((A + i)->accountNumber);
		free((A + i)->name);
	}
	
	for(int i = 0; i < TLength; i++)
	{
		free((T + i)->transactionID);
		free((T + i)->accountNumber);
		free((T + i)->description);
	}
	
	free(A);
	free(T);
}