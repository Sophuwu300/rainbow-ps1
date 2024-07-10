/*
 * ip.hpp
 * this is to get the ip address of the machine and convert it to a color
 */

#ifndef BASHPROMPT_IP_H
#define BASHPROMPT_IP_H
#include "common.hpp"

struct IP {
    int values[4] = {0,0,0,0};
    str string = "";
    void fromString(str s) {
        std::vector<str> parts = split(s, '.');
        for (int i = 0; i < 4; i++) values[i] = std::stoi(parts[i]);
        string = s;
    }
    void fromCmd(str cmd = "hostname -i") {
        fromString(docmd(cmd));
    }
    str toColor() {
        str ret = "";
        ret+= color2(values[0], values[1]);
        ret+= color2(values[2], values[3]);
        return ret;
    }
};


#endif //BASHPROMPT_IP_H
