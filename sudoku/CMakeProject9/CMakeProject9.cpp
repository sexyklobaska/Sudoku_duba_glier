#include <stdio.h>
#include <conio.h>

#define VELIKOST 9

void clearScreen() {
    printf("\033[H\033[J");
}

void tiskMřížky(int mřížka[VELIKOST][VELIKOST], int kurzorX, int kurzorY) {
    clearScreen(); 

    for (int i = 0; i < VELIKOST; i++) {
        for (int j = 0; j < VELIKOST; j++) {
            if (i == kurzorY && j == kurzorX) {
                printf("[%2d] ", mřížka[i][j]);
            }
            else {
                printf("%4d ", mřížka[i][j]);
            }
        }
        printf("\n");
    }
}

void printMenu() {
    printf("\n----- MENU -----\n");
    printf("1. Hrat Sudoku\n");
    printf("2. Nacist Sudoku\n");
    printf("3. Ulozit Sudoku\n");
    printf("4. Nacist ulozenou hru\n");
    printf("5. Konec\n");
    printf("----------------\n");
    printf("Vyberte moznost: ");
}

void loadSudoku(int sudokuMřížka[VELIKOST][VELIKOST], const char* filename) {
    printf("Loading Sudoku from file: %s...\n", filename);

    FILE* file;
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Nepodarilo se otevrit soubor pro cteni.\n");
        return;
    }

    for (int i = 0; i < VELIKOST; i++) {
        for (int j = 0; j < VELIKOST; j++) {
            if (fscanf(file, "%d", &sudokuMřížka[i][j]) != 1) {
                printf("Chyba pri cteni dat ze souboru.\n");
                fclose(file);
                return;
            }
        }
    }

    fclose(file);

    printf("Sudoku loaded.\n");
}

void ulozitSudoku(int sudokuMřížka[VELIKOST][VELIKOST]) {
    FILE* file;
    file = fopen("saved_sudoku.txt", "w");

    if (file == NULL) {
        printf("Nepodarilo se otevrit soubor pro zapis.\n");
        return;
    }

    for (int i = 0; i < VELIKOST; i++) {
        for (int j = 0; j < VELIKOST; j++) {
            fprintf(file, "%d ", sudokuMřížka[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    printf("Sudoku ulozeno do souboru 'saved_sudoku.txt'.\n");
}

int isRowValid(int row, int sudokuMřížka[VELIKOST][VELIKOST]) {
    int used[VELIKOST + 1] = { 0 };

    for (int j = 0; j < VELIKOST; j++) {
        int num = sudokuMřížka[row][j];
        if (num < 1 || num > VELIKOST || used[num]) {
            return 0; 
        }
        used[num] = 1;
    }

    return 1; 
}

int isColumnValid(int col, int sudokuMřížka[VELIKOST][VELIKOST]) {
    int used[VELIKOST + 1] = { 0 };

    for (int i = 0; i < VELIKOST; i++) {
        int num = sudokuMřížka[i][col];
        if (num < 1 || num > VELIKOST || used[num]) {
            return 0; 
        }
        used[num] = 1;
    }

    return 1; 
}

int isGridValid(int sudokuMřížka[VELIKOST][VELIKOST]) {
    for (int i = 0; i < VELIKOST; i++) {
        if (!isRowValid(i, sudokuMřížka) || !isColumnValid(i, sudokuMřížka)) {
            return 0; 
        }
    }


    for (int i = 0; i < VELIKOST; i += 3) {
        for (int j = 0; j < VELIKOST; j += 3) {
            int used[VELIKOST + 1] = { 0 };

            for (int x = i; x < i + 3; x++) {
                for (int y = j; y < j + 3; y++) {
                    int num = sudokuMřížka[x][y];
                    if (num < 1 || num > VELIKOST || used[num]) {
                        return 0;
                    }
                    used[num] = 1;
                }
            }
        }
    }

    return 1;
}

int main() {
    int sudokuMřížka[VELIKOST][VELIKOST];
    int kurzorX = 0, kurzorY = 0;
    int klávesa;
    int loadedSudoku = 0; 

    do {
        printMenu();

        
        klávesa = getch();

        switch (klávesa) {
        case '1': 
        {
            if (!loadedSudoku) {
               
                int vyber;
                printf("Vyberte soubor s hernim polem (1, 2, 3) nebo stisknete Enter pro nacteni ulozene hry: ");
                if (scanf("%d", &vyber) == 1) {
                    char filename[20];
                    snprintf(filename, sizeof(filename), "sudoku%d.txt", vyber);
                    loadSudoku(sudokuMřížka, filename);
                }
                else {
            
                    loadSudoku(sudokuMřížka, "saved_sudoku.txt");
                }
                loadedSudoku = 1; 
            }

            
            do {
                tiskMřížky(sudokuMřížka, kurzorX, kurzorY);

            
                klávesa = getch();

                switch (klávesa) {
                case 72:
                    kurzorY = (kurzorY - 1 + VELIKOST) % VELIKOST;
                    break;
                case 80:
                    kurzorY = (kurzorY + 1) % VELIKOST;
                    break;
                case 75:
                    kurzorX = (kurzorX - 1 + VELIKOST) % VELIKOST;
                    break;
                case 77:
                    kurzorX = (kurzorX + 1) % VELIKOST;
                    break;
                case 13: 
                    printf("Zadejte cislo na pozici [%d, %d]: ", kurzorY, kurzorX);
                    scanf("%d", &sudokuMřížka[kurzorY][kurzorX]);
                    while (getchar() != '\n');
                    break;
                case 's': 
                    ulozitSudoku(sudokuMřížka);
                    break;
                    
                }

            } while (klávesa != 27 && !isGridValid(sudokuMřížka)); 

            if (isGridValid(sudokuMřížka)) {
                clearScreen();
                printf("Congratulations! You've solved the Sudoku puzzle.\n");
                loadedSudoku = 0; 
                getch(); 
            }
        }
        break;
        case '2': 
        {
            int vyber;
            printf("Vyberte soubor s hernim polem (1, 2, 3) nebo stisknete Enter pro nacteni ulozene hry: ");
            if (scanf("%d", &vyber) == 1) {
                char filename[20];
                snprintf(filename, sizeof(filename), "sudoku%d.txt", vyber);
                loadSudoku(sudokuMřížka, filename);
            }
            else {
                
                loadSudoku(sudokuMřížka, "saved_sudoku.txt");
            }
            loadedSudoku = 1; 
        }
        break;
        case '3':
            ulozitSudoku(sudokuMřížka);
            break;
        case '4': 
            loadSudoku(sudokuMřížka, "saved_sudoku.txt");
            loadedSudoku = 1;
            break;
        case '5': 
            printf("Konec programu.\n");
            break;
        default:
            printf("Neplatna volba. Zkuste znovu.\n");
        }

    } while (klávesa != '5');

    return 0;
}
