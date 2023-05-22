#include "tp4.h"
#define TAILLE_LIGNE 1000
#define TAILLE_MOT 20

// * --------------------- * //
// * Création de structure * //
// * --------------------- * //

T_Position *creerT_Position(int ligne, int ordre, int phrase) {

  T_Position *new_Position = malloc(sizeof(T_Position));
  new_Position->numeroLigne = ligne;
  new_Position->ordre = ordre;
  new_Position->numeroPhrase = phrase;
  new_Position->suivant = NULL;

  return new_Position;
}

T_Noeud *creerT_Noeud(char *mot, int ligne, int ordre, int phrase) {

  char *mot_maj = malloc(sizeof(strlen(mot)) + 1);
  mot_maj = to_maj(mot);

  // creation du noeud
  T_Noeud *nouveau_noeud = malloc(sizeof(T_Noeud));
  nouveau_noeud->filsDroit = NULL;
  nouveau_noeud->filsGauche = NULL;
  nouveau_noeud->mot = mot_maj;
  nouveau_noeud->nbOccurences = 1;
  nouveau_noeud->listePositions = creerT_Position(ligne, ordre, phrase);

  return nouveau_noeud;
}

T_Index *creerT_Index(char *filename) {
  T_Index *nouvel_index = malloc(sizeof(T_Index));
  nouvel_index->racine = NULL;
  nouvel_index->nbMotsDistincts = 0;
  nouvel_index->nbMotsTotal = 0;
  nouvel_index->filename = filename;
  return nouvel_index;
}

Index_ordre_texte *creerT_Index_ordre(void) {
  Index_ordre_texte *index = malloc(sizeof(Index_ordre_texte));
  index->racine = NULL;
  return index;
}

T_Phrase *creerT_Phrase(int indice) {
  T_Phrase *phrase = malloc(sizeof(T_Phrase));
  phrase->premier_mot = NULL;
  phrase->indice = indice;
  phrase->nb_mot = 0;
  phrase->suivante = NULL;
  return phrase;
}

T_Mot *creerT_Mot(int indice, char *mot, int nb_ligne) {
  T_Mot *nouveau_mot = malloc(sizeof(T_Mot));
  nouveau_mot->indice = indice;
  nouveau_mot->suivant = NULL;
  nouveau_mot->nb_ligne = nb_ligne;
  nouveau_mot->mot = malloc(strlen(mot) + 1);
  strcpy(nouveau_mot->mot, mot);
  return nouveau_mot;
}

// * --------------------- * //
// * Fonctions principales * //
// * --------------------- * //

T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre,
                            int phrase) {

  // Conservation de la tête :

  // Création de la nouvelle position
  T_Position *new_Position = creerT_Position(ligne, ordre, phrase);

  T_Position *pred = malloc(sizeof(T_Position));
  pred = NULL;

  T_Position *last = malloc(sizeof(T_Position));
  last = listeP;

  while (last != NULL) {
    // si la position existe déjà, pas d'ajout
    if (last->numeroLigne == new_Position->numeroLigne &&
        last->numeroPhrase == new_Position->numeroPhrase &&
        last->ordre == new_Position->ordre) {
      printf("Ajout impossible. Position déjà existante");
      return listeP;
    }

    if (last->numeroLigne == new_Position->numeroLigne &&
        last->ordre > new_Position->ordre) {
      if (pred == NULL) { // si c'est la première position, je modifie le head
                          // et le new.suivant
        new_Position->suivant = last;
        listeP = new_Position;
        return listeP;
      } else {
        new_Position->suivant = last;
        pred->suivant = new_Position;
        return listeP;
      }
    }

    if (last->numeroLigne > new_Position->numeroLigne) {
      if (pred == NULL) { // si c'est la première position, je modifie le head
                          // et le new.suivant
        new_Position->suivant = last;
        listeP = new_Position;
        return listeP;
      } else {
        new_Position->suivant = last;
        pred->suivant = new_Position;
        return listeP;
      }
    }

    pred = last;
    last = last->suivant;
  }
  pred->suivant = new_Position; // si pas encore ajouté, on l'ajoute à la fin
  return listeP;
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre,
                     int phrase) {

  // on commence par creer le noeud a ajouter
  T_Noeud *nouveau_noeud = malloc(sizeof(T_Noeud));
  nouveau_noeud = creerT_Noeud(mot, ligne, ordre, phrase);
  // creation des noeuds temporaire
  T_Noeud *noeud_prec = malloc(sizeof(T_Noeud)); // noeud precedent
  noeud_prec = NULL;
  T_Noeud *noeud_actuel = malloc(sizeof(T_Noeud)); // noeud actuel
  noeud_actuel = index->racine;

  // si l'arbre etait vide :
  if (noeud_actuel == NULL) {
    index->nbMotsDistincts++;
    index->nbMotsTotal++;
    index->racine = nouveau_noeud;
    return 1;
  }

  while (noeud_actuel != NULL) {
    noeud_prec = noeud_actuel; // on parcourt jusqu'a aller à un noeud vide
    // si notre mot est situe avant notre mot actuel, on va a gauche, sinon a
    // droite
    // si le mot existe deja on s'arrete aussi
    if (strcmp(nouveau_noeud->mot, noeud_actuel->mot) == 0) {
      // on rajoute une occurence
      noeud_prec->nbOccurences++;
      // mot en plus dans l'index
      index->nbMotsTotal++;
      // on actualise la liste des positions avec la nouvelle position
      noeud_prec->listePositions =
          ajouterPosition(noeud_prec->listePositions, ligne, ordre, phrase);
      // les valeurs du nouveau noeud ont ete inserrer, on librer la memoire
      free(nouveau_noeud);
      return 1;
    } else if (strcmp(noeud_actuel->mot, nouveau_noeud->mot) > 0) {
      noeud_actuel = noeud_actuel->filsGauche;
    } else {
      noeud_actuel = noeud_actuel->filsDroit;
    }
  }

  // si le mot est avant le dernier noeud, on l'ajoute a gauche
  if (strcmp(nouveau_noeud->mot, noeud_prec->mot) < 0) {
    // nouveau mot dans l'index
    index->nbMotsDistincts++;
    index->nbMotsTotal++;
    noeud_prec->filsGauche = nouveau_noeud;
    return 1;
  }

  // sinon a droite
  else {
    // nouveau mot dans l'index<<<
    index->nbMotsDistincts++;
    index->nbMotsTotal++;
    noeud_prec->filsDroit = nouveau_noeud;
    return 1;
  }

  return 0;
}

int indexerFichier(T_Index *index, char *filename) {
  // initialisation des compteurs
  int compteur_mot = 0;
  int compteur_ordre = 0;
  int compteur_ligne = 0;
  int compteur_phrase = 0;

  FILE *fichier = NULL;
  char ligne[TAILLE_LIGNE] = "";

  // ouverture du fichier

  fichier = fopen(filename, "r");

  if (fichier != NULL) {
    // si le fichier n'est pas vide, il y a au moins 1 ligne et 1 mots, on passe
    // a 1
    compteur_ligne++;
    compteur_phrase++;

    // la boucle while balaye chaque ligne grace a fgets
    while (fgets(ligne, TAILLE_LIGNE, fichier) != NULL) {
      // si la ligne n'est pas vide il y a au moins 1 mots
      compteur_ordre++;
      // creation du premier mot (vide) et du compteur k pour chaque lettre de
      // ce mot
      char mot[TAILLE_MOT] = "";
      int k = 0;
      // parcourt de la ligne
      for (int i = 0; i < strlen(ligne); i++) {

        char c = ligne[i];

        // si le caractere n'est ni un espace, un point, ou un retour chariot,
        // c'est une lettre de notre mot
        if (c != ' ' && c != '.' && c != '\n') {
          mot[k] = c;
          k++;
        }

        // si il est juste different de retour chariot c'est un espace ou un
        // point, il faut l'ajouter a notre arbre
        else {
          if (strlen(mot) > 0) {
            // on l'ajoute alors
            ajouterOccurence(index, mot, compteur_ligne, compteur_ordre,
                             compteur_phrase);

            compteur_mot++; // on incrémente le nombre de mot lu.

            // on reinitialise notre mot
            for (k = 0; k < TAILLE_MOT; k++) {
              mot[k] = '\0';
            }
            k = 0;

            // incrementation du compteur de mot
            compteur_ordre++;
          }
        }

        // Si le caractère lu est un point, on change de phrase
        if (c == '.') {
          compteur_phrase++;
        }
      }
      // incrementation de la ligne, retour a 0 du nombre de mot
      compteur_ligne++;
      compteur_ordre = 0;
    }
  }
  // fermeture du fichier
  fclose(fichier);
  return compteur_mot;
}

T_Noeud *rechercherMot(T_Index index, char *mot) {
  // on passe le mot en majuscule
  char *mot_maj = malloc(sizeof(strlen(mot)) + 1);
  mot_maj = to_maj(mot);

  // recherche dans l'arbre
  T_Noeud *n = index.racine;
  while (n != NULL && strcmp(n->mot, mot_maj) != 0) {
    if (strcmp(mot_maj, n->mot) < 0)
      n = n->filsGauche;
    else
      n = n->filsDroit;
  }
  return n;
}

// * --------------------- * //
// * Fonctions D'Affichage * //
// * --------------------- * //

void afficherT_Position(T_Position *pos) {
  if (pos == NULL) {
    printf("Position : NULL");
  } else {
    printf("|---(l:%d, o:%d, p:%d)", pos->numeroLigne, pos->ordre,
           pos->numeroPhrase);
  }
}

void afficherListeT_Position(T_Position *liste) {
  if (liste == NULL) {
    printf("Liste : NULL");
    return;
  } else {
    T_Position *last = liste;
    while (last != NULL) {
      afficherT_Position(last);
      printf("\n");
      last = last->suivant;
    }
  }
  return;
}

void afficherT_Noeud(T_Noeud *n) {
  if (n == NULL) {
    printf("Noeud : NULL\n");
    return;
  } else {
    printf("|--%s\n", n->mot);
    afficherListeT_Position(n->listePositions);
    printf("|\n");
  }
  return;
}

void afficherIndex(T_Index index) {
  // Parcour d'un arbre par methode infixe iterative
  char lettre_prec = '!';
  // Informations :
  printf("Votre indexe contient %d mots distincts et %d au total.\n\n",
         index.nbMotsDistincts, index.nbMotsTotal);

  // Parcour de l'index
  T_Noeud *n = index.racine;
  pile *p = creer_pile();
  // On parcour tant que le noeud n'est pas vide ou que la pile n'est pas vide
  while (n != NULL || pile_vide(p) == 0) {
    // on empile d'abord tout ce qui se trouve à gauche
    while (n != NULL) {
      empiler(p, n);
      n = n->filsGauche;
    }

    // gestion de l'affichage
    n = depiler(p);
    if (n->mot[0] != lettre_prec) {
      printf("\n%c\n", n->mot[0]);
      afficherT_Noeud(n);
      lettre_prec = n->mot[0];
    } else {
      afficherT_Noeud(n);
    }

    // puis on se deplace à droite et in recommence
    n = n->filsDroit;
  }

  printf("\n\n");
  return;
}

void afficherOccurencesMot(T_Index index, char *mot) {
  // on recupere l'adresse du mot et on initialise nos tableaux de position
  T_Noeud *n = rechercherMot(index, mot);
    if (n!=NULL) {
        int *tab_phrase = malloc(n->nbOccurences * sizeof(int));
        int *tab_ordre = malloc(n->nbOccurences * sizeof(int));
        int *tab_ligne = malloc(n->nbOccurences * sizeof(int));
        
        // reuperation des positions
        T_Position *tmp = n->listePositions;
        for (int i = 0; i < n->nbOccurences; i++) {
            tab_ordre[i] = tmp->ordre;
            tab_phrase[i] = tmp->numeroPhrase;
            tab_ligne[i] = tmp->numeroLigne;
            tmp = tmp->suivant;
        }
        
        // indexation par ordre de phrase :
        Index_ordre_texte *index_ordre = malloc(sizeof(Index_ordre_texte));
        index_ordre = creerT_Index_ordre();
        indexerFichier_ordreTexte(index_ordre, index.filename);
        
        // affichage :
        printf("\nMot = \"%s\"\n", n->mot);
        printf("Occurences = %d\n", n->nbOccurences);
        for (int i = 0; i < n->nbOccurences; i++) {
            // positionnement dans l'index :
            T_Phrase *phrase = index_ordre->racine;
            T_Mot *mot_liste = NULL;
            for (int j = 1; j < tab_phrase[i]; j++) {
                phrase = phrase->suivante;
            }
            
            printf("| Ligne %d, mot %d :", tab_ligne[i], tab_ordre[i]);
            
            // affichage de la phrase :
            mot_liste = phrase->premier_mot;
            for (int k = 0; k < phrase->nb_mot; k++) {
                printf(" %s", mot_liste->mot);
                mot_liste = mot_liste->suivant;
            }
            printf(".\n");
        }
        printf("\n");
    } else {
        printf("Le mot n'existe pas dans le texte");
    }
  return;
}

void construireTexte(char *filename) {
  Index_ordre_texte *index = malloc(sizeof(Index_ordre_texte));
  index = creerT_Index_ordre();
  indexerFichier_ordreTexte(index, filename);

  T_Phrase *phrase = index->racine;
  T_Mot *mot_liste = NULL;

  // reference de phrase et de ligne
  int ligne_actuelle = -1;

  // Affichage de l'index
  while (phrase != NULL && phrase->nb_mot != 0) {
    mot_liste = phrase->premier_mot;

    for (int i = 0; i < phrase->nb_mot; i++) {
      // si la ligne change, on print un retour chariot
      if (ligne_actuelle != mot_liste->nb_ligne) {
        printf("\n");
        ligne_actuelle = mot_liste->nb_ligne;
      } else {
        // sinon juste un espace
        printf(" ");
      }
      // affichage du mot
      printf("%s", mot_liste->mot);
      mot_liste = mot_liste->suivant;
    }

    // changement de phrase
    printf(".");
    phrase = phrase->suivante;
  }
  printf("\n");
  return;
}

// * ----------------- * //
// * Fonctions Annexes * //
// * ----------------- * //

// * Piles * //

pile *creer_pile(void) {
  pile *pile = malloc(sizeof(pile));

  if (pile != NULL) {
    pile->sommet = NULL;
    pile->taille = 0;
    return pile;
  }
  return NULL;
}

element_pile *creer_element_pile(T_Noeud *n) {
  element_pile *element = malloc(sizeof(element_pile));
  if (element != NULL) {
    element->noeud = n;
    element->suivant = NULL;
    return element;
  }
  return NULL;
}

int pile_vide(pile *p) {
  if (p->taille == 0)
    return 1;
  else
    return 0;
}

T_Noeud *depiler(pile *p) {
  if (pile_vide(p))
    return NULL;
  else {
    T_Noeud *sortie = p->sommet->noeud;
    p->sommet = p->sommet->suivant;
    p->taille--;
    return sortie;
  }
  return NULL;
}

void empiler(pile *p, T_Noeud *n) {
  if (pile_vide(p)) {
    p->sommet = creer_element_pile(n);
    p->sommet->suivant = NULL;
    p->taille++;
  } else {
    element_pile *e = creer_element_pile(n);
    e->suivant = p->sommet;
    p->sommet = e;
    p->taille++;
  }
}

char *to_maj(char *mot) {
  char *mot_maj = malloc(sizeof(strlen(mot)) + 1);
  strcpy(mot_maj, mot);
  // passage du mot en majuscule
  for (int i = 0; mot_maj[i] != '\0'; i++) {
    if (mot_maj[i] >= 'a' && mot_maj[i] <= 'z') {
      mot_maj[i] -= 32;
    }
  }
  return mot_maj;
}

void indexerFichier_ordreTexte(Index_ordre_texte *index, char *filename) {
  T_Phrase *phrase = NULL;
  T_Mot *mot_liste = NULL;

  // initialisation des compteurs
  int compteur_ordre = 0;
  int compteur_ligne = 0;
  int compteur_phrase = 0;

  FILE *fichier = NULL;
  char ligne[TAILLE_LIGNE] = "";

  // ouverture du fichier

  fichier = fopen(filename, "r");

  if (fichier != NULL) {
    // si le fichier n'est pas vide, il y a au moins 1 ligne et 1 mots, on
    // passe a 1
    compteur_ligne++;
    compteur_phrase++;

    // la boucle while balaye chaque ligne grace a fgets
    while (fgets(ligne, TAILLE_LIGNE, fichier) != NULL) {
      // si la ligne n'est pas vide il y a au moins 1 mots
      compteur_ordre++;
      // creation du premier mot (vide) et du compteur k pour chaque lettre de
      // ce mot
      char mot[TAILLE_MOT] = "";
      int k = 0;
      // parcourt de la ligne
      for (int i = 0; i < strlen(ligne); i++) {

        char c = ligne[i];

        // si le caractere n'est ni un espace, un point, ou un retour chariot,
        // c'est une lettre de notre mot
        if (c != ' ' && c != '.' && c != '\n') {
          mot[k] = c;
          k++;
        }

        // si il est juste different de retour chariot c'est un espace ou un
        // point, il faut l'ajouter a notre liste
        else {
          if (strlen(mot) > 0) {
            // on l'ajoute alors

            // si la liste est vide :
            if (index->racine == NULL) {
              index->racine = creerT_Phrase(compteur_phrase);
              phrase = index->racine;
            }
            // si la phrase est vide, on l'ajoute en tete
            if (phrase->premier_mot == NULL) {
              phrase->premier_mot =
                  creerT_Mot(compteur_ordre, mot, compteur_ligne);
              phrase->nb_mot++;
              mot_liste = phrase->premier_mot;
            } else {
              // sinon on l'ajoute à la suite :
              mot_liste->suivant =
                  creerT_Mot(compteur_ordre, mot, compteur_ligne);
              mot_liste = mot_liste->suivant;
              phrase->nb_mot++;
            }

            // on reinitialise notre mot
            for (k = 0; k < TAILLE_MOT; k++) {
              mot[k] = '\0';
            }
            k = 0;

            // incrementation du compteur de mot
            compteur_ordre++;
          }
        }

        // Si le caractère lu est un point, on change de phrase
        if (c == '.') {
          compteur_phrase++;
          phrase->suivante = creerT_Phrase(compteur_phrase);
          phrase = phrase->suivante;
        }
      }
      // incrementation de la ligne, retour a 0 du nombre de mot
      compteur_ligne++;
      compteur_ordre = 0;
    }
  }
  // fermeture du fichier
  fclose(fichier);
  return;
}
