#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define accountsFile "Accounts.txt"
#define transactionsFile "Transactions.txt"

#define MAX_BUFFER_LENGTH 50

typedef struct date
{
	int day;
	int month;
	int year;
	
} date;

typedef struct account
{
	char *accountNumber;
	char *name;
	float balance;
	int transactionCount;
	
} account;

typedef struct transaction
{
	char *transactionID;
	char *accountNumber;
	date date;
	float balanceChange;
	char *description;
	
} transaction;

int ReadAccounts(account **AP);
int ReadTransaction(transaction **TP);
void displayAccounts(account *A, int length);
void displayTransactions(account *A, transaction *T, int ALength, int TLength);
void displayTransactionLog(account *AP, transaction *TP, int ALen, int TLen);
int ComparFunc(const void *x, const void *y);
void displayAccountsByTransactionCounts(account *AP, transaction *TP, int ALen, int TLen);
void displayTransactionsDoneDuringTimePeriod(transaction *TP, int TLen, float currentBalance, char *accName);
void changeBalance(account *A, int length);
void changeDate(transaction *T, int TLength);
void searchByDescription(account *A, transaction *T, int ALength, int TLength);

void FileError(FILE *f, char* name);
void AllocationError(void *p, char* name);
int isDateCorrect(int day, int month, int year);
void freeMem(account *A, transaction *T, int ALength, int TLength);

#endif