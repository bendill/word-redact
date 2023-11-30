#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Struct for a string with dynamic length (a linked list of chars)
struct string {
  char c; // The character held by this struct
  struct string *next; // A pointer to the next char in the string
};

// Function which creates and returns a new string struct, holding the inputted character
struct string stringCreate(char c) {
  struct string str;
  str.c = c;
  str.next = NULL; // End of linked list
  return str;
}

// Function to add a new character to the end of the inputted string
void stringAppend(struct string *str, char c) {
  // Finds the end of the string linked list
  while (str->next != NULL) {
    str = str->next;
  }
  // Allocates memory for a new string struct to contain the character
  struct string *newStr = (struct string *)malloc(sizeof(struct string));
  if (newStr != NULL) {
    // Creates the struct and changes the previous pointer to point to the new struct
    *newStr = stringCreate(c);
    str->next = newStr;
  }
}

// Returns an item in the string linked list, given a starting string struct and an index
struct string *stringGet(struct string *str, int index) {
  for (int i = 0; i < index; i++) {
    if (str->next == NULL) {
      return NULL;
    }
    str = str->next;
  }
  return str;
}

// Returns the length of the inputted string, given that the struct is the start of the string
int stringLength(struct string *str) {
  int length = 1;
  // Repeatedly increments 'length' until the end of the list is found
  while (str->next != NULL) {
    str = str->next;
    length++;
  }
  return length;
}

// Prints the inputted string on a single line
void stringPrint(struct string *str) {
  while (str != NULL) {
    printf("%c", str->c);
    str = str->next;
  }
}

// Struct for a linked list of strings, acting as a dynamic data structure of strings
struct stringList {
  struct string s; // The string being held by this struct
  struct stringList *next; // Pointer to the next string in the list
};

// Function that creates and returns a new string list item, given an inputted string
struct stringList stringListCreate(struct string s) {
  struct stringList strList;
  strList.s = s;
  strList.next = NULL; // End of the linked list, so no pointer
  return strList;
}

// Appends a string (linked list) to the end of a list of strings
void stringListAppend(struct stringList *strList, struct string s) {
  // Finds the end of the list
  while (strList->next != NULL) {
    strList = strList->next;
  }
  // Allocates space for a new string list item, and gets the new pointer
  struct stringList *newStrList = (struct stringList *)malloc(sizeof(struct stringList));
  if (newStrList != NULL) {
    // Creates a new string list item, places it in the allocated space, and sets the previous list pointer to point to it
    *newStrList = stringListCreate(s);
    strList->next = newStrList;
  }
}

// Prints all strings in the list, with each string on a new line
void stringListPrint(struct stringList *strList) {
  // Repeats until the end of the string list is found
  while (strList != NULL) {
    stringPrint(&strList->s);
    printf("\n");
    strList = strList->next;
  }
}

// Copies data from a text file to a list of strings, with each line of the file being a separate string in the list
struct stringList copyTextFile(char filepath[]) {
  struct stringList strList;

  FILE *fp = fopen(filepath, "r");
  if (fp == NULL) {
    printf("File not found\n");
  } else {
    bool newStr = true, newStrList = true; // True when a new string / list must be created
    struct string currentString;
    char c = fgetc(fp); // Gets the next character in the file
    while (c != EOF) { // Repeats until end of file
      if (c == '\n') {
        // The last character on the line
        if (newStr) {
          // A blank line in the file
          currentString = stringCreate('\0');
        }
        if (newStrList) {
          // The first line in the file, so creates a new string list
          strList = stringListCreate(currentString);
          newStrList = false;
        } else {
          // Not the first line, so the line is added to the string list
          stringListAppend(&strList, currentString);
        }
        newStr = true; // A new string must be created next loop
      } else if (newStr) {
        // First character on a line, so a new string is created
        currentString = stringCreate(c);
        newStr = false;
      } else {
        stringAppend(&currentString, c);
      }
      c = fgetc(fp); // Gets next character
    }
    if (!newStr) {
      // If the last line wasn't added, it's added now
      if (newStrList) {
        // Only one line in the file
        strList = stringListCreate(currentString);
      } else {
        stringListAppend(&strList, currentString);
      }
    }
  }
  return strList;
}

// Redacts all characters (which aren't spaces) from the string, given the number of characters to redact
void stringRedact(struct string *str, int length) {
  for (int i = 0; i < length; i++) {
    if (str->c != ' ' && str->c != '\0') {
      str->c = '*';
    }
    if (str->next == NULL) {
      break;
    }
    str = str->next;
  }
}

// If the inputted character is an upper case letter the lower case is returned
char lowerCase(char c) {
  if (65 <= c && c < 91) {
    // The character is in the ASCII range for captal letters
    c += 32;
  }
  return c;
}

// Returns true if the inputted substring matches the inputted string for each character in the substring, ignoring case
// The inputted string must be a pointer to the start of the potential substring section
bool substringAtPos(struct string *substr, struct string *str) {
  // Repeates for each character in the substring
  while (true) {
    if (lowerCase(substr->c) != lowerCase(str->c)) {
      // If there is a mismatch, false is returned
      return false;
    }
    if (substr->next == NULL) {
      break;
    }
    if (str->next == NULL) {
      // If the end of the string is found, false is returned
      return false;
    }
    // Moves on to the next characters
    substr = substr->next;
    str = str->next;
  }
  return true;
}

// Returns true if the inputted character is anything but a letter or a number
bool isSymbol(char c) {
  return !((48 <= c && c < 58) || (65 <= c && c < 91) || (97 <= c && c < 123));
}

// Redacts all occurances of the inputted word 'redact' from the inputted string
void stringRedactWord(struct string *str, struct string *redact) {
  int redactLength = stringLength(redact);
  bool valid = true; // True if the next run of characters isn't preceded by letters or numbers
  // Repeats for each item in the string
  while (true) {
    if (valid && substringAtPos(redact, str)) {
      // The string isn't preceded by numbers or letters, and it matches the redact word
      if (stringGet(str, redactLength) == NULL) {
        // The redacted word is at the end of the string, so it is redacted and the loop breaks
        stringRedact(str, redactLength);
        break;
      } else if (isSymbol(stringGet(str, redactLength)->c)) {
        // The set of characters isn't preceded or followed by numbers or characters, and matches the redact word, so it is redacted
        stringRedact(str, redactLength);
        str = stringGet(str, redactLength - 1); // Skips past the word just redacted
      }
    }
    valid = isSymbol(str->c); // The next set of characters can't be redacted if they are preceded by characters or numbers
    if (str->next == NULL) {
      break;
    }
    str = str->next;
  }
}

// Redacts the set of words in the 'redact' string list from the inputted string
void stringRedactWords(struct string *str, struct stringList *redact) {
  while (true) {
    // Goes through each word in 'redact' and redacts them from the inputted string
    stringRedactWord(str, &(redact->s));
    if (redact->next == NULL) {
      break;
    }
    redact = redact->next;
  }
}

// Redacts all words from the string list 'redact' from the strings in the inputted string list 'strList'
void stringListRedactWords(struct stringList *strList, struct stringList *redact) {
  while (true) {
    // Goes through each string in 'strList' and redacts the words
    stringRedactWords(&(strList->s), redact);
    if (strList->next == NULL) {
      break;
    }
    strList = strList->next;
  }
}

int main(void) {
  // Copies the data to string lists
  struct stringList debate = copyTextFile("debate.txt");
  struct stringList redact = copyTextFile("redact.txt");
  stringListRedactWords(&debate, &redact); // Redacts words from string list
  stringListPrint(&debate); // Prints redacted text
  return 0;
}