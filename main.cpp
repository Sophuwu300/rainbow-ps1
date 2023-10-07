#include <cstdlib>
#include <string>

int stoi(const std::string& str) {
    int result = 0;
    int mult = 1;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '-') mult = -1;
        else if (str[i] >= '0' && str[i] <= '9') result = result * 10 + mult*(str[i] - '0');
    }
    return result;
}

int main() {

    /*Bash prompt should:
        - show current folder
        - count how many commands were executed
        - show random emoticon
        - display in rainbow colors (choose rainbow length with env variable)
    */

    int bashline, rlenh, rlenv;
    double i, r, g, b;

    int var;
    int var2;
    if(char* env = std::getenv("XSOPHCOUN")) var = stoi(env);
    else var = 0;
    if(char* env = std::getenv("")) var2 = stoi(env);
    else var2 = 0;

    printf("export PS1=\"$SOPHVAR $HISTCMD:$LINENO\"");

    return 0;
}