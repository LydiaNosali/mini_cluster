#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#define TAILLE 100000000
#define VAL 1 

int main(int argc,char **argv){
	
    int *vecteur1,*vecteur2;
    int rank,numtasks;
    long res=0,resFinal; //résultats partiels et résultat final
    clock_t start,end;


    vecteur1=malloc(TAILLE*sizeof(int));
    vecteur2=malloc(TAILLE*sizeof(int));

    //initialisation:
    for(int i=0;i<TAILLE;i++){ vecteur1[i]=VAL; vecteur2[i]=VAL; }
    
    start=clock(); //début comptage
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    //création de vecteurs partiels pour chaque processus ayant une taille identique (somme des tailles des vecteurs patiels=taille vecteur initial)
    int taillepart=TAILLE/numtasks; 
    int* vecteur1part=malloc(taillepart*sizeof(long));
    int* vecteur2part=malloc(taillepart*sizeof(long));
    
    //partager les valeurs du vecteur1 entre le vecteur1part de chaque processus créé, par le processus 0  
    MPI_Scatter(vecteur1,taillepart,MPI_INT,
    	        vecteur1part,taillepart,MPI_INT,0,MPI_COMM_WORLD);
    
    //partager les valeurs du vecteur2 entre le vecteur2part de chaque processus créé, par le processus 0 
    MPI_Scatter(vecteur2,taillepart,MPI_INT,
    	        vecteur2part,taillepart,MPI_INT,0,MPI_COMM_WORLD);
    
    //calcul d'un produit scalaire partiel par chaque processus
    for(int i=0;i<taillepart;i++) res=res+vecteur1part[i]*vecteur2part[i]; 
    
    //ajout des valeurs des vecteurs non distribuées par MPI_Scatter (cas où la taille des vecteurs n'est pas un multiple du nombre de processus)
    if((rank==0) && (TAILLE % numtasks!=0)){
          int cpt=taillepart*numtasks;
          while(cpt<TAILLE){
            res=res+vecteur1[cpt]*vecteur2[cpt];
            cpt++;
          }
      }
    
    MPI_Barrier(MPI_COMM_WORLD); //attendre le fin des calculs partiels de tous les processus

    MPI_Reduce(&res,&resFinal,1,MPI_LONG,MPI_SUM,0,MPI_COMM_WORLD); //sommer les produits scalaires partiels par le processus 0
    
    if(rank==0){
      end=clock(); //fin du comptage
      printf("le résultat est: %ld\n",resFinal);   
      printf("Temps d'exécution: %ld impulsions\n",end-start);
    }

    free(vecteur1); free(vecteur2); free(vecteur1part); free(vecteur2part);
	MPI_Finalize();   
}
