#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <unistd.h>
#include <errno.h>

using std::cout;
using std::endl;
using std::string;

int main(void) {

    int tube[2];
    pid_t pid;
    string msg = "qrpuvssentr qr prggr cuenfr ra ebg13 !";
    char c;

    if( (pipe(tube) == -1) || (pid = fork()) == -1) {
        perror("Erreur :/");
        exit(errno);
    }

    if(pid > 0) { // Processus père

        // Fermeture de la lecture
        close(tube[0]);

        // On écrit le message dans le tube
        write(tube[1], msg.c_str(), msg.length());

        // Fermeture de l'écriture
        close(tube[1]);

        wait(pid);

    } else { // Processus fils

        // Fermeture de l'écriture
        close(tube[1]);

        // Lecture d'un caractère par caractère (0 indique que c == EOF)
        while( read(tube[0], &c, sizeof(char) ) != 0) {
            // Si c'est une lettre
            if(c >= 'a' && c <= 'z') {
                // Conversion d'une lettre vers un entier de longueur 3
                cout << ((c - 'a' + 13) % 26) + 'a';
            } else {
                cout << c;
            }
        }

        cout << endl;
    }

    cout << "Fin de " << getpid() << endl;
    return EXIT_SUCCESS;
}
