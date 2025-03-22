/*
 * ip.hpp
 * this is to get the ip address of the machine and convert it to a color
 */

#ifndef BASHPROMPT_IP_H
#define BASHPROMPT_IP_H
#include "common.hpp"

struct IP {
    int values[4];
    int fromString(str s) {
        int pos = 0;
        int c = 0;
        for (int i=0;i<4;i++) {
            values[i] = 0;
        }
        for (int i=0;pos<s.length()&&i<4;pos++){
            c=(int)(s.c_str()[pos]);
            if (c>=48&&c<=57) values[i] = (values[i]*10) + (c - 48);
            else if (i>=3) break;
            else i++;
        }
        c = 0;
        for (int i=0;i<4;i++) {
            if (values[i]>255||values[i]<0) return 0;
            c += values[i];
        }
        if (c == 0) return 0;
        return 1;
    }
    int fromEnv() {
        const char * env = getenv("IPStr");
        if (env != NULL) return fromString(std::string(env));
        return 0;
    }
    int fromCmd() {
        str s;
        const char * env = getenv("IPCmd");
        if (env != NULL && fromString(docmd(env).c_str())) return 1;
        return fromString(docmd("hostname -i").c_str());
    }
    int get() {
        int ok = fromEnv();
        if (!ok)ok = fromCmd();
        return ok;
    }
    str toColor() {
        str ret = "";
        ret+= color2(int2col(values[0]), int2col(values[1]));
        ret+= color2(int2col(values[2]), int2col(values[3]));
        return ret;
    }
    str toString() {
        str s="";
        for (int i = 0; i < 4; i++) {
            s+=std::to_string(values[i]);
            s+= ".";
        }
        s.pop_back();
        return s;
    }
};


#endif //BASHPROMPT_IP_H
