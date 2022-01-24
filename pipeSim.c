#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>



int main(void)
{

	int fdChild[2], fd2[2]; //file descriptors of pipes
	pid_t cpidMan, cpidGrep; //two forks for two differnet process
	printf("I’m SHELL process, with PID: %d - Main command is <man diff | grep LTYPE\n", (int) getpid());
	
	if(pipe(fdChild) < 0) //pipe is introduced
	{
		perror("pipe");
		exit(1);
	}
	if(pipe(fd2) < 0) //other pipe is introduced
	{
		perror("pipe");
		exit(1);
	}
	cpidMan = fork(); //first fork for the SHELL -> MAN 
	
	if(cpidMan < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (cpidMan == 0) //SHELL's child MAN
	{
		
		
		cpidGrep = fork(); //new fork for the new process (creating GREP)
		
		if(cpidGrep < 0)
		{
			perror("grep fork");
			exit(1);
		}
		else if (cpidGrep == 0) //MAN'S child GREP
		{
			printf("I’m GREP process, with PID: %d - Main command is: <grep LTYPE>\n", getpid());
			close(fdChild[1]);
			
			dup2(fdChild[0], STDIN_FILENO);

			close(fd2[0]);			
			char *myargsChild[3];
			myargsChild[0] = strdup("grep"); 
			myargsChild[1] = strdup("LTYPE");
			myargsChild[2] = strdup("-A 1");
			myargsChild[3] = NULL;
			
			write(fdChild[1], execvp(myargsChild[0], myargsChild), 10000);	
			
		}
		else //MAN process
		{
			printf("I’m MAN process, with PID: %d - My command is: <man diff>\n", getpid());
			close(fdChild[0]);
			
		
			dup2(fdChild[1], STDOUT_FILENO);
						
			char * myargsChild[3];
			myargsChild[0] = strdup("man"); 
			myargsChild[1] = strdup("diff"); 
			myargsChild[2] = NULL;
			execvp(myargsChild[0], myargsChild);
			
		}
	}
	else //SHELL process (our main process)
	{
		int wc = wait(NULL);	
		printf("I’m SHELL process, with PID: %d - execution is completed, you can find the results in output.txt\n", getpid());
		
		close(fdChild[1]);
		char a[10000];
		read(fdChild[0],a, 10000);	
		
		int newFd1 = open("output1.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		if(dup2(newFd1,STDOUT_FILENO < 0))
		{
			perror("dup2 failed");
			_exit(1);
		}
		
		
	}
	return 0;

}


/*

int main(void)
{

	int fdChild[2];
	pid_t cpidMan, cpidGrep;
	printf("I’m SHELL process, with PID: %d - Main command is <man find | grep -H>\n", (int) getpid());
	
	if(pipe(fdChild) < 0)
	{
		perror("pipe");
		exit(1);
	}
	
	cpidMan = fork();
	
	if(cpidMan < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (cpidMan == 0) //MAN
	{
		
		
		cpidGrep = fork();
		
		if(cpidGrep < 0)
		{
			perror("grep fork");
			exit(1);
		}
		else if (cpidGrep == 0) //GREP
		{
			printf("I’m GREP process, with PID: %d - Main command is: <grep -H>\n", getpid());
			close(fdChild[1]);
			
			int newFd = open("output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
			if(dup2(newFd,STDOUT_FILENO < 0))
			{
				perror("dup2 failed");
				_exit(1);
			}
			char nonesense[10000];
			
			read(fdChild[0], nonesense, 100000 );
			char *myargsChild[4];
			myargsChild[0] = strdup("grep");
			myargsChild[1] = strdup("string.txt"); 
			myargsChild[2] = strdup("-q");
			myargsChild[3] = NULL;
			execvp(myargsChild[0], myargsChild); 
			
			
		}
		else 
		{
			
			printf("I’m MAN process, with PID: %d - My command is: <man diff>\n", getpid());
			close(fdChild[0]);
			
			int newFd = open("string.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
			if(dup2(newFd,STDOUT_FILENO < 0))
			{
				perror("dup2 failed");
				_exit(1);
			}
			
									
			char * myargsChild[3];
			myargsChild[0] = strdup("man"); 
			myargsChild[1] = strdup("diff"); 
			myargsChild[2] = NULL;
			write(fdChild[1], execvp(myargsChild[0], myargsChild), 10000);
			
		}
	
	
	}
	else //SHELL
	{
		
		int wc = wait(NULL);	
		printf("I’m SHELL process, with PID: %d - execution is completed, you can find the results in output.txt\n", getpid());
		
		
		
		
	
	}
	
	
	return 0;

}

*/



