/*
 * File: prompt.cpp
 * This program is used to generate a bash prompt with rainbow colors.
 * It can also be used to generate the necessary environment variables
 *
 */
#include "common.hpp"
#include "ip.hpp"
#include "rainbow.hpp"

int randint(int n) {
    if (n<=0) return 0;
    FILE *f = fopen("/dev/urandom", "r");
    if (f == NULL) return 0;
    unsigned char buf;
    unsigned int ret = 0;
    for (int i = 0; i < 4; i++) {
        ret<<=8;
        fread(&buf, 1, 1, f);
        ret += buf;
    }
    //fread(&buf, 1, 4, f);
    fclose(f);
    ret = ret % n;
    return (int)ret;
}

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

int wave(int x) {return (int)(3+3*(sin(x/1.4)));}

rainbow rain(int wavefactor=6) {
    rainbow r;
    const char* rainenv = getenv("RAINBOW");
    if (rainenv == NULL)r.init(35 - randint(10));
    else {
        atoi(r.s, rainenv, atoi(r.c.b, rainenv, atoi(r.c.g, rainenv,atoi(r.c.r, rainenv, 0)+1)+1)+1);
        r.next();
    }
    exportenv("RAINBOW", r.c.str()+";"+std::to_string(r.s));
    for(int i = 0; i < abs(wave(wavefactor)); i++)r.next();
    return r;
}

void getpwd(escape &esc) {
    str pwd, home;
    if (!strEnv(pwd, "PWD")) return;
    strEnv(home, "HOME");
    int inHome = 0;
    if (!home.empty() && pwd.length() >= home.length() && pwd.substr(0, home.length()) == home) {
        pwd = pwd.substr(home.length());
        inHome = 1;
    }
    std::vector<str> parts = split(pwd, '/');


    int n = 2;
    int siz = parts.size();
    intenv(n, "PWDLEN");
    pwd="..";
    if (n <= 0 || n >= siz) {
        n = 0;
        if (inHome) pwd="~";
        else pwd="/";
    }
    else n = siz - n;
    for (esc.rain(" "+pwd); n < siz; n++) {
        esc.rain(" "+parts[n]);
    }
}

void checkBash() {
    str env;
    if (!strEnv(env, "SHELL")) {
        exit(1);
    }
    int l = env.length();
    if (l < 4 || env.substr(l - 4, 4) != "bash") {
        exit(1);
    }
}


int main(int argc,char** argv) {
    checkBash();

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
            if (ip.get()==0)return 1;
            printf("%s\n%s\n",ip.toString().c_str(),ip.toColor().c_str());
            return 0;
        }
    }
    int lineno = intenv("LINENO");
    if (lineno == 0)printf("%s\n","export LINENO");
    PS1.r = rain(lineno);

    str user=" ";
    const char* ipcol = getenv("IPCOLOR");
    if (ipcol == NULL || !(std::string(ipcol)=="none"||std::string(ipcol)=="NONE")) {
        user = envorcmd("USER", "whoami");
        user = " " + user + " ";
        IP ip;
        if (ip.get()) PS1.output+=ip.toColor()+" ";
    }


    PS1.add("\\${?}", "202");
    PS1.rain(user);
    PS1.rain(std::to_string(lineno)+" ");
    PS1.rain(emote());
    getpwd(PS1);
    PS1.rain(" $ ");
    PS1.set();

    return 0;
}
