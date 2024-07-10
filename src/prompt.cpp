/*
 * File: prompt.cpp
 * This program is used to generate a bash prompt with rainbow colors.
 * It can also be used to generate the necessary environment variables
 *
 */

#include "common.hpp"
#include "ip.hpp"
#include "rainbow.hpp"


void exportenv(str k, str v) {printf("export %s=\"%s\"\n", k.c_str(), v.c_str());}

int randint(int n) {return rand()%n;}

str mouthlist = ")3>]DPO";
str eyelist = ";:=";
str emote(){
    return eyelist.substr(randint(eyelist.length()-1),1) + mouthlist.substr(randint(mouthlist.length()-1),1);
}


struct escape {
    str output = "";
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



int main(int argc,char** argv) {
    escape PS1;
    for (int i=1;i<argc;i++) {
        str arg = argv[i];
        if (arg == "--help" || arg == "help" || arg == "-h" || arg == "-?") {
            printf("PROMPT_COMMAND='eval \"$(%s)\"'\n",argv[0]);
            return 0;
        }
        if (arg == "view") {
            esc = &DBG;
        }
        if (arg=="ip") {
            IP ip;
            ip.fromCmd();
            printf("%s\n",ip.toColor().c_str());
            return 0;
        }
    }
    int lineno = intenv("LINENO");
    if (lineno == 0)printf("%s\n","export LINENO COLUMNS LINES ");
    PS1.r = rain(lineno);
    srand((unsigned int)(intenv("RANDOM")+lineno));

    IP ip;
    ip.fromCmd();

    if (lineno%10==0) PS1.output+=ip.toColor();
    else PS1.add("<3", "255;0;200");
    PS1.add(" ");
    PS1.add("\\${?}", "202");
    PS1.add(" ");
    PS1.rain(std::to_string(lineno));
    PS1.add(" ");
    PS1.rain(emote());
    str pwd = getpwd();
    std::vector<str> dirs = split(pwd,'/');
    size_t a = pwd.find_last_of("/");
    pwd
    PS1.add(" ");
    PS1.rain("$");
    PS1.add(" ");
    PS1.set();

    return 0;
}
