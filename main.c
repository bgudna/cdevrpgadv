#include <ncurses.h>
#include <string.h>
#include <stdlib.h> // For random item generation
#include <time.h>   // For random seed
#include "places.h"

#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 100
#define MAX_INVENTORY_SIZE 5
#define MAX_ITEM_NAME_LEN 20

typedef struct {
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    struct {
        char name[MAX_ITEM_NAME_LEN];
        int value;
        char type[10]; // "weapon" or "food"
    } inventory[MAX_INVENTORY_SIZE];
    int num_items;
    int health;
    int mana;
    int experience;
} Adventurer;

void init_ncurses() {
    initscr();  // Initialize NCurses
    cbreak();   // Line buffering disabled
    noecho();   // Don't echo characters
    keypad(stdscr, TRUE); // Enable special keys
}

void print_center(int y, const char *text) {
    int x = (COLS - strlen(text)) / 2;
    mvprintw(y, x, "%s", text);
}

void draw_background() {
    clear();
    // Draw fantasy-themed background
    print_center(1, "~~~ Adventure Registration ~~~");
    print_center(3, "Welcome, brave adventurer!");
    print_center(5, "Please register to embark on your quest.");
    refresh();
}

void get_input(const char *prompt, char *buffer, int max_len) {
    echo();
    curs_set(1);
    print_center(7, prompt);
    mvgetnstr(7, 10, buffer, max_len);
    noecho();
    curs_set(0);
}

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

void display_inventory(const Adventurer *adv) {
    int i;
    print_center(15, "~~~ Inventory ~~~");
    print_center(17, "Your Inventory:");

    for (i = 0; i < adv->num_items; i++) {
        char item_text[MAX_ITEM_NAME_LEN + 60];
        snprintf(item_text, sizeof(item_text), "%d. %s (Value: %d, Type: %s)",
                 i + 1, adv->inventory[i].name, adv->inventory[i].value, adv->inventory[i].type);
        print_center(19 + i, item_text);
    }

    refresh();
}

void generate_items(Adventurer *adv) {
    const char *weapon_names[] = {"Sword", "Bow", "Staff", "Dagger", "Axe"};
    const char *food_names[] = {"Apple", "Bread", "Cheese", "Meat", "Potion"};
    int i;

    srand(time(NULL)); // Seed the random number generator

    for (i = 0; i < MAX_INVENTORY_SIZE; i++) {
        if (rand() % 2 == 0) { // Randomly choose weapon or food
            strcpy(adv->inventory[i].name, weapon_names[rand() % 5]);
            strcpy(adv->inventory[i].type, "weapon");
            adv->inventory[i].value = (rand() % 10) + 1; // Weapon value between 1 and 10
        } else {
            strcpy(adv->inventory[i].name, food_names[rand() % 5]);
            strcpy(adv->inventory[i].type, "food");
            adv->inventory[i].value = (rand() % 5) + 1; // Food value between 1 and 5
        }
    }

    adv->num_items = MAX_INVENTORY_SIZE;
}

void register_adventurer(Adventurer *adv) {
    draw_background();
    get_input("What is your adventurer name?:", adv->name, MAX_NAME_LEN);
    draw_background();
    get_input("What is your adventurer email?:", adv->email, MAX_EMAIL_LEN);

    // Simulate starting health, mana, and experience
    adv->health = 100;
    adv->mana = 50;
    adv->experience = 0;

    generate_items(adv); // Generate initial items for the adventurer

    clear();
    print_center(LINES / 2, "Registration successful! Your quest awaits...");
    refresh();
    getch(); // Wait for a key press
}

void display_status(const Adventurer *adv) {

    clear();
    print_center(1, "~~~ Current Status ~~~");
    print_center(3, "Name: ");
    print_center(4, adv->name);
    print_center(5, "Email: ");
    print_center(6, adv->email);
    print_center(8, "Health: ");
    print_center(9, "75");
    print_center(10, "Mana: ");
    print_center(11, "200");
    print_center(12, "Experience: ");
    print_center(13, "100");
    refresh();
}

int main() {
    Adventurer adventurer;

    init_ncurses();
    draw_background();
    register_adventurer(&adventurer);

    display_inventory(&adventurer);

    //getch(); // Wait for another key press before exiting
    
    goLook("around");
    while (1) {
        display_status(&adventurer);
        display_inventory(&adventurer);
        //refresh();
        getMeSomeInput();
        getAndParseAndGo();
    }
    
    endwin(); // End NCurses

    return 0;
}
