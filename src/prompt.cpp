/*
 * File: prompt.cpp
 * This program is used to generate a bash prompt with rainbow colors.
 * It can also be used to generate the necessary environment variables
 *
 */

#include <string>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <sys/ioctl.h>
#include <unistd.h>

typedef std::string str;

const str emotes[] = {":)", ":3", ";D",":]", ":D", "xD",";3", ";)", "=)",":P", ":O", ":b"};

str emote() {
    srand(std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    return emotes[rand() % 12];
}

str time_hhmm() {
    std::chrono::time_point now = std::chrono::system_clock::now();
    time_t in_time_t = std::chrono::system_clock::to_time_t(now);
    auto tt = std::localtime(&in_time_t);
    return std::to_string((int)(tt->tm_hour)) + ":" + std::to_string((int)(tt->tm_min));
}

void pipe(std::string inputted, char *bufout) {
    FILE* file = popen(inputted.c_str(), "r");
    fread(bufout, 1, 8, file);
    pclose(file);
}

std::string readfile(std::string filename){
    std::string b;
    FILE* file = fopen(filename.c_str(), "r");
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        b += buffer;
    }
    fclose(file);
    if (b.back()=='\n')b.pop_back();
    return b;
}

int stoi(std::string s) {
    int n = 0;
    for(;!s.empty();s.pop_back()){
        if( s.back() < '0' || s.back() > '9') continue;
        //s.back()
    }
    return n;
}

unsigned char unhex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

int main(int argc, char* argv[]) {
    if (getenv("USER") == NULL)system("export USER=$(whoami)");
    if (getenv("PWD") == NULL)system("export PWD=$(pwd)");
    if (getenv("LINENO") == NULL)system("export LINENO");

    std::string output = "";
    if (argc > 1) output += std::string(argv[1]);

    struct winsize term;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &term) == -1) return 1;

    output += emote();
    std::string lineno = getenv("LINENO");
    output += lineno;
    output += " ";
    output += time_hhmm();
    output += " ";
    std::string host = std::string(readfile("/etc/hostname"));
    if (host.back()=='\n')host.pop_back();

    std::string user;
    user=std::string(getenv("USER"));
    if (user.back()=='\n')user.pop_back();

    char ip[8];
    pipe("hostname -I | awk -F '.' ' { printf(\"%X%X%X%X\",int($1),int($2),int($3),int($4)); } ' ", ip);
    unsigned char ipaddr[4];
    for (int i = 0; i < 4; i++)ipaddr[i] = unhex(ip[2*i])<<4 | unhex(ip[2*i+1]);
    printf("\033[1;38;5;%dm%s\033[0m", ipaddr[0], user.c_str());
    printf("\033[38;5;%dm%s\033[0m", ipaddr[1], "@");
    printf("\033[1;38;5;%dm%s\033[0m", ipaddr[2], host.c_str());
    printf("\033[1;38;5;%dm%d", ipaddr[3], ipaddr[3]);
    printf("\033[0m");

    std::string pwd = std::string(getenv("PWD"));
    printf("%s", pwd.c_str());

    return 0;
}