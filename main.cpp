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
#include <algorithm>

using namespace std;


/*
 * Concat file path
 */
string pathConcat(string forward, string backward) {
    return forward + '/' + backward;
}

/*
 * Return if command is raw(just command like ls)
 * return true if is
 * false not
 */
bool ifRawCommand(const string &command) {
    if (command.size() > 1 && command.substr(0, 1) == "/")
        return false;
    else if (command.size() > 2 && command.substr(0, 2) == "./")
        return false;
    else if (command.size() > 3 && command.substr(0, 3) == "../")
        return false;
    else
        return true;
}

/* Return -1 if cannot exec
 * Return envnum if can exec
 * */
int ifExecCommand(vector<string> env_list, const string &command) {

    for (int i = 0; i < env_list.size(); i++) {
        struct stat sb;
        string commandPath = pathConcat(env_list[i], command);
        if (stat(commandPath.c_str(), &sb) == 0 && sb.st_mode & S_IXUSR)
            return i;
        else
            continue;
    }
    return -1;
}

/*
 * Return 1 if can exec
 * Return -1 if not
 */
int ifExecCommand(const string &command) {
    struct stat sb;
    if (stat(command.c_str(), &sb) == 0 && sb.st_mode & S_IXUSR)
        return 1;
    else
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

/*
 * Should be added into constructor
 */
void loadEnv() {
    char *env = getenv("PATH");
    string env_str(env);
    vector<string> env_list = stringSplit(env_str, ':');
    string command = "/bin/ls";
    int commandCheck = ifRawCommand(command) ? ifExecCommand(env_list, command) : ifExecCommand(command);
    if(commandCheck==-1)
        cout<<"Command "<<command<<" not found"<<endl;
    else
        cout<<pathConcat(env_list[commandCheck], command); //execute
    //int where = ifExecCommand(env_list, "git"); //whereis command is good
    return;
}

void basicShell(std::string input) {
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

void testShell() {
    while (true) {
        std::cout << "myShell$ ";
        char buf[1024];
        std::cin.getline(buf, 1024);
        std::string input(buf);
        basicShell(input);
    }
}

vector<string> spaceHandle(const string& inputCommand){
    vector<string> escape_pos;
    /*string::const_iterator it = inputCommand.begin();
    string::const_iterator it_last = it;
    while(it!=inputCommand.end()){
        if(*it=='\\') {
            if (it + 1 != inputCommand.end())
                if (*(it+1) == ' ')
                    formatInput.erase(it++);
        }
        else if(*it==' ' && *(++it)!=' ') {

        } else{
            it++;
        }
    }*/
    std::istringstream iss(inputCommand);
    for(std::string s; iss >> s; )
        escape_pos.push_back(s);
    return escape_pos;
}

int main() {
    char es[2014];
    std::cin.getline(es, 1024);
    string a(es);
    vector<string> escape_pos = spaceHandle(a);;
    //string real = spaceHandle(a);
    return 0;
}