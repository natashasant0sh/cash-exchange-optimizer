#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    char* key;
    double value;
} KeyValuePair;

typedef struct {
    KeyValuePair* array;
    int size;
    int capacity;
} HashMap;

void initHashMap(HashMap* hashMap) {
    hashMap->array = malloc(10 * sizeof(KeyValuePair));
    hashMap->size = 0;
    hashMap->capacity = 10;
}

typedef struct {
    char** names;
    int size;
} Group;

void put(HashMap* hashMap, char* key, double value) {
    // Check if the key already exists in the HashMap
    for (int i = 0; i < hashMap->size; i++) {
        if (strcmp(hashMap->array[i].key, key) == 0) {
            // If the key already exists, update its value
            hashMap->array[i].value = value;
            return;
        }
    }

    // If the key does not exist in the HashMap, add a new key-value pair
    if (hashMap->size == hashMap->capacity) {
        hashMap->capacity *= 2;
        hashMap->array = realloc(hashMap->array, hashMap->capacity * sizeof(KeyValuePair));
    }
    hashMap->array[hashMap->size].key = malloc(strlen(key) + 1);
    strcpy(hashMap->array[hashMap->size].key, key);
    hashMap->array[hashMap->size].value = value;
    hashMap->size++;
}

KeyValuePair max(HashMap* hashMap) {
    KeyValuePair maxPair = hashMap->array[0];
    for (int i = 1; i < hashMap->size; i++) {
        if (hashMap->array[i].value > maxPair.value) {
            maxPair = hashMap->array[i];
        }
    }
    return maxPair;
}

KeyValuePair min(HashMap* hashMap) {
    KeyValuePair minPair = hashMap->array[0];
    for (int i = 1; i < hashMap->size; i++) {
        if (hashMap->array[i].value < minPair.value) {
            minPair = hashMap->array[i];
        }
    }
    return minPair;
}

double roundToPlaces(double value, int places) {
    if (places < 0) {
        printf("Error: Number of decimal places cannot be negative.\n");
        return -1.0;
    }

    double multiplier = pow(10.0, places);
    return round(value * multiplier) / multiplier;
}


void findPath(HashMap* details) 
{
    int allZero = 1;
    for (int i = 0; i < details->size; i++) {
        if (details->array[i].value != 0.0) {
            allZero = 0;
            break;
        }
    }
    if (allZero) {
        return;
    }

    if (details->size <= 1) {
        return;
    }

    KeyValuePair maxCredit = max(details);
    KeyValuePair minDebit = min(details);

    if (maxCredit.value == 0 && minDebit.value == 0) {
        return;
    }

    if (maxCredit.value != minDebit.value) {
        double result = maxCredit.value + minDebit.value;
        result = roundToPlaces(result, 1);
        if (result >= 0.0) {
            printf("%s needs to pay %s: %.2f\n", minDebit.key, maxCredit.key, fabs(minDebit.value));
            put(details, maxCredit.key, result);
            put(details, minDebit.key, 0.0);
        } else {
            printf("%s needs to pay %s: %.2f\n", minDebit.key, maxCredit.key, fabs(maxCredit.value));
            put(details, maxCredit.key, 0.0);
            put(details, minDebit.key, result);
        }
        findPath(details);
    }
}

// Function to get key from value
char* getKeyFromValue(HashMap* hashMap, double value) {
    for (int i = 0; i < hashMap->size; i++) {
        if (hashMap->array[i].value == value) {
            return hashMap->array[i].key;
        }
    }
    return NULL;
}

double getValue(HashMap* hashMap, char* key) {
    for (int i = 0; i < hashMap->size; i++) {
        if (strcmp(hashMap->array[i].key, key) == 0) {
            return hashMap->array[i].value;
        }
    }
    // If the key is not found, return 0
    return 0.0;
}

void addDebt(HashMap* hashMap, void* debtor, char* creditor, double amount, int isGroup, double* splits) {
    if (isGroup) {
        Group* group = (Group*) debtor;
        for (int i = 0; i < group->size; i++) {
            char* debtorName = group->names[i];
            double debtorValue = getValue(hashMap, debtorName);
            double splitAmount = splits[i];
            put(hashMap, debtorName, debtorValue - splitAmount);
        }
        double creditorValue = getValue(hashMap, creditor);
        put(hashMap, creditor, creditorValue + amount);
    } else {
        char* debtorName = (char*) debtor;
        double debtorValue = getValue(hashMap, debtorName);
        double creditorValue = getValue(hashMap, creditor);
        put(hashMap, debtorName, debtorValue - amount);
        put(hashMap, creditor, creditorValue + amount);
    }
}

int main() {
    HashMap parm;
    initHashMap(&parm);
    Group groups[10];  // Array to store up to 10 groups
    int groupCount = 0;

    printf("Is this a group payment or an individual payment? (Enter 'group' or 'individual'): ");
    char paymentType[10];
    scanf("%s", paymentType);

    if (strcmp(paymentType, "group") == 0) {
        // Create a new group
        printf("Enter group name: ");
        char groupName[50];
        scanf("%s", groupName);

        printf("Enter names of group members (enter 'done' when finished): ");
        Group* group = &groups[groupCount++];
        group->names = malloc(10 * sizeof(char*));  // Start with space for 10 names
        group->size = 0;  // Start with 0 names
        char name[50];
        while (scanf("%s", name) == 1 && strcmp(name, "done") != 0) 
        {
            if (group->size % 10 == 0 && group->size > 0) 
            {
                // If the array is full, double its size
                group->names = realloc(group->names, (group->size * 2) * sizeof(char*));
            }
            group->names[group->size] = malloc(50);
            strcpy(group->names[group->size], name);
            group->size++;
        }

     while (1) {
        printf("Is this payment from one group member to another? (Enter 'yes' or 'no'): ");
        char intraGroupPayment[4];
        scanf("%s", intraGroupPayment);

        if (strcmp(intraGroupPayment, "yes") == 0) {
            printf("Enter debtor, creditor, and amount (or 'q' to quit): ");
            char debtor[50], creditor[50];
            double amount;
            if (scanf("%s %s %lf", debtor, creditor, &amount) != 3 || strcmp(debtor, "q") == 0) {
                break;
            }
            addDebt(&parm, debtor, creditor, amount, 0, NULL);
        } else 
        {
           printf("Enter name and amount (or 'q' to quit): ");
            char name[50];
            double amount;
            if (scanf("%s", name) != 1 || strcmp(name, "q") == 0) {
                break;
            }
            scanf("%lf", &amount);

            printf("How do you want to split the payment? (Enter 'equal' or 'amount' or 'percentage'): ");
            char splitType[10];
            scanf("%s", splitType);

            double splits[group->size];
            if (strcmp(splitType, "amount") == 0) {
                printf("Enter the amount for each group member: ");
                for (int i = 0; i < group->size; i++) {
                    printf("%s: ", group->names[i]);
                    scanf("%lf", &splits[i]);
                }
            } else if (strcmp(splitType, "percentage") == 0) {
                printf("Enter the percentage for each group member: ");
                for (int i = 0; i < group->size; i++) {
                    printf("%s: ", group->names[i]);
                    double percentage;
                    scanf("%lf", &percentage);
                    splits[i] = (percentage / 100.0) * amount;
                }
            }
            else if (strcmp(splitType, "equal") == 0)
            {
            double equalSplit = amount / group->size;
            for (int i = 0; i < group->size; i++) {
                splits[i] = equalSplit;
            }
            }

            addDebt(&parm, group, name, amount, 1, splits);
        }
    }
}
else if (strcmp(paymentType, "individual") == 0) {
        while (1) {
            printf("Enter debtor, creditor, and amount (or 'q' to quit): ");
            char debtor[50], creditor[50];
            double amount;
            if (scanf("%s %s %lf", debtor, creditor, &amount) != 3 || strcmp(debtor, "q") == 0) {
                break;
            }
            addDebt(&parm, debtor, creditor, amount, 0, NULL);
        }
    }

    findPath(&parm);

    return 0;
}