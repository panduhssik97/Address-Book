#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("file1.csv", "w");
    if (!fptr) {
        printf("Error saving contacts.\n");
        return;
    }
    fprintf(fptr, "%d\n", addressBook->contactCount);
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("file1.csv", "r");
    if (!fptr) {
        printf("No saved contacts found.\n");
        return;
    }
     int count;
    if (fscanf(fptr, "%d\n", &count) != 1) {
        fclose(fptr);
        return;
    }
    addressBook->contactCount = 0; 

    for (int i = 0; i < count; i++) 
    {
        Contact c;
        if (fscanf(fptr, "%[^,],%[^,],%[^\n]\n",
                   c.name, c.phone, c.email) == 3) {
            addressBook->contacts[addressBook->contactCount++] = c;
        }
    }

    fclose(fptr);
}
