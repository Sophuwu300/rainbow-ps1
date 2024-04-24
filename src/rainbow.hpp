#ifndef RAINBOW_H
#define RAINBOW_H 1

#include <string>
#include <cstdio>

struct rgb {
    int r, g, b;

    void clamp() {
        if (r > 255) r = 255; else if (r < 0) r = 0;
        if (g > 255) g = 255; else if (g < 0) g = 0;
        if (b > 255) b = 255; else if (b < 0) b = 0;
    }

    std::string str(char sep = ';') {
        clamp();
        return std::to_string((int)r) + sep + std::to_string((int)g) + sep + std::to_string((int)b);
    }

    void set(double rr, double gg, double bb) {r = rr;g = gg;b = bb; clamp();}
    void set(rgb c) {r = c.r;g = c.g;b = c.b; clamp();}

    void print(std::string s) {printf("\033[38;2;%sm%s\033[0m", str().c_str(), s.c_str());}
    void printAt(std::string s, int x, int y) {printf("\033[%d;%dH\033[38;2;%sm%s\033[0m", y, x, str().c_str(), s.c_str());}
    void setPx(int x, int y) {printf("\033[%d;%dH\033[48;2;%sm \033[0m", y, x, str().c_str());}

    double operator[](int i) {
        switch (i%3) {
            case 0: return r;
            case 1: return g;
            case 2: return b;
        }
        return 0;
    }
    void indexAdd(int i, double v) {
        switch (i%3) {
            case 0: r += v; break;
            case 1: g += v; break;
            case 2: b += v; break;
        }
        clamp();
    }
};

struct rainbow {
    rgb c;
    int s = 0; // step

    void init(int len = 10) {
        if (len == 0) len = 10;
        c.set(255, 0, 0);
        s = 5.0*255.0 / len;
    }

    void next() {
        if (s == 0) return;
        for (int i = 0; i < 3; i++) {
            if ((c[i+1]==0||c[i+1]==255)&&(c[i+2]==0||c[i+2]==255)) {
                if (c[i+1]==0&&c[i+2]==255) c.indexAdd(i, s);
                else if (c[i+1]==255&&c[i+2]==0) c.indexAdd(i, -s);
            }
        }
    }
    void print(std::string s) {c.print(s);}
    void print2d(std::string s, int len = 30) {
        rainbow r;
        r.init(len);
        r.c.set(c);
        for (int i = 0; i < s.length(); i++) {
            r.print(s.substr(i, 1));
            r.next();
        }
    }
};
#endif