#include "common.hpp"


paStr PRT = {"\\[\\e[","m\\]"};
paStr DBG = {"\033[","m"};
paStr* esc = &PRT;
str wrap(str s) {return esc->s + s + esc->e;}

void exportenv(str k, str v) {printf("export %s=\"%s\"\n", k.c_str(), v.c_str());}

int strEnv(str &dst, cstr env) {
    cstr val = getenv(env);
    if (val == NULL) return 0;
    str out = "";
    for (int i = 0; i < 1024 && val[i] != '\0' && val[i] != '\n'; i++) {
        out += val[i];
    }
    if (out.empty()) return 0;
    dst = out;
    return 1;
}

str docmd(str inp) {
    char buff[1024];
    int n = 0;
    inp += " 2>/dev/null"; // ignore errors
    try {
        FILE *file = popen(inp.c_str(), "r");
        n = fread(buff, 1, sizeof(buff), file);
        pclose(file);
    } catch (std::exception &e) {
        return str("");
    }
    str r = std::string(buff, n);
    if (r.back() == '\n') r.pop_back();
    return r;
}

str envorcmd(cstr env,cstr cmd) {
    str ret = "";
    try { ret = std::string(getenv(env)); }
    catch (std::exception e) { ret = docmd(cmd); }
    return ret;
}

int atoi(int &n, const char *c, int i=0){
    n=0;
    for (; c[i] >= '0' && c[i] <= '9'; i++) n = n*10 + (int)(c[i] - '0');
    return i;
}

int intenv(cstr env) {
    int n = 0;
    intenv(n, env);
    return n;
}

int intenv(int &n, cstr env) {
    const char* val = getenv(env);
    if (val == NULL || val[0] == '\0') return 0;
    atoi(n, val);
    return 1;
}

std::vector<str> split(str s, char delim) {
    std::vector<str> ret;
    str temp = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == delim) {
            if (temp == "") continue;
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
    ret+= "▌"; // "▄" "█ "
    return ret+wrap("0");
}