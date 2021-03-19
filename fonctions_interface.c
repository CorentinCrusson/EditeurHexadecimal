#include "fonctions_interface.h"

#include <ncurses.h> /* Pour toutes les fonctions/constantes ncurses */
#include <stdlib.h>  /* Pour exit, EXIT_FAILURE */

#define LARGEUR 8  /* Largeur de la fenêtre */
#define HAUTEUR 16 /* Hauteur de la fenêtre */

/*-----------------------------------------------------------------------------------------
------------------------- FONCTIONS MANIPULATION FENÊTRE ----------------------------------
-----------------------------------------------------------------------------------------*/

/* Permet de convertir les '\n' en '.' */
char convertOctetInChar(char c)
{
    if (c != '\n' && c != '\0')
    {
        return c;
    }
    else
    {
        return '.';
    }
}

/**
 * Cette fonction crée des fenêtres
 * @param[out] x la largeur de la fenêtre
 * @param[out] y la hauteur de la fenêtre
 * @param[out] start_x la colonne où démarre la fenêtre
 * @param[out] start_y la ligne où démarre la fenêtre
 * @param[out] message le message à écrire dans la fenêtre ( on part du principe que dans notre cas, le message n'est pas vide )
 * @param[out] scrollOK la capacité de scroll ou non dans la fenêtre (0 == FALSE et 1 == TRUE)
 */
WINDOW *initialisationFenetre(int x, int y, int start_x, int start_y, char message[128], int scrollOK)
{
    WINDOW *fenetre;
    /* Création de la fenêtre */
    fenetre = newwin(y, x, start_y, start_x);

    /* Attribut de Scroll ou non */
    if (scrollOK == 1)
    {
        scrollok(fenetre, TRUE);
    }

    /* Création de la bordure */
    box(fenetre, 0, 0);

    /* On nomme la fenêtre au niveau de l'interface */
    mvwprintw(fenetre, 0, 2, message);

    return fenetre;
}

/**
 * Remplit notre fenêtre caratère 
 * @param[out] fenetreHexa la fenêtre qu'on doit remplir
 * @param[out] x la position x de l'octet à surligner en rouge
 * @param[out] y la position y de l'octet à surligner en rouge
 */
void remplirFenetreHexa(WINDOW *fenetreHexa, char contenuBloc[128], int nBloc, int x, int y)
{
    int i, j, cpt;
    for (i = 0; i < HAUTEUR; i++)
    {
        mvwprintw(fenetreHexa, (i + 1), 1, "%.8X", (i * 8 + 128 * (nBloc - 1)));
        for (j = 8, cpt = (i * 8); j < LARGEUR * 4; j += 3, cpt++)
        {
            if (x >= (j + 2) && x <= (j + 3) && i == (y - 8))
            {
                wattron(fenetreHexa, COLOR_PAIR(2)); /* On rajoute l'attribut Couleur Rouge */
                mvwprintw(fenetreHexa, (i + 1), (j + 1), " %.2X", contenuBloc[cpt]);
                wattroff(fenetreHexa, COLOR_PAIR(2)); /* On enlève l'attribut Couleur Rouge */
            }
            else
            {
                mvwprintw(fenetreHexa, (i + 1), (j + 1), " %.2X", contenuBloc[cpt]); /* On précise bien %.2X qui permet de changer les affichages : "a" en "0a" */
            }
        }
    }
}

/**
 * Remplit notre fenêtre caratère 
 * @param[out] fenetreCara la fenêtre qu'on doit remplir
 * @param[out] x la position x de l'octet à surligner en rouge
 * @param[out] y la position y de l'octet à surligner en rouge
 */
void remplirFenetreCara(WINDOW *fenetreCara, char contenuBloc[128], int x, int y)
{
    int i, j, cpt;
    for (i = 0, cpt = 0; i < HAUTEUR; i++)
    {
        for (j = 0; j < LARGEUR; j++, cpt++)
        {
            if (j == (x - (LARGEUR * 4 + 4)) && i == (y - 8))
            {
                wattron(fenetreCara, COLOR_PAIR(2)); /* On rajoute l'attribut Couleur Rouge */
                mvwprintw(fenetreCara, (i + 1), (j + 1), "%c", convertOctetInChar(contenuBloc[cpt]));
                wattroff(fenetreCara, COLOR_PAIR(2)); /* On enlève l'attribut Couleur Rouge */
            }
            else
            {
                mvwprintw(fenetreCara, (i + 1), (j + 1), "%c", convertOctetInChar(contenuBloc[cpt]));
            }
        }
    }
}

/* Permet d'actualiser les fenêtres, lorsque qu'on change de bloc */
void actualisationFenetre(char *buffer, int nBloc, WINDOW *fenetreHexa, WINDOW *fenetreCara)
{
    remplirFenetreHexa(fenetreHexa, buffer, nBloc, 0, 0);
    wrefresh(fenetreHexa);
    remplirFenetreCara(fenetreCara, buffer, 0, 0);
    wrefresh(fenetreCara);
}