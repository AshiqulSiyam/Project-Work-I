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

char map[ROWS][COLS + 1] = {
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

int pacX = 1, pacY = 1;
int enemyX = 7, enemyY = 7;
int gameOver = 0;
int score = 0;
int powerPelletActive = 0;
int powerPelletTimer = 0;
bool enemyVulnerable = false;

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
        fwrite(users, sizeof(User), userCount, file);
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
        } else {
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
    Sleep(1500);

    return userIndex;
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
        choice = atoi(input);
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
    SetConsoleTitle("PAC MAN Game");
}

void initializeDots() {
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
            buffer[i * COLS + j].Char.AsciiChar =
                (i == pacX && j == pacY) ? 'p' :
                (i == enemyX && j == enemyY) ? (enemyVulnerable ? 'V' : 'M') :
                (map[i][j] == '.') ? '.' :
                (map[i][j] == 'O') ? 'O' :
                (map[i][j] == '#') ? 219 : ' ';

            buffer[i * COLS + j].Attributes =
                (i == pacX && j == pacY) ? COLOR_YELLOW :
                (i == enemyX && j == enemyY) ? (enemyVulnerable ? COLOR_CYAN : COLOR_RED) :
                (map[i][j] == '#') ? COLOR_BLUE :
                (map[i][j] == '.') ? COLOR_WHITE :
                (map[i][j] == 'O') ? COLOR_MAGENTA : 0;
        }
    }

    SMALL_RECT rect = {0, 0, COLS-1, ROWS-1};
    WriteConsoleOutput(hConsole, buffer, (COORD){COLS, ROWS}, coord, &rect);

    char scoreText[200];
    sprintf(scoreText, "Score: %d | Highscore: %d | Player: %s | W,A,S,D to move | Q to quit | Power: %d",
            score,
            (currentUser != -1) ? users[currentUser].highscore : 0,
            (currentUser != -1) ? users[currentUser].username : "Guest",
            powerPelletTimer);
    WriteConsoleOutputCharacter(hConsole, scoreText, strlen(scoreText), (COORD){0, ROWS}, &count);
}

// Simple wrapper to play a WAV file asynchronously.
// Place coin.wav, death.wav and eat_enemy.wav in the same folder as the executable.
// If PlaySound fails, a short Beep is used as fallback so there is always feedback.
void playSoundFile(const char* filename) {
    if (!PlaySoundA(filename, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT)) {
        // fallback beep
        Beep(800, 120);
    }
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
            // play coin sound (coin.wav)
            playSoundFile("coin.wav");
        } else if (map[newX][newY] == 'O') {
            score += 50;
            map[newX][newY] = ' ';
            powerPelletActive = 1;
            powerPelletTimer = POWER_PELLET_TIME;
            enemyVulnerable = true;
            // play power pellet sound (optional reuse coin.wav)
            playSoundFile("coin.wav");
        }
        pacX = newX;
        pacY = newY;
    }
}

void moveEnemy() {
    // Update power pellet timer
    if (powerPelletActive) {
        powerPelletTimer--;
        if (powerPelletTimer <= 0) {
            powerPelletActive = 0;
            enemyVulnerable = false;
        }
    }

    // If vulnerable, sometimes flee instead of chase
    if (enemyVulnerable && rand() % 100 < 40) {
        int dx = (pacX > enemyX) ? -1 : (pacX < enemyX) ? 1 : 0;
        int dy = (pacY > enemyY) ? -1 : (pacY < enemyY) ? 1 : 0;

        if (abs(dy) > 0 && map[enemyX][enemyY + dy] != '#') {
            enemyY += dy;
            return;
        }
        if (abs(dx) > 0 && map[enemyX + dx][enemyY] != '#') {
            enemyX += dx;
            return;
        }
    }
    // Normal chasing behavior
    else if (rand() % 100 < 70) {
        int dx = (pacX > enemyX) ? 1 : (pacX < enemyX) ? -1 : 0;
        int dy = (pacY > enemyY) ? 1 : (pacY < enemyY) ? -1 : 0;

        if (abs(dy) > 0 && map[enemyX][enemyY + dy] != '#') {
            enemyY += dy;
            return;
        }
        if (abs(dx) > 0 && map[enemyX + dx][enemyY] != '#') {
            enemyX += dx;
            return;
        }
    }

    // Random movement if no clear path
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int order[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int newX = enemyX + directions[order[i]][0];
        int newY = enemyY + directions[order[i]][1];

        if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS &&
            map[newX][newY] != '#') {
            enemyX = newX;
            enemyY = newY;
            return;
        }
    }
}

void checkGameStatus() {
    // Check if enemy caught pacman
    if (pacX == enemyX && pacY == enemyY) {
        if (enemyVulnerable) {
            // Eat the enemy
            score += 200;
            enemyX = ROWS / 2;
            enemyY = COLS / 2;
            // play enemy eaten sound
            playSoundFile("eat_enemy.wav");
            return;
        } else {
            // play death sound
            playSoundFile("death.wav");
            gameOver = 1;
            return;
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
    enemyX = 5; enemyY = 5;
    score = 0;
    gameOver = 0;
    powerPelletActive = 0;
    powerPelletTimer = 0;
    enemyVulnerable = false;
    initializeDots();
}

void playGame() {
    srand((unsigned int)time(NULL));
    initializeDots();
    initGameConsole();
    resetGame();

    while (!gameOver) {
        drawMap();

        if (_kbhit()) {
            char input = _getch();
            if (input == 'q' || input == 'Q') break;
            movePacman(input);
        }

        moveEnemy();
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
        playGame();
    }

    return 0;
}
