#include "common.hpp"


paStr PRT = {"\\[\\e[","m\\]"};
paStr DBG = {"\033[","m"};
paStr* esc = &PRT;
str wrap(str s) {return esc->s + s + esc->e;}

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
str hex = "0123456789ABCDEF";
str tohex(int n) {
    str ret = "";
    for (int i = 0; i < 2; i++) {
        ret = hex[n%16] + ret;
        n /= 16;
    }
    return ret;
}

int int2col(int n) {
    if (n>255||n<0) return 0;
    int col = 0;
    char* env = getenv("IPCOLOR");
    if (env != NULL) col=16;
    n=(n*(216+col)/256)+(16-col);
    return n;
}

int avg(int a[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) sum += a[i];
    return sum/n;
}

int map10(char a) {
    int x = a-'0';
    return 36*(5*x/10)+6*(5*(10-x)/10)+16;
}

str color2(int top, int bot) {
    str ret = "48;5;";
    ret += std::to_string(top);
    ret += ";38;5;" + std::to_string(bot);
    ret = wrap(ret);
    ret+="▄"; // "▌" "▀" "▄" "▌"
    return ret+wrap("0");
}