#ifndef _FONCTIONSFICHIER_
#define _FONCTIONSFICHIER_

/* Permet de vider une chaine de caractères */
void viderChaine(char *buffer);

/* Permet de déterminer la taille d'un fichier */
long donneTailleFichier(char *nomFichier);

/* Permet de déterminer le nombre de blocs d'un fichier de 128 octets*/
int donneNbBlocsAvecTaille(long tailleFichier);

/* Permet de lire les octets présents dans un nième Bloc d'un Fichier 
Et les renvoyer */
char *lireNBlocFicher(char *nomFichier, char *buffer, int X, int nBloc);

/* Permet de remplacer un nième octet d'un fichier par un caractère */
void remplacerNOctectFichier(char *nomFichier, int n, char cRemplacement);

#endif