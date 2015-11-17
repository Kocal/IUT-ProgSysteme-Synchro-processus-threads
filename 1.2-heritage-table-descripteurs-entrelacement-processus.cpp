#include <iostream>

#include <cstdlib>
#include <cstdio>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using std::string;
using std::cout;
using std::endl;

#define LECTURE 0
#define ECRITURE 1

int main(void) {

    string ping = "Ping-";
    string pong = "Pong\n";

    int max = 100;
    int fichier = open("match.txt",  O_CREAT | O_TRUNC | O_WRONLY, 0755);
    int tube_p[2], tube_f[2];
    pid_t pid, wpid;

    char c;

    if(fichier == -1) {
        perror("Erreur match.txt");
        exit(EXIT_FAILURE);
    }

    if(pipe(tube_p) == -1 || pipe(tube_f) == -1 || (pid = fork()) == -1) {
        perror("Erreur");
        exit(errno);
    }

    if(pid == -1) {
        perror("Erreur");
    } else if(pid > 0) { // Père

        close(tube_f[ECRITURE]);
        close(tube_p[LECTURE]);

        for(int i = 0; i < max; i++) {
            write(fichier, ping.c_str(), ping.length());
            write(tube_p[ECRITURE], "a", 1);
            read(tube_f[LECTURE], &c, 1);
        }

        close(fichier);
        close(tube_f[ECRITURE]);
        wait(NULL);

    } else { // Fils

        close(tube_f[LECTURE]);
        close(tube_p[ECRITURE]);

        for(int i = 0; i < max; i++) {
            write(fichier, pong.c_str(), pong.length());
            write(tube_f[ECRITURE], "a", 1);
            read(tube_p[LECTURE], &c, 1);
        }

        close(fichier);
        close(tube_p[ECRITURE]);
        wait(NULL);
    }

    std::cout << "Fin de " << getpid() << endl;
    return EXIT_SUCCESS;
}
