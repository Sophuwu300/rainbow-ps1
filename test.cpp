#include <string>
#include <chrono>
#include <ctime>

void appendTest(std::string test) {
    std::string buff = "";
    for (int j = 0; j < test.length(); j++) {
        buff.append("\033[38;2;").append(std::to_string(j))
                .append(";").append(std::to_string(j))
                .append(";").append(std::to_string(j))
                .append("m").append(test.substr(j, 1))
                .append("\033[0m ");
    }
    buff.append("\n");
    printf("\n");
}

void sprintfTest(std::string test) {
    char cBuff[40];
    std::string buff = "";
    for (int j = 0; j < test.length(); j++) {
        sprintf(cBuff, "\033[38;2;%d;%d;%dm%s\033[0m", j, j, j, test.substr(j, 1).c_str());
        buff.append(cBuff);
        cBuff[0] = '\0';
    }
    buff.append("\n");
    printf("\n");
}

void printfTest(std::string test) {
    for (int j = 0; j < test.length(); j++) {
        printf("\033[38;2;%d;%d;%dm%s\033[0m", j, j, j, test.substr(j, 1).c_str());
    }
    printf("\n");
}

struct timetype {
    std::chrono::time_point<std::chrono::steady_clock> t;
    int ms();
    void get();
    int operator-(timetype& t2) {
        return this->ms() - t2.ms();
    }
};
int timetype::ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(this->t.time_since_epoch()).count();
}
void timetype::get() {
    this->t = std::chrono::steady_clock::now();
}

void resolve(const char* name,timetype& start,timetype& end) {
    fprintf(stderr,"%s method: %d ms\n", name, end-start);
}

int main() {
    std::string test = "hello i am a really weird string that is really long and i am just typing random stuff to make it longer";
    timetype t1, t2, t3, t4;

    t1.get();
    for (int i = 0; i < 100000; i++) {
        appendTest(test);
    }
    t2.get();
    for (int i = 0; i < 100000; i++) {
        sprintfTest(test);
    }
    t3.get();
    for (int i = 0; i < 100000; i++) {
        printfTest(test);
    }
    t4.get();
    resolve("append", t1, t2);
    resolve("sprintf", t2, t3);
    resolve("printf", t3, t4);
}