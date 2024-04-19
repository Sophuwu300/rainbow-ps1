/*
 * File: prompt.cpp
 * This program is used to generate a bash prompt with rainbow colors.
 * It can also be used to generate the necessary environment variables
 *
 */

#include <filesystem>
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
    printf(" %s=%lf", name.c_str(), value);
}

int getEnv(double& v, const std::string& name) {
    if (char* env = std::getenv(name.c_str())) sscanf(env, "%lf", &v);
    else return 1;
    return 0;
}

int randint() {
    std::srand(std::time(nullptr));
    int n = std::rand() & 15;
    n*=3;
    n+=10;
    return n;
}

int getAllEnvs(rainbow& r, int& count){
    if (char* env = std::getenv("RAINBOWPSC")) {sscanf(env, "%d", &count);}
    else count = 0;
    return getEnv(r.s  , "RAINBOWPSS")||getEnv(r.c.r, "RAINBOWPSR")
    ||getEnv(r.c.g, "RAINBOWPSG")||getEnv(r.c.b, "RAINBOWPSB");
}

const std::string emotes[] =
{":)", ":3", ";D",
":]", ":D", "xD",
";3", ";)", "=)",
":P", ":O", ":>"};


std::string makePrompt(int& count) {
    std::srand(std::time(nullptr)*count);
    std::string path = std::filesystem::current_path();
    return std::to_string(count).append(" ")
    .append(emotes[std::rand() % 12]).append(" ")
    .append(path.substr(path.find_last_of('/')+1));
}

void printPrompt(rainbow& r, int& count) {
    std::string outbuff = "\\[\\e[38;2;255;0;180m\\]<3\\[\\e[0m\\] ";
    r.s=5.0*255.0/(double)(40+count%20-count%13+count%7);
    std::string prompt = makePrompt(count);
    for (int i = 0; i < prompt.length(); i++) {
        outbuff.append("\\[\\e[38;2;")
                .append(r.c.str()).append("m\\]")
                .append(prompt.substr(i, 1))
                .append("\\[\\e[0m\\]");
        r.next();
    }
    outbuff.append(" \\[\\e[38;2;").append(r.c.str()).append("m\\]\\\\$\\[\\e[0m\\] ");
    printf("%s", outbuff.c_str());
}

int main(int argc, char* argv[]) {

    if (argc > 1 && argv[1][0]=='h') {
        printf("To use this program\n");
        printf("add the following lines to your .bashrc:\n");
        printf("export PROMPT_COMMAND='`%s`'\n", argv[0]);
        printf("PS1=`%s \"!\"`\n", argv[0]);
        return 0;
    }
    rainbow r;
    int count = 0;
    int err = getAllEnvs(r, count);
    if (err || !(int)r.s || (!(int)r.c.r && !(int)r.c.g && !(int)r.c.b)) r.init(randint());
    else r.next();

    if (argc > 1 && argv[1][0]=='p') {
        printPrompt(r, count);
        return 0;
    }

    printf("export");
    exportEnv("RAINBOWPSS", r.s);
    exportEnv("RAINBOWPSR", r.c.r);
    exportEnv("RAINBOWPSG", r.c.g);
    exportEnv("RAINBOWPSB", r.c.b);
    printf(" RAINBOWPSC=%d", 1+count);
    printf("\n");

    return 0;
}