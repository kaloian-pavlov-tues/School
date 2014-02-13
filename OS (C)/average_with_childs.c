#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>

#define N 8
#define ARRAY_SIZE (100000 * N)

int main()
{
        int a[ARRAY_SIZE];
        int i;
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            a[i] = i % 255;
        }
        int count_start = 1;
        int parent_sum = 0;
        int parent_average;
        int count = 1 ;
        int status;
        int child_count;
        int child_sum = 0;
        int child_average;
        int sum = 0;
        for(i = 0; i < 800000; i++)
        {
                sum+= i %255;
        }
        printf("%d\n", sum/800000);
        pid_t next_child;
        for(child_count = 0; child_count < N ; child_count++)
        {
                next_child = fork();
                if(next_child < 0)
                {
                        perror("Fork: ");
                        exit(EXIT_FAILURE);
                }
                else if(next_child == 0)
                {
                        printf("Child with parent pid %d\n",getppid());
                        child_average = 0;
                        child_sum = 0;
                        for(count = (count_start-1) * ARRAY_SIZE/N; count < count_start * ARRAY_SIZE/N; count++)
                        {
                                child_sum += a[count];
                        }
                        child_average = child_sum/(ARRAY_SIZE/N);
                        exit(child_average);
                }
                else
                {
                        waitpid(next_child,&status,0);
                        printf("%d\n", WEXITSTATUS(status));
                        parent_sum += WEXITSTATUS(status);
                        count_start++;
                }
        }
        parent_average = parent_sum/N;
        printf("Parent average %d\n", parent_average);
        return 0;
}
