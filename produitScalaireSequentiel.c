#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TAILLE 100000000
#define VAL 1

int main(){
    int* vecteur1;
    int* vecteur2;
    clock_t start,end; //variables pour le calcul du temps d'exécution (unité considérée = impulsion)
    
    vecteur1=malloc(TAILLE*sizeof(int));
    vecteur2=malloc(TAILLE*sizeof(int));
    //initialisation des vecteurs
    for(int i=0;i<TAILLE;i++){
    	vecteur1[i]= VAL;
    	vecteur2[i]= VAL;
    }

    start=clock();//début du comptage 
    long res=0;
    //calcul du prduit scalaire
    for(int i=0;i<TAILLE;i++) res=res+vecteur1[i]*vecteur2[i];
    end=clock(); //fin du comptage

    printf("résultat = %ld \n",res);
    printf("Temps d'exécution: %ld impulsions\n",(end-start));

    free(vecteur1); free(vecteur2);
    return 0;
}


