#include "fonctions_fichier.h"
#include <fcntl.h>    /* Pour open */
#include <unistd.h>   /* Pour read - write*/
#include <sys/stat.h> /* Pour O_RDONLY */
#include <stdlib.h>   /* Pour exit, EXIT_FAILURE */
#include <stdio.h>    /* Pour printf */
#include "ncurses.h"  /* Fonctions Ncurses => pour le ncurses_stopper()*/

/*-----------------------------------------------------------------------------------------
------------------------- FONCTIONS MANIPULATION FICHIER ----------------------------------
-----------------------------------------------------------------------------------------*/

/* Permet de vider une chaine de caractères */
void viderChaine(char *buffer)
{
    int i;
    for (i = 0; i < 128; i++)
    {
        buffer[i] = '\0';
    }
}

/* Permet de déterminer la taille d'un fichier */
long donneTailleFichier(char *nomFichier)
{
    int fd;
    off_t size;

    /* Ouverture du fichier */
    fd = open(nomFichier, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur ouverture fichier ");
        exit(EXIT_FAILURE);
    }

    /* Déplacement au debut de fichier */
    size = lseek(fd, 0L, SEEK_END);
    if (size == (off_t)-1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur lors du deplacement dans le fichier ");
        exit(EXIT_FAILURE);
    }

    return (long)size;
}

/* Permet de déterminer le nombre de blocs d'un fichier de 128 octets*/
int donneNbBlocsAvecTaille(long tailleFichier)
{
    int nbBlocs;
    if (tailleFichier % 128 == 0)
        nbBlocs = tailleFichier / 128;
    else
        nbBlocs = tailleFichier / 128 + 1;

    return nbBlocs;
}

/* Permet de lire les octets présents dans un nième Bloc d'un Fichier 
Et les renvoyer */
char *lireNBlocFicher(char *nomFichier, char *buffer, int X, int nBloc)
{
    int i, res, fd;

    viderChaine(buffer);

    /* Ouverture du fichier */
    fd = open(nomFichier, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur ouverture fichier ");

        exit(EXIT_FAILURE);
    }

    /* Déplacement au debut de fichier */
    if (lseek(fd, 0L, SEEK_SET) == (off_t)-1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur lors du deplacement dans le fichier ");
        exit(EXIT_FAILURE);
    }

    /* Déplacement dans le fichier au nième bloc par X octets*/
    printf("\n");
    for (i = 1; i <= nBloc; i++)
    {
        if (i == nBloc)
        {
            /* Lis X octets du nbloc dans le fichier */
            res = read(fd, buffer, X);
            if (res == -1)
            {
                /* Arrêt de ncurses */
                ncurses_stopper();
                fprintf(stderr, "Erreur lors de la lecture ");
                perror(" ");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (lseek(fd, 128, SEEK_CUR) == (off_t)-1)
            {
                /* Arrêt de ncurses */
                ncurses_stopper();
                perror("Erreur lors du deplacement dans le fichier ");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* Fermeture du fichier */
    if (close(fd) == -1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur close ");
        exit(EXIT_FAILURE);
    }

    return &(buffer[0]);
}

/* Permet de remplacer un nième octet d'un fichier par un caractère */
void remplacerNOctectFichier(char *nomFichier, int n, char cRemplacement)
{
    int fd;

    /* Ouverture du fichier */
    fd = open(nomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur ouverture fichier ");
        exit(EXIT_FAILURE);
    }

    /* Déplacement au debut de fichier */
    if (lseek(fd, 0L, SEEK_SET) == (off_t)-1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur lors du deplacement dans le fichier ");
        exit(EXIT_FAILURE);
    }

    /* Déplacement dans le fichier au nième octet p*/
    if (lseek(fd, n, SEEK_CUR) == (off_t)-1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur lors du deplacement dans le fichier ");
        exit(EXIT_FAILURE);
    }

    /* Remplacement de l'octet dans le fichier par le caractère donné en paramètre de fonction */
    if (write(fd, &cRemplacement, 1) == -1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur write ");
        exit(EXIT_FAILURE);
    }
    /* Fermeture du fichier */
    if (close(fd) == -1)
    {
        /* Arrêt de ncurses */
        ncurses_stopper();
        perror("Erreur close ");
        exit(EXIT_FAILURE);
    }
}
