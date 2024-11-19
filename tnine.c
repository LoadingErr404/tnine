#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_OF_CONTACTS 50
#define MAX_NUM_OF_CHARS 100

typedef struct {
    char name[MAX_NUM_OF_CHARS];
    char phoneNumber[MAX_NUM_OF_CHARS];
}CONTACT;

typedef enum {
    ALL_CONTACTS, MATCHING_CONTACTS, NO_CONTACTS
}RESULT;

bool validateNumParam(const char *inputNum);
void loadInput(CONTACT *contacts, int *loadedContacts);
char* cutNewLineFromChars(char* inputCharArray);
bool validateName(const char *inputName);
bool validatePhone(const char *inputPhone);
bool findByPhoneNumber(const char *inputNumber, const char *inputPhone);
int charToInt(char charValue);
void decodeNumSequence(const char *inputNumber, char *decodedName);
bool findByNumSequence(const char *inputName, const char *decodedName);
char* strlwr(char *inputName);
void matchingContacts(const CONTACT *contacts, CONTACT *matchedContacts, int *loadedContacts, const char *inputNumber, const char *decodedName);
void printContacts(const CONTACT *contacts, int loadedContacts);

int main(const int argc, char *argv[]) {
    CONTACT contacts[MAX_NUM_OF_CONTACTS], matchedContacts[MAX_NUM_OF_CONTACTS];
    int loadedContacts = 0; //value rewritten by functions - no more need to loop through size of MAX_NUM_OF_CONTACTS
    char decodedName[MAX_NUM_OF_CHARS];

    loadInput(contacts, &loadedContacts);

    if (argc == 1) { //checks if we didn't enter a number we want to find
        printContacts(contacts, loadedContacts);
        return ALL_CONTACTS;
    }

    if(!validateNumParam(argv[1])) {
        fprintf(stderr, "%s isn't only number sequence \n", argv[1]);
        return NO_CONTACTS;
    }

    if(loadedContacts == 0) {
        printf("Not found\n");
        return NO_CONTACTS;
    }


    decodeNumSequence(argv[1], decodedName);
    matchingContacts(contacts, matchedContacts,&loadedContacts, argv[1], decodedName);

    printContacts(matchedContacts, loadedContacts);

    return MATCHING_CONTACTS;
}

bool validateNumParam(const char *inputNum) {
    /* const char *inputNum - number sequence we want to check
     * return value - true if number sequence consists of only numbers else false
     *
     * function checks if number sequence is made out of numbers only
     */

    for (int index = 0; index < strlen(inputNum); index++) {
        if((inputNum[index] < '0' || inputNum[index] > '9')) {
            return false;
        }
    }
    return true;
}

void loadInput(CONTACT *contacts, int *loadedContacts) {
    /* CONTACT *contacts - list of contacts we want to load contacts to
     * int *loadedContacts - number of contacts we loaded
     * return value - none; enter parameters modified by program
     *
     * reads stdin until no more inputs
     * '\n' in char arrays is replaced by '\0'
     * checks if loaded info is valid according to standards
     * if valid adds info to list
     */

    while (!feof(stdin)) {
        char phoneBuffer[MAX_NUM_OF_CHARS];
        char nameBuffer[MAX_NUM_OF_CHARS];

        fgets(nameBuffer, MAX_NUM_OF_CHARS, stdin); //more secure function than scanf() - no overflow
        fgets(phoneBuffer, MAX_NUM_OF_CHARS, stdin);

        cutNewLineFromChars(nameBuffer);
        cutNewLineFromChars(phoneBuffer);

        if(!validateName(nameBuffer) || !validatePhone(phoneBuffer)) {  //only valid inputs according to CZ standards
            fprintf(stderr, "Contact %s, %s is not valid - not added to list \n", nameBuffer, phoneBuffer);
            continue;
        }

        strcpy(contacts[*loadedContacts].name, nameBuffer);
        strcpy(contacts[*loadedContacts].phoneNumber, phoneBuffer);

        (*loadedContacts)++;
    }
}

char* cutNewLineFromChars(char *inputCharArray) {
    /* char *inputCharArray - entry char array we want to modify
     * return value -  modified char array
     *
     * if functions finds '\n' within char array it changes it to '\0'
     * returns modified char array
     */

    for (int index=0; index < MAX_NUM_OF_CHARS; index++) {
        if(inputCharArray[index] == '\n') {
            inputCharArray[index] = '\0';
            break;
        }
    }
    return inputCharArray;
}

bool validateName(const char *inputName) {
    /* const char *inputName - name we wat to validate
     * return value - true if name is valid, false if not
     *
     * function checks if name consists of only letters, spaces and dots
     * these rules can be expanded in the future for more standards
     */

    int index=0;

    while (inputName[index++] != '\0') {
        if (inputName[index-1] >= 'A' && inputName[index-1] <= 'Z') continue;
        if (inputName[index-1] >= 'a' && inputName[index-1] <= 'z') continue;
        if (inputName[index-1] == ' ' || inputName[index-1] == '.') continue;
        return false;
    }
    return true;
}

bool validatePhone(const char *inputPhone) {
    /* const char *inputPhone - phone we want to validate
     * return value - true if name is valid, false if not
     *
     * function checks if name consists of only numbers, spaces and dashes
     * these rules can be expanded in the future for more standards
     */

    int index=0;

    while(inputPhone[index++] != '\0') {
        if (inputPhone[0] == '+') continue;
        if (inputPhone[index-1] >= '0' && inputPhone[index-1] <= '9') continue;
        if (inputPhone[index-1] == ' ' || inputPhone[index-1] == '-') continue;
        return false;
    }
    return true;
}

bool findByPhoneNumber(const char *inputNumber, const char *inputPhone) {
    /* const char *inputNumber - number we entered as a main paramether (the number we want to find)
     * const char *inputPhone - phone number from contact list
     * return value - true if inputPhone contains inputNumber else false
     *
     * function checkes if inputNumber is within inputPhone
     */

    if (strstr(inputPhone, inputNumber) != NULL) {
        return true;
    }
    return false;
}

int charToInt(const char charValue) {
    /* const char charValue - char we want to change to int
     * return value - converted char to int
     *
     * If we take '3' (ASCII code 51) and subtract '0' (ASCII code 48), we’ll be left with the integer 3
     */

    return (charValue - '0');
}

void decodeNumSequence(const char *inputNumber, char *decodedName) {
    /* const char *inputNumber - number we entered as a main parameter (the number we want to decode)
     * char *decodedName - where we want to save decoded char sequence
     * return value - none
     *
     * const char numSequence - decoding table
     *
     * first function makes sure that we have clear decodedName to work with
     * checks if number on index is the same as on the next index
     * if so we count how many times was this number located
     * after counting we check what number it was -> index of row in decoding table
     * and how many times the number was counted -> index of column in decoding table
     * then decoded character is added to char array which represents decoded name
     */

    const char numSequenece[10][4] = {
    {'+', 0, 0, 0}, //0
    {0, 0, 0, 0}, //1
    {'a', 'b', 'c', 0}, //2
    {'d', 'e', 'f', 0}, //3
    {'g', 'h', 'i', 0}, //4
    {'j', 'k', 'l', 0}, //5
    {'m', 'n', 'o', 0}, //6
    {'p', 'q', 'r', 's'}, //7
    {'t', 'u', 'v', 0}, //8
    {'w', 'x', 'y', 'z'} //9
};

    int numCount = 0;
    char decodedChar;
    strcpy(decodedName, "");

    for (int index = 0; index < strlen(inputNumber); index++) {
        if((index+1 < (int)strlen(inputNumber)) && (inputNumber[index] == inputNumber[index+1])) {
            numCount++;
            continue;
        }

        if (numCount > 4) {
            decodedChar = "";
            fprintf(stderr, "Number %c overflowed count of 4 -> won't be decoded\n", charToInt(inputNumber[index]));
        }
        else {
            decodedChar = numSequenece[charToInt(inputNumber[index])][numCount];
        }

        numCount = 0;

        strcat(decodedName, &decodedChar);
    }
}

char* strlwr(char *inputName) {
    /* char *inputName - char array which we want to change to only lower case
     * return value - only lower case char array
     *
     * functions checks inputName character by character
     * if character was upper case -> changes to lower case
     * returns modified char array
     */

    for (int index = 0; index < strlen(inputName); index++) {
        if(inputName[index] >= 'A' && inputName[index] <= 'Z') {
            inputName[index] = (char)tolower(inputName[index]);
        }
    }
    return inputName;
}

bool findByNumSequence(const char *inputName, const char *decodedName) {
    /* const char *inputName - char array in which we search
     * const char *decodedName - char array which we want to find
     * return value - true if decodedName is found within inputName else false
     *
     * checks if decodedName is within inputName
     * returns result of check
     */

    char nameBuff[MAX_NUM_OF_CHARS];
    strcpy(nameBuff, inputName);

    if(strstr(strlwr(nameBuff), decodedName) != NULL) {
        return true;
    }

    return false;
}

void matchingContacts(const CONTACT *contacts, CONTACT *matchedContacts, int *loadedContacts, const char *inputNumber, const char *decodedName) {
    /* const CONTACT *contacts - list of loaded contacts we want to search in
     * CONTACT *matchedContacts - place where results of this function will be stored
     * int *loadedContacts - number of contacts that were loaded into contacts
     * const char *inputNumber - number we entered as a main parameter (number we want to search for)
     * const char *decodedName - char array which was decoded from inputNumber by decodeNumSequence()
     *
     * function goes through every contact in list
     * checks if there's a match between input number and phone number or between decodedName and name
     * contacts that match are added to list matchedContacts
     * value within loadedContacts is then rewritten to value of number of foundContacts -> fewer loops in future
     */

    int foundContacts = 0;

    for (int index = 0; index < (*loadedContacts); index++) {
        if(findByPhoneNumber(inputNumber, contacts[index].phoneNumber) || findByNumSequence(contacts[index].name, decodedName)) {
            strcpy (matchedContacts[foundContacts].name, contacts[index].name);
            strcpy(matchedContacts[foundContacts].phoneNumber, contacts[index].phoneNumber);
            foundContacts++;
        }
    }

    (*loadedContacts) = foundContacts;
}

void printContacts(const CONTACT *contacts, const int loadedContacts) {
    /* const CONTACT *contacts - list of contacts we want to print out
     * const int loadedContacts - number of contacts
     *
     * function prints every contact in format: name, phone
     */

    for (int index = 0; index < loadedContacts; index++) {
        printf("%s, %s\n", contacts[index].name, contacts[index].phoneNumber);
    }
}