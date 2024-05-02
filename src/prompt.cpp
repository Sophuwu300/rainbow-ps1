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

const str emotes[] = {":)", ":3", ";D",":]", ":D", "xD",";3", ";)", "=)",":P", ":O", ":b"};
str emote(){
    return emotes[randint(12)];
}

str intpad(int n, int len) {
    str s = std::to_string(n);
    while (s.length() < len) s = "0" + s;
    return s;
}

int str2int(str &s) {
    int n = 0;
    while (!s.empty()) {
        if (s[0] < '0' || s[0] > '9') {
            s.erase(0, 1);
            break;
        }
        n = n * 10 + s[0] - '0';
        s.erase(0, 1);
    }
    return n;
}

str time_hhmm() {
    std::chrono::time_point now = std::chrono::system_clock::now();
    time_t in_time_t = std::chrono::system_clock::to_time_t(now);
    auto tt = std::localtime(&in_time_t);
    return intpad((int)(tt->tm_hour),2) + ":" + intpad((int)(tt->tm_min),2);
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

struct PS1 {
    str value="";
    str color(str fg="", str bg="") {
        if (!fg.empty()) fg = "\\[\\e[38;5;" + fg + "m\\]";
        if (!bg.empty()) bg = "\\[\\e[48;5;" + bg + "m\\]";
        return  fg + bg;
    }
    void add(str s, str fg="", str bg="") {
         value+= color(fg, bg) + s + "\\[\\e[0m\\]";
    }
    void print() {
        std::cout << "export PS1='" << value << "';" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    //rainbow r;
    std::cout << "export lineno=$LINENO;" << std::endl;
    int lineno = 1;
    if(getenv("lineno")!=NULL) {
        str ln = std::string(getenv("lineno"));
        lineno = str2int(ln);
    }
    PS1 ps1;
    str user = envorcmd("USER", "whoami");
    if (user == "sophuwu"){
        user="e===3===3 ==3  =3   ===3 ";
        user=user.substr((lineno%5)*5, 5);
    } else user=" "+user;
    str ip = docmd("hostname -I | awk -F '.' ' { for(i=1;i<5;i++){printf(\"%.3d\", $i);}; } ' ");
    ps1.add("$? ", ip.substr(0,3));
    str em = emote();
    ps1.add(em.substr(0,1), ip.substr(3,3));
    ps1.add(em.substr(1,1), ip.substr(6,3));
    ps1.add(user, ip.substr(9,3));
    ps1.print();
    /*str pwd;
    try { pwd = std::string(getenv("PWD")); }
    catch (std::exception e) { pwd = ""; }
    if (user.back()=='\n')user.pop_back();
    output += " ";
    output += pwd;*/
    return 0;
}
