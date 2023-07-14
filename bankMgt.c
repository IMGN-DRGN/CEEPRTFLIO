#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXI 30

struct AccountDetails {
    char name[MAXI];
    char add[MAXI];
    char dob[MAXI];
    char email[MAXI];
    char nationality[MAXI];
    double sal;
    int nin;
    int phone;
};

void createAcc(FILE *fptr);
void displayAccountInformation(FILE *fptr);
void savedAccounts(FILE *fptr);
void deleteAcc(FILE *fptr);
void exitProgram();

int main() {
    printf("\t\t\t ========== AQUATIC BANK ===============\n\n");
    FILE *fptr = fopen("acc.txt", "w");
    if (fptr == NULL) {
        printf("Error: Unable to open file\n");
        exit(1);
    }

    while (1) {
        printf("1. Create Account\n2. Display Account\n3. Saved Accounts\n4. Delete Account\n5. Exit\n\nChoose Option: ");
        int option;
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Create Account\n");
                createAcc(fptr);
                break;
            case 2:
                printf("Display Account\n");
                displayAccountInformation(fptr);
                break;
            case 3:
                printf("Delete Account\n");
                deleteAcc(fptr);
                break;
            case 4:
                savedAccounts(fptr);
                break;
            case 5:
                printf("Exiting...\n");
                sleep(3);
                printf("We Hope To See You Soon...\n");
                fclose(fptr);
                exitProgram();
                break;
            default:
                printf("Error: Invalid input\n");
                break;
        }
        printf("\n");
    }

    return 0;
}

void createAcc(FILE *fptr) {
    struct AccountDetails *AD = malloc(sizeof(struct AccountDetails));

    printf("1. Savings Account\n2. Current Account\n\nEnter Option: ");
    int option;
    scanf("%d", &option);
    getchar(); // Consume newline character
    printf("\n");

    switch (option) {
        case 1:
            printf("Savings Account\n");
            break;
        case 2:
            printf("Current Account\n");
            break;
        default:
            printf("Error: Wrong Input\n");
            free(AD);
            return;
    }

    printf("Enter Full Names: ");
    getchar(); // Consume newline character
    scanf("%29[^\n]%*c", AD->name);
    printf("\n");

    printf("Address: ");
    scanf("%29[^\n]%*c", AD->add);
    printf("\n");

    printf("Date Of Birth: ");
    fgets(AD->dob, MAXI, stdin);
    AD->dob[strcspn(AD->dob, "\n")] = '\0';
    getchar();

    printf("Email Address: ");
    scanf("%29[^\n]%*c", AD->email);
    printf("\n");

    printf("Salary: ");
    char Salary[MAXI];
    fgets(Salary, MAXI, stdin);
    AD->sal = atof(Salary);
    getchar(); // Consume newline character
    printf("\n");

    printf("Nationality: ");
    fgets(AD->nationality, MAXI, stdin);
    getchar();

    printf("NIN: ");
    scanf("%d", &(AD->nin));
    printf("\n");

    printf("Phone Number: ");
    scanf("%d", &(AD->phone));
    printf("\n");

    fprintf(fptr, "Name: %s\n", AD->name);
    fprintf(fptr, "Address: %s\n", AD->add);
    fprintf(fptr, "Date of Birth: %s\n", AD->dob);
    fprintf(fptr, "Email: %s\n", AD->email);
    fprintf(fptr, "Salary: %.2f\n", AD->sal);
    fprintf(fptr, "Nationality: %s\n", AD->nationality);
    fprintf(fptr, "NIN: %d\n", AD->nin);
    fprintf(fptr, "Phone Number: %d\n", AD->phone);
    printf("File Saved Successfully\n");

    free(AD);
}

void displayAccountInformation(FILE *fptr){
    rewind(fptr);

    char line[MAXI];
    printf("Enter Account Name : ");
    getchar();
    scanf("%29[^\n]%*c", line);

    int found = 0;
    char name[MAXI];
    while (fgets(name, MAXI, fptr) != NULL){
        if (strcmp(name, line) == 0)
        {
            found = 1;
            break;
        }
    }
    if (found)
    {
        printf("Account Found :\n");
        printf("%s", name);
    }else{
        printf("Account Not Found\n");
    }
    
    
}

void savedAccounts(FILE *fptr){
    rewind (fptr);

    char line[MAXI];
    while (fgets(line, MAXI, fptr))
    {
        printf("%s", line);
    }
    
}

void deleteAcc(FILE *fptr){
    struct AccountDetails *AD = malloc(sizeof(struct AccountDetails));
    printf("To Delete Account Enter NIN : ");
    scanf("%d", &(AD->nin));

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Error: Unable to create temporary file\n");
        free(AD);
        return;
    }
    int deleted = 0;

    struct AccountDetails currentAccount;
    //struct AccountDetails savingsAccount;

     // Read each account from the original file
    while (fscanf(fptr, "Name: %[^\n]\n", currentAccount.name) == 1) {
        fscanf(fptr, "Address: %[^\n]\n", currentAccount.add);
        fscanf(fptr, "Date of Birth: %[^\n]\n", currentAccount.dob);
        fscanf(fptr, "Email: %[^\n]\n", currentAccount.email);
        fscanf(fptr, "Salary: %lf\n", &(currentAccount.sal));
        fscanf(fptr, "Nationality: %[^\n]\n", currentAccount.nationality);
        fscanf(fptr, "NIN: %d\n", &(currentAccount.nin));
        fscanf(fptr, "Phone Number: %d\n", &(currentAccount.phone));

        // If the NIN matches, skip writing the account to the temporary file
        if (currentAccount.nin == AD->nin) {
            deleted = 1;
            continue;
        }
      // Write the account to the temporary file
        fprintf(tempFile, "Name: %s\n", currentAccount.name);
        fprintf(tempFile, "Address: %s\n", currentAccount.add);
        fprintf(tempFile, "Date of Birth: %s\n", currentAccount.dob);
        fprintf(tempFile, "Email: %s\n", currentAccount.email);
        fprintf(tempFile, "Salary: %.2f\n", currentAccount.sal);
        fprintf(tempFile, "Nationality: %s\n", currentAccount.nationality);
        fprintf(tempFile, "NIN: %d\n", currentAccount.nin);
        fprintf(tempFile, "Phone Number: %d\n", currentAccount.phone);
    }

    fclose(fptr);
    fclose(tempFile);

    // Delete the original file
    remove("acc.txt");

    if (deleted) {
        // Rename the temporary file to the original file name
        rename("temp.txt", "acc.txt");
        printf("Account Deleted\n");
    } else {
        // Remove the temporary file if no account was deleted
        remove("temp.txt");
        printf("Account not found\n");
    }

    free(AD);
}


void exitProgram() {
    printf("Thank you for using AQUATIC BANK. Goodbye!\n");
    exit(0);
}
