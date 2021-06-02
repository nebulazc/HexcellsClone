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
#include "const.h"
#include <stdio.h>


// === Game Logic ===
static const int hitboxVal = 2; // used to control hitbox circle radius

int revealedCells[504]; // current level 'progress', stores which cells have been revealed or marked by player
int solution[504]; // holds level solution which is then compared to revealedCells
int chk; // 'Remaining Moves' counter, amount of cells revealedCells is diffrent from solution
int currentLevel = -1; // tracks current level

// save file data, when a game first starts it loads a default "000..." data (no levels beaten), then if a save exists it overwrites it with proper save data 
char gameProgress[LEVELCOUNT] = "000000000000000"; // 0 is unbeaten, 1 is beaten level

// stores level data
int levelSeed[504] = { 0 };
int levelRevealed[20] = { 0 };
int levelFlagged[13] = { 0 };

int lastMove = -1; // stores last move and whether undoLastMove() is available

int musicPlaying = 1; // is music playing, 1 is yes

// === Rendering ===
int currentLayout = 1; // level layout
// for level change animation
int onTransition = 0;
int transitionFrameCounter = 0;

// for flagged cell animation
int flaggedFrameCounter = 0;
int flaggedFrameDir = 1;

// assets which will be loaded in main() during initialization
Font defaultFont;
Texture2D mouseTexture, exitTexture, restartTexture, musicTexture, undoTexture;
Music music;

// =======================================
//
// FUNCTIONS
//
// =======================================

Color flaggedCellColor(void);
void updateColor();
void drawCell(Vector2 center, int sides, float radius, int rawData);
int getClickedCellIndex();
void drawMenuCell(Vector2 center, int sides, float radius, Color color, Color outlineColor, int text);
void drawRestartButton();
void drawExitButton();
void drawUndoButton();
void drawMusicButton();
void drawMenu();
void drawTutorialText();
void drawLevelTransition();
void drawFrame();

void revealCell(int index);
void flagCell(int index);
void resetProgress();
void loadLevel();
void updateMenu();
void loadSaveData();
void updateGame();
void undoLastMove();

// =======================================
// RENDERING
// =======================================

Color flaggedCellColor(void) {
    Color b = {255,255,255,255};
    switch (flaggedFrameCounter / (flaggedGradientFullCycle / 24)) {
    case 0:
    case 1:
    case 2:
    case 3:
        b.r = 6;
        b.g = 164;
        b.b = 235;
        break;
    case 4:
        b.r = 6;
        b.g = 164;
        b.b = 235;
        break;
    case 5:
        b.r = 0;
        b.g = 166;
        b.b = 234;
        break;
    case 6:
        b.r = 0;
        b.g = 167;
        b.b = 232;
        break;
    case 7:
        b.r = 0;
        b.g = 169;
        b.b = 231;
        break;
    case 8:
        b.r = 1;
        b.g = 170;
        b.b = 229;
        break;    
    case 9:
        b.r = 7;
        b.g = 172;
        b.b = 227;
        break;
    case 10:
        b.r = 15;
        b.g = 173;
        b.b =226;
        break;
    case 11:
        b.r = 23;
        b.g = 174;
        b.b = 224;
        break;
    case 12:
        b.r = 30;
        b.g = 176;
        b.b = 222;
        break;
    case 13:
        b.r = 37;
        b.g =177;
        b.b = 220;
        break;
    case 14:
        b.r = 43;
        b.g = 178;
        b.b = 218;
        break;
    case 15:
        b.r = 49;
        b.g = 179;
        b.b = 216;
        break;
    case 16:
        b.r = 55;
        b.g = 180;
        b.b = 214;
        break;
    case 17:
        b.r = 61;
        b.g = 181;
        b.b = 213;
        break;
    case 18:
        b.r = 67;
        b.g = 182;
        b.b = 211;
        break;
    case 19:
        b.r = 72;
        b.g = 183;
        b.b = 209;
        break;
    case 20:
        b.r = 78;
        b.g = 184;
        b.b = 207;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
        b.r = 83;
        b.g = 185;
        b.b = 205;
        break;
    default:
        break;
    }
    return b;
}


void updateColor() {
    if (flaggedFrameDir == 1) {
        flaggedFrameCounter++;
    } else {
        flaggedFrameCounter--;
    }
    if (flaggedFrameCounter > flaggedGradientFullCycle || flaggedFrameCounter < 0) {
        flaggedFrameDir *= -1;
    }
}


void drawCell(Vector2 center, int sides, float radius, int rawData) {
    // CELL BODY
    if (rawData == -1) {
        // orange cell
        DrawPoly((Vector2){center.x + shadowOffsetX[currentLayout], center.y + shadowOffsetY[currentLayout]}, sides, radius, 0, SHADOWCOLOR); // Shadow
        DrawPoly(center, sides, radius, 0, NEWORANGE);
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++) {
            DrawPolyLines(center, sides, radius - width, 0, DARKORANGE); // Cell Outline
        }
        DrawPolyLines(center, sides, radius, 0, WHITETEXT);
        DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT);
        DrawPolyLines(center, sides, radius + 2, 0, WHITETEXT);
    } else if (rawData == -2) {
        // marked cell (blue)
        DrawPoly((Vector2){center.x + shadowOffsetX[currentLayout], center.y + shadowOffsetY[currentLayout]}, sides, radius, 0, SHADOWCOLOR); // Shadow
        DrawPoly(center, sides, radius, 0, flaggedCellColor());
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++) {
            DrawPolyLines(center, sides, radius - width, 0, flaggedCellColor()); // Cell Outline
        }
        DrawPolyLines(center, sides, radius, 0, WHITETEXT);
        DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT);
    } else if (rawData == -3) {
        // restart/exit cell
        // DrawPoly((Vector2){center.x + shadowOffsetX[currentLayout], center.y + shadowOffsetY[currentLayout]}, sides, radius, 0, SHADOWCOLOR); // Shadow
        DrawPoly(center, sides, radius, 0, (Color){255,177,41,70});
        for (int width = 2; width < cellOutlineWidth[currentLayout]; width++) {
            DrawPolyLines(center, sides, radius - width, 0, (Color){254,161,3,30}); // Cell Outline
        }
        DrawPolyLines(center, sides, radius, 0, (Color){235,235,235,30});
        DrawPolyLines(center, sides, radius + 1, 0, (Color){235,235,235,30});
    } else {
        // every other cell
        int type = rawData/10;
        int value = rawData%10;

        if (type < 3) {
            // cell with text
            if (value < 7) {
                // revealed cell
                DrawPoly((Vector2){center.x + shadowOffsetX[currentLayout], center.y + shadowOffsetY[currentLayout]}, sides, radius, 0, SHADOWCOLOR); // Shadow
                DrawPoly(center, sides, radius, 0, NEWDARKGRAY);
                for (int width = 2; width < cellOutlineWidth[currentLayout]; width++) {
                    DrawPolyLines(center, sides, radius - width, 0, DARKERGRAY);
                }
                DrawPolyLines(center, sides, radius, 0, WHITETEXT);
                DrawPolyLines(center, sides, radius + 1, 0, WHITETEXT); 
            } else {
                // bomb cell
                DrawPoly((Vector2){center.x + shadowOffsetX[currentLayout], center.y + shadowOffsetY[currentLayout]}, sides, radius, 0, SHADOWCOLOR); // Shadow
                DrawPoly(center, sides, radius, 0, RED);
                for (int width = 2; width < cellOutlineWidth[currentLayout]; width++) {
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
            } else if (value == 9) {
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


int getClickedCellIndex() {
    // loops by every cell and checks if mouse is over any of them
    Vector2 mouse = GetMousePosition();
    for (int row = 0; row < numberOfRows[currentLayout]; row++) {
        for (int col = 0; col < numberOfColumns[currentLayout]; col++) {
            bool collision = CheckCollisionPointCircle(mouse, (Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, cellRadius[currentLayout] - hitboxVal);
            if (collision) { return (row*numberOfColumns[currentLayout] + col); }
        }
    }
    return -1;
}


void drawMenuCell(Vector2 center, int sides, float radius, Color color, Color outlineColor, int text) {
    DrawPoly((Vector2){center.x + shadowOffsetX[1], center.y + shadowOffsetY[1]}, sides, radius, 0, SHADOWCOLOR); // Shadow
    DrawPoly(center, sides, radius, 0, color);
    for (int width = 2; width < cellOutlineWidth[1]; width++) {
        DrawPolyLines(center, sides, radius - width, 0, outlineColor);
    }

    DrawPolyLines(center, sides, radius, 0, LIGHTLIGHTGRAY);
    DrawPolyLines(center, sides, radius + 1, 0, LIGHTLIGHTGRAY);

    DrawTextEx(defaultFont, TextFormat("%i", (int)text), (Vector2){center.x - MeasureTextEx(defaultFont, TextFormat("%i", (int)text), cellTextSize[1], fontSpacing).x/2, center.y + cellTextOffsetY[1]}, cellTextSize[1], fontSpacing, WHITETEXT); 
}


void drawRestartButton() {
    Vector2 mouse = GetMousePosition();
    // check if mouse is hovering over restart button
    bool collision = CheckCollisionPointCircle(mouse, (Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, cellRadius[1] - hitboxVal);
    if (collision) {
        drawCell((Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1]) - (1*35)}, 6, cellRadius[1], -1);
        DrawTextureEx(restartTexture, (Vector2){GetScreenWidth()-120, GetScreenHeight()-20 - (1*35)}, 0, 0.3125, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        loadLevel(); // restart level
        }
    } else {
        drawCell((Vector2){playareaOffsetX[1] + 21*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -3);
        DrawTextureEx(restartTexture, (Vector2){GetScreenWidth()-120, GetScreenHeight()-20}, 0, 0.3125, (Color){255,255,255,150});
    } 
}


void drawExitButton() {
    Vector2 mouse = GetMousePosition();
    // check if mouse is hovering over exit button
    bool collision = CheckCollisionPointCircle(mouse, (Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, cellRadius[currentLayout] - hitboxVal);
    if (collision) {
        drawCell((Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1]) - (1*35)}, 6, cellRadius[1], -1);
        DrawTextureEx(exitTexture, (Vector2){GetScreenWidth()-120 + distanceBetweenCellsX[1], GetScreenHeight()-20 - (1*35)}, 0, 0.3125, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentLevel = -1; // change level to main menu
        currentLayout = 1; // use main menu layout
        resetProgress(); // load menu 'level' 
        }
    } else {
        drawCell((Vector2){playareaOffsetX[1] + 22*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -3);
        DrawTextureEx(exitTexture, (Vector2){GetScreenWidth()-120 + distanceBetweenCellsX[1], GetScreenHeight()-20}, 0, 0.3125, (Color){255,255,255,150});
    } 
}


void drawUndoButton() {
    if (lastMove == -2) {
        drawCell((Vector2){playareaOffsetX[1] + 20*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -3);
        DrawTextureEx(undoTexture, (Vector2){GetScreenWidth()-120 - distanceBetweenCellsX[1], GetScreenHeight()-20}, 0, 0.3125, (Color){255,255,255,150});
    } else {
        Vector2 mouse = GetMousePosition();
        // check if mouse is hovering over exit button
        bool collision = CheckCollisionPointCircle(mouse, (Vector2){playareaOffsetX[1] + 20*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, cellRadius[currentLayout] - hitboxVal);
        if (collision) {
            drawCell((Vector2){playareaOffsetX[1] + 20*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1]) - (1*35)}, 6, cellRadius[1], -1);
            DrawTextureEx(undoTexture, (Vector2){GetScreenWidth()-120 - distanceBetweenCellsX[1], GetScreenHeight()-20 - (1*35)}, 0, 0.3125, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                undoLastMove();
            }
        } else {
            drawCell((Vector2){playareaOffsetX[1] + 20*distanceBetweenCellsX[1], playareaOffsetY + (14*distanceBetweenCellsY[1])}, 6, cellRadius[1], -3);
            DrawTextureEx(undoTexture, (Vector2){GetScreenWidth()-120 - distanceBetweenCellsX[1], GetScreenHeight()-20}, 0, 0.3125, (Color){255,255,255,150});
        } 
    }
}


void drawMusicButton() {
    if (musicPlaying == 1) {
        DrawTextureEx(musicTexture, (Vector2){GetScreenWidth()-42, 0}, 0, 0.04, DARKERGRAY);
    } else {
        DrawTextureEx(musicTexture, (Vector2){GetScreenWidth()-42, 0}, 0, 0.04, (Color){ 50, 50, 50, 40});
    }
}


void drawMenu() {
    // draw main menu screen
    BeginDrawing();
    ClearBackground(BACKGROUND);
    drawCell((Vector2){GetScreenWidth()/2, 130}, 6, cellRadius[2], -2);
    DrawText("Hexcells", GetScreenWidth()/2 - MeasureText("Hexcells", 100) / 2 - 6, GetScreenHeight() / 2 - 294, 100, SHADOWCOLOR);
    DrawText("Hexcells", GetScreenWidth()/2 - MeasureText("Hexcells", 100) / 2, GetScreenHeight() / 2 - 300, 100, GRAY);
    DrawText("clone by Mateusz Wisniewski", GetScreenWidth()/2 - MeasureText("clone by Mateusz Wisniewski", 40) / 2, GetScreenHeight() / 2 - 200, 40, LIGHTGRAY);
    DrawText("Music by Eric Matyas (www.soundimage.org)", 1, GetScreenHeight()-20, 20, LIGHTGRAY);

    for (int k = 0; k < 7; k++) {
        for (int l = 0; l < 2; l++) {
            int x = k + (l*7);
            if (gameProgress[x] == '0') {
               drawMenuCell((Vector2){k*distanceBetweenCellsX[1] + 575 + l*secondRowOffset[1], GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY[1]}, 6, cellRadius[1], NEWORANGE, DARKORANGE, x+1);
            } else {
                drawMenuCell((Vector2){k*distanceBetweenCellsX[1] + 575 + l*secondRowOffset[1], GetScreenHeight()/2 + 300 + l*distanceBetweenCellsY[1]}, 6, cellRadius[1], flaggedCellColor(), flaggedCellColor(), x+1);
            }
        }
    }
    DrawText("LEVELS", GetScreenWidth()/2 - MeasureText("LEVELS", 60) / 2, GetScreenHeight()/2 + 170, 60, SHADOWCOLOR);
    drawMusicButton();
    EndDrawing();
}


void drawTutorialText() {
    // draw tutorial on selected levels
    switch (currentLevel) {
    case 0:
        DrawTexture(mouseTexture, GetScreenWidth()/2 - 64/2, GetScreenHeight()-130, WHITE);
        DrawTextEx(defaultFont, "Destroy a hex", (Vector2){GetScreenWidth()/2 - 160, GetScreenHeight() - 100}, 20, fontSpacing, DARKORANGE);
        DrawTextEx(defaultFont, "Mark it as a bomb", (Vector2){GetScreenWidth()/2 + 52, GetScreenHeight() - 100}, 20, fontSpacing, NEWDARKBLUE);
        break;
    case 6:
        DrawTextEx(defaultFont, "Numbers outside the grid show the number \nof bombs in that row", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
        break;
    case 7:
        DrawTextEx(defaultFont, "Numbers like this {3} give additional information that the 3 bombs are consecutive", (Vector2){GetScreenWidth()/2 - MeasureTextEx(defaultFont, "Numbers like this {3} give additional information that the 3 bombs are consecutive", 32, fontSpacing).x/2 , GetScreenHeight() - 120}, 32, fontSpacing, DARKGRAY);
        break;
    case 9:
        DrawTextEx(defaultFont, "Numbers like this {3} give additional\ninformation that the 3 bombs\nare consecutive", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
        break;
    case 11:
        DrawTextEx(defaultFont, "Numbers like this -3- give additional\ninformation that the 3 bombs\nare NOT connected", (Vector2){80, GetScreenHeight()/2 - 100}, 32, fontSpacing, DARKGRAY);
        break;
    case 14:
        DrawTextEx(defaultFont, "Congratulations for beating the game!", (Vector2){(GetScreenWidth() - MeasureTextEx(defaultFont, "Congratulations for beating the game!", 32, fontSpacing).x)/2, GetScreenHeight()/2 + 60}, 32, fontSpacing, DARKGRAY);
        break;
    default:
        break;
    }
}


void drawLevelTransition() {
    if (onTransition == 1) {
        transitionFrameCounter++;
        if (transitionFrameCounter < 10) {
            DrawRectangle(0,0,screenWidth,screenHeight,(Color){255,255,255,30 + (transitionFrameCounter*25)});
        } else if (transitionFrameCounter == 10) {
            DrawRectangle(0,0,screenWidth,screenHeight,(Color){255,255,255,255});
            currentLevel = currentLevel + 1;
            loadLevel();
            DrawRectangle(0,0,screenWidth,screenHeight,(Color){255,255,255,255});
        } else if (transitionFrameCounter > 10) {
            DrawRectangle(0,0,screenWidth,screenHeight,(Color){255,255,255,255 - ((transitionFrameCounter - 10)*25)});
            if (transitionFrameCounter == 20) {
                onTransition = 0;
                transitionFrameCounter = 0;
            }
        }
    }
}


void drawFrame() {
    // draw level
    BeginDrawing();
    ClearBackground(BACKGROUND);

    DrawText(TextFormat("LEVEL %i", (int)currentLevel + 1), 0, GetScreenHeight()-63, 80, (Color){230,230,230,200});

    // draw board
    for (int row = 0; row < numberOfRows[currentLayout]; row++) {
        for (int col = 0; col < numberOfColumns[currentLayout]; col++) {
                int toc = levelSeed[row*numberOfColumns[currentLayout] + col]; // type of cell
                int revealed = revealedCells[row*numberOfColumns[currentLayout] + col]; // is the cell revealed/flagged

                if (toc != 0) {   
                    // if cell is not empty (it exists)
                    switch (revealed) {
                    case 0:
                        // not revealed
                        drawCell((Vector2){(row%2 * secondRowOffset[currentLayout]) + playareaOffsetX[currentLayout] + col*distanceBetweenCellsX[currentLayout], playareaOffsetY + (row*distanceBetweenCellsY[currentLayout])}, 6, cellRadius[currentLayout], -1);
                        break;
                    case 1:
                        // revealed
                        switch (toc) {
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
    drawLevelTransition();
    EndDrawing();
}



// =======================================
// GAME LOGIC
// =======================================


void revealCell(int index) {
    // handle left-click on cell
    if (index != -1) {
        if (levelSeed[index] != 0) {
            if (revealedCells[index] == 0) {
                revealedCells[index] = 1;
                if (lastMove != -2) {lastMove = index;}
            }
        }
    }
    
}


void flagCell(int index) {
    // handle right-click on cell
    if (index != -1) {
        if (levelSeed[index] != 0) {
            if (revealedCells[index] == 0) {
                revealedCells[index] = 2;
            } else if (revealedCells[index] == 2) {
                revealedCells[index] = 0;
            }
        } 
    }
}


void resetProgress() {
    // reset all variables used for game logic
    for (int i = 0; i < 504; i++) {
        revealedCells[i] = 0;
        solution[i] = 0;
    }
    chk = 0;
}


void loadLevel() {
    resetProgress(); // resets level
    FILE *levelFile;
    char levelFilename[7];
    sprintf(levelFilename, "Levels/%d", currentLevel);

    levelFile = fopen(levelFilename, "rb");
    fread(&currentLayout, sizeof(int), 1, levelFile);
    for (int i = 0; i < 20; i++) {
        fread(&levelRevealed[i], sizeof(int), 1, levelFile);
    }
    for (int i = 0; i < 13; i++) {
        fread(&levelFlagged[i], sizeof(int), 1, levelFile);
    }
    for (int i = 0; i < 504; i++) {
        fread(&levelSeed[i], sizeof(int), 1, levelFile);
    }
    fclose(levelFile);
    
    // currentLayout = levellayout; // load layout

    // reveal cells at the start of the level
    for (int i = 0; i < 20; i++) {
        if (levelRevealed[i] >= 0) {
            revealedCells[ levelRevealed[i] ] = 1; 
        }
    }
    // flag cells at the start of the level
    for (int i = 0; i < 13; i++) {
        if (levelFlagged[i] >= 0) {
            revealedCells[ levelFlagged[i] ] = 2; 
        } 
    }

    // build level solution to later compare to
    for (int i = 0; i < (numberOfColumns[currentLayout] * numberOfRows[currentLayout]); i++) {
        if (levelSeed[i] < 7) {
            if (levelSeed[i] > 0) {
                solution[i] = 1;
            } else if (levelSeed[i] == -1) {
                solution[i] = 1;
            }
            
        } else if (levelSeed[i] == 7) {
            solution[i] = 2;
        } else if (levelSeed[i] < 70) {
            solution[i] = 1;
        }
    }
    lastMove = -1;
}


void updateMenu() {
    // handle left click on menu screen
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        for (int row = 0; row < 2; row++) {
            for (int col = 0; col < 7; col++) {
                bool collision = CheckCollisionPointCircle(mouse, (Vector2){col*distanceBetweenCellsX[1] + 575 + row*secondRowOffset[1], GetScreenHeight()/2 + 300 + row*distanceBetweenCellsY[1]}, cellRadius[1] - hitboxVal);
                if (collision) {
                    currentLevel = col + (row*7);
                    loadLevel(); // load level the user chose
                    return;
                }
            }
        }
        bool collision = CheckCollisionPointRec(mouse, (Rectangle){GetScreenWidth()-42, 0, 42, 40});
        if (collision) {
            if (musicPlaying == 1) {
                SetMusicVolume(music, 0);
            } else {
                SetMusicVolume(music, 0.05);
            }
            musicPlaying *= -1;
        }
    }
}


void undoLastMove() {
    if (lastMove >= 0) {
        revealedCells[lastMove] = 0;
        lastMove = -2;
    }
}


void loadSaveData() {
    FILE *f = fopen("saveData", "r");
    fgets(gameProgress, LEVELCOUNT, f);
    fclose(f);
}


void updateGame() {
    if (currentLevel < 14) {
        // check if revealedCells is same as solution, if yes chk = 0
        chk = 0;
        for (int i = 0; i < (numberOfColumns[currentLayout] * numberOfRows[currentLayout]); i++) {
            if (revealedCells[i] != solution[i]) {
                chk = chk + 1;
            }
        }
        if (chk == 0) {
            // victory, change to next level

            // save game progress to a file
            gameProgress[currentLevel] = '1';
            FILE *f = fopen("saveData", "w");
            fputs(gameProgress, f);
            fclose(f);

            //load next level
            onTransition = 1;
        }
    }
    
    //handle mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        revealCell(getClickedCellIndex());
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        flagCell(getClickedCellIndex());
    }  
}


int main(void) {
    // initialization
    SetConfigFlags(FLAG_MSAA_4X_HINT); // anti-aliasing
    InitWindow(screenWidth, screenHeight, "PutHexcells");
    SetTargetFPS(30);

    // music
    InitAudioDevice();
    music = LoadMusicStream("resources/Puzzle-Game-2_Looping.mp3");
    music.looping = true;
    SetMusicVolume(music, 0.05);
    PlayMusicStream(music);

    // textures
    restartTexture = LoadTexture("Resources/restart.png");
    exitTexture = LoadTexture("Resources/exit.png");
    mouseTexture = LoadTexture("Resources/mouse.png");
    musicTexture = LoadTexture("Resources/music.png");
    undoTexture = LoadTexture("Resources/undo.png");

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
        updateColor();
        if (currentLevel >= 0) {
            updateGame();
            drawFrame();
            drawRestartButton();
            drawExitButton();
            drawUndoButton();

        } else {
            updateMenu();
            drawMenu();
        }

    }

    // de-initialization
    UnloadTexture(exitTexture);
    UnloadTexture(restartTexture);
    UnloadTexture(mouseTexture);
    UnloadTexture(musicTexture);
    UnloadTexture(undoTexture);
    UnloadMusicStream(music);
    UnloadFont(defaultFont); 
    CloseAudioDevice(); 
    CloseWindow();        // close window and OpenGL context

    return 0;
}
