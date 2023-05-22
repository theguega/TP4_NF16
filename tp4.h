#ifndef tp4_h
#define tp4_h

#define MAXP 20
//#define MAXF 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// * ---------- * //
// * Structures * //
// * ---------- * //

typedef struct Position {
  int numeroLigne;
  int ordre;
  int numeroPhrase;
  struct Position *suivant;
} T_Position;

// index par ordre alphabetique sous forme d'ABR
typedef struct Noeud {
  char *mot;
  int nbOccurences;
  T_Position *listePositions;
  struct Noeud *filsGauche;
  struct Noeud *filsDroit;
} T_Noeud;

typedef struct Index {
  T_Noeud *racine;
  int nbMotsDistincts;
  int nbMotsTotal;
  char *filename;
} T_Index;

// pile
typedef struct element_pile {
  struct Noeud *noeud;
  struct element_pile *suivant;
} element_pile;

typedef struct pile {
  struct element_pile *sommet;
  int taille;
} pile;

// index par ordre d'apparition sans doublon (mot ajouter plusieurs fois) sous
// forme de liste chainee
typedef struct mot {
  int indice;
  int nb_ligne;
  struct mot *suivant;
  char *mot;
} T_Mot;

typedef struct phrase {
  int indice;
  int nb_mot;
  struct phrase *suivante;
  T_Mot *premier_mot;
} T_Phrase;

typedef struct texte {
  T_Phrase *premiere_phrase;
} T_Texte;

typedef struct Index_ordre_texte {
  T_Phrase *racine;
} Index_ordre_texte;

// * --------------------- * //
// * Création de structure * //
// * --------------------- * //

T_Position *creerT_Position(int ligne, int ordre, int phrase);
T_Noeud *creerT_Noeud(char *mot, int ligne, int ordre, int phrase);
T_Index *creerT_Index(char *filename);
pile *creer_pile(void);
Index_ordre_texte *creerT_Index_ordre(void);
T_Mot *creerT_Mot(int indice, char *mot, int nb_ligne);
T_Phrase *creerT_Phrase(int indice);

// * --------------------- * //
// * Fonctions principales * //
// * --------------------- * //

T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre,
                            int phrase);
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre,
                     int phrase);
int indexerFichier(T_Index *index, char *filename);
T_Noeud *rechercherMot(T_Index index, char *mot);
void construireTexte(char *filename);

// * --------------------- * //
// * Fonctions D'Affichage * //
// * --------------------- * //

void afficherT_Position(T_Position *T);
void afficherListeT_Position(T_Position *liste);
void afficherT_Noeud(T_Noeud *n);
void afficherIndex(T_Index index);
void afficherOccurencesMot(T_Index index, char *mot);

// * ----------------- * //
// * Fonctions Annexes * //
// * ----------------- * //

pile *creer_pile(void);
element_pile *creer_element_pile(T_Noeud *noeud);
int pile_vide(pile *p);
T_Noeud *depiler(pile *p);
void empiler(pile *p, T_Noeud *noeud);

char *to_maj(char *mot);

void indexerFichier_ordreTexte(Index_ordre_texte *index, char *filename);

#endif /* structure_h */
