#include "common.hpp"


paStr PRT = {"\\[\\e[","m\\]"};
paStr DBG = {"\033[","m"};
paStr* esc = &PRT;
str wrap(str s) {return esc->s + s + esc->e;}

str docmd(const char* inputted) {
    FILE* file = popen(inputted, "r");
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
    catch (std::exception e) { ret = docmd(cmd.c_str()); }
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

int int2col(int n) {
    if (n>255||n<0) return 0;
    int col = 0;
    const char* env = getenv("IPCOLOR");
    if (env != NULL && (std::string(env)=="alt"||std::string(env)=="ALT"))
        col=16;
    n=(n*(216+col)/256)+(16-col);
    return n;
}


str color2(int top, int bot) {
    str ret = "48;5;";
    ret += std::to_string(top);
    ret += ";38;5;" + std::to_string(bot);
    ret = wrap(ret);
    ret+="▄"; // "▌" "▀" "▄" "▌"
    return ret+wrap("0");
}