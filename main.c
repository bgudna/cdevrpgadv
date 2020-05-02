#include<stdio.h>
#include<string.h>
#include "places.h"

static char input[100];

static int getMeSomeInput() {
    printf("\nWhat now?: ");
    return fgets(input, sizeof(input), stdin) != NULL;
}

static int getAndParseAndGo() {
    char *verb = strtok(input, "\n");
    char *noun = strtok(NULL, "\n");

    if (verb != NULL) {
        if(strcmp(verb, "quit") == 0) {
            return 0;
        } else if (strcmp(verb, "look") == 0) {
            //printf("You see nothing, your eyes are bleeding\n");
            goLook(noun);
        } else if (strcmp(verb, "go") == 0) {
            //printf("You can't go anywhere, your eyes are bleeding from the encounter\n");
            goGo(noun);
        } else {
            printf("I can't really '%s' right now\n", verb);
        }
    }
    return 1;
}

int main() {
    printf("Welcome to the screaming brain.\n");
    printf("Something else to be said about this...\n");
    goLook("around");
    while (getMeSomeInput() && getAndParseAndGo());
    
    printf("alrighty then!\n");
    return 0;
}
