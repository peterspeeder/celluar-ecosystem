#include "raylib.h"
#include <stdlib.h>

#define EMPTY 0
#define SAND 1
#define GROUND 2
#define WATER 3

#define WIDTH 50
#define HEIGHT 50
#define CELL_SIZE 10

void think(int grid[WIDTH][HEIGHT]) {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        int dir = (rand() % 2) ? 1 : -1;  // Random direction +1 or -1
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y] == SAND) {
                if (y + 1 < HEIGHT && grid[x][y + 1] == EMPTY) {
                    grid[x][y] = EMPTY;
                    grid[x][y + 1] = SAND;
                } else if (x + dir >= 0 && x + dir < WIDTH && y + 1 < HEIGHT && grid[x + dir][y + 1] == EMPTY) {
                    grid[x][y] = EMPTY;
                    grid[x + dir][y + 1] = SAND;
                } else if (y + 1 < HEIGHT && grid[x][y + 1] == WATER) {
                    grid[x][y] = WATER;
                    grid[x][y + 1] = SAND;
                }
            } else if (grid[x][y] == WATER) {
                if (y + 1 < HEIGHT && grid[x][y + 1] == EMPTY) {
                    grid[x][y] = EMPTY;
                    grid[x][y + 1] = WATER;
                } else if (x + dir >= 0 && x + dir < WIDTH && y + 1 < HEIGHT && grid[x + dir][y + 1] == EMPTY) {
                    grid[x][y] = EMPTY;
                    grid[x + dir][y + 1] = WATER;
                }
            }
        }
    }
}

int main(void) {
    int grid[WIDTH][HEIGHT];
    int currentType = SAND;
    int screenWidth = WIDTH * CELL_SIZE;
    int screenHeight = HEIGHT * CELL_SIZE;

    // Initialize the grid to EMPTY
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            grid[x][y] = EMPTY;
        }
    }

    // Ground
    for (int x = 0; x < WIDTH; x++) {
        grid[x][HEIGHT - 1] = GROUND;
    }

    InitWindow(screenWidth, screenHeight, "Falling Sand Algorithm");
    SetTargetFPS(60);

    Font pixelFont = LoadFont("path/to/pixel-font.ttf");

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 pos = GetMousePosition();
            int gridX = pos.x / CELL_SIZE;
            int gridY = pos.y / CELL_SIZE;
            if (gridX >= 0 && gridX < WIDTH && gridY >= 0 && gridY < HEIGHT) {
                grid[gridX][gridY] = currentType;
            }
        }

        if (IsKeyPressed(KEY_S)) {
            currentType = SAND;
        } else if (IsKeyPressed(KEY_W)) {
            currentType = WATER;
        }

        think(grid);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextEx(pixelFont, "Sand", (Vector2){700, 20}, 20, 0, YELLOW);
        DrawTextEx(pixelFont, "Water", (Vector2){700, 50}, 20, 0, BLUE);

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                Color color;
                switch (grid[x][y]) {
                    case EMPTY: color = LIGHTGRAY; break;
                    case SAND: color = YELLOW; break;
                    case GROUND: color = DARKGRAY; break;
                    case WATER: color = BLUE; break;
                }
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

