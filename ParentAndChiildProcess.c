#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{

int n;
pid_t pid, pid1;

	if (argc == 1) {
		fprintf(stderr,"Usage: ./hw1 <starting value>\n");
		
		return -1;
	}

	n = atoi(argv[1]); //  n is the input starting value
	if (n<1)  // error Checking for positive no
	{
		printf("Enter a Positive no\n");
		return -1;
	}	

	pid = fork();

	if (pid == 0){
			
			printf("\n %d ", n);
			while (n!=1)
			{
				if (n%2 == 0)
				{
					n = n/2;
				}
				else if (n%2 == 1)
				{
					n = 3 * (n) + 1;
				}	
			
				printf(", %d ", n);
			}
			pid1 = getpid();
			// Print Child Process
			printf("\nChild :pid = %d \n", pid);
			printf("Child :pid1 = %d \n", pid1);

		}
		else if (pid > 0)
		{
			wait(NULL);
			pid1 = getpid();
			// Print Parent Process
			printf("Parent :pid = %d \n", pid);
			printf("Parent :pid1 = %d \n", pid1);
			
		}

		else 
		{
			printf(stderr,"Fork Failed");
			return 1;
		}


	return 0;
}
