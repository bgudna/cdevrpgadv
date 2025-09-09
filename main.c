#include <ncurses.h>
#include <string.h>
#include <stdlib.h> // For random item generation
#include <time.h>   // For random seed

#define MAX_NAME_LEN 50
#define MAX_INVENTORY_SIZE 10
#define MAX_ITEM_NAME_LEN 20
#define NUM_LOCATIONS 3
#define MAX_ENEMY_NAME_LEN 30
#define MAX_LOCATION_NAME_LEN 30

// Item types
typedef enum {
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_CONSUMABLE,
    ITEM_TYPE_QUEST
} ItemType;

// Item structure
typedef struct {
    char name[MAX_ITEM_NAME_LEN];
    char description[100];
    ItemType type;
    int value; // Could be attack bonus, defense bonus, or healing amount
    int rarity; // 1-5 scale (5 being rarest)
} Item;

// Enemy structure
typedef struct {
    char name[MAX_ENEMY_NAME_LEN];
    int health;
    int max_health;
    int attack;
    int defense;
    int exp_reward;
    int gold_reward;
} Enemy;

// Stats structure
typedef struct {
    int strength;
    int intelligence;
    int agility;
    int health;
    int max_health;
    int mana;
    int max_mana;
    int experience;
    int level;
    int defense;
} Stats;

// Adventurer structure
typedef struct {
    char name[MAX_NAME_LEN];
    Stats stats;
    Item inventory[MAX_INVENTORY_SIZE];
    int num_items;
    int current_location;
    int gold;
} Adventurer;

// Location structure
typedef struct {
    char name[MAX_LOCATION_NAME_LEN];
    char description[100];
    int has_enemy;
    Enemy enemy;
    int num_items;
    Item items[5]; // Max 5 items per location
} Location;

// Function declarations
void init_ncurses();
void print_center(int y, const char *format, ...);
void draw_background(const Adventurer *adv, const Location *locations);
void get_input(const char *prompt, char *buffer, int max_len);
void display_status_line(const Adventurer *adv, const Location *locations);
void register_adventurer(Adventurer *adv);
void move_to_location(Adventurer *adv, int new_location, const Location *locations);
void main_game_loop(Adventurer *adv, const Location *locations);
void create_character(Adventurer *adv);
void display_character_sheet(const Adventurer *adv);
void add_item_to_inventory(Adventurer *adv, const Item *item);
int has_item(const Adventurer *adv, const char *item_name);
void use_item(Adventurer *adv, const char *item_name);
void display_inventory(const Adventurer *adv);
void generate_world(Location *locations);
void generate_location_items(Location *location);
void generate_enemy(Location *location);
void display_location_info(const Location *location);
void encounter_enemy(Adventurer *adv, const Location *location);
void display_combat_menu(const Adventurer *adv, const Enemy *enemy);
void combat_round(Adventurer *adv, Enemy *enemy);
void gain_experience(Adventurer *adv, int exp_gained);
void level_up(Adventurer *adv);
void display_location_menu(const Location *location);
int calculate_damage(int attack, int defense);

// Global game state
int game_running = 1;

void init_ncurses() {
    initscr();  // Initialize NCurses
    cbreak();   // Line buffering disabled
    noecho();   // Don't echo characters
    keypad(stdscr, TRUE); // Enable special keys
    curs_set(0); // Hide cursor
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
    mvprintw(0, 0, "Player: %s | Location: %s | Level: %d | HP: %d/%d | MP: %d/%d | Gold: %d", 
             adv->name, locations[adv->current_location].name, 
             adv->stats.level, adv->stats.health, adv->stats.max_health,
             adv->stats.mana, adv->stats.max_mana, adv->gold);
    attroff(A_REVERSE); // Disable reverse attribute
}

void create_character(Adventurer *adv) {
    char player_name[MAX_NAME_LEN];
    int class_choice;

    clear();
    print_center(1, "~~~ Character Creation ~~~");
    print_center(3, "What is your adventurer name?:");
    refresh();
    
    echo(); // Enable echoing of characters
    curs_set(1); // Show cursor
    mvgetnstr(4, 10, player_name, MAX_NAME_LEN);
    curs_set(0); // Hide cursor
    noecho(); // Disable echoing of characters

    // Copy player name to adventurer struct
    strncpy(adv->name, player_name, MAX_NAME_LEN - 1);
    adv->name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-terminated string

    clear();
    print_center(1, "~~~ Character Creation ~~~");
    print_center(3, "Choose your class:");
    print_center(5, "1. Warrior (High Health, Low Mana)");
    print_center(6, "2. Mage (Low Health, High Mana)");
    print_center(7, "3. Rogue (Balanced Stats)");
    print_center(9, "Enter choice (1-3):");
    refresh();

    // Get class choice
    int ch;
    while ((ch = getch()) != '1' && ch != '2' && ch != '3') {
        // Wait for valid input
    }
    
    class_choice = ch - '0';

    // Initialize stats based on class choice
    switch (class_choice) {
        case 1: // Warrior
            adv->stats.strength = 15;
            adv->stats.intelligence = 8;
            adv->stats.agility = 10;
            adv->stats.max_health = 120;
            adv->stats.health = 120;
            adv->stats.max_mana = 30;
            adv->stats.mana = 30;
            break;
        case 2: // Mage
            adv->stats.strength = 8;
            adv->stats.intelligence = 15;
            adv->stats.agility = 10;
            adv->stats.max_health = 70;
            adv->stats.health = 70;
            adv->stats.max_mana = 80;
            adv->stats.mana = 80;
            break;
        case 3: // Rogue
            adv->stats.strength = 12;
            adv->stats.intelligence = 12;
            adv->stats.agility = 15;
            adv->stats.max_health = 90;
            adv->stats.health = 90;
            adv->stats.max_mana = 50;
            adv->stats.mana = 50;
            break;
    }
    
    adv->stats.level = 1;
    adv->stats.experience = 0;
    adv->gold = 50;
    adv->num_items = 0;

    clear(); // Clear screen after registration
    print_center(LINES / 2, "Character created successfully! Your quest awaits...");
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

void display_character_sheet(const Adventurer *adv) {
    clear();
    print_center(1, "~~~ Character Sheet ~~~");
    print_center(3, "Name: %s", adv->name);
    print_center(4, "Level: %d", adv->stats.level);
    print_center(5, "Health: %d/%d", adv->stats.health, adv->stats.max_health);
    print_center(6, "Mana: %d/%d", adv->stats.mana, adv->stats.max_mana);
    print_center(7, "Gold: %d", adv->gold);
    print_center(9, "Strength: %d", adv->stats.strength);
    print_center(10, "Intelligence: %d", adv->stats.intelligence);
    print_center(11, "Agility: %d", adv->stats.agility);
    print_center(13, "Experience: %d", adv->stats.experience);
    print_center(15, "Press any key to continue...");
    refresh();
    getch(); // Wait for a key press to continue
}

void display_inventory(const Adventurer *adv) {
    clear();
    print_center(1, "~~~ Inventory ~~~");
    
    if (adv->num_items == 0) {
        print_center(3, "Inventory is empty.");
    } else {
        for (int i = 0; i < adv->num_items; i++) {
            mvprintw(3 + i, 5, "%d. %s (%s)", i+1, adv->inventory[i].name, 
                     adv->inventory[i].type == ITEM_TYPE_WEAPON ? "Weapon" :
                     adv->inventory[i].type == ITEM_TYPE_ARMOR ? "Armor" :
                     adv->inventory[i].type == ITEM_TYPE_CONSUMABLE ? "Consumable" : "Quest");
        }
    }
    
    print_center(LINES - 2, "Press any key to continue...");
    refresh();
    getch(); // Wait for a key press
}

void add_item_to_inventory(Adventurer *adv, const Item *item) {
    if (adv->num_items < MAX_INVENTORY_SIZE) {
        strcpy(adv->inventory[adv->num_items].name, item->name);
        strcpy(adv->inventory[adv->num_items].description, item->description);
        adv->inventory[adv->num_items].type = item->type;
        adv->inventory[adv->num_items].value = item->value;
        adv->inventory[adv->num_items].rarity = item->rarity;
        adv->num_items++;
    }
}

int has_item(const Adventurer *adv, const char *item_name) {
    for (int i = 0; i < adv->num_items; i++) {
        if (strcmp(adv->inventory[i].name, item_name) == 0) {
            return 1;
        }
    }
    return 0;
}

void use_item(Adventurer *adv, const char *item_name) {
    for (int i = 0; i < adv->num_items; i++) {
        if (strcmp(adv->inventory[i].name, item_name) == 0) {
            if (adv->inventory[i].type == ITEM_TYPE_CONSUMABLE) {
                // Apply item effect
                if (strcmp(adv->inventory[i].name, "Health Potion") == 0) {
                    int heal_amount = 30;
                    adv->stats.health += heal_amount;
                    if (adv->stats.health > adv->stats.max_health) {
                        adv->stats.health = adv->stats.max_health;
                    }
                    clear();
                    print_center(1, "You used a Health Potion and recovered %d HP!", heal_amount);
                    print_center(3, "Press any key to continue...");
                    refresh();
                    getch();
                }
                
                // Remove item from inventory
                for (int j = i; j < adv->num_items - 1; j++) {
                    adv->inventory[j] = adv->inventory[j + 1];
                }
                adv->num_items--;
                return;
            }
        }
    }
}

void generate_location_items(Location *location) {
    // Add some items to locations
    location->num_items = 0;
    
    // Add some standard items to the first location (Town)
    if (strcmp(location->name, "Town") == 0) {
        Item item1;
        strcpy(item1.name, "Health Potion");
        strcpy(item1.description, "Restores 30 HP");
        item1.type = ITEM_TYPE_CONSUMABLE;
        item1.value = 30;
        item1.rarity = 2;
        location->items[location->num_items++] = item1;
        
        Item item2;
        strcpy(item2.name, "Iron Sword");
        strcpy(item2.description, "A sturdy sword");
        item2.type = ITEM_TYPE_WEAPON;
        item2.value = 5;
        item2.rarity = 3;
        location->items[location->num_items++] = item2;
    }
    // Add items to Forest
    else if (strcmp(location->name, "Forest") == 0) {
        Item item1;
        strcpy(item1.name, "Health Potion");
        strcpy(item1.description, "Restores 30 HP");
        item1.type = ITEM_TYPE_CONSUMABLE;
        item1.value = 30;
        item1.rarity = 2;
        location->items[location->num_items++] = item1;
        
        Item item2;
        strcpy(item2.name, "Forest Moss");
        strcpy(item2.description, "A mysterious green moss");
        item2.type = ITEM_TYPE_QUEST;
        item2.value = 0;
        item2.rarity = 1;
        location->items[location->num_items++] = item2;
    }
    // Add items to Cave
    else if (strcmp(location->name, "Cave") == 0) {
        Item item1;
        strcpy(item1.name, "Gold Coin");
        strcpy(item1.description, "A shiny gold coin");
        item1.type = ITEM_TYPE_QUEST;
        item1.value = 0;
        item1.rarity = 1;
        location->items[location->num_items++] = item1;
        
        Item item2;
        strcpy(item2.name, "Ancient Sword");
        strcpy(item2.description, "A sword from ancient times");
        item2.type = ITEM_TYPE_WEAPON;
        item2.value = 10;
        item2.rarity = 4;
        location->items[location->num_items++] = item2;
    }
}

void generate_enemy(Location *location) {
    // Set that this location has an enemy
    location->has_enemy = 1;
    
    // Create enemy based on location type
    if (strcmp(location->name, "Forest") == 0) {
        strcpy(location->enemy.name, "Goblin");
        location->enemy.health = 40;
        location->enemy.max_health = 40;
        location->enemy.attack = 8;
        location->enemy.defense = 2;
        location->enemy.exp_reward = 25;
        location->enemy.gold_reward = 10;
    } else if (strcmp(location->name, "Cave") == 0) {
        strcpy(location->enemy.name, "Orc");
        location->enemy.health = 60;
        location->enemy.max_health = 60;
        location->enemy.attack = 12;
        location->enemy.defense = 4;
        location->enemy.exp_reward = 40;
        location->enemy.gold_reward = 20;
    } else {
        // Town has no enemy
        location->has_enemy = 0;
    }
}

void display_location_info(const Location *location) {
    clear();
    print_center(1, "~~~ %s ~~~", location->name);
    print_center(3, "%s", location->description);
    
    if (location->num_items > 0) {
        print_center(5, "Items here:");
        for (int i = 0; i < location->num_items; i++) {
            mvprintw(6 + i, 5, "- %s", location->items[i].name);
        }
    }
    
    if (location->has_enemy) {
        print_center(10, "You hear a menacing presence...");
    }
    
    print_center(LINES - 2, "Press any key to continue...");
    refresh();
    getch(); // Wait for a key press
}

void encounter_enemy(Adventurer *adv, const Location *location) {
    if (!location->has_enemy) return;
    
    Enemy *enemy = &location->enemy;
    
    clear();
    print_center(1, "~~~ Combat! ~~~");
    print_center(3, "A wild %s appears!", enemy->name);
    print_center(5, "HP: %d/%d", enemy->health, enemy->max_health);
    print_center(7, "What will you do?");
    print_center(9, "1. Fight");
    print_center(10, "2. Run Away");
    refresh();
    
    int ch = getch();
    if (ch == '1') {
        // Start combat
        while (enemy->health > 0 && adv->stats.health > 0) {
            combat_round(adv, enemy);
            
            // Check if enemy is defeated
            if (enemy->health <= 0) {
                clear();
                print_center(1, "~~~ Victory! ~~~");
                print_center(3, "You defeated the %s!", enemy->name);
                print_center(5, "Gained %d XP", enemy->exp_reward);
                print_center(6, "Gained %d Gold", enemy->gold_reward);
                gain_experience(adv, enemy->exp_reward);
                adv->gold += enemy->gold_reward;
                refresh();
                getch();
                break;
            }
            
            // Check if player is defeated
            if (adv->stats.health <= 0) {
                clear();
                print_center(1, "~~~ Game Over ~~~");
                print_center(3, "You have been defeated by the %s!", enemy->name);
                print_center(5, "Better luck next time...");
                refresh();
                getch();
                game_running = 0;
                break;
            }
            
            // Small delay for readability
            napms(500);
        }
    } else if (ch == '2') {
        // Simple run away chance
        int escape_chance = 70; // 70% chance to escape
        if (rand() % 100 < escape_chance) {
            clear();
            print_center(1, "~~~ Escape Successful! ~~~");
            print_center(3, "You escaped from the %s.", enemy->name);
            refresh();
            getch();
        } else {
            clear();
            print_center(1, "~~~ Escape Failed! ~~~");
            print_center(3, "You couldn't escape from the %s.", enemy->name);
            refresh();
            getch();
            // Enemy attacks on failed escape
            combat_round(adv, enemy);
        }
    }
}

void display_combat_menu(const Adventurer *adv, const Enemy *enemy) {
    clear();
    print_center(1, "~~~ Combat ~~~");
    print_center(3, "%s (HP: %d/%d)", enemy->name, enemy->health, enemy->max_health);
    print_center(5, "%s (HP: %d/%d)", adv->name, adv->stats.health, adv->stats.max_health);
    print_center(7, "1. Attack");
    print_center(8, "2. Use Item");
    print_center(9, "3. Run Away");
    refresh();
}

void combat_round(Adventurer *adv, Enemy *enemy) {
    display_combat_menu(adv, enemy);
    
    // Player attacks first
    int player_damage = calculate_damage(adv->stats.strength, enemy->defense);
    enemy->health -= player_damage;
    if (enemy->health < 0) enemy->health = 0;
    
    clear();
    print_center(1, "~~~ Combat Round ~~~");
    print_center(3, "%s attacks %s for %d damage!", adv->name, enemy->name, player_damage);
    
    // Enemy attacks back
    int enemy_damage = calculate_damage(enemy->attack, adv->stats.defense);
    adv->stats.health -= enemy_damage;
    if (adv->stats.health < 0) adv->stats.health = 0;
    
    print_center(5, "%s attacks %s for %d damage!", enemy->name, adv->name, enemy_damage);
    refresh();
    getch();
}

int calculate_damage(int attack, int defense) {
    int damage = attack - defense/2;
    if (damage < 1) damage = 1;
    return damage;
}

void gain_experience(Adventurer *adv, int exp_gained) {
    adv->stats.experience += exp_gained;
    
    // Check for level up
    if (adv->stats.experience >= adv->stats.level * 100) {
        level_up(adv);
    }
}

void level_up(Adventurer *adv) {
    adv->stats.level++;
    
    // Improve stats based on class
    switch (adv->stats.level % 3) {
        case 0: // Warrior
            adv->stats.max_health += 10;
            adv->stats.health = adv->stats.max_health;
            adv->stats.strength += 2;
            break;
        case 1: // Mage
            adv->stats.max_mana += 10;
            adv->stats.mana = adv->stats.max_mana;
            adv->stats.intelligence += 2;
            break;
        case 2: // Rogue
            adv->stats.max_health += 5;
            adv->stats.max_mana += 5;
            adv->stats.health = adv->stats.max_health;
            adv->stats.mana = adv->stats.max_mana;
            adv->stats.agility += 2;
            break;
    }
    
    clear();
    print_center(1, "~~~ Level Up! ~~~");
    print_center(3, "Congratulations! You reached level %d!", adv->stats.level);
    refresh();
    getch();
}

void display_location_menu(const Location *location) {
    clear();
    print_center(1, "~~~ %s ~~~", location->name);
    print_center(3, "%s", location->description);
    
    if (location->num_items > 0) {
        print_center(5, "Items here:");
        for (int i = 0; i < location->num_items; i++) {
            mvprintw(6 + i, 5, "%d. Pick up %s", i+1, location->items[i].name);
        }
    }
    
    if (location->has_enemy) {
        print_center(10, "You hear a menacing presence...");
    }
    
    print_center(LINES - 3, "1. Move to another location");
    print_center(LINES - 2, "2. Look around");
    print_center(LINES - 1, "Press any key to continue...");
    refresh();
    getch(); // Wait for a key press
}

void main_game_loop(Adventurer *adv, const Location *locations) {
    int ch;

    while (game_running) {
        display_status_line(adv, locations);
        refresh();

        ch = getch(); // Get player input

        switch (ch) {
            case 'q':
                game_running = 0; // Quit game
                break;
            case 's':
                // Show status screen
                display_character_sheet(adv);
                break;
            case 'i':
                // Show inventory screen
                display_inventory(adv);
                break;
            case 'l':
                // Look around at current location
                display_location_info(&locations[adv->current_location]);
                break;
            case '1':
            case '2':
            case '3':
                if (ch - '1' < NUM_LOCATIONS) {
                    move_to_location(adv, ch - '1', locations);
                    
                    // Check for enemy encounter
                    if (locations[adv->current_location].has_enemy) {
                        encounter_enemy(adv, &locations[adv->current_location]);
                    }
                }
                break;
            case 'g':
                // Pick up items from current location (if any)
                {
                    Location *current_location = (Location *)&locations[adv->current_location];
                    if (current_location->num_items > 0) {
                        // Simple item pickup - add first item to inventory
                        Item *item = &current_location->items[0];
                        add_item_to_inventory(adv, item);
                        
                        clear();
                        print_center(1, "You picked up %s!", item->name);
                        refresh();
                        getch();
                        
                        // Remove item from location (simplified)
                        current_location->num_items--;
                    }
                }
                break;
            case 'u':
                // Use an item (if player has items)
                if (adv->num_items > 0) {
                    // Simple item use - just use the first item
                    use_item(adv, adv->inventory[0].name);
                }
                break;
        }
    }
}

int main() {
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    Adventurer adventurer;
    Location locations[NUM_LOCATIONS] = {
        {"Town", "A bustling town with shops and villagers.", 0, {0}, 0, {{0}}},
        {"Forest", "A dense forest filled with mysterious creatures.", 0, {0}, 0, {{0}}},
        {"Cave", "A dark cave with hidden treasures.", 0, {0}, 0, {{0}}},
    };

    // Generate world
    for (int i = 0; i < NUM_LOCATIONS; i++) {
        generate_location_items(&locations[i]);
        generate_enemy(&locations[i]);
    }

    init_ncurses();
    create_character(&adventurer);
    main_game_loop(&adventurer, locations);

    endwin(); // End NCurses

    return 0;
}
