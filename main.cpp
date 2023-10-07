#include <cstdlib>
#include <string>
#include <ctime>
#include "rainbow.h"
int stoi(const std::string& str, int& i, int mode = 0) {
    int result = 0;
    int mult = 1;
    for (; i < str.length(); i++) {
        if (mode && str[i] == '.') break;
        if (str[i] == '-') mult = -1;
        else if (str[i] >= '0' && str[i] <= '9') result = result * 10 + mult*(str[i] - '0');
    }
    return result;
}

double stof(const std::string& str) {
    int i = 0;
    double whole = stoi(str, i, 1);
    double frac = stoi(str, i);
    for (; frac > 1; frac /= 10);
    return whole + frac;
}

void exportEnv(const std::string& name, double value) {
    printf(" %s=%f", name.c_str(), value);
}

int getEnv(double& v, const std::string& name) {
    if (char* env = std::getenv(name.c_str())) v = stof(env);
    else return 1;
    return 0;
}

int randint() {
    std::srand(std::time(nullptr));
    int n = std::rand() & 15;
    n+=10;
    return n;
}

int main(int argc, char* argv[]) {

    /*Bash prompt should:
        - show current folder
        - count how many commands were executed
        - show random emoticon
        - display in rainbow colors (choose rainbow length with env variable)
    */

    rainbow r;
    if (argc == 2 && argv[1][0] == 's') r.init(15+stio(argv[1]));
    else if (getEnv(r.s  , "PSRS")||getEnv(r.c.r, "PSRR")
    ||getEnv(r.c.g, "PSRG")||getEnv(r.c.b, "PSRB")
    || !(int)r.s || (!(int)r.c.r && !(int)r.c.g && !(int)r.c.b))
        r.init(randint());
    else r.next();

    printf("export");
    exportEnv("PSRS", r.s);
    exportEnv("PSRR", r.c.r);
    exportEnv("PSRG", r.c.g);
    exportEnv("PSRB", r.c.b);
    printf("%c", '\n');

    return 0;
}