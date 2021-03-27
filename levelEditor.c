/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "raylib.h"
#include "const.h"

//----------------------------------------------------------------------------------
// LOGIC STUFF
//----------------------------------------------------------------------------------

static const int hitboxVal = 2;

int clickedCellIndex = -1;
int lastClickedCellIndex = -1;
int revealedCells[308];
int victory = 0;
int solution[308];
int chk;
int currentLevel = 0;

typedef struct 
{
    int seed[NUMBEROFCOLUMNS * NUMBEROFROWS];
    int revealed[13];
    int flagged[13]
} Levels;

Levels level[1] = {
    {
        {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        },
        {0,0,0,0,0,0,0,0,0,0,0,0,0},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
    }
};



void getClickedCellIndex(void)
{
    Vector2 mouse = GetMousePosition();
    for (int row = 0; row < NUMBEROFROWS; row++)
        {
            for (int col = 0; col < NUMBEROFCOLUMNS; col++)
            {
                {
                    bool collision = CheckCollisionPointCircle(mouse, (Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, cellRadius - hitboxVal);
                    if (collision)
                    {
                        clickedCellIndex = (row*NUMBEROFCOLUMNS + col);
                    }
                }
            }
        }
}

void changeCell(int dir) {
    if (dir == 1)
    {
        // go right
        level[currentLevel].seed[clickedCellIndex] += 1;
    } else
    {
        level[currentLevel].seed[clickedCellIndex] -= 1;
    } 
}


void drawCell(Vector2 center, int sides, float radius, Color color, Color outlineColor, int text) {
    DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR); // Shadow
    DrawPoly(center, sides, radius, 0, color);
    for (int width = 2; width < cellOutlineWidth; width++)
    {
        DrawPolyLines(center, sides, radius - width, 0, outlineColor);
    }

    DrawPolyLines(center, sides, radius, 0, LIGHTLIGHTGRAY);
    DrawPolyLines(center, sides, radius + 1, 0, LIGHTLIGHTGRAY);

    // glow
    if (color.r != 62 && color.g != 62 && color.b != 62)
    {
        DrawPolyLines(center, sides, radius + 2, 0, (Color){color.r, color.g, color.b, 140});
        DrawPolyLines(center, sides, radius + 3, 0, (Color){color.r, color.g, color.b, 100});
        DrawPolyLines(center, sides, radius + 4, 0, (Color){color.r, color.g, color.b, 70});
        DrawPolyLines(center, sides, radius + 5, 0, (Color){color.r, color.g, color.b, 50});
        DrawPolyLines(center, sides, radius + 6, 0, (Color){color.r, color.g, color.b, 30});
        DrawPolyLines(center, sides, radius + 7, 0, (Color){color.r, color.g, color.b, 10});
        DrawPolyLines(center, sides, radius + 8, 0, (Color){color.r, color.g, color.b, 10});
    }
    if (text != -1) { DrawText(TextFormat("%i", (int)text), center.x - MeasureText(TextFormat("%i", (int)text), 30)/2, center.y + cellTextOffsetY, 30, WHITETEXT); }
}

int main(void) 
{
    // Initialization
    //--------------------------------------------------------------------------------------

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "PutHexcells");
    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        if (currentLevel >= 0)
        {
            updateGame();
            drawFrame();
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void drawFrame(void) {
    BeginDrawing();
    
    ClearBackground(BACKGROUND);
    
    // Drawing board
    for (int row = 0; row < NUMBEROFROWS; row++)
        {
        for (int col = 0; col < NUMBEROFCOLUMNS; col++)
            {
                int toc = level[currentLevel].seed[row*NUMBEROFCOLUMNS + col];
                int revealed = revealedCells[row*NUMBEROFCOLUMNS + col];

                if (toc != 0)
                {
                    switch (revealed)
                    {
                    case 0:
                        drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, NEWORANGE, DARKORANGE, -1);
                        break;
                    case 1:
                        switch (toc)
                        {
                        case -1:
                            drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, NEWDARKGRAY, DARKERGRAY, 0);
                            break;
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                            drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, NEWDARKGRAY, DARKERGRAY, toc);
                            break;
                        case 7:
                            drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX,playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, RED, RED, -1);   
                            break;
                        default:
                            break;
                        }
                        break;
                    case 2:
                        drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, NEWBLUE, NEWDARKBLUE, -1);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    
    DrawText(TextFormat("%i", (int)clickedCellIndex), GetScreenWidth() - 40, 40/2 - 10, 20, BLACK);
    // DrawText(TextFormat("%i", (int)remainingCellCount), GetScreenWidth()/2 - 300, 40/2 - 10, 20, BLACK);
    // DrawText(TextFormat("%i", (int)bombCount), GetScreenWidth()/2 + 300, 40/2 - 10, 20, RED);
    DrawFPS(1, 1);
    EndDrawing();
}

void updateGame(void) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        getClickedCellIndex();
        changeCell(1);
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        getClickedCellIndex();
        changeCell(-1);
    }
    if (IsKeyPressed(KEY_R))
    {
        getClickedCellIndex();
        revealedCells[clickedCellIndex] += 1;
    }
    if (IsKeyPressed(KEY_E))
    {
        getClickedCellIndex();
        revealedCells[clickedCellIndex] -= 1;
    }
      
}
