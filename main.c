#include "tp4.h"
#include <stdio.h>

int main(void) {
    //Nom du fichier
    char * filename = "/Users/theoguegan/Documents/UTC/TC04/NF16/TPs/TP4/TP4/test.txt";

    // Indexation d'un fichier :
      T_Index *index = creerT_Index(filename);
      if (indexerFichier(index, index->filename)) {
        printf("Votre fichier a bien ete indexé \n\n");
      } else {
        printf("Erreur lors de l'indexation\n\n");
      }

      afficherIndex(*index);

      afficherT_Noeud(rechercherMot(*index, "AHAH"));
      printf("\n");
      afficherT_Noeud(rechercherMot(*index, "Bonjour"));

      afficherOccurencesMot(*index, "arbre");

      construireTexte(filename);

  return 0;
}
