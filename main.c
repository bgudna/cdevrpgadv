#include <ncurses.h>
#include <string.h>
#include <stdlib.h> // For random item generation
#include <time.h>   // For random seed

#define MAX_NAME_LEN 50
#define MAX_INVENTORY_SIZE 5
#define MAX_ITEM_NAME_LEN 20
#define NUM_LOCATIONS 3

typedef struct {
    char name[MAX_NAME_LEN];
    struct {
        char name[MAX_ITEM_NAME_LEN];
        int value;
        char type[10]; // "weapon" or "food"
    } inventory[MAX_INVENTORY_SIZE];
    int num_items;
    int health;
    int mana;
    int current_location;
} Adventurer;

typedef struct {
    char name[50];
    char description[100];
} Location;

void init_ncurses() {
    initscr();  // Initialize NCurses
    cbreak();   // Line buffering disabled
    noecho();   // Don't echo characters
    keypad(stdscr, TRUE); // Enable special keys
    curs_set(1); // Enable cursor
    timeout(0); // Non-blocking getch
}

void print_center(int y, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[100]; // Assuming a maximum buffer size of 100 characters

    vsnprintf(buffer, sizeof(buffer), format, args);
    int x = (COLS - strlen(buffer)) / 2;

    mvprintw(y, x, "%s", buffer);

    va_end(args);
}

void draw_background(const Adventurer *adv, const Location *locations) {
    clear();
    print_center(1, "~~~ Text Adventure Game ~~~");
    print_center(3, "Welcome, brave adventurer!");
    print_center(5, "Please register to embark on your quest.");
    refresh();
}

void get_input(const char *prompt, char *buffer, int max_len) {
    print_center(7, prompt);
    echo(); // Enable echoing of characters
    curs_set(1); // Show cursor
    mvgetnstr(8, 10, buffer, max_len);
    curs_set(0); // Hide cursor
    noecho(); // Disable echoing of characters
}

void display_status_line(const Adventurer *adv, const Location *locations) {
    attron(A_REVERSE); // Enable reverse attribute for highlighted text
    mvprintw(0, 0, "Player: %s | Location: %s", adv->name, locations[adv->current_location].name);
    attroff(A_REVERSE); // Disable reverse attribute
}

void register_adventurer(Adventurer *adv) {
    char player_name[MAX_NAME_LEN];

    draw_background(adv, NULL);
    get_input("What is your adventurer name?:", player_name, MAX_NAME_LEN);

    // Copy player name to adventurer struct
    strncpy(adv->name, player_name, MAX_NAME_LEN - 1);
    adv->name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-terminated string

    // Simulate starting health, mana, and location
    adv->health = 100;
    adv->mana = 50;
    adv->current_location = 0;
    adv->num_items = 0;

    clear(); // Clear screen after registration
    print_center(LINES / 2, "Registration successful! Your quest awaits...");
    refresh();
    getch(); // Wait for a key press
}

void move_to_location(Adventurer *adv, int new_location, const Location *locations) {
    adv->current_location = new_location;
    clear();
    print_center(LINES / 2, "You have moved to %s.", locations[adv->current_location].name);
    refresh();
    getch(); // Wait for a key press
}

void main_game_loop(Adventurer *adv, const Location *locations) {
    int ch;

    while (1) {
        display_status_line(adv, locations);
        refresh();

        ch = getch(); // Get player input

        switch (ch) {
            case 'q':
                return; // Quit game
            case 's':
                // Show status screen
                clear();
                print_center(1, "~~~ Current Status ~~~");
                print_center(3, "Name: %s", adv->name);
                print_center(5, "Health: %d", adv->health);
                print_center(6, "Mana: %d", adv->mana);
                print_center(8, "Current Location: %s", locations[adv->current_location].name);
                print_center(10, "Press any key to continue...");
                refresh();
                getch(); // Wait for a key press to continue
                break;
            case '1':
            case '2':
            case '3':
                if (ch - '1' < NUM_LOCATIONS) {
                    move_to_location(adv, ch - '1', locations);
                }
                break;
        }
    }
}

int main() {
    Adventurer adventurer;
    Location locations[NUM_LOCATIONS] = {
        {"Town", "A bustling town with shops and villagers."},
        {"Forest", "A dense forest filled with mysterious creatures."},
        {"Cave", "A dark cave with hidden treasures."},
    };

    init_ncurses();
    register_adventurer(&adventurer);
    main_game_loop(&adventurer, locations);

    endwin(); // End NCurses

    return 0;
}
