#include <stdio.h>
#include <string.h>

int main() {
    int numMembers, i, j, expenseIndex = 0;
    float totalExpense = 0, oldTotal = 0;
    float *ptrTotal = &totalExpense;

    char memberNames[50][30];
    char oldMembers[50][30];
    int oldMemberCount = 0;

    char expenseName[100][30];
    float expenseAmount[100];
    char choice;

    char username[30];
    char filename[40];
    FILE *fp;

    printf("===== EXPENSE SHARING WITH SMART HISTORY MERGE =====\n");

    // Username
    printf("Enter username: ");
    scanf("%s", username);

    strcpy(filename, username);
    strcat(filename, ".txt");

    int fileExists = 0;

    // ------------ Check if file exists ------------
    fp = fopen(filename, "r");
    if (fp != NULL) {
        fileExists = 1;

        printf("\nHistory found for %s.\n", username);

        char tag[50];

        fscanf(fp, "%s", tag); // MEMBERS
        fscanf(fp, "%d", &oldMemberCount);

        printf("\n--- OLD MEMBERS ---\n");
        for (i = 0; i < oldMemberCount; i++) {
            fscanf(fp, "%s", oldMembers[i]);
            printf("%s\n", oldMembers[i]);
        }

        fscanf(fp, "%s", tag); // TOTAL
        fscanf(fp, "%f", &oldTotal);

        printf("Old Total Expense: %.2f\n", oldTotal);

        fclose(fp);

        int menu;
        printf("\n1. Add expenses to same file\n");
        printf("2. Start fresh (new file)\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &menu);

        if (menu == 2) {
            oldTotal = 0;
            oldMemberCount = 0;
            fileExists = 0; // wipe old file
        } else if (menu == 3) {
            return 0;
        }
        else if( menu<0 || menu>3)
		{
        	printf("...........Invalid Input...........");
        	return 0;
		}
    }

    // ------------ ENTER NEW MEMBERS ------------
    printf("\nEnter number of members (1-50): ");
    scanf("%d", &numMembers);

    printf("\nEnter names of %d members:\n", numMembers);
    for (i = 0; i < numMembers; i++) {
        printf("Member %d: ", i + 1);
        scanf("%s", memberNames[i]);
    }

    // ------------ EXPENSE INPUT LOOP ------------
    do {
        printf("\nExpense %d:\n", expenseIndex + 1);

        printf("Enter expense name: ");
        scanf("%s", expenseName[expenseIndex]);

        printf("Enter amount: ");
        scanf("%f", &expenseAmount[expenseIndex]);

        if (expenseAmount[expenseIndex] < 0) {
            printf("Invalid amount.\n");
            return 0;
        }

        *ptrTotal += expenseAmount[expenseIndex];
        expenseIndex++;

        printf("Add another? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y');

    // ------------ CALCULATIONS ------------
    float newPerHead = totalExpense / numMembers;
    float oldPerHead = (oldMemberCount > 0) ? (oldTotal / oldMemberCount) : 0;

    // store final values
    char finalMembers[100][30];
    float finalPerHead[100];
    int finalCount = 0;

    // OLD MEMBERS MERGE
    for (i = 0; i < oldMemberCount; i++) {
        int found = 0;
        for (j = 0; j < numMembers; j++) {
            if (strcmp(oldMembers[i], memberNames[j]) == 0) {
                found = 1;
                break;
            }
        }

        strcpy(finalMembers[finalCount], oldMembers[i]);

        if (found)
            finalPerHead[finalCount] = oldPerHead + newPerHead;
        else
            finalPerHead[finalCount] = oldPerHead;

        finalCount++;
    }

    // NEW MEMBERS MERGE
    for (i = 0; i < numMembers; i++) {
        int found = 0;
        for (j = 0; j < oldMemberCount; j++) {
            if (strcmp(memberNames[i], oldMembers[j]) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(finalMembers[finalCount], memberNames[i]);
            finalPerHead[finalCount] = newPerHead;
            finalCount++;
        }
    }

    float combinedTotal = oldTotal + totalExpense;

    printf("\n========== FINAL EXPENSE HISTORY ==========\n");
    printf("Old Total: %.2f\n", oldTotal);
    printf("New Total: %.2f\n", totalExpense);
    printf("Combined: %.2f\n\n", combinedTotal);

    printf("----- FINAL PER HEAD (ALL MEMBERS) -----\n");
    for (i = 0; i < finalCount; i++) {
        printf("%s -> %.2f\n", finalMembers[i], finalPerHead[i]);
    }

    // ------------ SAVE TO FILE ------------
    fp = fopen(filename, "a");

    fprintf(fp, "MEMBERS\n");
    fprintf(fp, "%d\n", finalCount);
    for (i = 0; i < finalCount; i++) {
        fprintf(fp, "%s\n", finalMembers[i]);
    }

    fprintf(fp, "TOTAL\n%.2f\n", combinedTotal);

    fclose(fp);

    printf("\nData saved successfully in %s\n", filename);

    return 0;
}
