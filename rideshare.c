#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>



int teamACount=0;
int teamBCount=0;
int LastOneIsDriver =0;

sem_t semaphoreA,semaphoreB;
pthread_barrier_t barrier;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;


void *function(void *argum) 
{
    
    char* args = (char *) argum;
    char teamName = *args;
    
    
    if (teamName == 'A')
    {
        pthread_mutex_lock(&lock);
        teamACount++;
        printf("Thread ID: %ld , Team: %c, I am currently looking for a car\n", pthread_self(), teamName);
        

        if ((teamACount ==2 && teamBCount >= 2)) 
        {
            sem_post(&semaphoreA);
            sem_post(&semaphoreB);
            sem_post(&semaphoreA);
            sem_post(&semaphoreB);
            teamACount=teamACount-2;
            teamBCount=teamBCount-2;
          
        }
        
        else if (teamBCount ==4)
        {
            sem_post(&semaphoreB);
            sem_post(&semaphoreB);
            sem_post(&semaphoreB);
            sem_post(&semaphoreB);
            teamBCount=teamBCount-4;
          
        }

        else if (teamACount ==4)
        {
            sem_post(&semaphoreA);
            sem_post(&semaphoreA);
            sem_post(&semaphoreA);
            sem_post(&semaphoreA);
            teamACount=teamACount-4;
          
        }
        
        pthread_mutex_unlock(&lock);
        sem_wait(&semaphoreA);
    }

    else if (teamName == 'B')
    {
        pthread_mutex_lock(&lock);
        teamBCount++;
        printf("Thread ID: %ld , Team: %c, I am currently looking for a car\n", pthread_self(), teamName);
        
     
        if ((teamACount >=2 && teamBCount == 2)) 
        {
            sem_post(&semaphoreA);
            sem_post(&semaphoreB);
            sem_post(&semaphoreA);
            sem_post(&semaphoreB);
            teamACount=teamACount-2;
            teamBCount=teamBCount-2;
          
        }
        else if (teamACount ==4)
        {
            sem_post(&semaphoreA);
            sem_post(&semaphoreA);
            sem_post(&semaphoreA);
            sem_post(&semaphoreA);
            teamACount=teamACount-4;      
          
        }
        else if (teamBCount ==4)
        {
            sem_post(&semaphoreB);
            sem_post(&semaphoreB);
            sem_post(&semaphoreB);
            sem_post(&semaphoreB);
            teamBCount=teamBCount-4;
            
          
        }
        pthread_mutex_unlock(&lock);
        sem_wait(&semaphoreB);        
    }

    pthread_barrier_wait(&barrier);

   
    if (teamName == 'A')
    {
        pthread_mutex_lock(&lock);
        printf("Thread ID: %ld , Team: %c, I have found a spot on the car\n", pthread_self(), teamName);
       
        LastOneIsDriver++;
        
        if (LastOneIsDriver % 4==0 && LastOneIsDriver != 0)
        {
            
            printf("Thread ID: %ld , Team: %c, I am the captain and driving the car\n", pthread_self(), teamName);
        }
        pthread_mutex_unlock(&lock);
        
    }
    else if(teamName == 'B')
    {
        pthread_mutex_lock(&lock);
        printf("Thread ID: %ld , Team: %c, I have found a spot on the car\n", pthread_self(), teamName);
      
        LastOneIsDriver++;
        
        if ((LastOneIsDriver % 4==0 && LastOneIsDriver != 0)) 
        {
            
            printf("Thread ID: %ld , Team: %c, I am the captain and driving the car\n", pthread_self(), teamName);
        }
        pthread_mutex_unlock(&lock);
    
    }
    

}



int main(int argc, int* argv[])
{
    pthread_t *threadA, *threadB;
    int sizeofteamA = atoi(argv[1]);
    int sizeofteamB = atoi(argv[2]);
    char a='A';
    char b='B';

    pthread_barrier_init(&barrier, NULL, 4);
    sem_init(&semaphoreA, 0,0);
    sem_init(&semaphoreB, 0,0);

    if (sizeofteamA % 2 == 0 && sizeofteamB % 2 == 0 && (sizeofteamA+sizeofteamB)%4 == 0)
    {
        threadA=(pthread_t *)malloc(sizeofteamA* sizeof(pthread_t ));
        for(int i=0;i<sizeofteamA;i++)
        {
            pthread_create(&threadA[i],NULL,function,&a); //Default Attributes
        }
        threadB=(pthread_t *)malloc(sizeofteamB * sizeof(pthread_t ));
        for( int i=0;i<sizeofteamB;i++)
        {
            pthread_create(&threadB[i],NULL,function,&b); //Default Attributes
        }

         for(int i=0;i<sizeofteamA;i++)
        {
            pthread_join(threadA[i],NULL);
        }
        for(int i=0;i<sizeofteamB;i++)
        {
            pthread_join(threadB[i],NULL);
        }
 
    }
    else{
        printf("The main terminates\n");
    }

   

    printf("The main terminates\n");
    
    sem_destry(&semaphoreA);
    sem_destry(&semaphoreB);
    pthread_barrier_destroy(&barrier);
    

	return 0;
}
