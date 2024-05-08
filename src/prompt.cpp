/*
 * File: prompt.cpp
 * This program is used to generate a bash prompt with rainbow colors.
 * It can also be used to generate the necessary environment variables
 *
 */

#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <exception>
#include <map>
#include <cmath>

#include "rainbow.hpp"

typedef std::string str;

int randint(int max) {
    srand(std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    return rand() % max;
}

#define setenv(a) std::cout << #a << "=\"" << a << "\"" << std::endl
#define exportenv(k,v) std::cout << "export " << k << "=\"" << v << "\"" << std::endl

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

int intenv(const char* env) {
    int n=0;
    const char* val = getenv(env);
    if (val == NULL) return -1;
    for (int i = 0; val[i] >= '0' && val[i] <= '9'; i++) n = n*10 + (int)val[i] - (int)'0';
    return n;
}

str time_ddd_hh_mm() {
    //strftime(buffer, 80, "%a %H:%M", now);
    time_t t = time(0);
    struct tm * now = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%a %H:%M %Z", now);
    return str(buffer);
}

typedef std::pair<str,str> paStr;
struct escape {
    paStr PS = paStr("\\[\\e[","m\\]");
    paStr XT = paStr("\033[","m");
    str output = "";
    bool debugON = false;
    str wrap(str s) { if(debugON)return XT.first+s+XT.second; return PS.first + s + PS.second; }
    str color(str color,bool background=false) {
        if (color.length()==0) return "";
        if (color.length() <= 3)color="5;"+color;
        else color="2;"+color;
        if (background) return "48;" + color;
        return  "38;" + color;
    }
    void add(str s, str fg="", str bg="", bool bold=false) {
        if (bold==1) s = wrap("1") + s;
        if (fg!="") s = wrap(color(fg)) + s;
        if (bg!="") s = wrap(color(bg,true)) + s;
        output += s + wrap("0");
    }
    void rainbow(rainbow &r, str s) {
        for (int i = 0; i < s.length(); i++) {
            add(s.substr(i,1), r.c.str());
            r.next();
        }
    }
    void set() { exportenv("PS1",output); }
};

int wave(int x) {return (int)(6+1.54*3.14159*(sin(x/(3.14159))));}

int main(int argc,char** argv) {
    escape PS1;
    for (int i=1;i<argc;i++) {
        str arg = argv[i];
        if (arg == "--help" || arg == "help" || arg == "-h" || arg == "-?") {
            std::cout << "Usage: " << argv[0] << " [view|ps]" << std::endl;
            return 0;
        }
        if (arg == "view") {
            PS1.debugON = true;
        }
    }
    int lineno = intenv("RBP_N")+1;
    rainbow r;
    r.c.set(intenv("RBP_R"), intenv("RBP_G"), intenv("RBP_B"));
    r.s = intenv("RBP_S");
    if (r.s < 1 || (r.c[0]==r.c[1] && r.c[1]==r.c[2]))
        r.init(25-randint(15));
    else r.next();
    if (!PS1.debugON) {
        for (int i = 0; i < 3; i++) exportenv("RBP_" + str("RGB").substr(i % 3, 1), r.c[i]);
        exportenv("RBP_S", r.s);
        exportenv("RBP_N", lineno);
    }
    for(int i = 0; i < abs(wave(lineno)); i++) {
        r.next();
    }

    std::vector<str> host;
    host.push_back(envorcmd("HOSTNAME","hostname"));
    host.push_back(envorcmd("USER","whoami"));
    str ip = docmd("hostname -I | awk -F '.' ' { for(i=1;i<5;i++){printf(\"%.3d\", $i);}; } ' ");
    PS1.add("$?", "190;180;30");
    PS1.add("|", "100;100;100");
    PS1.add(time_ddd_hh_mm(),ip.substr(3*((1+lineno)%4),3));
    PS1.add("|","100;100;100");
    PS1.rainbow(r, std::to_string(lineno));
    PS1.add(" ");
    PS1.rainbow(r, emote());
    PS1.add(" ");
    PS1.set();

    /*str pwd;
    try { pwd = std::string(getenv("PWD")); }
    catch (std::exception e) { pwd = ""; }
    if (user.back()=='\n')user.pop_back();
    output += " ";
    output += pwd;*/
    return 0;
}
