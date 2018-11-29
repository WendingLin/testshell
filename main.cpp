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
#include <vector>
#include <sstream>

using namespace std;


vector<string> stringSplit(const string &str, const char symbol){
    vector<string> container;
    stringstream ss(str);
    while(ss.good()){
        string buf;
        getline(ss, buf, symbol);
        container.push_back(buf);
    }
    return container;
}

void loadenv(){
    char* env = getenv("PATH");
    string env_str(env);
    vector<string> env_list = stringSplit(env_str, ':');
}

void basic_shell(std::string input){
    pid_t fpid;
    int status;
    fpid=fork();
    if (fpid < 0)
        printf("error in fork!");
    else if (fpid == 0) {
        char *argv[] = { "-l", NULL};
        char *envp[] = {NULL};
        execve("ls", argv, envp);
    } else {
        //kill(fpid, SIGABRT);
        waitpid(-1, &status, 0);
        int exit_status = WIFEXITED(status);
        int signal_status = WIFSIGNALED(status);
        if(signal_status)
            cout<<"Program was killed by signal "<<signal_status<<endl;
        if(exit_status)
            cout<<"Program exited with status "<<exit_status<<endl;
    }
}


void testshell(){
        while(true){
        std::cout<<"myShell$ ";
        char buf[1024];
        std::cin.getline(buf, 1024);
        std::string input(buf);
        basic_shell(input);
    }
}

int main() {

    loadenv();
    return 0;
}