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
#include <sys/sysinfo.h>
#include <filesystem>

#include "rainbow.hpp"

typedef std::string str;

int randint(int max) {
    srand(std::chrono::duration_cast<std::chrono::nanoseconds>(
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

int atoi(int &n, const char *c, int i=0){
    n=0;
    for (; c[i] >= '0' && c[i] <= '9'; i++) n = n*10 + (int)c[i] - (int)'0';
    return i;
}

int intenv(const char* env) {
    int n=0;
    const char* val = getenv(env);
    if (val == NULL) return n;
    atoi(n, val);
    return n;
}

str time_hh_mm() {
    time_t t = time(0);
    struct tm * now = localtime(&t);
    char buffer[80];
    int n = strftime(buffer, 80, "%H:%M", now);
    return str(buffer, n);
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
    struct rainbow r;
    void rain(str s) {
        for (int i = 0; i < s.length(); i++) {
            add(s.substr(i,1), r.c.str());
            r.next();
        }
    }
    void set() { exportenv("PS1",output); }
};

int wave(int x) {return (int)(6+1.54*3.14159*(sin(x/(3.14159))));}

rainbow rain(int wavefactor=6) {
    rainbow r;
    const char* rainenv = getenv("RAINBOW");
    if (rainenv == NULL)r.init(25-randint(15));
    else {
        atoi(r.s, rainenv, atoi(r.c.b, rainenv, atoi(r.c.g, rainenv,atoi(r.c.r, rainenv, 0)+1)+1)+1);
        r.next();
    }
    exportenv("RAINBOW", r.c.str()+";"+std::to_string(r.s));
    for(int i = 0; i < abs(wave(wavefactor)); i++)r.next();
    return r;
}

str getpwd() {
    str pwd;
    try { pwd = std::string(getenv("PWD")); }
    catch (std::exception e) { pwd = ""; }
    if (pwd.back()=='\n')pwd.pop_back();
    return pwd;
}

std::vector<str> split(str s, char delim) {
    std::vector<str> ret;
    str temp = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == delim) {
            ret.push_back(temp);
            temp = "";
        } else temp += s[i];
    }
    ret.push_back(temp);
    return ret;
}

int main(int argc,char** argv) {
    escape PS1;
    for (int i=1;i<argc;i++) {
        str arg = argv[i];
        if (arg == "--help" || arg == "help" || arg == "-h" || arg == "-?") {
            std::cout << "PROMPT_COMMAND='eval \"$(" << argv[0] << ")\"'" << std::endl;
            return 0;
        }
        if (arg == "view") {
            PS1.debugON = true;
        }
    }
    int lineno = intenv("LINENO");
    if (lineno == 0)std::cout << "export LINENO COLUMNS LINES ";
    PS1.r = rain(lineno);

    std::vector<str> st;
    st.push_back(envorcmd("USER","whoami"));
    st.push_back(envorcmd("HOSTNAME","hostname"));
    st.push_back(time_hh_mm());
    int len = 0;
    for (int i = 0; i < st.size(); i++)if (st[i].length()>len) len = st[i].length();

    for (int i = 0; i < st.size(); i++){
        while (st[i].length() < len) st[i] = " "+st[i];
        st[i] = " " + st[i] + " ";
    }
    str ip = docmd("hostname -I | awk -F '.' ' { for(i=1;i<5;i++){printf(\"%.3d\", $i);}; } ' ");
    PS1.add("\\$?", "190;180;30");
    PS1.add(emote(), std::to_string(randint(214)+30));
    PS1.add("|", "150;150;150");
    str s = std::string(st[lineno%st.size()]);
    int s_len = (s.length()/4);
    if (s.length()%2==0) s_len++;
    for (int i = 0; i<4; i++) PS1.add(s.substr(i*s_len,s_len),ip.substr(3*((i+lineno)%4),3));
    PS1.add("| ","150;150;150");
    PS1.rain(std::to_string(lineno));
    PS1.add(" ");
    str pwd = getpwd();
    std::vector<str> dirs = split(pwd,'/');
    if (dirs.size() == 0) dirs.push_back("");
    if (dirs[0].length() == 0) dirs[0] = "/";
    int i = dirs.size()-2;
    if (i < 0) i = 0;
    else PS1.add("..", "150;150;150");
    for (; i < dirs.size(); i++) {
        PS1.add("/", "150;150;150");
        PS1.rain(dirs[i]);
    }
    PS1.add(" ");
    PS1.rain("$");
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
