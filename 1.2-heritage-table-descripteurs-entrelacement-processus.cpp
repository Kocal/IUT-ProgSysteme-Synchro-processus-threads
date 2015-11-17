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

int main(void) {

    string ping = "Ping-";
    string pong = "Pong\n";

    int fichier = open("match.txt",  O_CREAT | O_TRUNC | O_WRONLY, 0755);
    pid_t pid = NULL;

    if(fichier == -1) {
        perror("Erreur match.txt");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 3; i++) {
        pid = fork();

        if( pid == -1 ) {
            perror("Erreur fork");
            exit(EXIT_FAILURE);
        } else if( pid > 0 ) { // Pere
            write(fichier, ping.c_str(), ping.length());
        } else { // Fils
            write(fichier, pong.c_str(), pong.length());
        }
    }

    close(fichier);
    cout << "Fin de " << getpid() << endl;
    return EXIT_SUCCESS;
}
