// snakegame_global.h
#ifndef SNAKEGAME_GLOBAL_H
#define SNAKEGAME_GLOBAL_H

// Global constants definition
const int CELL_SIZE = 30;           // Size of each grid cell in pixels
const int GRID_WIDTH = 21;          // Grid width in number of cells
const int GRID_HEIGHT = 21;         // Grid height in number of cells
const int INITIAL_SPEED = 200;      // Initial movement interval in milliseconds
const int MIN_SPEED = 80;           // Minimum movement interval (fastest speed)
const int SPEED_INCREMENT = 5;      // Speed increase per body growth

const int FOOD_COUNT = 3;
const int BOMB_TIMEOUT = 5000;

const int GRID_CENTER_X = GRID_WIDTH / 2;
const int GRID_CENTER_Y = GRID_HEIGHT / 2;

#endif // SNAKEGAME_GLOBAL_H
