#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

// Mohamed Yasser Anwar Mahmoud AlKayd
// Bank Transaction Verification Program

// - Start of the Program -

// = Error Messages =

//- Error Message 1 -
void errorMsg1(char *msg){
printf("\n%s", msg);
printf("%s", "\nProgram syntax: \n ./tv STATE TRANSACTIONS\n");
printf("%s", "\nLegal Usage Example 1:\n ./tv state.csv transaction.csv\n");
printf("%s", "\nLegal Usage Example 2:\n ./tv ../state.csv /data/log/transaction.csv\n\n");
exit(1);
}

//- Error Message 2 -
void errorMsg2(char *msg, char *filename){
printf("\n%s: %s \n", msg, filename);
printf("%s", "\nProgram syntax: \n ./tv STATE TRANSACTIONS\n");
printf("%s", "\nLegal Usage Example 1:\n ./tv state.csv transaction.csv\n");
printf("%s", "\nLegal Usage Example 2:\n ./tv ../state.csv /data/log/transaction.csv\n\n");
exit(2);
}

//- Error Message 3 -
void errorMsg3(){
printf("%s\n","File state.csv is empty. Unable to validate transaction.csv");
exit(3);
}

//= Creating the required Struct =
typedef struct ACCOUNT {
	 	int accountNumber;
	 	double startingBalance;
	 	double endingBalance;
	 	struct ACCOUNT *next;
	}AccountNode;

//= Helper method to make sure the linked list is sorted =
struct ACCOUNT* sortedHelper(struct ACCOUNT* head, struct ACCOUNT* current){
char *errormessage = "Error Message: Duplicate account number [account, start, end]:"; 
struct ACCOUNT* temporary;
if(head == NULL){
	head = current;
}
else if(head->accountNumber > current->accountNumber){
	current->next = head;
	head = current;
}
else if(head->accountNumber == current->accountNumber){
	printf("%s,%d,%f,%f\n",errormessage,current->accountNumber, current->startingBalance, current->endingBalance);
}
else{
	temporary = head;
	while(temporary->next != NULL && temporary->next->accountNumber < current->accountNumber){
			temporary = temporary->next;
	}
	if(temporary->next !=NULL && temporary->next->accountNumber == current->accountNumber){
			printf("%s,%d,%f,%f\n",errormessage,current->accountNumber, current->startingBalance, current->endingBalance);
			return head;
		}
	current->next = temporary->next;
	temporary->next = current;
}
return head;
}

//= Helper method to read the transactions correctly =
struct ACCOUNT* transactionhelper(struct ACCOUNT* head, int accnum1){
	struct ACCOUNT* tmp1 = head;
	while(tmp1 != NULL){
			if(tmp1->accountNumber == accnum1){
				return tmp1;
			}
		tmp1 = tmp1->next;
	}
	return NULL;
}

//= MAIN =
int main(int argc, char *argv[]){

//- Command Line Arguement Error Checking = 4/4 -
	printf("%s","\n");
	AccountNode* head = NULL;
	if(argc != 3){
		errorMsg1("Error: Wrong number of arguements!\n");
        }

//- Checking that the filenames are valid = 5/5 - 	
	FILE *fileCheck1 = fopen(argv[1], "rt");
	if(fileCheck1 == NULL){
		errorMsg2("Error: Unable to open filename ", argv[1]);
	}	
	FILE *fileCheck2 = fopen(argv[2], "rt");
	if(fileCheck2 == NULL){
		  errorMsg2("Error: Unable to open filename", argv[2]);
          }	 

//- Creating the required variables -	
	FILE* stream = fopen(argv[1], "rt");
    	char line[1024];
	char mode;
	int accnum, accnum1;
	double startb, endb, absolutevalueamount;

	FILE* stream2 = fopen(argv[2], "rt");
	fgets(line, 1024, stream2);
	char* deposit = "d";
	char* withdraw = "w";
	struct ACCOUNT* temporary1;

//- Checking if state is empty and transactions is not empty -
	if ((fgets(argv[1],1024,stream)==NULL)&&(fgets(argv[2],1024,stream2)!=NULL)){
		errorMsg3();
	}

//- Reading the file that contains the current state of the bank account -
	while (fgets(line, 1024, stream)){
		struct ACCOUNT* temporary;
		struct ACCOUNT* current = (struct ACCOUNT*)malloc(sizeof(struct ACCOUNT));
        sscanf(line,"%d,%lf,%lf", &accnum, &startb, &endb);
		current= (AccountNode*) malloc(sizeof(AccountNode));
		current->accountNumber = accnum;
		current->startingBalance = startb;
		current->endingBalance = endb;
		head = sortedHelper(head, current);
	}
	
//- Reading the file that contains the transactions that occured during the day -
	while (fgets(line, 1024, stream2)){
        sscanf(line,"%d,%c,%lf", &accnum1, &mode, &absolutevalueamount);
		temporary1 = transactionhelper(head, accnum1);	
		if(temporary1 == NULL){
			printf("%s,%d,%c,%f", "\nAccount not found(account, mode, amount):",accnum1, mode, absolutevalueamount);
		}
		else{
		if(mode == 'd'){
			temporary1->startingBalance = temporary1->startingBalance + absolutevalueamount;
		}
		else if(mode == 'w'){
			temporary1->startingBalance = temporary1->startingBalance - absolutevalueamount;
		}
		if(temporary1->startingBalance<0){
			printf("%s,%d,%c,%f,%f","Balance below zero error (account, mode, transaction, startingBalance before): ", accnum1, mode, absolutevalueamount, temporary1->startingBalance);
			temporary1->startingBalance = 0;
		}
		}
	}

//- Returning all the statistics of the account after each transaction -
struct ACCOUNT* temp2 = head;
double epsilon = 0.000000005;
puts("\n");
while(temp2 != NULL){
	if(((temp2->startingBalance)-(temp2->endingBalance)) != 0){
		printf("%s,%d,%f,%f","\nEnd of day balances do not agree (account, starting, ending):", temp2->accountNumber, temp2->startingBalance,temp2->endingBalance);
	}
	temp2=temp2->next;
}
puts("\n");
	
//- Deallocating memory -
	struct ACCOUNT* final = head;
	while(final!=NULL){
		struct ACCOUNT* temp = final->next;
		free(final);
		final=temp;
	}
	fclose(fileCheck1);
	fclose(fileCheck2);
	
}
//= END OF PROGRAM =
