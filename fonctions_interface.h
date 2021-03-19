#ifndef _FONCTIONSINTERFACE_
#define _FONCTIONSINTERFACE_

#include <ncurses.h> /* Pour toutes les fonctions/constantes ncurses */

/* Permet de convertir les '\n' en '.' */
char convertOctetInChar(char c);

/**
 * Cette fonction crée des fenêtres
 * @param[out] x la largeur de la fenêtre
 * @param[out] y la hauteur de la fenêtre
 * @param[out] start_x la colonne où démarre la fenêtre
 * @param[out] start_y la ligne où démarre la fenêtre
 * @param[out] message le message à écrire dans la fenêtre ( on part du principe que dans notre cas, le message n'est pas vide )
 * @param[out] scrollOK la capacité de scroll ou non dans la fenêtre (0 == FALSE et 1 == TRUE)
 */
WINDOW *initialisationFenetre(int x, int y, int start_x, int start_y, char message[128], int scrollOK);

/**
 * Remplit notre fenêtre caratère 
 * @param[out] fenetreHexa la fenêtre qu'on doit remplir
 * @param[out] x la position x de l'octet à surligner en rouge
 * @param[out] y la position y de l'octet à surligner en rouge
 */
void remplirFenetreHexa(WINDOW *fenetreHexa, char contenuBloc[128], int nBloc, int x, int y);

/**
 * Remplit notre fenêtre caratère 
 * @param[out] fenetreCara la fenêtre qu'on doit remplir
 * @param[out] x la position x de l'octet à surligner en rouge
 * @param[out] y la position y de l'octet à surligner en rouge
 */
void remplirFenetreCara(WINDOW *fenetreCara, char contenuBloc[128], int x, int y);

/* Permet d'actualiser les fenêtres, lorsque qu'on change de bloc */
void actualisationFenetre(char *buffer, int nBloc, WINDOW *fenetreHexa, WINDOW *fenetreCara);

#endif