# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>

int main(void)
{
    int fd[2];
    pipe(fd);
    int pid;
    int pid1;
    if((pid = fork()) == 0)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("ls", "ls", NULL);
        exit(1);
    }
    // else
    //     wait(NULL);
    //     waitpid(pid, 0, 0);
    
    if((pid1 = fork()) == 0)
    {
        // dup2(fd[0], 0);
        // dup2(fd[1], 1);
        // close(fd[0]);
        // close(fd[1]);
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execlp("cat", "cat","-e" ,NULL);
        exit(1);
    }
    // else
    //     wait(0);
    //     waitpid(pid1, 0, 0);
    // if(fork() == 0)
    // {
    //     close(fd[1]);
    //     dup2(fd[0], 0);
    //     close(fd[0]);
    //     execlp("wc", "wc" ,NULL);
    //     exit(1);
    // }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, 0, 0);
    waitpid(pid, 0, 0);
    // wait(NULL);
    // wait(NULL);
}