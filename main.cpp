/*
 *  fork_test.c
 *  version 1
 *  Created on: 2010-5-29
 *      Author: wangth
 */
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

/*
    pid_t child;
    test("USER");

    int i=0;

    child = fork();
    if (child < 0) {

        printf("create failed!\n");

        exit(1);

    } else if (0 == child) {

        printf("this is the child process pid= %d\n", getpid());
        for (i = 0; i < 5; i++) {

            printf("this is the child process print %d !\n", i + 1);

        }
        printf("the child end\n");

    } else {

        printf("this is the father process, ppid=%d\n", getppid());

        printf("father wait the child end\n");

        wait(&child);

        printf("father end\n");

    }

}
*/



void basic_shell(std::string input){
    pid_t fpid;
    int status;
    fpid=fork();
    if (fpid < 0)
        printf("error in fork!");
    else if (fpid == 0) {
        char *argv[] = { "-l", NULL};
        char *envp[] = {NULL};
        execve("/bin/ls", argv, envp);
    } else {
        waitpid(fpid, &status, 0);
        int exit_status = WIFEXITED(status);
        int signal_status = WIFSIGNALED(status);
        if(signal_status)
            cout<<"Program was killed by signal "<<signal_status<<endl;
        if(exit_status)
            cout<<"Program exited with status "<<exit_status<<endl;
    }
}



int main() {
    while(true){
        std::cout<<"myShell$ ";
        char buf[1024];
        std::cin.getline(buf, 1024);
        std::string input(buf);
        basic_shell(input);
    }

    return 0;
}