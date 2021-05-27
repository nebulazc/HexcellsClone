#include <stdio.h>

// level layout
// revealed[20]
// flagged[13]
// seed[504]


int layout = 0;

int revealed[20] =  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int flagged[13] =  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int seed[504] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,9,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0
        ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1
        };


int main() {
    FILE *file;
    file = fopen("export.bin", "wb");
    if (file == NULL) {
        printf("Cant open\n");
        return 1;
    }
    fwrite(&layout, sizeof(int), 1, file);
    for (int i = 0; i < 20; i++)
    {
        fwrite(&revealed[i], sizeof(int), 1, file);
    }
    for (int i = 0; i < 13; i++)
    {
        fwrite(&flagged[i], sizeof(int), 1, file);
    }
    for (int i = 0; i < 504; i++)
    {
        fwrite(&seed[i], sizeof(int), 1, file);
    }
    fclose(file);

    file = fopen("export.bin", "rb");
    if (file == NULL)
    {
        printf("Cant open2\n");
        return 1;
    }
    int n = -15;
    fread(&n, sizeof(int), 1, file);
    printf("\nLayout: ");
    printf("%d ",n);
    printf("\nRevealed: ");
    for (int i = 0; i < 20; i++)
    {
        fread(&n, sizeof(int), 1, file);
        printf("%d ",n);
    }
    printf("\nFlagged: ");
    for (int i = 0; i < 13; i++)
    {
        fread(&n, sizeof(int), 1, file);
        printf("%d ",n);
    }
    printf("\nSeed: ");
    for (int i = 0; i < 504; i++)
    {
        fread(&n, sizeof(int), 1, file);
        printf("%d ",n);
    }
    fclose(file);

    

    return 0;
}