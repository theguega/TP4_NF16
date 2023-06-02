#include "tp4.h"
#include <stdio.h>

int main(void) {
  T_Index *index = NULL;
  // ============= MENU UTILISATEUR ============= //
  char choix = '0';
  while (choix != '7') {
    printf("\n======================================");
    printf("\n1. Charger un fichier");
    printf("\n2. Caractéristiques de l'index");
    printf("\n3. Afficher index");
    printf("\n4. Rechercher un mot");
    printf("\n5. Afficher les occurrences d’un mot");
    printf("\n6. Construire le texte à partir de l’index");
    printf("\n7. Quitter");
    printf("\n======================================");
    printf("\n   Votre choix ? ");
    choix = getchar();
    viderBuffer(); // supprimer de l'entrée standard le retour charriot et les
    // éventuels caractères supplémentaires tapés par
    // l'utilisateur

    switch (choix) {

    case '1':
      if (index == NULL) {
        printf("Entrez le nom du fichier : ");
        char filename[20];
        scanf("%s", filename);
        viderBuffer();
        index = creerT_Index(filename);
        if (indexerFichier(index, index->filename)) {
          printf("Votre index a bien ete cree, il contient %d mots\n\n",
                 index->nbMotsTotal);
        } else {
          printf("Erreur lors de l'indexation\n\n");
        }
      } else {
        printf("Vous ne pouvez pas indexer 2 fichiers en meme temps.\n\n");
      }
      break;

    case '2':
      if (index == NULL) {
        printf("Vous n'avez pas cree d'index.\n\n");
      } else {
        printf("L'index du fichier '%s' contient %d mots dont %d mots "
               "distincs\n\n",
               index->filename, index->nbMotsTotal, index->nbMotsDistincts);
      }
      break;

    case '3':
      if (index == NULL) {
        printf("Vous n'avez pas cree d'index.\n\n");
      } else {
        afficherIndex(*index);
      }
      break;

    case '4':
      if (index == NULL) {
        printf("Vous n'avez pas cree d'index.\n\n");
      } else {
        printf("Entrez le mot que vous recherchez : ");
        char mot_chercher[20];
        scanf("%s", mot_chercher);
        viderBuffer();
        afficherT_Noeud(rechercherMot(*index, mot_chercher));
        printf("\n\n");
      }
      break;

    case '5':
      if (index == NULL) {
        printf("Vous n'avez pas cree d'index.\n\n");
      } else {
        printf("Entrez le mot que vous voulez afficher : ");
        char mot_afficher[20];
        scanf("%s", mot_afficher);
        viderBuffer();
        afficherOccurencesMot(*index, mot_afficher);
        printf("\n\n");
      }
      break;

    case '6':
      if (index == NULL) {
        printf("Vous n'avez pas cree d'index.\n\n");
      } else {
        printf("Entrez le nom du fichier dans lequel vous voulez enregistrer "
               "votre index : ");
        char filename_2[20];
        scanf("%s", filename_2);
        viderBuffer();
        construireTexte(*index, filename_2);
      }
      break;

    case '7':
      printf("\n======== PROGRAMME TERMINE ========\n");
      supprimer_Arbre(index->racine);
      free(index);
      break;

    default:
      printf("\n\nERREUR : votre choix n'est valide ! ");
    }
  }
  return 0;
}

//Users/theoguegan/Documents/UTC/TC04/NF16/TPs/TP4/TP4/test.txt
