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
//----------------------------------------------------------------------------------
// RENDERING STUFF
//----------------------------------------------------------------------------------
static const int cellOutlineWidth = 8;
#define NEWORANGE       (Color){ 255, 177, 41, 255 }
#define DARKORANGE      (Color){ 254, 161, 3, 255 }
#define DARKERGRAY      (Color){ 50, 50, 50, 255 }
#define NEWDARKGRAY     (Color){ 62, 62, 62, 255 }
#define NEWBLUE         (Color){ 6, 164, 235, 255 }
#define NEWDARKBLUE     (Color){ 14, 157, 217, 255 }
#define LIGHTLIGHTGRAY  (Color){222, 222, 222, 255}
#define BACKGROUND      (Color){237, 239, 237, 255}
#define SHADOWCOLOR     (Color){223, 223, 223, 170}

static const int playareaOffsetX = 30;
static const int playareaOffsetY = 60;
static const int screenWidth = 1600;
static const int screenHeight = 900;

static const int secondRowOffset = 35;
static const int distanceBetweenCellsX = 70; // between centers of cells
static const int distanceBetweenCellsY = 60;
static const int cellRadius = 36;
static const int cellTextOffsetX = -4;
static const int cellTextOffsetY = -10;
static const int shadowOffsetX = -8;
static const int shadowOffsetY = 5;

//----------------------------------------------------------------------------------
// LOGIC STUFF
//----------------------------------------------------------------------------------

#define NUMBEROFROWS        14
#define NUMBEROFCOLUMNS     22

static const int hitboxVal = 2;

int clickedCellIndex = -1;
// int lvl1Seed[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,10,144};
// int lvl1Seed[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,6,7,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,34};
int revealedCells[308] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bombCount, remainingCellCount;
int victory = 0;
int solution[308] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int chk = 12;
int currentLevel = 0;


typedef struct 
{
    int seed[NUMBEROFCOLUMNS * NUMBEROFROWS];
    int revealed[13];
    int flagged[13]
}Levels;

Levels level[2] = {
    {
//         {
// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  0,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0,7,6,7,0,0,0,0,0,0,0,0,0,0,0,0,
//  0,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0,-1,-1,-1,-1,1,7,0,0,0,0,0,0,0,0,0,
//  0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
// },
        {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,7,7,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,
        0,0,0,0,0,0,7,6,7,0,0,0,0,0,-1,-1,-1,0,0,0,0,0,
        0,0,0,0,0,0,7,7,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,-1,-1,1,7,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        },
        {51,59,207,207,207,207,207,207,207,207,207,207,207},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
    },
    {
        {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,7,2,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,7,2,7,2,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,7,2,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,7,2,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,3,5,7,1,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,7,2,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        },
        {217,215,172,170,258,260,93,48,69,70,113,114,136},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
    }
};


int getClickedCellIndex(void)
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

void revealCell(int index) {
    if (level[currentLevel].seed[index] != 0)
    {
        if (revealedCells[index] == 0)
        {
            revealedCells[index] = 1;
        }
    }
}

void flagCell(int index) {
    if (level[currentLevel].seed[index] != 0)
    {
    if (revealedCells[index] == 0)
        {
            revealedCells[index] = 2;
        }
    else if (revealedCells[index] == 2)
        {
            revealedCells[index] = 0;
        }
    } 
}

void resetProgress(void) {
    for (int i = 0; i < 308; i++)
    {
        revealedCells[i] = 0;
        solution[i] = 0;
    }
    clickedCellIndex = -1;
    chk = 0;
    victory = 0;
}

void loadLevel(void) {
    // remainingCellCount = lvl1Seed[145];
    // bombCount = lvl1Seed[144];
    resetProgress();

    for (int i = 0; i < 13; i++)
    {
        revealedCells[ level[currentLevel].revealed[i] ] = 1;
    }
    for (int i = 0; i < 13; i++)
    {
        if (level[currentLevel].flagged[i] >= 0)
        {
            revealedCells[ level[currentLevel].flagged[i] ] = 2;
        } 
    }

    
    for (int i = 0; i < (NUMBEROFCOLUMNS * NUMBEROFROWS); i++)
    {
        if (level[currentLevel].seed[i] < 7)
        {
            if (level[currentLevel].seed[i] > 0)
            {
                solution[i] = 1;
            } else if (level[currentLevel].seed[i] == -1)
            {
                solution[i] = 1;
            }
            
            
        } else
        {
            solution[i] = 2;
        }
    }       
}

void checkWin(void) {
    chk = 0;
    for (int i = 0; i < (NUMBEROFCOLUMNS * NUMBEROFROWS); i++)
    {
        if (revealedCells[i] != solution[i])
        {
            chk = chk + 1;
        }
    }
    if (chk == 0)
    {
        DrawText("WIN", GetScreenWidth()/2 + 300, 40/2 - 10, 20, GREEN);
        victory = 1;
    }
}

void drawCell(Vector2 center, int sides, float radius, Color color, Color outlineColor) {
    DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR); // Shadow
    DrawPoly(center, sides, radius, 0, color);
    for (int width = 2; width < cellOutlineWidth; width++)
    {
        DrawPolyLines(center, sides, radius - width, 0, outlineColor);
    }
    DrawPolyLines(center, sides, radius, 0, LIGHTLIGHTGRAY);
    DrawPolyLines(center, sides, radius + 1, 0, LIGHTLIGHTGRAY);
}

void loadMenu(void) {
    BeginDrawing();
    ClearBackground(BACKGROUND);
    for (int k = 0; k < 12; k++)
    {
        for (int l = 0; l < 2; l++)
        {
            if (k == 11 && l == 1)
            {
                // nothing
            } else
            {
                drawCell((Vector2){k*distanceBetweenCellsX + 400 + l*secondRowOffset, GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY}, 6, cellRadius, NEWORANGE, DARKORANGE);
                DrawText(TextFormat("%i", (int)(k + (l*12))), cellTextOffsetX + k*distanceBetweenCellsX + 400 + l*secondRowOffset,cellTextOffsetY + GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY, 20, WHITE);
            } 
        }
    }
    DrawText("LEVELS", GetScreenWidth()/2 - MeasureText("LEVELS", 60) / 2, GetScreenHeight()/2 + 170, 60, SHADOWCOLOR);
    
    EndDrawing();
}

int main(void) 
{
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(screenWidth, screenHeight, "PutHexcells");

    SetTargetFPS(30);               // Set our game to run at 60 frames-per-second
    loadLevel();
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        if (currentLevel >= 0)
        {
            updateGame();
            drawFrame();
        } else
        {
            loadMenu();
        }
        
        
        
        //----------------------------------------------------------------------------------
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
    DrawText(TextFormat("%i" ,(int)victory), GetScreenWidth()/2 + 300, 40/2 - 10, 20, RED);
    // DrawText(TextFormat("%i" ,solution), GetScreenWidth()/2 - 300, 40/2 - 10, 20, GREEN);
    // DrawText(TextFormat("%i" ,revealedCells), GetScreenWidth()/2 - 300, 40/2 + 10, 20, GREEN);
    
    // Drawing board
    for (int row = 0; row < NUMBEROFROWS; row++)
        {
        for (int col = 0; col < NUMBEROFCOLUMNS; col++)
            {
                int toc = level[currentLevel].seed[row*NUMBEROFCOLUMNS + col];
                int revealed = revealedCells[row*NUMBEROFCOLUMNS + col];

                if (toc != 0)
                {
                    if (revealed == 1)
                    {
                        if (toc < 7)
                        {
                            // DrawPoly((Vector2){(row%2 * 35) + playareaOffsetX + col*75 + 75,playareaOffsetY + 40 + (row*65)}, 6, 40, 0, NEWDARKGRAY);
                            // // Border
                            // for (int width = 0; width < cellOutlineWidth; width++)
                            // {
                            //     DrawPolyLines((Vector2){(row%2 * 35) + playareaOffsetX + col*75 + 75,playareaOffsetY + 40 + (row*65)}, 6, 40 - width, 0, DARKERGRAY);
                            // }
                            drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, NEWDARKGRAY, DARKERGRAY);

                            if (toc == -1)
                            {
                                DrawText("0", (row%2 * secondRowOffset) + playareaOffsetX + cellTextOffsetX + col*distanceBetweenCellsX, playareaOffsetY + cellTextOffsetY + (row*distanceBetweenCellsY), 20, WHITE);
                            } else
                            {
                                DrawText(TextFormat("%i", (int)toc), (row%2 * secondRowOffset) + playareaOffsetX + cellTextOffsetX + col*distanceBetweenCellsX, playareaOffsetY + cellTextOffsetY + (row*distanceBetweenCellsY), 20, WHITE);
                            }
                            
                        } else
                        {
                            drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX,playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, RED, RED);
                        }
                    }
                    else if (revealed == 2)
                    {
                        drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, NEWBLUE, NEWDARKBLUE);
                    }
                    else
                    {
                        drawCell((Vector2){(row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY)}, 6, cellRadius, NEWORANGE, DARKORANGE);
                    }
                }
                
                // DrawCircle( (row%2 * secondRowOffset) + playareaOffsetX + col*distanceBetweenCellsX, playareaOffsetY + (row*distanceBetweenCellsY), cellRadius - hitboxVal, (Color){10, 200, 10, 30});
                // if (toc == 1)
                //     {
                //     DrawPoly((Vector2){(row%2 * 35) + playareaOffsetX + col*75 + 75,playareaOffsetY + 40 + (row*65)}, 6, 40, 0, PURPLE);
                //     }
                // else if (toc == 2)
                //     {
                //     DrawPoly((Vector2){(row%2 * 35) + playareaOffsetX + col*75 + 75,playareaOffsetY + 40 + (row*65)}, 6, 40, 0, RED);
                //     }   
                // }
            }
        }
            // for (int i = 1; i < 17; i++)
            // {
            //     DrawPoly((Vector2){ playareaOffsetX + 35+i*75, playareaOffsetY + 105}, 6, 40, 0, RED);
            // }
    int chk = 0;
        for (int i = 0; i < (NUMBEROFCOLUMNS * NUMBEROFROWS); i++)
        {
            if (revealedCells[i] != solution[i])
            {
                chk = chk + 1;
            }
        }
        if (chk == 0)
        {
            DrawText("WIN", GetScreenWidth()/2 + 300, 40/2 - 10, 20, PINK);
            victory = 1;
            currentLevel = currentLevel + 1;
            loadLevel();
        }
    
    if (currentLevel == 0)
    {
        // Show tutorial text
        DrawTexture(LoadTexture("mouse.png"), GetScreenWidth()/2 - 64/2, GetScreenHeight()-130, WHITE);
        DrawText("Destroy a hex", GetScreenWidth()/2 - 200, GetScreenHeight() - 100, 20, DARKORANGE);
        DrawText("Mark it as a bomb", GetScreenWidth()/2 + 52, GetScreenHeight() - 100, 20, NEWDARKBLUE);
    }
    


    DrawText(TextFormat("%i", (int)clickedCellIndex), GetScreenWidth() - 40, 40/2 - 10, 20, BLACK);
    DrawText(TextFormat("%i", (int)chk), GetScreenWidth() - 200, 40/2 - 10, 20, PINK);
    // DrawText(TextFormat("%i", (int)remainingCellCount), GetScreenWidth()/2 - 300, 40/2 - 10, 20, BLACK);
    // DrawText(TextFormat("%i", (int)bombCount), GetScreenWidth()/2 + 300, 40/2 - 10, 20, RED);
    DrawFPS(1, 1);
    EndDrawing();
}

void updateGame(void) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        getClickedCellIndex();
        revealCell(clickedCellIndex);
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        getClickedCellIndex();
        flagCell(clickedCellIndex);
    }
}

