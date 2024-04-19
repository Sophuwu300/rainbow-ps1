#include <string>
#include <cstdlib>
#include <cstdio>

typedef std::string str;
typedef unsigned int ud;
typedef unsigned char ub;

struct ip4{
    str hexid = "0x";
    ub val[4]={0,0,0,0};
    str s(char join = 0, int useColor = 0);
    str getid();
};
str ip4::s(char join, int useColor) {
    return str(std::to_string(val[0]) + "." + std::to_string(val[1]) + "." + std::to_string(val[2]) + "." + std::to_string(val[3]));
}
str ip4::getid() {
    std::system("hostid");
    for (char i = 0; i < 4; i++) {
        p[i] = (ub)
    }
    return id;
}

int main() {

    ud id = hostid();
    printf("hostid: %lx\n", id);
    printf("hostid: %lx\n", );

    unsigned char ipr[4];
    for (char i = 0; i < 4; i++) {

    }
    printf("ip: %u.%u.%u.%u\n", ipr[0], ipr[1], ipr[2], ipr[3]);
   return 0;

}