#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cstdio>
#include<windows.h>
#include<random>
using namespace std;

bool isKnown(char i) {
    return i>=48 && i<=57;
}
string getSymbol(int i) {
    const string a[13] = {"¡õ","¢Ù","¢Ú","¢Û","¢Ü","¢Ý","¢Þ","¢ß","¢à","¢á","¡ö","¡ô","¡ï"};
    if(i>=0 && i<10) return a[i];
    if(isKnown(i)) return a[i-48];
    if(i==63) return a[10];
    if(i==43) return a[11];
    if(i==42) return a[12];
    char str[10];
    sprintf(str,"%02d",i);
    return str;
}

typedef struct gamegame {

    char map[51][51] = {0};
    char mark[51][51] = {63};
    int max_m = 0;
    int max_n = 0;
    int mine_amount = 0;
    int state = 0;

    int init(int width,int height,int mine) {
        state = 0;
        
        if(width < 9 || width > 50) return -1;
        if(height < 9 || height > 50) return -1;
        if(mine < 0 || mine > width*height) return -2;

        max_m = width; max_n = height; mine_amount = mine;

        state = 2;

        for(int n=1; n <= max_n; n++) {
            for(int m=1; m <= max_m; m++) {
                putMark(m,n,'?');
            }
        }
        return 0;
    }

    int generate(int pm,int pn) {

        if(state != 2) return -2;
        if(!(pm>=1 && pm<=max_m && pn>=1 && pn<=max_n)) return -1;

        int cnt=0;
        for(int n=1; n <= max_n; n++) {
            for(int m=1; m <= max_m; m++) {
                bool putMine=true;
                if( !( int(rand())%(max_m*max_n)<=mine_amount ) ) putMine=false;
                else

                for(int dm=-1;dm<=1&&putMine;dm++) {
                    for(int dn=-1;dn<=1&&putMine;dn++) {
                        if (m==pm+dm&&n==pn+dn) putMine=false;
                    }
                }

                if(putMine) {
                    put(m,n,'*'); cnt++;
                } else {
                    put(m,n,'?');
                }
            }
        }
        state = 1;
        return 0;
    }

    int put(int m,int n,char ch) {
        if(!(m>=1 && m<=max_m && n>=1 && n<=max_n)) return -1;

        map[m][n]=ch;
        return 0;
    }

    char get(int m,int n) {
        if(state == 0) return -2;
        if(!(m>=1 && m<=max_m && n>=1 && n<=max_n)) return -1;

        return map[m][n];
    }

    int putMark(int m,int n,char mark_type) {
        if(state != 1 && state != 2) return -2;
        if(!(m>=1 && m<=max_m && n>=1 && n<=max_n)) return -1;

        mark[m][n]=mark_type;
        return 0;
    }

    int putFlag(int m,int n) {
        char mark=getMark(m,n);
        if(isKnown(mark)) return -3;
        
        if(mark==43) return putMark(m,n,63);
        return putMark(m,n,43);
    }

    char getMark(int m,int n) {
        if(!(m>=1 && m<=max_m && n>=1 && n<=max_n)) return -1;

        return mark[m][n];
    }

    int probe(int m, int n) {
        if(state == 2) generate(m,n);

        if(state != 1) return -2;
        if(!(m>=1 && m<=max_m && n>=1 && n<=max_n)) return -1;

        
        if(isKnown(getMark(m,n))) {
            return 3;
        } else 
        if(getMark(m,n) == 43) {
            return 4;
        } else 
        if(get(m,n)==42) {
            state = 3;
            return 2;
        } else {
            putMark(m,n,getMineAmount(m,n)+48);
            if(getMark(m,n)==48) {
                for(int dm=-1;dm<=1;dm++) {
                    for(int dn=-1;dn<=1;dn++) {
                        probe(m+dm,n+dn);
                    }
                }
            }

            int flag=1;
            for(int mm=1;mm<=max_m;mm++) {
                for(int nn=1;nn<=max_n;nn++) {
                    if(!(
                        get(mm,nn)=='?' && 
                        isKnown(getMark(mm,nn))
                    ) && !(
                        get(mm,nn)=='*' && 
                        (getMark(mm,nn)=='?' || getMark(mm,nn)=='+')
                    )) {
                        flag=0;
                        break;
                    }
                }
                if(flag == 0) break;
            }
            if(flag) {state=4;return 1;}

            return 0;
        }
    }

    int getMineAmount(int m,int n) {
        if(state != 1) return -2;
        if(!(m>=1 && m<=max_m && n>=1 && n<=max_n)) return -1;

        int cnt=0;
        for(int dm=-1;dm<=1;dm++) {
            for(int dn=-1;dn<=1;dn++) {
                if(get(m+dm,n+dn)==42) cnt++;
            }
        }
        return cnt;
    }

    int print(int wm,int wn) {
        if(state == 0) return -2;
        for(int n=0; n <= max_n; n++) {

            for(int m=0; m <= max_m; m++) {
                if(m==wm&&n==wn) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
                    cout << getSymbol(getMark(m,n));
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
                } else {

                    if(n==0) {printf("%02d",m);continue;};
                    if(m==0) {printf("%02d",n);continue;};

                    cout << getSymbol(getMark(m,n));
                }
                
            }
            cout << endl;
        }
        return 0;
    }

    int print() {
        return print(-99,-99);
    }

    int cheatprint() {
        if(state == 0) return -2;
        for(int n=0; n <= max_n; n++) {
            for(int m=0; m <= max_m; m++) {
                if(n==0) {printf("%02d",m);continue;};
                if(m==0) {printf("%02d",n);continue;};
                cout << getSymbol(get(m,n));
            }
            cout << endl;
        }
        return 0;
    }

} gamegame;

int help() {
    cout << "Welcome to Minesweeper!\n"
            "Rules: Sorry, I am too lazy to say.\n"
            "Attention: \n"
            "- The map is compelely randomly generated.\n"
            "- The amount of mines that you input is only able to control the approximate amount.\n"
            "- It's all because the programmer who makes this program is a noob. :(\n"
            "Commands: \n"
            "- help - Open the help guide\n"
            "- init <width> <height> <the amount of mines> - Generate a map\n"
            "- flag <m> <n> - Put a flag on the specified coordinates\n"
            "- probe <m> <n> - Probe on the specified coordinates\n"
            "- print - Print the map\n"
            "- cheat - Print the map like a god\n"
            "- quit - Quit the program\n"
            "About: \n"
            "- By inuEbisu_\n";
    return 0;
}
int main() {
    gamegame game;
    system("title Minesweeper");
    system("cls");
    help();
    while(1) {
        cout << "\n> ";
        string cmd;
        cin >> cmd;
        system("cls");
        if(cmd == "help") {
            help();
        } else
        if(cmd == "init") {
            int m,n,mine,kode; cin >> m >> n >> mine;
            kode = game.init(m,n,mine);
            switch (kode) {
                case -2:
                    cout << "ERROR -2: Too many mines.\n";
                    break;
                case -1:
                    cout << "ERROR -1: Too wide or too high; Or too narrow or too short.\n";
                    break;
                case 0: 
                    cout << "OK.\n";
                    game.print(); 
                    break;
                default:
                    cout << "Unknown Error:" << kode << endl;
            }
        } else
        if(cmd == "flag") {
            int m,n,kode; cin >> m >> n;
            kode = game.putFlag(m,n);
            switch (kode) {
                case -3:
                    cout << "ERROR -3: The position has been probed already.\n";
                    game.print();
                    break;
                case -2: 
                    cout << "ERROR -2: Please init ;)\n";
                    break;
                case -1: 
                    cout << "ERROR -1: The position you gave is invalid.\n"; 
                    game.print(); 
                    break;
                case 0:
                    cout << "OK.\n";
                    game.print();
                    break;
                default:
                    cout << "Unknown Error:" << kode << endl;
            }
        } else
        if(cmd == "probe") {
            int m,n,kode; cin >> m >> n;
            kode = game.probe(m,n);
            switch (kode) {
                case -2: 
                    cout << "ERROR -2: Please init ;)\n";
                    break;
                case -1: 
                    cout << "ERROR -1: The position you gave is invalid.\n"; 
                    game.print(); 
                    break;
                case 0: 
                    cout << "OK.\n";
                    game.print(); 
                    break;
                case 1: 
                    cout << "YOU WIN!!\n"; 
                    game.print(); 
                    break;
                case 2:
                    cout << "You lose...\n";
                    game.print(m,n);
                    break;
                case 3:
                    cout << "ERROR 3: The position has been probed already.\n";
                    game.print();
                    break;
                case 4:
                    cout << "ERROR 4: The position has been planted a flag.\n";
                    game.print();
                    break;
                default:
                    cout << "Unknown Error:" << kode << endl;
            }
        } else
        if(cmd == "print") {
            cout << "OK.\n";
            game.print();
        } else
        if(cmd == "cheat") {
            cout << "OK.\n";
            game.cheatprint();
        } else
        if(cmd == "quit") {
            break;
        } else {
            cout << "Unknown Commands.\n";
        }
    }
    return 0;
}