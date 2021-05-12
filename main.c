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
#include <stdio.h>


// === Game Logic ===
static const int hitboxVal = 2; // used to control hitbox circle radius

int revealedCells[504]; // current level 'progress', stores which cells have been revealed or marked by player
int solution[504]; // holds level solution which is then compared to revealedCells
int chk; // 'Remaining Moves' counter, amount of cells revealedCells is diffrent from solution
int currentLevel = -1; // tracks current level

// save file data, when a game first starts it loads a default "000..." data (no levels beaten), then if a save exists it overwrites it with proper save data 
char gameProgress[LEVELCOUNT] = "000000000000000"; // 0 is unbeaten, 1 is beaten level


// === Rendering ===
int currentLayout = 1; // level layout

// assets which will be loaded in main() during initialization
Font defaultFont;
Texture2D mouseTexture, exitTexture, restartTexture;


int getClickedCellIndex(void) {
    // loops by every cell and checks if mouse is over any of them
    Vector2 mouse = GetMousePosition();
    for (int row = 0; row < NUMBEROFROWS[currentLayout]; row++)
        {
            for (int col = 0; col < NUMBEROFCOLUMNS[currentLayout]; col++)
            {
                bool collision = CheckCollisionPointCircle(mouse, (Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, cellRadius[currentLayout] - hitboxVal);
                if (collision) { return (row*NUMBEROFCOLUMNS[currentLayout] + col); }
            }
        }
    return -1;
}


void revealCell(int index) {
    // handle left-click on cell
    if (index != -1) {
        if (level[currentLevel].seed[index] != 0)
        {
            if (revealedCells[index] == 0)
            {
                revealedCells[index] = 1;
            }
        }
    }
    
}


void flagCell(int index) {
    // handle right-click on cell
    if (index != -1)
    {
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
}


void resetProgress(void) {
    // reset all variables used for game logic
    for (int i = 0; i < 504; i++)
    {
        revealedCells[i] = 0;
        solution[i] = 0;
    }
    chk = 0;
}


void loadLevel(void) {
    // loads level from levels.h
    resetProgress(); // resets level

    currentLayout = level[currentLevel].layout; // load layout

    // reveal cells at the start of the level
    for (int i = 0; i < 20; i++)
    {
        if (level[currentLevel].revealed[i] >= 0)
        {
            revealedCells[ level[currentLevel].revealed[i] ] = 1; 
        }
    }
    // flag cells at the start of the level
    for (int i = 0; i < 13; i++)
    {
        if (level[currentLevel].flagged[i] >= 0)
        {
            revealedCells[ level[currentLevel].flagged[i] ] = 2; 
        } 
    }

    // build level solution to later compare to
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


void drawCell(Vector2 center, int sides, float radius, int rawData) {
    // CELL BODY
    if (rawData == -1) {
        // orange cell
        DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR); // Shadow
        DrawPoly(center, sides, radius, 0, NEWORANGE);
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++)
        {
            DrawPolyLines(center, sides, radius - width, 0, DARKORANGE); // Cell Outline
        }
        DrawPolyLines(center, sides, radius, 0, WHITETEXT);
        DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT);
    } else if (rawData == -2) {
        // marked cell (blue)
        DrawPoly((Vector2){center.x + shadowOffsetX, center.y + shadowOffsetY}, sides, radius, 0, SHADOWCOLOR); // Shadow
        DrawPoly(center, sides, radius, 0, NEWBLUE);
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++) {
            DrawPolyLines(center, sides, radius - width, 0, NEWDARKBLUE); // Cell Outline
        }
        DrawPolyLines(center, sides, radius, 0, WHITETEXT);
        DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT);
    } else {
        // every other cell
        int type = rawData/10;
        int value = rawData%10;

        if (type < 3) {
            // cell with text
            if (value < 7)
            {
                // revealed cell
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
                // bomb cell
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

        // CELL TEXT
        switch (type) {
        case 0:
            if (value < 7) {
                // normal cell 
                DrawTextEx(defaultFont, TextFormat("%i", (int)value), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("%i", (int)value), cellTextSize[currentLayout], fontSpacing).x / 2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout],1, WHITETEXT);
            } else if (value == 9)
            {
                // ending screen cell
                DrawTextEx(defaultFont, ":)", (Vector2){center.x - MeasureTextEx(defaultFont, ":)", cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, WHITETEXT);
            }
             break;
        case 1:
            // normal cell with text in {}
            DrawTextEx(defaultFont, TextFormat("{%i}", (int)value), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("{%i}", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, WHITETEXT);
            break;
        case 2:
            // normal cell with text in --
            DrawTextEx(defaultFont, TextFormat("-%i-", (int)value), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("-%i-", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, WHITETEXT);
            break;
        case 4:
            // no cell, floating text
            DrawTextEx(defaultFont, TextFormat("%i ", (int)value), (Vector2){(cellRadius[currentLayout] * 0.6F) + center.x - MeasureTextEx(defaultFont, TextFormat("%i ", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, DARKERGRAY);
            break;
        case 5:
            // floating text in {}
            DrawTextEx(defaultFont, TextFormat("{%i} ", (int)value), (Vector2){(cellRadius[currentLayout] * 0.6F) + center.x - MeasureTextEx(defaultFont, TextFormat("{%i}---", (int)value), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], 1, DARKERGRAY);
            break;
        case 6:
            // floating text in --
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

    DrawTextEx(defaultFont, TextFormat("%i", (int)text), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("%i", (int)text), cellTextSize[currentLayout], fontSpacing).x/2, center.y + cellTextOffsetY[currentLayout]}, cellTextSize[currentLayout], fontSpacing, WHITETEXT); 
}

void drawRestartButton() {
    Vector2 mouse = GetMousePosition();
    // check if mouse is hovering over restart button
    bool collision = CheckCollisionPointCircle(mouse, (Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, cellRadius[1] - hitboxVal);
    if (collision)
    {
        drawCell((Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1]) - (1*35)}, 6, cellRadius[1], -1);
        DrawTextureEx(restartTexture, (Vector2){GetScreenWidth()-120, GetScreenHeight()-20 - (1*35)}, 0, 0.3125, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
        loadLevel(); // restart level
        }
    } else
    {
        drawCell((Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -1);
        DrawTextureEx(restartTexture, (Vector2){GetScreenWidth()-120, GetScreenHeight()-20}, 0, 0.3125, WHITE);
    } 
}

void drawExitButton() {
    Vector2 mouse = GetMousePosition();
    // check if mouse is hovering over exit button
    bool collision = CheckCollisionPointCircle(mouse, (Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, cellRadius[currentLayout] - hitboxVal);
    if (collision)
    {
        drawCell((Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1]) - (1*35)}, 6, cellRadius[1], -1);
        DrawTextureEx(exitTexture, (Vector2){GetScreenWidth()-120 + distanceBetweenCellsX[1], GetScreenHeight()-20 - (1*35)}, 0, 0.3125, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
        currentLevel = -1; // change level to main menu
        currentLayout = 1; // use main menu layout
        resetProgress(); // load menu 'level' 
        }
    } else
    {
        drawCell((Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -1);
        DrawTextureEx(exitTexture, (Vector2){GetScreenWidth()-120 + distanceBetweenCellsX[1], GetScreenHeight()-20}, 0, 0.3125, WHITE);
    } 
}


void drawMenu(void) {
    // draw main menu screen
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
            int x = k + (l*7);
            if (gameProgress[x] == '0')
            {
               drawMenuCell((Vector2){k*distanceBetweenCellsX[1] + 575 + l*secondRowOffset[1], GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY[1]}, 6, cellRadius[1], NEWORANGE, DARKORANGE, x+1);
            } else {
                drawMenuCell((Vector2){k*distanceBetweenCellsX[1] + 575 + l*secondRowOffset[1], GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY[1]}, 6, cellRadius[1], NEWBLUE, NEWDARKBLUE, x+1);
            }
        }
    }
    DrawText("LEVELS", GetScreenWidth()/2 - MeasureText("LEVELS", 60) / 2, GetScreenHeight()/2 + 170, 60, SHADOWCOLOR);
    EndDrawing();
}


void updateMenu(void) {
    // handle left click on menu screen
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
                    loadLevel(); // load level the user chose
                }
            }
        }
    }
}


void drawTutorialText() {
    // draw tutorial on selected levels
    if (currentLevel == 0)
    {
        DrawTexture(mouseTexture, GetScreenWidth()/2 - 64/2, GetScreenHeight()-130, WHITE);
        DrawTextEx(defaultFont, "Destroy a hex", (Vector2){GetScreenWidth()/2 - 160, GetScreenHeight() - 100}, 20, fontSpacing, DARKORANGE);
        DrawTextEx(defaultFont, "Mark it as a bomb", (Vector2){GetScreenWidth()/2 + 52, GetScreenHeight() - 100}, 20, fontSpacing, NEWDARKBLUE);
    } else if (currentLevel == 6)
    {
        DrawTextEx(defaultFont, "Numbers outside the grid show the number \nof bombs in that row", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
    } else if (currentLevel == 7)
    {
        DrawTextEx(defaultFont, "Numbers like this {3} give additional information that the 3 bombs are consecutive", (Vector2){GetScreenWidth()/2 - MeasureTextEx(defaultFont, "Numbers like this {3} give additional information that the 3 bombs are consecutive", 32, fontSpacing).x/2 , GetScreenHeight() - 120}, 32, fontSpacing, DARKGRAY);
    } else if (currentLevel == 9)
    {
        DrawTextEx(defaultFont, "Numbers like this {3} give additional\ninformation that the 3 bombs\nare consecutive", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
    } else if (currentLevel == 11)
    {
        DrawTextEx(defaultFont, "Numbers like this -3- give additional\ninformation that the 3 bombs\nare NOT connected", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
    } else if (currentLevel == 14) {
        DrawTextEx(defaultFont, "Congratulations for beating the game!", (Vector2){(GetScreenWidth() - MeasureTextEx(defaultFont, "Congratulations for beating the game!", 32, fontSpacing).x)/2, GetScreenHeight()/2 + 60}, 32, fontSpacing, DARKGRAY);
    }
}


void loadSaveData() {
    FILE *f = fopen("saveData", "r");
    fgets(gameProgress, LEVELCOUNT, f);
    fclose(f);
}


int main(void) 
{
    // initialization
    SetConfigFlags(FLAG_MSAA_4X_HINT); // anti-aliasing
    InitWindow(screenWidth, screenHeight, "PutHexcells");
    SetTargetFPS(30);

    // music
    InitAudioDevice();
    Music music = LoadMusicStream("resources/Puzzle-Game-2_Looping.mp3");
    music.looping = true;
    SetMusicVolume(music, 0.05);
    PlayMusicStream(music);

    // textures
    restartTexture = LoadTexture("Resources/restart.png");
    exitTexture = LoadTexture("Resources/exit.png");
    mouseTexture = LoadTexture("Resources/mouse.png");

    // font
    defaultFont = LoadFontEx("Resources/Roboto-Medium.ttf", 32, 0, 0);
    GenTextureMipmaps(&defaultFont.texture);
    SetTextureFilter(defaultFont.texture, FILTER_TRILINEAR);
    
    //load save
    loadSaveData();
    
    // game loop
    while (!WindowShouldClose())    // detect window close button or ESC key
    {   
        UpdateMusicStream(music); // update music

        if (currentLevel >= 0) // if player is in a level
        {
            updateGame();
            drawFrame();
            drawRestartButton();
            drawExitButton();

        } else // if player is in a main menu (currentLevel = -1)
        {
            updateMenu();
            drawMenu();
        }

    }

    // de-initialization
    UnloadTexture(exitTexture);
    UnloadTexture(restartTexture);
    UnloadTexture(mouseTexture);
    UnloadMusicStream(music);
    UnloadFont(defaultFont); 
    CloseAudioDevice(); 
    CloseWindow();        // close window and OpenGL context

    return 0;
}


void drawFrame() {
    // draw level
    BeginDrawing();
    ClearBackground(BACKGROUND);
    DrawText(TextFormat("LEVEL %i", (int)currentLevel + 1), 0, GetScreenHeight()-80, 100, (Color){240,240,240,200});

    // draw board
    for (int row = 0; row < NUMBEROFROWS[currentLayout]; row++)
        {
        for (int col = 0; col < NUMBEROFCOLUMNS[currentLayout]; col++)
            {
                int toc = level[currentLevel].seed[row*NUMBEROFCOLUMNS[currentLayout] + col]; // type of cell
                int revealed = revealedCells[row*NUMBEROFCOLUMNS[currentLayout] + col]; // is the cell revealed/flagged

                if (toc != 0)
                {   
                    // if cell is not empty (it exists)
                    switch (revealed)
                    {
                    case 0:
                        // not revealed
                        drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], -1);
                        break;
                    case 1:
                        // revealed
                        switch (toc)
                        {
                        case -1:
                            // cell with number 0 in it
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], 0);
                            break;
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                            // cell with numbers 1-6 in it
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], toc);
                            break;
                        case 7:
                            // bomb cell
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout],playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], 7);   
                            break;
                        default:
                            // every other cell
                            drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], toc);
                            break;
                        }
                        break;
                    case 2:
                        // flagged
                        drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], -2);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    drawTutorialText();
    EndDrawing();
}


void updateGame(void) {
    if (currentLevel < 14)
    {
        // check if revealedCells is same as solution, if yes chk = 0
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
            // victory, change to next level

            // save game progress to a file
            gameProgress[currentLevel] = '1';
            FILE *f = fopen("saveData", "w");
            fputs(gameProgress, f);
            fclose(f);

            //load next level
            currentLevel = currentLevel + 1;
            loadLevel();
        }
    }
    
    //handle mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        revealCell(getClickedCellIndex());
    }
    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        flagCell(getClickedCellIndex());
    }  
}
