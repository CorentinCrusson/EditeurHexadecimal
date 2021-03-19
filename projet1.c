#include <stdlib.h>              /* Pour EXIT_FAILURE */
#include <stdio.h>               /* Pour printf */
#include <ncurses.h>             /* Pour printw, attron, attroff, COLOR_PAIR, getch */
#include "ncurses.h"             /* Fonctions Ncurses */
#include "fonctions_interface.h" /* Fonctions pour les fenêtres / l'interface */
#include "fonctions_fichier.h"   /* Fonctions pour les fichiers */
#include <string.h>              /* Pour strlen */

#define LARGEUR 8     /* Largeur de la fenêtre */
#define HAUTEUR 16    /* Hauteur de la fenêtre */
#define KEY_ENTREE 10 /* Touche Entree */
#define KEY_SUPPR 330 /* Touche Suppr */

/**
 * Définition de la palette
 */
void palette()
{
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
}

int main(int argc, char *argv[])
{
  int sourisX, sourisY, bouton, ch, blocActuel, nbBlocs, rangOctet, modificationEnCours, oldCh;

  char *nomFichier;
  char *buffer;

  size_t tailleNomFichier;

  WINDOW *fenetreInformation, *fenetreHexa, *fenetreCara;
  WINDOW *sous_fenetreInformation;

  /*-----------------------------------------------------------------------------------------
  ---------------------------- INITIALISATION NCURSES ---------------------------------------
  -----------------------------------------------------------------------------------------*/
  ncurses_initialiser();
  ncurses_souris();
  ncurses_couleurs();
  palette();

  /*-----------------------------------------------------------------------------------------
  -------------------- SAUVEGARDE DU NOM DU FICHIER EN MEMOIRE ----------------------------
  -----------------------------------------------------------------------------------------*/

  /* Vérification et récupération des arguments */
  if (argc != 2)
  {
    /* Vérification et récupération des arguments */
    fprintf(stderr, "Usage: %s nomFichier\n", argv[0]);
    fprintf(stderr, "\t\tnomFichier : nom du fichier à ouvrir\n");

    /* Arrêt de ncurses */
    ncurses_stopper();

    exit(EXIT_FAILURE);
  }

  /* Calcul de la taille à allouer */
  tailleNomFichier = strlen(argv[1]) + 1;

  /* Allocation de la mémoire */
  nomFichier = malloc(tailleNomFichier * sizeof(char));

  /* On attribue le nom du fichier avec le nom passé en paramètre du programme */
  strcpy(nomFichier, argv[1]);

  /*-----------------------------------------------------------------------------------------
  ------------------------- VERIFICATION DIMENSIONS TERMINAL --------------------------------
  -----------------------------------------------------------------------------------------*/

  /* On vérifie les dimensions du terminal pour qu'il soit compatible avec notre projet */
  if ((COLS < (LARGEUR * 6 - 3)) || (LINES < (LARGEUR + HAUTEUR + 2)))
  {
    ncurses_stopper();
    fprintf(stderr, "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n", COLS, LINES, (LARGEUR * 6 - 3), (LARGEUR + HAUTEUR + 2));
    exit(EXIT_FAILURE);
  }

  /*-----------------------------------------------------------------------------------------
  -------------------- LECTURE DU 1ER BLOC du FICHIER ---------------------------------------
  -----------------------------------------------------------------------------------------*/

  /* Allocation de la mémoire du Buffer */
  buffer = malloc(128 * sizeof(char));

  /* Calcul du Nombre de Blocs de notre Fichier*/
  nbBlocs = donneNbBlocsAvecTaille(donneTailleFichier(nomFichier));

  /* Bloc de Fichier mis sur 1 par défault */
  blocActuel = 1;

  /* On stocke le contenu du fichier pour le bloc n°1 dans un buffer */
  lireNBlocFicher(nomFichier, buffer, 128, blocActuel);

  /*-----------------------------------------------------------------------------------------
  -------------------------------- CRÉATION INTERFACE ---------------------------------------
  -----------------------------------------------------------------------------------------*/

  /* Initialisation de nos fenêtres avec leurs différentes tailles
  La sous-fenêtre ne nécessitait pas l'écriture d'une fonction */
  fenetreInformation = initialisationFenetre(COLS - 2, 8, 0, 0, "Informations : ", 0);
  sous_fenetreInformation = subwin(fenetreInformation, 6, COLS - 5, 1, 1);
  fenetreHexa = initialisationFenetre((LARGEUR * 4) + 2, HAUTEUR + 2, 0, 8, "Hexa ", 0);
  fenetreCara = initialisationFenetre(LARGEUR + 2, HAUTEUR + 2, (LARGEUR * 4) + 2 + 1, 8, "Car. ", 0);

  /* On active la possibilité de scroll dans notre sous_fenêtre information */
  scrollok(sous_fenetreInformation, TRUE);

  /* On remplit nos fenêtres */
  actualisationFenetre(buffer, blocActuel, fenetreHexa, fenetreCara);

  /* On rafraichit notre fenêtre information et notre sous-fenêtre information 
  Les fenêtres Hexa et Cara sont déjà rafraichis lorsque qu'on les actualise */
  wrefresh(fenetreInformation);
  wrefresh(sous_fenetreInformation);

  /*-----------------------------------------------------------------------------------------
  ----------------------------------- BOUCLE PRINCIPALE -------------------------------------
  -----------------------------------------------------------------------------------------*/
  /* En attente d'interraction */
  rangOctet = -1;
  modificationEnCours = -1;
  oldCh = 0;
  while ((ch = getch()) != KEY_F(2))
  {
    switch (ch)
    {
    case KEY_UP:
      if (blocActuel - 1 > 0)
        blocActuel -= 1;

      lireNBlocFicher(nomFichier, buffer, 128, blocActuel);
      actualisationFenetre(buffer, blocActuel, fenetreHexa, fenetreCara);
      break;

    case KEY_DOWN:
      if (blocActuel + 1 <= nbBlocs)
        blocActuel += 1;

      lireNBlocFicher(nomFichier, buffer, 128, blocActuel);
      actualisationFenetre(buffer, blocActuel, fenetreHexa, fenetreCara);
      break;

    case KEY_ENTREE:
      if (rangOctet != -1)
      {
        if (modificationEnCours == -1)
        {
          wprintw(sous_fenetreInformation, "Vous êtes entrain de modifier l'octet n° %d. Entrez un caractère : \n", rangOctet);
          modificationEnCours = 0;
          oldCh = ch;

          wrefresh(sous_fenetreInformation);
          wrefresh(fenetreInformation);
        }
      }
      break;

    case KEY_SUPPR:
      if (rangOctet != -1)
      {
        wprintw(sous_fenetreInformation, "Vous venez de supprimer un octet à la position (%d,%d)\n", sourisX, sourisY);
        remplacerNOctectFichier(nomFichier, rangOctet, '\0');

        lireNBlocFicher(nomFichier, buffer, 128, blocActuel);
        actualisationFenetre(buffer, blocActuel, fenetreHexa, fenetreCara);
        wrefresh(fenetreInformation);
        wrefresh(sous_fenetreInformation);
      }
      break;

    case KEY_MOUSE:
      if (souris_getpos(&sourisX, &sourisY, &bouton) == OK)
      {
        if (bouton & BUTTON1_CLICKED)
        {
          /*wprintw(sous_fenetreInformation, "Clic bouton à la position (%d, %d)\n", sourisX, sourisY - 1);*/

          /**
             * On affiche en rouge l'élément séléctionné 
             * Si la souris en position X est supérieur à 36 (LARGEUR * 4 + 4) alors nous sommes dans la fenêtre "Car." 
             * Sinon si la souris en position X est inférieur à 32 ( LARGEUR * 4) alors nous sommes dans la fenêtre "Hexa"
             */
          if (sourisX >= (LARGEUR * 4 + 4) && sourisX <= (LARGEUR * 4 + 4 + (LARGEUR - 1))) /* Fenêtre Car. */
          {
            remplirFenetreHexa(fenetreHexa, buffer, blocActuel, ((sourisX - (LARGEUR * 4 + 4)) * 3 + 10), sourisY - 1);
            remplirFenetreCara(fenetreCara, buffer, sourisX, sourisY - 1);
            rangOctet = (sourisX - (LARGEUR * 4 + 4) + 1) + (LARGEUR * ((sourisY - 1) - LARGEUR)) + (128 * (blocActuel - 1)) - 1; /* -1 car on commence à l'octet 0 */
            wprintw(sous_fenetreInformation, "Octet Selectionné : %d\n", rangOctet);
          }
          /* Si la positionX est un multiple de 3 alors nous sommes pas entrain de séléctionner un octet mais un espace 
              donc on bloque le remplissage de la fenêtre Car. */
          else if (sourisX <= LARGEUR * 4 && sourisX > 8 && sourisX % 3 != 0) /* Fenêtre Hexa. */
          {
            remplirFenetreHexa(fenetreHexa, buffer, blocActuel, sourisX, sourisY - 1);
            remplirFenetreCara(fenetreCara, buffer, ((LARGEUR * 4 + 4) + ((sourisX - 10) / 3)), sourisY - 1);
            rangOctet = (((sourisX - 10) / 3) + 1) + (LARGEUR * ((sourisY - 1) - LARGEUR)) + (128 * (blocActuel - 1)) - 1; /* -1 car on commence à l'octet 0 */
            wprintw(sous_fenetreInformation, "Octet Selectionné : %d\n", rangOctet);
          }
          else
          {
            modificationEnCours = -1;
            oldCh = 0;
          }
        }
      }

      /* On rafraichit nos fenêtres */
      wrefresh(fenetreInformation);
      wrefresh(sous_fenetreInformation);
      wrefresh(fenetreHexa);
      wrefresh(fenetreCara);
      break;

    default:
      if (modificationEnCours == 0 && ch != oldCh)
      {
        wprintw(sous_fenetreInformation, "Modification Effectuée sur l'octet n°%d ! \n", rangOctet);
        remplacerNOctectFichier(nomFichier, rangOctet, ch);

        lireNBlocFicher(nomFichier, buffer, 128, blocActuel);
        actualisationFenetre(buffer, blocActuel, fenetreHexa, fenetreCara);

        wrefresh(fenetreInformation);
        wrefresh(sous_fenetreInformation);
        modificationEnCours = -1;
      }
      break;
    }
  }

  /*-----------------------------------------------------------------------------------------
  ----------------------------- EXTINCTION DU PROGRAMME -------------------------------------
  -----------------------------------------------------------------------------------------*/

  /* Suppression des fenêtres */
  delwin(fenetreInformation);
  delwin(sous_fenetreInformation);
  delwin(fenetreHexa);
  delwin(fenetreCara);

  /* On libère la mémoire de nos pointeurs après utilisation */
  free(nomFichier);
  free(buffer);

  /* On ferme la fenêtre principale */
  endwin();

  /* Arrêt de ncurses */
  ncurses_stopper();

  return EXIT_SUCCESS;
}
