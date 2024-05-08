/*
 * File: prompt.cpp
 * This program is used to generate a bash prompt with rainbow colors.
 * It can also be used to generate the necessary environment variables
 *
 */

#include <string>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <exception>

#include "rainbow.hpp"

typedef std::string str;

int randint(int max) {
    srand(std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    return rand() % max;
}

str mouthlist = ")3>]DPO";
str eyelist = ";:=";
str emote(){
    return eyelist.substr(randint(eyelist.length()-1),1) + mouthlist.substr(randint(mouthlist.length()-1),1);
}

str docmd(std::string inputted) {
    FILE* file = popen(inputted.c_str(), "r");
    char buff[1024];
    int n = fread(buff, 1, sizeof(buff), file);
    pclose(file);
    str r = std::string(buff, n);
    if (r.back() == '\n') r.pop_back();
    return r;
}

str envorcmd(str env, str cmd) {
    str ret = "";
    try { ret = std::string(getenv(env.c_str())); }
    catch (std::exception e) { ret = docmd(cmd); }
    return ret;
}

str color(str s, str fg="") {
    if (!fg.empty()) fg = "\\[\\e[38;5;" + fg + "m\\]";
    return fg + s + "\\[\\e[0m\\]";
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        str arg = argv[1];
        if (arg == "--help" || arg == "help" || arg == "-h" || arg == "-?") {
            // Print help message
            return 0;
        }
    }

    str user = envorcmd("USER", "whoami");
    while (user.length()%4 != 0) user += " ";
    int size = user.length()/4;
    str ip = docmd("hostname -I | awk -F '.' ' { for(i=1;i<5;i++){printf(\"%.3d\", $i);}; } ' ");
    for (int i = 0; i < 4; i++) {
        std::cout << color(user.substr(i*size,size), ip.substr(i*3,3));
    }
    std::cout << std::endl;

    /*str pwd;
    try { pwd = std::string(getenv("PWD")); }
    catch (std::exception e) { pwd = ""; }
    if (user.back()=='\n')user.pop_back();
    output += " ";
    output += pwd;*/
    return 0;
}
