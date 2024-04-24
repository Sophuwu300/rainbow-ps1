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

const std::string emotes[] =
{":)", ":3", ";D",":]", ":D", "xD",
";3", ";)", "=)",":P", ":O", ":b"};

std::string emote() {
    srand(time(0));
    return emotes[rand() % 12];
}

std::string padint(int n, int max) {
    std::string s = std::to_string(n);
    for(max=std::to_string(max).length();s.length()<max; s="0"+s);
    return s;
}

void hm_time(int &h, int &m) {
    std::chrono::time_point now = std::chrono::system_clock::now();
    time_t in_time_t = std::chrono::system_clock::to_time_t(now);
    auto tt = std::localtime(&in_time_t);
    h = (int)(tt->tm_hour);
    m = (int)(tt->tm_min);
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
    return b.substr(0, b.size() - 1);
}

const char *hex="0123456789ABCDEF";
unsigned char unhex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

int main(int argc, char* argv[]) {

    std::string host = readfile("/etc/hostname");
    char ip[8];
    pipe("hostname -I | awk -F '.' ' { printf(\"%X%X%X%X\",int($1),int($2),int($3),int($4)); } ' ", ip);
    unsigned char ipaddr[4];
    for (int i = 0; i < 4; i++)ipaddr[i] = unhex(ip[2*i])<<4 | unhex(ip[2*i+1]);
    for (int i = 0; i < 4; i++)printf("\033[38;5;%dm%c", ipaddr[i] & 0xFF, hex[ipaddr[i]>>4&0xFF]);
    printf("\033[0m\n");



    int h, m;
    hm_time(h, m);
    printf("%.2d:%.2d\n", h, m);
    printf("%s %s\n", host.c_str(), emote().c_str());

    return 0;
}