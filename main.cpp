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
#include <sys/stat.h>

using namespace std;

/*
 * Concat file path
 */
string pathConcat(string forward, string backward){
    return forward+'/'+backward;
}

/* Return -1 if cannot exec
 * Return envnum if can exec
 * */
int ifexecCommand(vector<string> env_list, string filename) {

    for (int i = 0; i < env_list.size(); i++) {
        struct stat sb;
        string commandPath = pathConcat(env_list[i], filename);
        if (stat(commandPath.c_str(), &sb) == 0 && sb.st_mode & S_IXUSR)
            return i;
        else
            continue;
    }
    return -1;
}

vector<string> stringSplit(const string &str, const char symbol) {
    vector<string> container;
    stringstream ss(str);
    while (ss.good()) {
        string buf;
        getline(ss, buf, symbol);
        container.push_back(buf);
    }
    return container;
}


void loadenv() {
    char *env = getenv("PATH");
    string env_str(env);
    vector<string> env_list = stringSplit(env_str, ':');
    int where = ifexecCommand(env_list, "git"); //whereis command is good
    return;
}



void basic_shell(std::string input) {
    pid_t fpid;
    int status;
    fpid = fork();
    if (fpid < 0)
        printf("error in fork!");
    else if (fpid == 0) {
        char *argv[] = {"-l", NULL};
        char *envp[] = {NULL};
        execve("ls", argv, envp);
    } else {
        //kill(fpid, SIGABRT);
        waitpid(-1, &status, 0);
        int exit_status = WIFEXITED(status);
        int signal_status = WIFSIGNALED(status);
        if (signal_status)
            cout << "Program was killed by signal " << signal_status << endl;
        if (exit_status)
            cout << "Program exited with status " << exit_status << endl;
    }
}

void testshell() {
    while (true) {
        std::cout << "myShell$ ";
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