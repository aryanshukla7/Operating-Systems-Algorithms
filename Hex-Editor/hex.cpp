#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string.h>
#include <sys/ioctl.h>
using namespace std;


char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c", buf);
    return buf;
 }


 int main() {
    int i = 16;
    int j = 19;
    int nextLine = 1;
    while(1) {
        if(nextLine == 16) {
            cout << "\n";
            i = 16;
            j = 19;
            nextLine = 1;
        }
        char x = getch();
        string c = "C";
        string d = "D";
        string escSeq = "\033[";
        string space = escSeq + to_string(i) + c;
        i+=2;
        cout << space << hex << (int)x << " ";
        string space2 = escSeq + to_string(j) + d;
        cout << space2;
        j+=2;
        nextLine++;
    }
 }