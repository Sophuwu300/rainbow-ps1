/*
 * common.hpp
 * this is where all the common functions and variables are stored
 */
#ifndef BASHPROMPT_COMMON_H
#define BASHPROMPT_COMMON_H


#include <cstdio>
#include <cmath> // for sin
#include <vector>
#include <string>
typedef std::string str;

struct paStr {str s, e;};
extern paStr PRT;
extern paStr DBG;
extern paStr* esc;
extern str wrap(str s);

extern str docmd(std::string inputted);
extern str envorcmd(str env, str cmd);
extern int atoi(int &n, const char *c, int i);
extern int intenv(const char* env);
extern std::vector<str> split(str s, char delim);

extern int int2col(int n);

extern str color2(int top, int bot);

#endif //BASHPROMPT_COMMON_H
