#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdio_ext.h>
#include<ctype.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook, int sortCriteria) 
{
    if (addressBook->contactCount == 0) {
        printf("No contacts available.\n");
        return;
    }
    int choice;
    
    do
    {
        printf("\nSort contacts by:\n");
        printf("1. Name\n2. Phone\n3. Email\nEnter choice: ");
        if(scanf("%d", &choice)!=1)
        {
        if(choice<1 || choice>3)
        {
            __fpurge(stdin);
            printf("Invalid choice!choose between 1-3..");
            continue;
        }
    }
    } while (choice<1 || choice>3);

     for (int i = 0; i < addressBook->contactCount - 1; i++) 
     {
        for (int j = i + 1; j < addressBook->contactCount; j++) 
        {
            int cmp = 0;
            if (choice == 1) {
                cmp = strcmp(addressBook->contacts[i].name, addressBook->contacts[j].name);
            } else if (choice == 2) {
                cmp = strcmp(addressBook->contacts[i].phone, addressBook->contacts[j].phone);
            } else if (choice == 3) {
                cmp = strcmp(addressBook->contacts[i].email, addressBook->contacts[j].email);
            }

            if (cmp > 0) {
                Contact temp = addressBook->contacts[i];
                addressBook->contacts[i] = addressBook->contacts[j];
                addressBook->contacts[j] = temp;
            }
        }
    }

    printf("\n------------------------ CONTACT LIST ----------------------\n");
    printf("%-5s %-20s %-15s %-30s\n", "No.", "Name", "Phone", "Email");
    
    for (int i = 0; i < addressBook->contactCount; i++) {
        printf("%-4d %-20s %-15s %-30s\n", 
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
}
    

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
   // populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

int is_namevalid(const char *name);
int is_numvalid(const char *phone);
int is_mailvalid(const char *email);



void createContact(AddressBook *addressBook)
{
    if (addressBook->contactCount >= MAX_CONTACTS) 
    {
        printf("Address book is full!\n");
        return;
    }

    Contact newContact;

    // Validate Name
    do {
        printf("Enter name: ");
        scanf(" %[^\n]", newContact.name);

        if (!is_namevalid(newContact.name)) {
            printf("Invalid name! Only alphabets and spaces allowed. Try again.\n");
        }
    } while (!is_namevalid(newContact.name));

    
    int duplicate;
    do {
        duplicate = 0;
        printf("Enter phone (10 digits): ");
        scanf(" %[^\n]", newContact.phone);

        if (!is_numvalid(newContact.phone)) {
            printf("Invalid phone number! Must be exactly 10 digits. Try again.\n");
            continue;
        }

        
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcmp(addressBook->contacts[i].phone, newContact.phone) == 0) {
                printf("Error: This phone number already exists for contact '%s'.\n",
                       addressBook->contacts[i].name);
                duplicate = 1;
                break;
            }
        }
    } while (!is_numvalid(newContact.phone) || duplicate);

    int same;
    do {
        same = 0;
        printf("Enter email: ");
        scanf(" %[^\n]", newContact.email);

        if (!is_mailvalid(newContact.email)) {
            printf("Invalid email! Must contain '@' and '.'. Try again.\n");
            continue;
        }
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcmp(addressBook->contacts[i].email, newContact.email) == 0) {
                printf("Error: This email already exists for contact '%s'.\n",
                       addressBook->contacts[i].name);
                same = 1;
                break;
            }
        }

    } while (!is_mailvalid(newContact.email) || same);


    addressBook->contacts[addressBook->contactCount++] = newContact;
    printf("Contact added successfully!\n");
}

void searchContact(AddressBook *addressBook) 
{
    if (addressBook->contactCount == 0) {
        printf("No contacts yet! Please create some first.\n");
        return;
    }

    int choice;
    char query[50];
    do
    {
         printf("\nSearch by:\n");
         printf("1. Name\n2. Phone\n3. Email\nEnter choice: ");
         if(scanf("%d",&choice)!=1)
         {
         if(choice<1 || choice>3)
         {
            __fpurge(stdin);
            printf("Invalid choice!choose between 1-3....");
            continue;
         }
        }
     } while (choice<1 || choice>3);
    
    printf("Enter value: ");
    scanf(" %[^\n]", query);

    int foundCount = 0;

    printf("\n---------------- SEARCH RESULTS ----------------\n");
    printf("%-4s %-20s %-15s %-30s\n", "No.", "Name", "Phone", "Email");

    for (int i = 0; i < addressBook->contactCount; i++) {
        int match = 0;
        if (choice == 1 && strstr(addressBook->contacts[i].name, query)) {
            match = 1;
        } else if (choice == 2 && strcmp(addressBook->contacts[i].phone, query)==0) {
            match = 1;
        } else if (choice == 3 && strcmp(addressBook->contacts[i].email, query)==0) {
            match = 1;
        }

        if (match) {
            printf("%-4d %-20s %-15s %-30s\n",
                   i + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("No matching contacts found.\n");
    }
}

void editContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) {
        printf("No contacts available to edit.\n");
        return;
    }

    int choice;
    char query[50];

    do
    {
        printf("\nSearch contact to edit by:\n");
        printf("1. Name\n2. Phone\n3. Email\nEnter choice: ");
        if(scanf("%d",&choice)!=1)
        {
        if(choice<1 || choice>3)
        {
            __fpurge(stdin);
            printf("Invalid choice!choose between 1-3");
            continue;
        }
    }
    } while (choice<1 || choice>3);

    printf("Enter value: ");
    scanf(" %[^\n]", query);

    int foundIndexes[50];
    int foundCount = 0;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if ((choice == 1 && strcmp(addressBook->contacts[i].name, query) == 0) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, query) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, query) == 0)) {
            foundIndexes[foundCount++] = i;
        }
    }

    if (foundCount == 0) {
        printf("Contact not found.\n");
        return;
    }

    int idx = -1;
    if (foundCount == 1) {
        idx = foundIndexes[0];  
    } else {
        // Multiple matches
        printf("\nMultiple contacts found:\n");
        for (int i = 0; i < foundCount; i++) {
            int k = foundIndexes[i];
            printf("[%d] %s | %s | %s\n", 
                   i + 1, 
                   addressBook->contacts[k].name,
                   addressBook->contacts[k].phone,
                   addressBook->contacts[k].email);
        }

        char phoneQuery[20];
        printf("Enter phone number of the contact you want to edit: ");
        scanf(" %[^\n]", phoneQuery);

        for (int i = 0; i < foundCount; i++) {
            int k = foundIndexes[i];
            if (strcmp(addressBook->contacts[k].phone, phoneQuery) == 0) {
                idx = k;
                break;
            }
        }

        if (idx == -1) {
            printf("No contact with that phone number found among duplicates.\n");
            return;
        }
    }

    printf("\nEditing: %s | %s | %s\n",
           addressBook->contacts[idx].name,
           addressBook->contacts[idx].phone,
           addressBook->contacts[idx].email);

    int editChoice;
    char buffer[100];
    do
    {
        printf("\nEdit Menu:\n");
        printf("1. Edit Name\n2. Edit Phone\n3. Edit Email\nEnter choice: ");
        if(scanf("%d", &editChoice)!=1)
        {
        if(choice<1 || choice>3)
        {
            __fpurge(stdin);
            printf("Invalid choice!choose between 1-3..");
            continue;
        }
    }
    } while (choice<1 || choice>3);
    
    
   
    switch (editChoice) {
        case 1: // Edit Name
            do {
                printf("Enter new name: ");
                scanf(" %[^\n]", buffer);

                if (!is_namevalid(buffer)) {
                    printf("Invalid name! Only alphabets and spaces allowed.\n");
                } else {
                    strcpy(addressBook->contacts[idx].name, buffer);
                    printf("Name updated!\n");
                    break;
                }
            } while (1);
            break;

        case 2: // Edit Phone
            do {
                printf("Enter new phone (10 digits): ");
                scanf(" %[^\n]", buffer);

                if (!is_numvalid(buffer)) {
                    printf("Invalid phone number! Must be 10 digits.\n");
                    continue;
                }

                // Check duplicates
                int duplicate = 0;
                for (int i = 0; i < addressBook->contactCount; i++) {
                    if (i != idx && strcmp(addressBook->contacts[i].phone, buffer) == 0) {
                        printf("Error: Phone number already exists for contact '%s'.\n",
                               addressBook->contacts[i].name);
                        duplicate = 1;
                        break;
                    }
                }
                if (!duplicate) {
                    strcpy(addressBook->contacts[idx].phone, buffer);
                    printf("Phone updated!\n");
                    break;
                }
            } while (1);
            break;

        case 3: // Edit Email
            do {
                printf("Enter new email: ");
                scanf(" %[^\n]", buffer);

                if (!is_mailvalid(buffer)) {
                    printf("Invalid email! Must contain '@' and '.'.\n");
                    continue;
                }
                int duplicate = 0;
                for (int i = 0; i < addressBook->contactCount; i++) {
                    if (i != idx && strcmp(addressBook->contacts[i].email, buffer) == 0) {
                        printf("Error: Email already exists for contact '%s'.\n",
                               addressBook->contacts[i].name);
                        duplicate = 1;
                        break;
                    }
                }
                if (!duplicate) {
                    strcpy(addressBook->contacts[idx].email, buffer);
                    printf("Email updated!\n");
                    break;
                }
            } while (1);
            break;

        default:
            printf("Invalid choice.\n");
            break;
    }
}



void deleteContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) {
        printf("No contacts yet! Please create some first.\n");
        return;
    }

    int choice;
    char list[50];

    do
    {
        printf("\nDelete by:\n");
        printf("1. Name\n2. Phone\n3. Email\nEnter choice: ");
        if(scanf("%d", &choice)!=1)
        {
        if(choice<1 || choice>3)
        {
            __fpurge(stdin);
            printf("Invalid choice!choose between 1-3..");
            continue;
        }
        }
    } while (choice<1 || choice>3);

    printf("Enter value: ");
    scanf(" %[^\n]", list);

    int foundIndexes[MAX_CONTACTS];
    int foundCount = 0;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if ((choice == 1 && strcmp(addressBook->contacts[i].name, list) == 0) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, list) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, list) == 0)) {
            foundIndexes[foundCount++] = i;
        }
    }

    if (foundCount == 0) {
        printf("No contact found.\n");
        return;
    }

    int idx = -1;

    if (foundCount == 1) {
        idx = foundIndexes[0]; 
    } else {
        printf("\nMultiple contacts found:\n");
        for (int i = 0; i < foundCount; i++) {
            int pos = foundIndexes[i];
            printf("[%d] %s | %s | %s\n",
                   i + 1,
                   addressBook->contacts[pos].name,
                   addressBook->contacts[pos].phone,
                   addressBook->contacts[pos].email);
        }

        int selection;
        printf("Enter the index of the contact you want to delete: ");
        scanf("%d", &selection);

        if (selection < 1 || selection > foundCount) 
        {
            printf("Invalid selection.\n");
            return;
        }

        idx = foundIndexes[selection - 1];
    }

    printf("\nDeleting contact: %s | %s | %s\n",
           addressBook->contacts[idx].name,
           addressBook->contacts[idx].phone,
           addressBook->contacts[idx].email);
    for (int j = idx; j < addressBook->contactCount - 1; j++) 
    {
        addressBook->contacts[j] = addressBook->contacts[j + 1];
    }
    addressBook->contactCount--;

    printf("Contact deleted successfully!\n");
}


int is_namevalid(const char *name)
{
    int len = strlen(name);
    if (len == 0)
        return 0;
    if (name[0] == ' ' || name[0] == '.' || 
        name[len - 1] == ' ' || name[len - 1] == '.')
        return 0;
    for (int i = 0; i < name[i]; i++) 
    {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '.') 
            return 0; 
        if (name[i] == ' ' && name[i+1] == ' ')
            return 0;
        if (name[i] == '.' && name[i+1] == '.')
            return 0;
    }
    return 1;
}

int is_numvalid(const char *phone)
{
    if (strlen(phone) != 10)
    return 0;
    for (int i = 0; phone[i]; i++)
    {
        if (!isdigit(phone[i])) 
        return 0;
    }
    return 1;

}
int is_mailvalid(const char *email)
{
    if(strchr(email, '@') && strchr(email, '.'))
    {
    
    int len = strlen(email);

    if (len < 5) 
        return 0;
    if (email[0] == '@' || email[0] == '.' ||
        email[len - 1] == '@' || email[len - 1] == '.')
        return 0;
    for (int i = 0; i < len; i++) 
    {
        if (isupper(email[i]))
            return 0;
    }

    return 1;
}
}
    