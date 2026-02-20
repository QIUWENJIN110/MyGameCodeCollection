# MyGameCodeCollection

This repository contains two classic game projects developed in C++ with the Qt framework, showcasing my technical skills in game logic, UI rendering, and event-driven programming. These projects are part of my application portfolio for the MSc Computer Game Technology program at Abertay University.

---

## Game2048

- **Language & Framework**: C++ / Qt 5.14.2
- **Core Features**:
  - Implemented the core 2048 tile merging logic using a 4×4 grid data structure
  - Supported keyboard arrow key input for tile movement
  - Added automatic tile generation and win/lose condition detection
  - Real-time score tracking and UI updates
- **Key Technical Points**:
  - Used Qt's `QGridLayout` and `QPushButton` for the game board
  - Applied signal-slot mechanism to handle user input
  - Implemented tile animation effects using Qt's animation framework

---

## SnakeGame

- **Language & Framework**: C++ / Qt 5.14.2
- **Core Features**:
  - Classic snake movement and growth mechanics
  - Random food generation and collision detection
  - Game over condition when the snake hits the wall or its own body
  - Real-time score display and game restart functionality
- **Key Technical Points**:
  - Used Qt's `QPainter` for 2D rendering of the snake and food
  - Implemented a timer-based game loop for smooth movement
  - Handled keyboard events to control snake direction
  - Applied object-oriented design with separate classes for the snake, food, and game logic

---

## How to Build & Run

1.  Ensure Qt 5.14.2 (or compatible version) is installed on your system
2.  Open the project `.pro` file in Qt Creator
3.  Configure the project for your target platform (Windows/macOS/Linux)
4.  Build and run the application

---

## License

This project is open-sourced under the MIT License. See the [LICENSE](LICENSE) file for details.
