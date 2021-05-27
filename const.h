// number of levels in the game
#define LEVELCOUNT  15

// colors
#define NEWORANGE       (Color){ 255, 177, 41, 255 }
#define DARKORANGE      (Color){ 254, 161, 3, 255 }
#define DARKERGRAY      (Color){ 50, 50, 50, 255 }
#define NEWDARKGRAY     (Color){ 62, 62, 62, 255 }
#define NEWBLUE         (Color){ 6, 164, 235, 255 }
#define NEWDARKBLUE     (Color){ 14, 157, 217, 255 }
#define LIGHTLIGHTGRAY  (Color){222, 222, 222, 255}
#define BACKGROUND      (Color){250, 250, 250, 255}
#define SHADOWCOLOR     (Color){223, 223, 223, 200}
#define WHITETEXT       (Color){235, 235, 235, 255}

// hexcellsclone has 3 level layout grids: 14x8, 14x22, 18x28
// on level load currentlayout is updated and used to pick correct rendering values from here
// e.g if currentLayout = 1, to render our level we use NUMBEROFROWS[1], cellRadius[1], ...

static const int NUMBEROFROWS[3] = {8,14,18};
static const int NUMBEROFCOLUMNS[3] = {14,22,28};

static const int playareaOffsetX[3] = {64,30,40};
static const int playareaOffsetY = 60;
static const int screenWidth = 1600;
static const int screenHeight = 900;


static const int secondRowOffset[3] = {53,35,27};
static const int distanceBetweenCellsX[3] = {105,70,55}; // between centers of cells
static const int distanceBetweenCellsY[3] = {90,60,47};
static const int cellRadius[3] = {52,36,28};
static const int cellTextOffsetY[3] = {-24,-15,-13};
static const int cellTextSize[3] = {50,30,26};
static const int cellOutlineWidth[3] = {10, 6, 5};

static const int fontSpacing = 1;

static const int shadowOffsetX = -8;
static const int shadowOffsetY = 5;

static const int flaggedGradientRealCycle = 24;
static const int flaggedGradientFullCycle = flaggedGradientRealCycle * 4;
