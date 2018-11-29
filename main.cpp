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
    if (commandCheck == -1)
        cout << "Command " << command << " not found" << endl;
    else
        cout << pathConcat(env_list[commandCheck], command); //execute
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

void backSlashReplace(vector<string>& input_args){
    for(size_t i = 0; i<input_args.size();i ++){
        if (input_args[i].size()<2)
            continue;
        else{
            for(string::iterator it = input_args[i].begin(); it!=input_args[i].end();it++){
                if(it+1==input_args[i].end())
                    break;
                else{
                    if(*it=='\\' && *(it+1)==' ')
                        input_args[i].erase(it);
                }
            }
        }
    }
}

vector<string> inputHandle(const string &input_command) {
    vector<string> input_args;
    size_t start = 0;
    size_t length = 0;
    bool if_scanstr = false;
    for (size_t i = 0; i < input_command.size(); i++) {
        if (if_scanstr) {
            if (input_command[i] == ' ') {
                if (input_command[i - 1] == '\\')
                    length++;
                else {
                    input_args.push_back(input_command.substr(start, length));
                    if_scanstr = false;
                }
            } else {
                length++;
            }
        } else {
            if (input_command[i] == ' ') {
                continue;
            } else {
                start = i;
                length = 1;
                if_scanstr = true;
            }
        }
    }
    if (if_scanstr)
        input_args.push_back(input_command.substr(start, length));
    backSlashReplace(input_args);
    return input_args;
}

int main() {
    char es[2014];
    std::cin.getline(es, 1024);
    string a(es);
    vector<string> input_args = inputHandle(a);;
    //string real = spaceHandle(a);
    return 0;
}