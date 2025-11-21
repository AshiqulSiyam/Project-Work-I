#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#pragma comment(lib, "winmm.lib")

#define MAX_USERS 100
#define MAX_NAME_LEN 50
#define MAX_PASS_LEN 50
#define ROWS 30
#define COLS 64
#define POWER_PELLET_TIME 20 // Duration of power pellet effect in moves

// Color definitions
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_WHITE 15
#define COLOR_CYAN 11
#define COLOR_MAGENTA 13
#define COLOR_GRAY 8

HANDLE hConsole;

typedef struct {
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    int highscore;
} User;

User users[MAX_USERS];
int userCount = 0;
int currentUser = -1;

// Difficulty system
typedef enum {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
} Difficulty;

Difficulty currentDifficulty = EASY;
int numEnemies = 1;

char easyMap[ROWS][COLS + 1] = {
    "############################################################",
    "#                                                          #",
    "#   ##################     ######    ###################   #",
    "#                  ####      ##     ####                   #",
    "#    ####   ###    ####      ##     ####    ###    ####    #",
    "#    ####   ###    ####    ######   ####    ###    ####    #",
    "#    ####                                          ####    #",
    "#     ##################     ##     ##################     #",
    "#                            ##                            #",
    "#   ###############   ###    ##    ###   ###############   #",
    "#   ###               ###          ###               ###   #",
    "#   ###  ###   ##########  ##  ##  ##########  ###   ###   #",
    "#   ###               ###          ###               ###   #",
    "#   ###############   ###  ##  ##  ###   ###############   #",
    "#                                                          #",
    "#   ####    ###   #######   #  #   #######   ###    ####   #",
    "#   ####    ###   #######   #  #   #######   ###    ####   #",
    "#   ####    ###       ###   #  #   ###       ###    ####   #",
    "#   ####                    #  #                    ####   #",
    "#   ####     ############          ############     ####   #",
    "#   ####     ###                            ###     ####   #",
    "#   ####     ###                            ###     ####   #",
    "#   ####     ##################################     ####   #",
    "#   ####                                            ####   #",
    "#   ####   #############     ##     #############   ####   #",
    "#   ####   #############     ##     #############   ####   #",
    "#   ####        ####         ##          ####       ####   #",
    "#               ####     ##########      ####              #",
    "#       ####             ##########             ####       #",
    "############################################################"
};

char mediumMap[ROWS][COLS + 1] = {
    "############################################################",
    "#                                                          #",
    "# ####  ####  ####  ####  ####  ####  ####  ####  ####  # #",
    "#      #    #      #    #      #    #      #    #      #   #",
    "# #### # ## # #### # ## # #### # ## # #### # ## # #### ### #",
    "#      #    #      #    #      #    #      #    #      #   #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#                                                          #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#    #      #    #      #    #      #    #      #    #     #",
    "# ## # #### # ## # #### # ## # #### # ## # #### # ## # ### #",
    "#    #      #    #      #    #      #    #      #    #     #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#                                                          #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#    #      #    #      #    #      #    #      #    #     #",
    "# ## # #### # ## # #### # ## # #### # ## # #### # ## # ### #",
    "#    #      #    #      #    #      #    #      #    #     #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#                                                          #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#    #      #    #      #    #      #    #      #    #     #",
    "# ## # #### # ## # #### # ## # #### # ## # #### # ## # ### #",
    "#    #      #    #      #    #      #    #      #    #     #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#                                                          #",
    "# #### #### #### #### #### #### #### #### #### #### #### # #",
    "#                                                          #",
    "#                                                          #",
    "############################################################"
};

char hardMap[ROWS][COLS + 1] = {
    "############################################################",
    "#                                                          #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "# ### ### ### ### ### ### ### ### ### ### ### ### ### ### #",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#                                                          #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "# ### ### ### ### ### ### ### ### ### ### ### ### ### ### #",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#                                                          #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "# ### ### ### ### ### ### ### ### ### ### ### ### ### ### #",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#                                                          #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "# ### ### ### ### ### ### ### ### ### ### ### ### ### ### #",
    "#  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ##  ## #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#                                                          #",
    "###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###  ###",
    "#                                                          #",
    "#                                                          #",
    "############################################################"
};

char map[ROWS][COLS + 1];

#define MAX_ENEMIES 5
int enemyX[MAX_ENEMIES] = {7, 15, 25, 10, 20};
int enemyY[MAX_ENEMIES] = {7, 30, 40, 35, 15};
bool enemyVulnerable[MAX_ENEMIES] = {false, false, false, false, false};

int pacX = 1, pacY = 1;
int gameOver = 0;
int score = 0;
int powerPelletActive = 0;
int powerPelletTimer = 0;

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void clearScreen() {
    system("cls");
}

void printCentered(const char* text) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int len = strlen(text);
    int pos = (width - len) / 2;
    printf("%*s%s", pos, "", text);
}

void printHeader() {
    printf("\n");

    setColor(COLOR_YELLOW);
    printCentered("  _______ _    _ ______    _____ _    _  ____   _____ _______ \n");
    setColor(COLOR_RED);
    printCentered(" |__   __| |  | |  ____|  / ____| |  | |/ __ \\ / ____|__   __|\n");
    setColor(COLOR_CYAN);
    printCentered("    | |  | |__| | |__    | |  __| |__| | |  | | (___    | |   \n");
    setColor(COLOR_BLUE);
    printCentered("    | |  |  __  |  __|   | | |_ |  __  | |  | |\\___ \\   | |   \n");
    setColor(COLOR_MAGENTA);
    printCentered("    | |  | |  | | |____  | |__| | |  | | |__| |____) |  | |   \n");
    setColor(COLOR_GREEN);
    printCentered("    |_|  |_|  |_|______|  \\_____|_|  |_|\\____/|_____/   |_|   \n");
    printCentered("\n");

    printf("\n");
}
void loadUsers() {
    FILE* file = fopen("users.dat", "rb");
    if (file) {
        userCount = fread(users, sizeof(User), MAX_USERS, file);
        fclose(file);
    } else {
        // Create new file if it doesn't exist
        file = fopen("users.dat", "wb");
        if (file) fclose(file);
    }
}

void saveUsers() {
    FILE* file = fopen("users.dat", "wb");
    if (file) {
        size_t written = fwrite(users, sizeof(User), userCount, file);
        if (written != userCount) {
            setColor(COLOR_RED);
            printf("Error: Incomplete data write!\n");
            setColor(COLOR_WHITE);
        }
        fclose(file);
    } else {
        setColor(COLOR_RED);
        printf("Error saving user data!\n");
        setColor(COLOR_WHITE);
    }
}

int findUser(const char* username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

void getPasswordInput(char* password) {
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < MAX_PASS_LEN - 1) {
        if (ch == 8) { // Backspace
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else if (ch >= 32 && ch <= 126) { // Only printable ASCII
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

void registerUser() {
    clearScreen();
    printHeader();

    setColor(COLOR_CYAN);
    printCentered("=== REGISTER ===\n\n");
    setColor(COLOR_WHITE);

    if (userCount >= MAX_USERS) {
        setColor(COLOR_RED);
        printCentered("User database is full!\n");
        setColor(COLOR_WHITE);
        Sleep(1500);
        return;
    }

    User newUser;

    printCentered("Enter username (max 49 chars): ");
    setColor(COLOR_MAGENTA);
    fgets(newUser.username, MAX_NAME_LEN, stdin);
    newUser.username[strcspn(newUser.username, "\n")] = '\0'; // Remove newline
    setColor(COLOR_WHITE);

    if (strlen(newUser.username) == 0) {
        setColor(COLOR_RED);
        printCentered("Username cannot be empty!\n");
        setColor(COLOR_WHITE);
        Sleep(1500);
        return;
    }

    if (findUser(newUser.username) != -1) {
        setColor(COLOR_RED);
        printCentered("Username already exists!\n");
        setColor(COLOR_WHITE);
        Sleep(1500);
        return;
    }

    printCentered("Enter password (max 49 chars): ");
    setColor(COLOR_MAGENTA);
    getPasswordInput(newUser.password);
    setColor(COLOR_WHITE);

    if (strlen(newUser.password) == 0) {
        setColor(COLOR_RED);
        printCentered("Password cannot be empty!\n");
        setColor(COLOR_WHITE);
        Sleep(1500);
        return;
    }

    newUser.highscore = 0;
    users[userCount++] = newUser;
    saveUsers();

    setColor(COLOR_GREEN);
    printCentered("Registration successful!\n");
    setColor(COLOR_WHITE);
    Sleep(1500);
}

int loginUser() {
    clearScreen();
    printHeader();

    setColor(COLOR_CYAN);
    printCentered("=== LOGIN ===\n\n");
    setColor(COLOR_WHITE);

    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];

    printCentered("Username: ");
    setColor(COLOR_MAGENTA);
    fgets(username, MAX_NAME_LEN, stdin);
    username[strcspn(username, "\n")] = '\0';
    setColor(COLOR_WHITE);

    int userIndex = findUser(username);
    if (userIndex == -1) {
        setColor(COLOR_RED);
        printCentered("User not found!\n");
        setColor(COLOR_WHITE);
        Sleep(1500);
        return -1;
    }

    printCentered("Password: ");
    setColor(COLOR_MAGENTA);
    getPasswordInput(password);
    setColor(COLOR_WHITE);

    if (strcmp(users[userIndex].password, password) != 0) {
        setColor(COLOR_RED);
        printCentered("Incorrect password!\n");
        setColor(COLOR_WHITE);
        Sleep(1500);
        return -1;
    }

    setColor(COLOR_GREEN);
    printCentered("Login successful!\n");
    setColor(COLOR_WHITE);
    playLoginSound();
    Sleep(1500);

    return userIndex;
}

int selectDifficulty() {
    int choice;
    char input[10];

    do {
        clearScreen();
        printHeader();

        setColor(COLOR_CYAN);
        printCentered("=== SELECT DIFFICULTY ===\n\n");
        setColor(COLOR_WHITE);

        setColor(COLOR_GREEN);
        printCentered("1. Easy   - 1 Enemy\n");
        setColor(COLOR_YELLOW);
        printCentered("2. Medium - 2 Enemies\n");
        setColor(COLOR_RED);
        printCentered("3. Hard   - 5 Enemies\n\n");
        setColor(COLOR_WHITE);

        printCentered("Enter your choice (1-3): ");
        setColor(COLOR_MAGENTA);
        fgets(input, sizeof(input), stdin);
        char *endptr;
        choice = (int)strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') choice = 0;
        setColor(COLOR_WHITE);

        switch (choice) {
            case 1:
                currentDifficulty = EASY;
                numEnemies = 1;
                return 1;
            case 2:
                currentDifficulty = MEDIUM;
                numEnemies = 2;
                return 1;
            case 3:
                currentDifficulty = HARD;
                numEnemies = 5;
                return 1;
            default:
                setColor(COLOR_RED);
                printCentered("Invalid choice! Please enter 1-3.\n");
                setColor(COLOR_WHITE);
                Sleep(1000);
        }
    } while (1);
}

void showMainMenu() {
    int choice;
    char input[10];

    do {
        clearScreen();
        printHeader();

        setColor(COLOR_BLUE);
        printCentered("=== MAIN MENU ===\n\n");
        setColor(COLOR_WHITE);

        printCentered("1. Login\n");
        printCentered("2. Register\n");
        printCentered("3. Play as Guest\n");
        printCentered("4. Exit\n\n");

        printCentered("Enter your choice: ");
        setColor(COLOR_YELLOW);
        fgets(input, sizeof(input), stdin);
        char *endptr;
        choice = (int)strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') choice = 0;
        setColor(COLOR_WHITE);

        switch (choice) {
            case 1:
                currentUser = loginUser();
                if (currentUser != -1) return;
                break;
            case 2:
                registerUser();
                break;
            case 3:
                currentUser = -1;
                return;
            case 4:
                exit(0);
            default:
                setColor(COLOR_RED);
                printCentered("Invalid choice! Please enter 1-4.\n");
                setColor(COLOR_WHITE);
                Sleep(1000);
        }
    } while (1);
}

void loadMap() {
    if (currentDifficulty == EASY) {
        memcpy(map, easyMap, sizeof(easyMap));
    } else if (currentDifficulty == MEDIUM) {
        memcpy(map, mediumMap, sizeof(mediumMap));
    } else {
        memcpy(map, hardMap, sizeof(hardMap));
    }
}

void initGameConsole() {
    hConsole = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    SetConsoleActiveScreenBuffer(hConsole);
    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    SetConsoleTitle("THE GHOST Game");
}

void initializeDots() {
    loadMap();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (map[i][j] == ' ') {
                // Randomly place power pellets (5% chance)
                if (rand() % 20 == 0) {
                    map[i][j] = 'O';
                } else {
                    map[i][j] = '.';
                }
            }
        }
    }
}

void drawMap() {
    COORD coord = {0, 0};
    DWORD count;
    CHAR_INFO buffer[ROWS * COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            char displayChar = ' ';
            int color = 0;
            
            if (i == pacX && j == pacY) {
                displayChar = 'p';
                color = COLOR_YELLOW;
            } else {
                bool enemyHere = false;
                for (int e = 0; e < numEnemies; e++) {
                    if (i == enemyX[e] && j == enemyY[e]) {
                        displayChar = enemyVulnerable[e] ? 'V' : 'M';
                        color = enemyVulnerable[e] ? COLOR_CYAN : COLOR_RED;
                        enemyHere = true;
                        break;
                    }
                }
                
                if (!enemyHere) {
                    if (map[i][j] == '.') {
                        displayChar = '.';
                        color = COLOR_WHITE;
                    } else if (map[i][j] == 'O') {
                        displayChar = 'O';
                        color = COLOR_MAGENTA;
                    } else if (map[i][j] == '#') {
                        displayChar = 219;
                        color = COLOR_BLUE;
                    }
                }
            }
            
            buffer[i * COLS + j].Char.AsciiChar = displayChar;
            buffer[i * COLS + j].Attributes = color;
        }
    }

    SMALL_RECT rect = {0, 0, COLS-1, ROWS-1};
    WriteConsoleOutput(hConsole, buffer, (COORD){COLS, ROWS}, coord, &rect);

    char scoreText[200];
    snprintf(scoreText, sizeof(scoreText), "Score: %d | Highscore: %d | Player: %s | W,A,S,D to move | Q to quit | Power: %d",
            score,
            (currentUser != -1) ? users[currentUser].highscore : 0,
            (currentUser != -1) ? users[currentUser].username : "Guest",
            powerPelletTimer);
    WriteConsoleOutputCharacter(hConsole, scoreText, strlen(scoreText), (COORD){0, ROWS}, &count);
}

// Enhanced sound function with better path handling and fallback sounds
// Place coin.wav, death.wav, eat_enemy.wav, and power_pellet.wav in the same folder as the executable.
// If files don't exist, programmatic beeps are used as fallback.
void playSoundFile(const char* filename, int fallbackFreq, int fallbackDuration) {
    char fullPath[MAX_PATH];
    char exePath[MAX_PATH];
    
    // Get the directory where the executable is located
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    char* lastSlash = strrchr(exePath, '\\');
    if (lastSlash) {
        *lastSlash = '\0';
        snprintf(fullPath, MAX_PATH, "%s\\%s", exePath, filename);
    } else {
        strncpy(fullPath, filename, MAX_PATH - 1);
        fullPath[MAX_PATH - 1] = '\0';
    }
    
    // Try to play the sound file
    BOOL soundPlayed = PlaySoundA(fullPath, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    
    // If file doesn't exist or PlaySound failed, try current directory
    if (!soundPlayed) {
        soundPlayed = PlaySoundA(filename, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    }
    
    // If still failed, use fallback beep
    if (!soundPlayed) {
        Beep(fallbackFreq, fallbackDuration);
    }
}

// Convenience functions for different sound effects
void playCoinSound() {
    playSoundFile("coin.wav", 800, 100);  // High pitch, short beep
}

void playPowerPelletSound() {
    playSoundFile("power_pellet.wav", 1000, 150);  // Higher pitch, longer beep
}

void playEatEnemySound() {
    playSoundFile("eat_enemy.wav", 600, 200);  // Lower pitch, longer beep
}

void playDeathSound() {
    playSoundFile("death.wav", 300, 500);  // Very low pitch, long beep
}

void playMoveSound() {
    playSoundFile("move.wav", 400, 50);  // Very short, low beep for movement
}

void playLoginSound() {
    playSoundFile("login.wav", 500, 200);  // Medium pitch, short beep for login
}

void movePacman(char dir) {
    int newX = pacX, newY = pacY;
    if (dir == 'w' || dir == 'W') newX--;
    if (dir == 's' || dir == 'S') newX++;
    if (dir == 'a' || dir == 'A') newY--;
    if (dir == 'd' || dir == 'D') newY++;

    if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS && map[newX][newY] != '#') {
        if (map[newX][newY] == '.') {
            score += 10;
            map[newX][newY] = ' ';
            // play coin sound
            playCoinSound();
        } else if (map[newX][newY] == 'O') {
            score += 50;
            map[newX][newY] = ' ';
            powerPelletActive = 1;
            powerPelletTimer = POWER_PELLET_TIME;
            for (int i = 0; i < numEnemies; i++) {
                enemyVulnerable[i] = true;
            }
            // play power pellet sound
            playPowerPelletSound();
        }
        // Note: Movement sound removed to avoid excessive beeping
        // Uncomment the line below if you want movement sounds
        // playMoveSound();
        pacX = newX;
        pacY = newY;
    }
}

void moveEnemies() {
    if (powerPelletActive) {
        powerPelletTimer--;
        if (powerPelletTimer <= 0) {
            powerPelletActive = 0;
            for (int i = 0; i < numEnemies; i++) {
                enemyVulnerable[i] = false;
            }
        }
    }

    for (int e = 0; e < numEnemies; e++) {
        if (enemyVulnerable[e] && rand() % 100 < 40) {
            int dx = (pacX > enemyX[e]) ? -1 : (pacX < enemyX[e]) ? 1 : 0;
            int dy = (pacY > enemyY[e]) ? -1 : (pacY < enemyY[e]) ? 1 : 0;

            if (abs(dy) > 0 && map[enemyX[e]][enemyY[e] + dy] != '#') {
                enemyY[e] += dy;
                continue;
            }
            if (abs(dx) > 0 && map[enemyX[e] + dx][enemyY[e]] != '#') {
                enemyX[e] += dx;
                continue;
            }
        }
        else if (rand() % 100 < 70) {
            int dx = (pacX > enemyX[e]) ? 1 : (pacX < enemyX[e]) ? -1 : 0;
            int dy = (pacY > enemyY[e]) ? 1 : (pacY < enemyY[e]) ? -1 : 0;

            if (abs(dy) > 0 && map[enemyX[e]][enemyY[e] + dy] != '#') {
                enemyY[e] += dy;
                continue;
            }
            if (abs(dx) > 0 && map[enemyX[e] + dx][enemyY[e]] != '#') {
                enemyX[e] += dx;
                continue;
            }
        }

        int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int order[4] = {0, 1, 2, 3};

        for (int i = 0; i < 4; i++) {
            int j = rand() % 4;
            int temp = order[i];
            order[i] = order[j];
            order[j] = temp;
        }

        for (int i = 0; i < 4; i++) {
            int newX = enemyX[e] + directions[order[i]][0];
            int newY = enemyY[e] + directions[order[i]][1];

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS &&
                map[newX][newY] != '#') {
                enemyX[e] = newX;
                enemyY[e] = newY;
                break;
            }
        }
    }
}

void checkGameStatus() {
    for (int e = 0; e < numEnemies; e++) {
        if (pacX == enemyX[e] && pacY == enemyY[e]) {
            if (enemyVulnerable[e]) {
                score += 200;
                enemyX[e] = 5 + e * 3;
                enemyY[e] = 5 + e * 8;
                enemyVulnerable[e] = false;
                playEatEnemySound();
                return;
            } else {
                playDeathSound();
                gameOver = 1;
                return;
            }
        }
    }

    // Check if all dots eaten
    int dotsRemaining = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (map[i][j] == '.' || map[i][j] == 'O') {
                dotsRemaining = 1;
                break;
            }
        }
        if (dotsRemaining) break;
    }

    if (!dotsRemaining) {
        gameOver = 2;
    }
}

void resetGame() {
    pacX = 1; pacY = 1;
    int startPositions[5][2] = {{7, 7}, {15, 30}, {25, 40}, {10, 35}, {20, 15}};
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyX[i] = startPositions[i][0];
        enemyY[i] = startPositions[i][1];
        enemyVulnerable[i] = false;
    }
    score = 0;
    gameOver = 0;
    powerPelletActive = 0;
    powerPelletTimer = 0;
}

void playGame() {
    srand((unsigned int)time(NULL));
    initGameConsole();
    initializeDots();
    resetGame();

    while (!gameOver) {
        drawMap();

        if (_kbhit()) {
            char input = _getch();
            if (input == 'q' || input == 'Q') break;
            movePacman(input);
        }

        moveEnemies();
        checkGameStatus();
        Sleep(200);
    }

    CloseHandle(hConsole);
    system("cls");

    if (gameOver == 1) {
        setColor(COLOR_RED);
        printf("\n\n  Game Over! Enemy caught you!\n");
    } else if (gameOver == 2) {
        setColor(COLOR_GREEN);
        printf("\n\n  Congratulations! You won!\n");
    }

    if (currentUser != -1 && score > users[currentUser].highscore) {
        users[currentUser].highscore = score;
        saveUsers();
        setColor(COLOR_YELLOW);
        printf("  New High Score!\n");
    }

    setColor(COLOR_WHITE);
    printf("\n  Final Score: %d\n", score);
    if (currentUser != -1) {
        printf("  High Score: %d\n", users[currentUser].highscore);
    }
    printf("\n  Press any key to return to menu...");
    _getch();
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    loadUsers();

    while (1) {
        showMainMenu();
        selectDifficulty();
        playGame();
    }

    return 0;
}
