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
*   raylib is licensed under an unmodified zlib/libpng license (View rpaylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "raylib.h"
#include "levels.h"

//----------------------------------------------------------------------------------
// LOGIC STUFF
//----------------------------------------------------------------------------------

static const int hitboxVal = 2;

int clickedCellIndex = -1;
int lastClickedCellIndex = -1;
int revealedCells[504];
int victory = 0;
int solution[504];
int chk;
int currentLevel = -1;

//----------------------------------------------------------------------------------
// RENDERING STUFF
//----------------------------------------------------------------------------------

int currentLayout = 1;
Font defaultFont;
Texture2D mouseTexture, exitTexture, restartTexture;




void getClickedCellIndex(void)
{
    Vector2 mouse = GetMousePosition();
    for (int row = 0; row < NUMBEROFROWS[currentLayout]; row++)
        {
            for (int col = 0; col < NUMBEROFCOLUMNS[currentLayout]; col++)
            {
                {
                    bool collision = CheckCollisionPointCircle(mouse, (Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, cellRadius[currentLayout] - hitboxVal);
                    if (collision)
                    {
                        clickedCellIndex = (row*NUMBEROFCOLUMNS[currentLayout] + col);
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
    for (int i = 0; i < 504; i++)
    {
        revealedCells[i] = 0;
        solution[i] = 0;
    }
    clickedCellIndex = -1;
    chk = 0;
    victory = 0;
}

void loadLevel(void) {
    resetProgress();
    currentLayout = level[currentLevel].layout;


    for (int i = 0; i < 20; i++)
    {
        if (level[currentLevel].revealed[i] >= 0)
        {
            revealedCells[ level[currentLevel].revealed[i] ] = 1;
        }
    }
    for (int i = 0; i < 13; i++)
    {
        if (level[currentLevel].flagged[i] >= 0)
        {
            revealedCells[ level[currentLevel].flagged[i] ] = 2;
        } 
    }

    
    for (int i = 0; i < (NUMBEROFCOLUMNS[currentLayout] * NUMBEROFROWS[currentLayout]); i++)
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
            
            
        } else if (level[currentLevel].seed[i] == 7)
        {
            solution[i] = 2;
        } else if (level[currentLevel].seed[i] < 70) {
            solution[i] = 1;
        }
    }       
}

void checkWin(void) {
    chk = 0;
    for (int i = 0; i < (NUMBEROFCOLUMNS[currentLayout] * NUMBEROFROWS[currentLayout]); i++)
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


void drawCell(Vector2 center, int sides, float radius, int rawData) {

    if (rawData == -1) {
        // orange cell
        DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR);
        DrawPoly(center, sides, radius, 0, NEWORANGE);
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++)
        {
            DrawPolyLines(center, sides, radius - width, 0, DARKORANGE);
        }
        DrawPolyLines(center, sides, radius, 0, WHITETEXT);
        DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT);
    } else if (rawData == -2) {
        // marked cell (blue)
        DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR);
        DrawPoly(center, sides, radius, 0, NEWBLUE);
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++) {
            DrawPolyLines(center, sides, radius - width, 0, NEWDARKBLUE);
        }
        DrawPolyLines(center, sides, radius, 0, WHITETEXT);
        DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT);
    } else {
        // every other cell
        int rotation = rawData/100;
        int type = rawData/10;
        int value = rawData%10;

        if (type < 3) {
            if (value < 7)
            {
                DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR); // Shadow
                DrawPoly(center, sides, radius, 0, NEWDARKGRAY);
                for (int width = 2; width < cellOutlineWidth[currentLayout]; width++)
                {
                    DrawPolyLines(center, sides, radius - width, 0, DARKERGRAY);
                }
                DrawPolyLines(center, sides, radius, 0, WHITETEXT);
                DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT); 
            } else
            {
                DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR); // Shadow
                DrawPoly(center, sides, radius, 0, RED);
                for (int width = 2; width < cellOutlineWidth[currentLayout]; width++)
                {
                    DrawPolyLines(center, sides, radius - width, 0, RED);
                }
                DrawPolyLines(center, sides, radius, 0, WHITETEXT);
                DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT); 
            }
        }

        switch (type) {
        case 0:
            if (value < 7) {
                DrawTextEx(defaultFont, TextFormat("%i", (int)value), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("%i", (int)value), cellTextSize[currentLayout], fontSpacing).x / 2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout],1, WHITETEXT);
            } break;
        case 1:
            DrawTextEx(defaultFont, TextFormat("{%i}", (int)value), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("{%i}", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, WHITETEXT);
            break;
        case 2:
            DrawTextEx(defaultFont, TextFormat("-%i-", (int)value), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("-%i-", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, WHITETEXT);
            break;
        case 4:
            DrawTextEx(defaultFont, TextFormat("%i ", (int)value), (Vector2){(cellRadius[currentLayout] * 0.6F) + center.x - MeasureTextEx(defaultFont, TextFormat("%i ", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, DARKERGRAY);
            break;
        case 5:
            DrawTextEx(defaultFont, TextFormat("{%i} ", (int)value), (Vector2){(cellRadius[currentLayout] * 0.6F) + center.x - MeasureTextEx(defaultFont, TextFormat("{%i}---", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, DARKERGRAY);
            break;
        case 6:
            DrawTextEx(defaultFont, TextFormat("-%i-", (int)value), (Vector2){(cellRadius[currentLayout] * 0.6F) + center.x - MeasureTextEx(defaultFont, TextFormat("-%i-- -", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, DARKERGRAY);
            break;

        default:
            break;
        }
    }
}


void drawMenuCell(Vector2 center, int sides, float radius, Color color, Color outlineColor, int text) {
        DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR); // Shadow
        DrawPoly(center, sides, radius, 0, color);
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++)
        {
            DrawPolyLines(center, sides, radius - width, 0, outlineColor);
        }

        DrawPolyLines(center, sides, radius, 0, LIGHTLIGHTGRAY);
        DrawPolyLines(center, sides, radius + 1, 0, LIGHTLIGHTGRAY);

        
        DrawText(TextFormat("%i", (int)text), center.x - MeasureText(TextFormat("%i", (int)text), cellTextSize[currentLayout])/2, center.y + cellTextOffsetY[currentLayout], cellTextSize[currentLayout], WHITETEXT); 
}

void drawRestartButton() {
    Vector2 mouse = GetMousePosition();
    bool collision = CheckCollisionPointCircle(mouse, (Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, cellRadius[1] - hitboxVal);
    if (collision)
    {
        drawCell((Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1]) - (1*35)}, 6, cellRadius[1], -1);
        DrawTextureEx(restartTexture, (Vector2){GetScreenWidth()-120, GetScreenHeight()-20 - (1*35)}, 0, 0.3125, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
        loadLevel();
        }
    } else
    {
        drawCell((Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -1);
        DrawTextureEx(restartTexture, (Vector2){GetScreenWidth()-120, GetScreenHeight()-20}, 0, 0.3125, WHITE);
    } 
}

void drawExitButton() {
    Vector2 mouse = GetMousePosition();
    bool collision = CheckCollisionPointCircle(mouse, (Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, cellRadius[currentLayout] - hitboxVal);
    if (collision)
    {
        drawCell((Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1]) - (1*35)}, 6, cellRadius[1], -1);
        DrawTextureEx(exitTexture, (Vector2){GetScreenWidth()-120 + distanceBetweenCellsX[1], GetScreenHeight()-20 - (1*35)}, 0, 0.3125, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
        currentLevel = -1;
        currentLayout = 1;
        resetProgress();
        }
    } else
    {
        drawCell((Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -1);
        DrawTextureEx(exitTexture, (Vector2){GetScreenWidth()-120 + distanceBetweenCellsX[1], GetScreenHeight()-20}, 0, 0.3125, WHITE);
    } 
}

void loadMenu(void) {
    BeginDrawing();
    ClearBackground(BACKGROUND);
    
    DrawText("Hexcells", GetScreenWidth()/2 - MeasureText("Hexcells", 100) / 2 - 6, GetScreenHeight() / 2 - 294, 100, SHADOWCOLOR);
    DrawText("Hexcells", GetScreenWidth()/2 - MeasureText("Hexcells", 100) / 2, GetScreenHeight() / 2 - 300, 100, GRAY);
    DrawText("clone by Mateusz Wisniewski", GetScreenWidth()/2 - MeasureText("clone by Mateusz Wisniewski", 40) / 2, GetScreenHeight() / 2 - 200, 40, LIGHTGRAY);
    DrawText("Music by Eric Matyas (www.soundimage.org)", 1, GetScreenHeight()-20, 20, LIGHTGRAY);

    for (int k = 0; k < 7; k++)
    {
        for (int l = 0; l < 2; l++)
        {
            drawMenuCell((Vector2){k*distanceBetweenCellsX[1] + 575 + l*secondRowOffset[1], GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY[1]}, 6, cellRadius[1], NEWORANGE, DARKORANGE, k + (l*7));
            // DrawCircle(k*distanceBetweenCellsX + 400 + l*secondRowOffset, GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY, cellRadius - hitboxVal, GREEN);
        }
    }
    DrawText("LEVELS", GetScreenWidth()/2 - MeasureText("LEVELS", 60) / 2, GetScreenHeight()/2 + 170, 60, SHADOWCOLOR);
    
    EndDrawing();
}

void updateMenu(void) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        for (int row = 0; row < 2; row++)
            {
            for (int col = 0; col < 7; col++)
            {
                bool collision = CheckCollisionPointCircle(mouse, (Vector2){col*distanceBetweenCellsX[1] + 575 + row*secondRowOffset[1], GetScreenHeight()/2 + 300 + row*distanceBetweenCellsY[1]}, cellRadius[1] - hitboxVal);
                if (collision)
                {
                    currentLevel = col + (row*7);
                    loadLevel();
                }
            }
        }
    }
}


int main(void) 
{
    // Initialization
    //--------------------------------------------------------------------------------------

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "PutHexcells");
    SetTargetFPS(30);
    InitAudioDevice();
    Music music = LoadMusicStream("resources/Puzzle-Game-2_Looping.mp3");
    music.looping = true;
    SetMusicVolume(music, 0.05);
    PlayMusicStream(music);

    restartTexture = LoadTexture("Resources/restart.png");
    exitTexture = LoadTexture("Resources/exit.png");
    mouseTexture = LoadTexture("Resources/mouse.png");



    defaultFont = LoadFontEx("Resources/Roboto-Medium.ttf", 32, 0, 0);
    GenTextureMipmaps(&defaultFont.texture);
    SetTextureFilter(defaultFont.texture, FILTER_TRILINEAR);

    // loadLevel();
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        UpdateMusicStream(music); 
        if (currentLevel >= 0)
        {
            updateGame();
            drawFrame();
            drawRestartButton();
            drawExitButton();

        } else
        {
            updateMenu();
            loadMenu();
        }

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(exitTexture);
    UnloadTexture(restartTexture);
    UnloadTexture(mouseTexture);
    UnloadMusicStream(music);
    UnloadFont(defaultFont); 
    CloseAudioDevice(); 
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void drawFrame() {
    BeginDrawing();
    
    ClearBackground(BACKGROUND);
    DrawText(TextFormat("LEVEL %i", (int)currentLevel), 0, GetScreenHeight()-80, 100, (Color){240,240,240,200});
    // DrawText(TextFormat("%i" ,(int)victory), GetScreenWidth()/2 + 300, 40/2 - 10, 20, RED);
    
    // Drawing board
    for (int row = 0; row < NUMBEROFROWS[currentLayout]; row++)
        {
        for (int col = 0; col < NUMBEROFCOLUMNS[currentLayout]; col++)
            {
                int toc = level[currentLevel].seed[row*NUMBEROFCOLUMNS[currentLayout] + col];
                int revealed = revealedCells[row*NUMBEROFCOLUMNS[currentLayout] + col];

                if (toc != 0)
                {
                    switch (revealed)
                    {
                    case 0:
                        drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], -1);
                        break;
                    case 1:
                        switch (toc)
                        {
                        case -1:
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], 0);
                            break;
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], toc);
                            break;
                        case 7:
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout],playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], 7);   
                            break;
                        default:
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], toc);
                            break;
                        }
                        break;
                    case 2:
                        drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], -2);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    

    if (currentLevel == 0)
    {
        // Show tutorial text
        DrawTexture(mouseTexture, GetScreenWidth()/2 - 64/2, GetScreenHeight()-130, WHITE);
        DrawTextEx(defaultFont, "Destroy a hex", (Vector2){GetScreenWidth()/2 - 160, GetScreenHeight() - 100}, 20, fontSpacing, DARKORANGE);
        DrawTextEx(defaultFont, "Mark it as a bomb", (Vector2){GetScreenWidth()/2 + 52, GetScreenHeight() - 100}, 20, fontSpacing, NEWDARKBLUE);
    } else if (currentLevel == 6)
    {
        // show tutorial #2 text
        DrawTextEx(defaultFont, "Numbers outside the grid show the number \nof bombs in that row", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
    } else if (currentLevel == 7)
    {
        // show tutorial #2 text
        DrawTextEx(defaultFont, "Numbers like this {3} give additional information that the 3 bombs are consecutive", (Vector2){GetScreenWidth()/2 - MeasureTextEx(defaultFont, "Numbers like this {3} give additional information that the 3 bombs are consecutive", 32, fontSpacing).x/2 , GetScreenHeight() - 120}, 32, fontSpacing, DARKGRAY);
    } else if (currentLevel == 9)
    {
        DrawTextEx(defaultFont, "Numbers like this {3} give additional\ninformation that the 3 bombs\nare consecutive", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
    } else if (currentLevel == 11)
    {
        DrawTextEx(defaultFont, "Numbers like this -3- give additional\ninformation that the 3 bombs\nare NOT connected", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
    }


    
    
    
    
    
    // DrawText(TextFormat("%i", (int)lastClickedCellIndex), GetScreenWidth() - 40, 40/2 - 10, 20, BLACK);
    // DrawText(TextFormat("%i", (int)chk), GetScreenWidth() - 200, 40/2 - 10, 20, PINK);
    // DrawText(TextFormat("%i", (int)remainingCellCount), GetScreenWidth()/2 - 300, 40/2 - 10, 20, BLACK);
    // DrawText(TextFormat("%i", (int)bombCount), GetScreenWidth()/2 + 300, 40/2 - 10, 20, RED);
    // DrawFPS(1, 1);
    EndDrawing();
}

void updateGame(void) {
    chk = 0;
    for (int i = 0; i < (NUMBEROFCOLUMNS[currentLayout] * NUMBEROFROWS[currentLayout]); i++)
    {
        if (revealedCells[i] != solution[i])
        {
            chk = chk + 1;
        }
    }
    if (chk == 0)
    {
        victory = 1;
        currentLevel = currentLevel + 1;
        loadLevel();
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        getClickedCellIndex();
        revealCell(clickedCellIndex);
        lastClickedCellIndex = clickedCellIndex;
        clickedCellIndex = -1;
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        getClickedCellIndex();
        flagCell(clickedCellIndex);
        lastClickedCellIndex = clickedCellIndex;
        clickedCellIndex = -1;
    }  
}
