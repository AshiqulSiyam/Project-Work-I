# THE GHOST - Pac-Man Style Game

## Project Information

**Course:** Project Work I  
**Department:** Computer Science and Engineering (CSE)  
**Institution:** Shahjalal University of Science and Technology (SUST)

### Submitted To
**Md. Shadmim Hasan Sifat**  
Lecturer  
Department of CSE, SUST

### Submitted By
**Team Number:** 17  
**Team Name:** The Ghost

#### Team Members
1. **Ashiqul Islam Siyam** (Team Leader)  
   Registration: 2023331021

2. **Rahat Khan**  
   Registration: 2023331003

3. **Abdullah Siam**  
   Registration: 2023331059

---

## Project Overview

**THE GHOST** is a classic Pac-Man style arcade game developed in C programming language for Windows platform. The game features a player-controlled character navigating through mazes while avoiding enemies and collecting dots to achieve high scores.

---

## Features

### Game Modes
- **Easy Mode:** Single enemy, beginner-friendly maze layout
- **Medium Mode:** Two enemies, grid-pattern maze with moderate difficulty
- **Hard Mode:** Five enemies, complex dense maze for advanced players

### User Management
- **User Registration:** Create new player accounts with username and password
- **Secure Login:** Password-protected authentication system
- **Guest Mode:** Play without registration
- **High Score Tracking:** Persistent storage of player achievements

### Gameplay Features
- **Multiple Difficulty Levels:** Three distinct maze layouts with varying enemy counts
- **Power Pellets:** Temporary invincibility to eat enemies
- **Score System:** Points for collecting dots, power pellets, and eating enemies
- **Enemy AI:** Intelligent chase and flee behaviors
- **Real-time Display:** Live score, high score, and power pellet timer

### Audio System
- **Sound Effects:** Login, coin collection, power pellet, enemy eaten, and death sounds
- **Fallback System:** Programmatic beeps when audio files are unavailable

### Visual Design
- **Colorful Interface:** Multi-colored ASCII art and UI elements
- **Dynamic Maps:** Three unique maze designs for different difficulties
- **Smooth Animations:** Real-time character and enemy movement

---

## Technical Specifications

### Programming Language
- **C Language** with Windows API

### Libraries Used
- `stdio.h` - Standard input/output
- `stdlib.h` - Memory allocation and process control
- `conio.h` - Console input/output (Windows)
- `windows.h` - Windows API functions
- `mmsystem.h` - Multimedia sound system
- `time.h` - Random number generation
- `string.h` - String manipulation
- `stdbool.h` - Boolean data type

### Platform
- **Windows OS** (Windows 7 and above)

### Compiler
- **GCC (MinGW)** or **Microsoft Visual C++**

---

## Installation & Setup

### Prerequisites
- Windows Operating System
- GCC Compiler (MinGW) or Visual Studio

### Compilation

#### Using GCC:
```bash
gcc -o game.exe game.c -lwinmm
```

#### Using Visual Studio:
1. Create a new C project
2. Add `game.c` to the project
3. Build the solution

### Running the Game
```bash
game.exe
```

### Audio Files (Optional)
Place the following `.wav` files in the same directory as the executable for enhanced audio experience:
- `login.wav`
- `coin.wav`
- `power_pellet.wav`
- `eat_enemy.wav`
- `death.wav`

*Note: The game will use fallback beep sounds if audio files are not present.*

---

## How to Play

### Controls
- **W** - Move Up
- **A** - Move Left
- **S** - Move Down
- **D** - Move Right
- **Q** - Quit Game

### Objective
- Collect all dots (`.`) to win
- Avoid enemies (`M`) or they will catch you
- Collect power pellets (`O`) to make enemies vulnerable (`V`)
- Eat vulnerable enemies for bonus points
- Achieve the highest score possible

### Scoring System
- **Dot:** 10 points
- **Power Pellet:** 50 points
- **Eating Enemy:** 200 points

---

## Game Architecture

### Core Components

1. **User Management System**
   - Registration and login functionality
   - Secure password input (masked)
   - Persistent data storage in `users.dat`

2. **Game Engine**
   - Real-time rendering using Windows Console API
   - Collision detection
   - Enemy AI with chase and flee behaviors
   - Power pellet timer system

3. **Map System**
   - Three pre-designed maze layouts
   - Dynamic map loading based on difficulty
   - Procedural dot and power pellet placement

4. **Audio System**
   - File-based sound effects
   - Fallback beep generation
   - Asynchronous sound playback

---

## Security Features

### Input Validation
- **Secure String Parsing:** Replaced `atoi()` with `strtol()` for error checking
- **Password Validation:** Only printable ASCII characters (32-126) accepted
- **Buffer Overflow Protection:** Using `snprintf()` and `strncpy()` with bounds checking

### Error Handling
- File operation validation
- Write completion verification
- Boundary checking for array access

---

## File Structure

```
Project-Work-I/
│
├── game.c              # Main game source code
├── users.dat           # User data storage (generated at runtime)
├── coin.wav            # Coin collection sound (optional)
├── death.wav           # Death sound (optional)
├── eat_enemy.wav       # Enemy eaten sound (optional)
├── power_pellet.wav    # Power pellet sound (optional)
├── login.wav           # Login sound (optional)
└── README.md           # Project documentation
```

---

## Development Highlights

### Code Quality
- **Modular Design:** Separated functions for different game components
- **Memory Safety:** Proper bounds checking and buffer management
- **Error Handling:** Comprehensive validation and error reporting
- **Code Documentation:** Clear comments and function descriptions

### Best Practices
- Secure input handling
- Efficient memory usage
- Clean code structure
- Consistent naming conventions

---

## Future Enhancements

- [ ] Add more difficulty levels
- [ ] Implement multiplayer mode
- [ ] Add level progression system
- [ ] Create custom map editor
- [ ] Add more enemy types with different behaviors
- [ ] Implement leaderboard system
- [ ] Add graphical user interface (GUI)
- [ ] Cross-platform support (Linux, macOS)

---

## Known Limitations

- Windows-only compatibility (uses Windows-specific APIs)
- Console-based graphics (no GUI)
- Single-player only
- Fixed maze sizes

---

## Acknowledgments

We would like to express our gratitude to:
- **Md. Sadmim Hasan Sifat** for guidance and supervision
- **Department of CSE, SUST** for providing the opportunity
- Classic Pac-Man game for inspiration

---

## License

This project is developed for educational purposes as part of Project Work I course at SUST.

---

## Contact Information

For queries or feedback, please contact:

**Ashiqul Islam Siyam** (Team Leader)  
Registration: 2023331021  
Email: [Contact through university portal]

---

**© 2024 Team The Ghost - SUST CSE**
