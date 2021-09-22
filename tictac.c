#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Tabla de joc va fi obtinuta dintr-o matrice 22x22.
/*
    \  /       
     \/           
     /\            
    /  \            
 */

/*
    **
   *  *
   *  *
    **
 */

/* Codificarea semnelor.
 * ' ' = 0
 * '=' = 1
 * '|' = 2
 * '/' = 3
 * '\' = 4
 * '*' = 5
 */

int** AllocateGrid (int dimension)
{
    int i, j, size;
    int **grid;

    if (dimension == 3)
        size = 22;
    else
        size = 36;

    grid = calloc (size, sizeof (int *));

    // Alocam gridul.
    if (grid == NULL){
        return NULL;
    }

    for (i = 0; i < size; i++){
        grid [i] = calloc (size, sizeof (int));
        if (grid [i] == NULL){
            return NULL;
        }
    }

    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++)
            grid [i][j] = 0;
    }

    return grid;
}

void CleanGrid (int **grid, int dimension)
{
    int i, size;

    if (dimension == 3)
        size = 22;
    else
        size = 36;

    for (i = 0; i < size; i++)
        free (grid [i]);
    free (grid);
}

int** AllocateGame (int dimension)
{
    int i, j, **game;

    // Alocam jocul efectiv.
    game = calloc (dimension, sizeof (int *));
    if (game == NULL){
        return NULL;
    }

    for (i = 0; i < dimension; i++){
        game [i] = calloc (dimension, sizeof (int));
        if (game [i] == NULL){
            return NULL;
        }
    }

    for (i = 0; i < dimension; i++)
        for (j = 0; j < dimension; j++)
            game [i][j] = 0;

    return game;
}

void CleanGame (int **game, int dimension)
{
    int i;

    for (i = 0; i < dimension; i++)
        free (game [i]);
    free (game);
}

int** CreateGrid (int **grid, int dimension)
{
    int i, j, size;

    if (dimension == 3)
        size = 22;
    else
        size = 36;

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            if (i % 7 == 0)
                grid [i][j] = 1;
            else if (j % 7 == 0)
                grid [i][j] = 2;

    return grid;
}

int** WriteX (int **game, int **grid, int dimension)
{
    int i, j, row, col;

    for (i = 0; i < dimension; i++){
        for (j = 0; j < dimension; j++){
            if (game [i][j] == 1){
                row = i * 7 + 2;
                col = j * 7 + 2;

                grid [row][col] = 4;
                grid [row++][col + 3] = 3;
                grid [row][col + 1] = 4;
                grid [row++][col + 2] = 3;
                grid [row][col + 1] = 3;
                grid [row++][col + 2] = 4;
                grid [row][col] = 3;
                grid [row][col + 3] = 4;
            }
        }
    }

    return grid;
}

int** Write0 (int **game, int **grid, int dimension)
{
    int i, j, row, col;

    for (i = 0; i < dimension; i++){
        for (j = 0; j < dimension; j++){
            if (game [i][j] == -1){
                row = i * 7 + 2;
                col = j * 7 + 2;

                grid [row][col + 1] = 5;
                grid [row++][col + 2] = 5;
                grid [row][col] = 5;
                grid [row++][col + 3] = 5;
                grid [row][col] = 5;
                grid [row++][col + 3] = 5;
                grid [row][col + 1] = 5;
                grid [row][col + 2] = 5;
            }
        }
    }

    return grid;
}

void PrintGrid (int **game, int **grid, int dimension)
{
    int i, j, size;

    if (dimension == 3)
        size = 22;
    else
        size = 36;

    WriteX (game, grid, dimension);
    Write0 (game, grid, dimension);

    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++)
            if (grid [i][j] == 0)
                printf (" ");
            else if (grid [i][j] == 1)
                printf ("=");
            else if (grid [i][j] == 2)
                printf ("|");
            else if (grid [i][j] == 3)
                printf ("/");
            else if (grid [i][j] == 4)
                printf ("\\");
            else if (grid [i][j] == 5)
                printf ("*");
        printf ("\n");
    }

    printf ("\n\n\n");
}

/* Functia verifica daca s-a terminat jocul de X si 0. Returneaza 1 daca X a
 * castigat, 2 daca 0 a castigat si 0 daca a fost egal. Daca jocul nu s-a
 * incheiat, atunci returneaza -1.
 */
int VerifyEnd3x3 (int **game)
{
    int i, j, a, b;

    // Verificam daca s-a castigat pe linie.
    for (i = 0; i < 3; i++){
        a = game [i][0];
        if (a == 0)
            continue;
        
        b = game [i][1];
        if (b == 0 || a != b)
            continue;
        
        b = game [i][2];
        if (b == 0 || a != b)
            continue;

        if (a == 1)
            return 1;
        else
            return 2;
    }

    // Verificam daca s-a castigat pe coloana.
    for (j = 0; j < 3; j++){
        a = game [0][j];
        if (a == 0)
            continue;
        
        b = game [1][j];
        if (b == 0 || a != b)
            continue;
        
        b = game [2][j];
        if (b == 0 || a != b)
            continue;

        if (a == 1)
            return 1;
        else
            return 2;
    }

    // Verificam daca s-a castigat pe diagonala principala.
    if (game [0][0] == game [1][1] && game [0][0] == game [2][2] && game[0][0] != 0){
        if (game [0][0] == 1)
            return 1;
        else
            return 2;
    }

    // Verificam daca s-a castigat pe diagonala secundara.
    if (game [0][2] == game [1][1] && game [0][2] == game [2][0] && game [0][2] != 0){
        if (game [0][2] == 1)
            return 1;
        else
            return 2;
    }

    /* Daca se ajunge pana aici inseamna ca nu s-a castigat, asa ca daca toata
     * matricea este formata din 1 si -1 inseamna ca este egalitate.
     */
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (game [i][j] == 0)
                return -1;
    
    return 0;
}

// Functia verifica daca toate elementele din sir sunt egale si diferite de 0.
int VerifyArray (int *array)
{
    int i, j;

    for (i = 0; i < 3; i++){
        for (j = i + 1; j < 4; j++)
            if (array [i] != array [j])
                return 0;
    }

    if (array [0] == 0)
        return 0;
    else if (array [0] == 1)
        return 1;
    else
        return 2;
}

int VerifyEnd5x5 (int **game)
{
    int i, j, k, *aux, end;

    aux = calloc (4, sizeof (int));
    if (aux == NULL){
        printf ("Probleme la alocare.\n");
        return -1;
    }

    // Verificam daca s-a castigat pe linie.
    for (i = 0; i < 5; i ++){
        for (j = 0; j < 2; j++){
            k = 0;
            aux [k++] = game [i][j];
            aux [k++] = game [i][j + 1];
            aux [k++] = game [i][j + 2];
            aux [k] = game [i][j + 3];
            end = VerifyArray (aux);

            if (end != 0)
                return end;
        }
    }

    // Verificam daca s-a castigat pe coloana.
    for (j = 0; j < 5; j++){
        for (i = 0; i < 2; i++){
            k = 0;
            aux [k++] = game [i][j];
            aux [k++] = game [i + 1][j];
            aux [k++] = game [i + 2][j];
            aux [k] = game [i + 3][j];
            end = VerifyArray (aux);

            if (end != 0)
                return end;
        }
    }

    // Verificam daca s-a castigat pe o diagonala paralela cu cea principala.
    for (i = 0; i < 2; i++){
        k = 0;
        for (j = 0; j < 4; j++){
            aux [k++] = game [i + j][i + j];
        }
        end = VerifyArray (aux);

        if (end != 0)
            return end;
    }

    k = 0;
    for (i = 0; i < 4; i++){
        aux [k++] = game [i][i + 1];
    }
    end = VerifyArray (aux);

    if (end != 0)
        return end;

    k = 0;
    for (i = 0; i < 4; i++){
        aux [k++] = game [i + 1][i];
    }
    end = VerifyArray (aux);

    if (end != 0)
        return end;

    // Verificam daca s-a castigat pe o diagonala paralela cu cea secundara.
    for (i = 0; i < 2; i++){
        k = 0;
        for (j = 0; j < 4; j++){
            aux [k++] = game [i + j][4 - i -j];
        }
        end = VerifyArray (aux);

        if (end != 0)
            return end;
    }

    k = 0;
    for (i = 0; i < 4; i++){
        aux [k++] = game [i][3 - i];
    }
    end = VerifyArray (aux);

    if (end != 0)
        return end;

    k = 0;
    for (i = 1; i < 5; i++){
        aux [k++] = game [i][5 - i];
    }
    end = VerifyArray (aux);

    if (end != 0)
        return end;

    // Daca nu s-a castigat inseamna ca este egal sau nu s-a terminat jocul.
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            if (game [i][j] == 0)
                return -1;

    return 0;
}

int VerifyEnd (int **game, int dimension)
{
    if (dimension == 3)
        return VerifyEnd3x3 (game);
    return VerifyEnd5x5 (game);
}

// player = 1 daca este X-ul sau 0, in caz contrar.
int minimax (int **game, int depth, int player, int dimension)
{
    int score, i, j, move, best_move, best_score;

    if (VerifyEnd (game, dimension) == 1)
        return depth + 1;
    else if (VerifyEnd (game, dimension) == 2)
        return -(depth + 1);
    else if (VerifyEnd (game, dimension) == 0)
        return 0;

    if (depth == 0)
        return 0;

    if (player == 1)
        best_score = -10;
    else
        best_score = 10;

    for (i = 0; i < dimension; i++){
        for (j = 0; j < dimension; j++){
            if (game [i][j] == 0){
                move = dimension * i + j + 1;
                if (player == 1){
                    game [i][j] = 1;
                    score = minimax (game, depth - 1, 0, dimension);

                    if (score > best_score){
                        best_score = score;
                        best_move = move;
                    }
                }
                else{
                    game [i][j] = -1;
                    score = minimax (game, depth - 1, 1, dimension);

                    if (score < best_score){
                        best_score = score;
                        best_move = move;
                    }
                }

                game [i][j] = 0;
            }
        }
    }

    return best_score;
}

/* Functia este folosita in caz ca AI-ul joaca cu 0 si trebuie sa opreasca
 * jucatorul sa faca 3 la rand. Stim, deci ca jucatorul este cu X-ul si
 * verificam daca jucatorul a ales sa joace 2 X-uri unul langa altul, caz in
 * care il oprim din a-l pune pe al 3-lea. Functia este folosita doar in cazul
 * 5x5.
 */
int Stop3 (int **game)
{
    int i, j, move = -1, row = -1, col;

    for (i = 0; i < 5 && row == -1; i++){
        for (j = 0; j < 5; j++){
            if (game [i][j] == 1){
                row = i;
                col = j;
                break;
            }
        }
    }

    if (game [row][col + 1] == 1){
        if (col + 2 < 5)
            move = row * 5 + (col + 2) + 1;
    }
    else if (game [row + 1][col - 1] == 1){
        if (row + 2 < 5 && col - 2 >= 0)
            move = (row + 2) * 5 + (col - 2) + 1;
    }
    else if (game [row + 1][col] == 1){
        if (row + 2 < 5)
            move = (row + 2) * 5 + col + 1;
    }
    else if (game [row + 1][col + 1] == 1){
        if (row + 2 < 5 && col + 2 < 5)
            move = (row + 2) * 5 + (col + 2) + 1;
    }

    return move;
}

/* Functia incearca sa pozitioneze relativ bine primele 3 mutari, dupa care
 * se trece la minimax care vede cateva mutari in fata. Problema minimax-ului
 * este la inceput deoarece nu poate vedea decat foarte putine mutari inainte.
 * Aceasta functia returneaza mutarea calculatorului. Acesta muta la inceput
 * random in interiorul patratului de 3x3 din centru. Apoi muta ca sa faca
 * 2 una langa alta, daca adversarul nu are posibilitatea de a face el deja 3
 * una langa alta, caz in care il opreste.
 */
int start (int **game, int player, int no_used_spaces_game)
{
    // player = 1 inseamna ca AI-ul joaca cu X.
    int i, j, k = 0, move = 0, *free_spaces, no_free_spaces = 0, random;
    int used_spaces [9], number,  row = 0, col = 0, center [3][3];

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            if (game [i + 1][j + 1] == 0)
                used_spaces [k++] = 0;
            else
                used_spaces [k++] = 1;
        }
    }

    // La prima mutare pune undeva in centru la intamplare.
    if (no_used_spaces_game <= 1){
        for (i = 0; i < 9; i++)
            if (used_spaces [i] == 0)
                no_free_spaces++;
    }
    else{
        // number ne indica ce valoare trebuie introdusa pe tabla de joc.
        if (player == 1)
            number = 1;
        else
            number = -1;

        for (i = 0; i < 3; i++){
            for (j = 0; j < 3; j++){
                if (game [i + 1][j + 1] == number){
                    row = i;
                    col = j;
                    break;
                }
            }
        }

        move = row * 3 + col;

        for (i = 0; i < 3; i++){
            for (j = 0; j < 3; j++){
                center [i][j] = game [i + 1][j + 1];
            }
        }

        for (i = -1; i < 2; i++){
            for (j = -1; j < 2; j++){
                if ((row + i >= 0 && row + i <= 2) && (col + j >= 0 && col + j <= 2)){
                    if (center [row + i][col + j] == 0)
                        no_free_spaces++;
                }
            }
        }
    }

    free_spaces = calloc (no_free_spaces, sizeof (int));
    if (free_spaces == NULL){
        printf ("Probleme la alocare.\n");
        return -1;
    }

    k = 0;
    if (no_used_spaces_game <= 1){
        for (i = 0; i < 9; i++)
            if (used_spaces [i] == 0)
                free_spaces [k++] = i;
    }
    else{
        for (i = -1; i < 2; i++){
            for (j = -1; j < 2; j++){
                if ((row + i >= 0 && row + i <= 2) && (col + j >= 0 && col + j <= 2)){
                    if (center [row + i][col + j] == 0){
                        move = (row + i) * 3 + col + j;
                        free_spaces [k++] = move;
                    }
                }
            }
        }
    }

    srand (time (NULL));
    random = rand () % no_free_spaces;

    move = free_spaces [random];
    i = move / 3 + 1;
    j = move % 3 + 1;
    move = 5 * i + j + 1;

    free (free_spaces);

    return move;
}

void TwoPlayerGame (int **game, int **grid, int dimension)
{
    int x, i = 0, end = -1;

    while (end == -1){
        printf ("Numar: ");
        scanf ("%d", &x);
        x--;

        if (x > dimension * dimension){
            printf ("Casuta dorita nu exista pe tabla, incercati un numar mai"
                    " mic.\n");
            continue;
        }

        if (game [x / dimension][x % dimension] != 0){
            printf ("Casuta dorita este deja ocupata, incercati din nou.\n");
            continue;
        }

        i++;
        if (i % 2 == 1)
            game [x / dimension][x % dimension] = 1;
        else
            game [x / dimension][x % dimension] = -1;

        end = VerifyEnd (game, dimension);
        PrintGrid (game, grid, dimension);
    }

    if (end == 0)
        printf ("Partida s-a terminat la egalitate.\n");
    else if (end == 1)
        printf ("X-ul a castigat.\n");
    else
        printf ("0-ul a castigat.\n");
}

void OnePlayerGame (int **game, int **grid, int dimension)
{
    int x, i, end = -1, j, score, best_score, move = 0, first, player, ok;
    int depth = 9, no_of_used_spaces = 20;

    srand (time (NULL));
    first = rand () % 2;

    if (first == 0){
        printf ("Calculatorul incepe.\n");
        player = 1;
    }
    else{
        printf ("Tu incepi.\n");
        player = 0;
    }

    while (end == -1){
        if (first % 2 == 1){
            printf ("Numarul: ");
            scanf ("%d", &x);
            x--;

            if (x > dimension * dimension){
                printf ("Casuta dorita nu exista pe tabla, incercati un numar"
                        " mai mic.\n");
                continue;
            }

            if (game [x / dimension][x % dimension] != 0){
                printf ("Casuta dorita este deja ocupata, incercati din nou.\n");
                continue;
            }

            if (player == 0)
                game [x / dimension][x % dimension] = 1;
            else
                game [x / dimension][x % dimension] = -1;
        }
        else{
            ok = 0;
            if (dimension == 5){
                no_of_used_spaces = 0;
                for (i = 0; i < dimension; i++){
                    for (j = 0; j < dimension; j++){
                        if (game [i][j] != 0)
                            no_of_used_spaces++;
                    }
                }
            }

            if (no_of_used_spaces < 2){
                move = start (game, player, no_of_used_spaces);
                ok = 1;
            }
            else if (no_of_used_spaces < 4){
                ok = 1;
                if (player == 0){
                    move = Stop3 (game);
                    if (move == -1){
                        move = start (game, player, no_of_used_spaces);
                    }
                }
                else
                    move = start (game, player, no_of_used_spaces);
            }
            else if (no_of_used_spaces < 5)
                depth = 3;
            else if (no_of_used_spaces < 7)
                depth = 4;
            else if (no_of_used_spaces < 11)
                depth = 5;
            else if (no_of_used_spaces < 14)
                depth = 6;
            else if (no_of_used_spaces <= 15)
                depth = 7;
            else if (no_of_used_spaces == 16)
                depth = 8;
            else
                depth = 9;

            if (player == 0)
                best_score = 10;
            else
                best_score = -10;

            for (i = 0; i < dimension && ok == 0; i++){
                for (j = 0; j < dimension; j++){
                    if (game [i][j] == 0){

                        if (player == 0){ // Daca AI-ul este cu 0.
                            game [i][j] = -1;
                            score = minimax (game, depth, 1, dimension);

                            if (best_score > score){
                                move = dimension * i + j + 1;
                                best_score = score;
                            }
                        }
                        else{
                            game [i][j] = 1;
                            score = minimax (game, depth, 0, dimension);

                            if (best_score < score){
                                move = dimension * i + j + 1;
                                best_score = score;
                            }
                        }
                        game [i][j] = 0;
                    }
                }
            }
            move--;

            if (player == 0)
                game [move / dimension][move % dimension] = -1;
            else
                game [move / dimension][move % dimension] = 1;
        }

        end = VerifyEnd (game, dimension);
        PrintGrid (game, grid, dimension);
        first++;
    }

    if (end == 0)
        printf ("Partida s-a terminat la egalitate.\n");
    else if (end == 1)
        printf ("X-ul a castigat.\n");
    else
        printf ("0-ul a castigat.\n");
}

int OnePlayerGameRandom (int **game, int **grid, int dimension)
{
    int first, i = 0, j, k, x, cpu_move, end = -1, no_free_spaces;
    int *used_spaces, *free_spaces, *aux;

    // Daca first e 1, atunci jucatorul va avea X-ul, altfel va avea 0-ul.
    srand (time (NULL));
    first = rand () % 2;

    if (first == 0)
        printf ("Calculatorul incepe.\n");
    else
        printf ("Tu incepi.\n");
    
    free_spaces = calloc (dimension * dimension + 1, sizeof (int));
    if (free_spaces == NULL){
        printf ("Probleme la alocare.\n");
        return -1;
    }
    free_spaces [0] = dimension * dimension;

    used_spaces = calloc (dimension * dimension, sizeof (int));
    if (used_spaces == NULL){
        printf ("Probleme la alocare.\n");
        return -1;
    }

    for (j = 0; j < dimension * dimension; j++)
        used_spaces [j] = 0;

    for (j = 1; j < dimension * dimension + 1; j++)
        free_spaces [j] = j;

    while (end == -1){
        if (first % 2 == 0){
            cpu_move = rand () % free_spaces [0] + 1;
            x = free_spaces [cpu_move];
        }
        else{
            printf ("Numar: ");
            scanf ("%d", &x);
        }

        x--;
        if (x > dimension * dimension){
            printf ("Casuta dorita nu exista pe tabla, incercati un numar mai"
                    " mic.\n");
            continue;
        }

        if (game [x / dimension][x % dimension] != 0){
            printf ("Casuta dorita este deja ocupata, incercati din nou.\n");
            continue;
        }

        first++; // Daca first e impar, atunci e randul jucatorului.
        used_spaces [x] = 1;
        i++;

        if (i % 2 == 1)
            game [x / dimension][x % dimension] = 1;
        else
            game [x / dimension][x % dimension] = -1;

        no_free_spaces = free_spaces[0];
        free (free_spaces);

        // Retinem doar variantele din care poate alege calculatorul.
        aux = calloc (no_free_spaces, sizeof (int));
        if (aux != NULL){
            free_spaces = aux;
        }
        else{
            printf ("Probleme la alocare.\n");
            return -1;
        }

        free_spaces [0] = --no_free_spaces;
        k = 1;
        for (j = 0; j < dimension * dimension; j++){
            if (used_spaces [j] == 0){
                free_spaces [k] = j + 1;
                k++;
            }
        }

        end = VerifyEnd (game, dimension);
        PrintGrid (game, grid, dimension);     
    }
    
    if (end == 0)
        printf ("Partida s-a terminat la egalitate.\n");
    else if (end == 1)
        printf ("X-ul a castigat.\n");
    else
        printf ("0-ul a castigat.\n");

    free (free_spaces);
    free (used_spaces);

    return 0;
}

int main()
{
    int i, j, out, mode, dimension;
    int **grid, **game;

    printf ("Alege tipul jocului pe care vreti sa il jucati:\n");
    printf ("(1) 3x3 (Clasic)\n");
    printf ("(2) 5x5\n");
    printf ("Dimensiunea dorita este: ");
    scanf ("%d", &mode);

    while (mode != 1 && mode != 2){
        printf ("Nu ati introdus o optiune valida. Scrieti 1 sau 2 in functie"
                " de jocul pe care il doriti.\n");
        printf ("Dimensiunea dorita este: ");
        scanf ("%d", &mode);   
    }

    if (mode == 1)
        dimension = 3;
    else
        dimension = 5;

    grid = AllocateGrid (dimension);
    if (grid == NULL){
        printf ("Probleme la alocare.\n");
        return 1;
    }

    game = AllocateGame (dimension);
    if (game == NULL){
        printf ("Probleme la alocare.\n");
        return 1;
    }

    printf ("Alege numarul modului in care vreti sa jucati jocul:\n");
    printf ("(1) Player VS Player\n");
    printf ("(2) Player VS Computer EASY\n");
   // if (dimension == 3)
        printf ("(3) Player VS Computer HARD\n");
    printf ("Modul dorit este: ");

    scanf ("%d", &mode);
    while ((mode != 1 && mode != 2) && (mode != 1 && mode != 3)){
        printf ("Nu ati introdus o optiune valida. Scrieti 1, 2 sau 3 in"
                " functie de modul pe care il doriti.\n");
        printf ("Modul dorit este: ");
        scanf ("%d", &mode);
    }

    // Partea in care jocul este jucat propriu-zis.
    grid = CreateGrid (grid, dimension);
    PrintGrid (game, grid, dimension);

    if (mode == 1)
        TwoPlayerGame (game, grid, dimension);
    else if (mode == 2)
        OnePlayerGameRandom (game, grid, dimension);
    else
        OnePlayerGame (game, grid, dimension);

    // Partea de dezalocare totala a memoriei.
    CleanGrid (grid, dimension);
    CleanGame (game, dimension);

    return 0;
}