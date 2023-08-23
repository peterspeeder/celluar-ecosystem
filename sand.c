#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define EMPTY 0
#define SAND 1
#define GROUND 2
#define WATER 3
#define FIRE 4


#define WIDTH 50
#define HEIGHT 50
#define CELL_SIZE 10



void think(int grid[WIDTH][HEIGHT]) {
    int newGrid[WIDTH][HEIGHT];
    
    // Copy the current grid state to newGrid
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            newGrid[x][y] = grid[x][y];
        }
    }

    for (int y = HEIGHT - 1; y >= 0; y--) {
        int dir = (rand() % 2) ? 1 : -1;  // Random direction +1 or -1
        for (int x = 0; x < WIDTH; x++) {
            // Logic for SAND
            if (grid[x][y] == SAND) {
                if (y + 1 < HEIGHT && grid[x][y + 1] == EMPTY) {
                    newGrid[x][y] = EMPTY;
                    newGrid[x][y + 1] = SAND;
                } else if (x + dir >= 0 && x + dir < WIDTH && y + 1 < HEIGHT && grid[x + dir][y + 1] == EMPTY) {
                    newGrid[x][y] = EMPTY;
                    newGrid[x + dir][y + 1] = SAND;
                } else if (y + 1 < HEIGHT && grid[x][y + 1] == WATER) {
                    newGrid[x][y] = WATER;
                    newGrid[x][y + 1] = SAND;
                }
            }
            // Logic for WATER
            else if (grid[x][y] == WATER) {
                if (y + 1 < HEIGHT && grid[x][y + 1] == EMPTY) {
                    newGrid[x][y] = EMPTY;
                    newGrid[x][y + 1] = WATER;
                } else if (x + dir >= 0 && x + dir < WIDTH && grid[x + dir][y] == EMPTY) {
                    newGrid[x][y] = EMPTY;
                    newGrid[x + dir][y] = WATER;
                } else if (y + 1 < HEIGHT && grid[x][y + 1] == SAND) {
                    if (y + 2 < HEIGHT && grid[x][y + 2] == EMPTY) {
                        newGrid[x][y] = EMPTY;
                        newGrid[x][y + 2] = WATER;
                    }
                } else if (y - 1 >= 0 && grid[x][y - 1] == SAND) {
                    int tempY = y - 1;
                    while (tempY >= 0 && grid[x][tempY] == SAND) {
                        tempY--;
                    }
                    if (tempY >= 0 && grid[x][tempY] == EMPTY) {
                        newGrid[x][y] = EMPTY;
                        newGrid[x][tempY] = WATER;
                    }
                }
            }
            
            // Logic for Fire
            else if (grid[x][y] == FIRE) {
                // Fire dissipates randomly
                if (rand() % 5 == 0) {
                    newGrid[x][y] = EMPTY;
                }

                // Fire moves upwards
                if (y - 1 >= 0 && grid[x][y - 1] == EMPTY) {
                    newGrid[x][y] = EMPTY;
                    newGrid[x][y - 1] = FIRE;
                }
            }
        }
    }

    // Finally, copy newGrid back into grid
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            grid[x][y] = newGrid[x][y];
        }
    }
}

int main(void) {
    int grid[WIDTH][HEIGHT];
    int currentType = SAND;
    int screenWidth = WIDTH * CELL_SIZE;
    int screenHeight = HEIGHT * CELL_SIZE;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            grid[x][y] = EMPTY;
        }
    }

    for (int x = 0; x < WIDTH; x++) {
        grid[x][HEIGHT - 1] = GROUND;
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Falling Sand Algorithm");

    SetTargetFPS(60);

    float spawnRate = 1.0;
    float spawnAccumulator = 5.0;

    int fireRadius = 3; // Radius for the fireball

    while (!WindowShouldClose()) {
        spawnAccumulator += spawnRate;

        // Mouse clicking for Fireball effect
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && spawnAccumulator >= 1.0) {
            Vector2 pos = GetMousePosition();
            int centerX = pos.x / CELL_SIZE;
            int centerY = pos.y / CELL_SIZE;
            
            if (currentType == FIRE) {
                for (int dx = -fireRadius; dx <= fireRadius; dx++) {
                    for (int dy = -fireRadius; dy <= fireRadius; dy++) {
                        int gridX = centerX + dx;
                        int gridY = centerY + dy;
                        if (gridX >= 0 && gridX < WIDTH && gridY >= 0 && gridY < HEIGHT) {
                            if (sqrt(dx * dx + dy * dy) <= fireRadius) {
                                grid[gridX][gridY] = FIRE;
                            }
                        }
                    }
                }
                spawnAccumulator -= 1.0;
            } else {
                if (centerX >= 0 && centerX < WIDTH && centerY >= 0 && centerY < HEIGHT) {
                    grid[centerX][centerY] = currentType;
                }
                spawnAccumulator -= 1.0;
            }
        }

        if (IsKeyPressed(KEY_S)) {
            currentType = SAND;
        } else if (IsKeyPressed(KEY_W)) {
            currentType = WATER;
        } else if (IsKeyPressed(KEY_F)) {
            currentType = FIRE;
        }

        think(grid);
        
        int newScreenWidth = GetScreenWidth();
        int newScreenHeight = GetScreenHeight();

        float xScale = (float)newScreenWidth / screenWidth;
        float yScale = (float)newScreenHeight / screenHeight;


        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                Color color;
                switch (grid[x][y]) {
                    case EMPTY: color = LIGHTGRAY; break;
                    case SAND: color = YELLOW; break;
                    case GROUND: color = DARKGRAY; break;
                    case WATER: color = BLUE; break;
                    case FIRE: color = RED; break;
                }
                DrawRectangle(
                        x * CELL_SIZE * xScale,
                        y * CELL_SIZE * yScale,
                        CELL_SIZE * xScale,
                        CELL_SIZE * yScale,
                        color
                );
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


