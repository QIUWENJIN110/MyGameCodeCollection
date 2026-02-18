# SnakeGame - Qt-based Snake Game
## Project Overview
A classic snake game developed with Qt 5.14.2, focusing on **interactive experience and dynamic audio adaptation**. It integrates core game logic with multimedia control to realize linkage between gameplay and sound effects, serving as a practical case of Qt GUI + multimedia development.

## Core Features
1. **Classic Snake Gameplay**: Control snake movement via arrow keys; food is randomly generated, and the snake’s body lengthens with score accumulation when food is eaten. Game over is triggered when the snake collides with boundaries or itself, with support for resetting to restart.
2. **Dynamic Speed Linkage**: Each time the snake eats food, its movement speed accelerates automatically, and the playback rate of background music (BGM) increases synchronously (up to 2.0x speed), achieving immersive linkage between gameplay and sound effects.
3. **Visual Interaction**: Render snake head/body/food with SVG assets, adapting to different resolutions for a concise and smooth interface.

## Tech Stack
- Development Framework: Qt 5.14.2 (QGraphicsView/QGraphicsScene for graphic rendering)
- Core Technologies: QMediaPlayer for multimedia control (speed playback), QTimer for snake speed management, collision detection algorithm
- Runtime Environment: Windows 10/11 (64-bit), MinGW 7.3.0 compiler

## Key Highlights
1. **Dynamic Audio Adaptation**: Break through the limitations of QSound and use QMediaPlayer’s `setPlaybackRate()` interface to realize real-time linkage between BGM and snake speed, enhancing game immersion.
2. **Engineering Adaptation**: Resolve Qt media plugin dependency issues to ensure the program runs independently outside the development environment.
3. **Scalable Code**: Split game logic (movement/collision/audio control) into modules, facilitating the addition of difficulty levels, skin switching, and other features in the future.

## Running Instructions
1. Environment Requirements: Qt 5.14.2 (MinGW 7.3.0 64-bit), Windows 10/11;
2. Running Method: Open the project file `SnakeGame.pro` in Qt Creator, build the project, execute `windeployqt` to complete dependencies, and run the generated executable file directly.