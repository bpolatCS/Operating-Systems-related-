#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


int size;
char **board;
int row = 0;
int col=0;
char a = 'X';
int turn =0;
char whoWon = '-';


pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

bool isItWin(int x, int y, char sign)
{
    bool win = false;

    if (win != true)
    {
        win = true;
        for (int i = 0; i < size; i++) //horizantal check
        {
            if (board[x][i] != sign)
            {
                win = false;
                break;
            }
                
        }
    }
    if (win != true)
    {
        win = true;
        for (int i = 0; i < size; i++) //vertical check
        {
            if (board[i][y] != sign)
            {
                win = false;
                break;
            }
             
        }
    }
    
    if (win != true)
    {
        win = true;
        for (int i = 0; i < size; i++)
        {
            if (board[size-i-1][i] != sign) //diagonal-1 check
            {
                win = false;
                break;
            }       
        }
    }
     if (win != true)
    {
        win = true;
        for (int i = 0; i < size; i++)  //diagonal -2 check
        {
            if (board[i][i] != sign)
            {
                win = false;
                break;
            }               
        }
    }
    if (win)
    {
        whoWon = sign;
    }
    
    return win;

}


void *function(void *argum) 
{
    
    char* args = (char *) argum;
    char mychar = *args;
    bool put = true;
    while (put)
    {
        pthread_mutex_lock(&lock);
        if (mychar != a) // preventing entrance order. one-by-one. respect the player turn. global a is initally x so x is lock first
        {
             pthread_mutex_unlock(&lock);
             continue;
        }
        if (mychar == 'X') //after one thread join global a var needs to be updated to other player. So when other players turn arrive it can enter it.
        {
            a = 'O';
        }
        else
        {
            a = 'X';
        }    
        
        if (isItWin(row,col,a) || (turn == size*size)) // if the game already finished, other thread also need to brek from the loop.
        { 
            put = false;
        }

        if(!put)
        {
            
            pthread_mutex_unlock(&lock);
            break;
        }
        while (1)
        {
           
            row = rand() % size;  //getting new coordiantes
            col = rand() % size;
           
            if (board[row][col] == '-')
            {
                board[row][col] = mychar;  //putting the coordinate players sign (X or O)
                turn++;

                printf("Player %c played on: (%d,%d)\n",board[row][col],row,col);
                break;
            } 
        }
        if (isItWin(row,col,mychar) || (turn == size*size))  //after each turn check if the game come to an end.
        {
            put = false;
        }
        
        pthread_mutex_unlock(&lock);
    }
    
}


int main(int argc, char* argv[])
{
    pthread_t playerX, playerO;
    char x='X';
    char o='O';

    srand(time(NULL));
    clock_t start = clock();
  
    size = atoi(argv[1]);
    printf("Board size: %d X %d\n",size,size);
 
    int r= size;
    int c= size;

    board = (char**)malloc(r * sizeof(char*));  //creation of board
    for (int i = 0; i < r; i++)
        board[i] = (char*)malloc(c * sizeof(char));

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)  //initalization each coordiante '-'
        {    
            board[i][j] = '-';
        
        }

    pthread_create(&playerX, NULL, function, &x);
    pthread_create(&playerO, NULL, function, &o);

     // join waits for the threads to finish
    pthread_join(playerX, NULL);
    pthread_join(playerO, NULL);

    printf("Game end \n");
    printf("%c\n", whoWon);
    
    if (whoWon == '-')
    {
        printf("It is a tie\n");
    }
    else
        printf("Winner is %c\n", whoWon);    
    
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("[");    
            printf("%c",board[i][j]);
            printf("] ");
        }
        printf("\n");
    }

	return 0;
}