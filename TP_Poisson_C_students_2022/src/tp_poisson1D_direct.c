/******************************************/
/* tp2_poisson1D_direct.c                 */
/* This file contains the main function   */
/* to solve the Poisson 1D problem        */
/******************************************/
#include "lib_poisson1D.h"
#include "atlas_headers.h"
#include<time.h>

int main(int argc,char *argv[])
/* ** argc: Nombre d'arguments */
/* ** argv: Valeur des arguments */
{
  int ierr;
  int jj;
  int nbpoints, la;
  int ku, kl, kv, lab;
  int *ipiv;
  int info;
  int NRHS;
  double T0, T1;
  double *RHS, *RHS1, *EX_SOL, *X;
  double **AAB;
  double *AB;

  double temp, relres;

  NRHS=1;
  nbpoints=10;
  la=nbpoints-2;
  T0=-5.0;
  T1=5.0;

  printf("--------- Poisson 1D ---------\n\n");
  RHS=(double *) malloc(sizeof(double)*la);
  RHS1=(double *) malloc(sizeof(double)*la);
  EX_SOL=(double *) malloc(sizeof(double)*la);
  X=(double *) malloc(sizeof(double)*la);

  // TODO : you have to implement those functions
  set_grid_points_1D(X, &la);
  set_dense_RHS_DBC_1D(RHS,&la,&T0,&T1);
  set_analytical_solution_DBC_1D(EX_SOL, X, &la, &T0, &T1);
  
  write_vec(RHS, &la, "RHS.dat");
  write_vec(EX_SOL, &la, "EX_SOL.dat");
  write_vec(X, &la, "X_grid.dat");

  kv=1;
  ku=1;
  kl=1;
  lab=kv+kl+ku+1;

  AB = (double *) malloc(sizeof(double)*lab*la);

  set_GB_operator_colMajor_poisson1D(AB, &lab, &la, &kv);

   write_GB_operator_colMajor_poisson1D(AB, &lab, &la, "AB.dat");

// exo4, question1 

cblas_dgbmv(CblasColMajor, CblasConjTrans, la, la, kl, ku, 1.0, AB+1, lab, EX_SOL, 1, 0.0, RHS1, 1);

write_vec(RHS1, &la, "dgbvm.dat");


//exo4 question 2

cblas_daxpy(la, -1, RHS1, 1, RHS, 1);
double norm = cblas_dnrm2(la, RHS, 1);

printf("la validation de dgbmv()= %f\n", norm);

 
  printf("Solution with LAPACK\n");
  /* LU Factorization */
  info=0;
  ipiv = (int *) calloc(la, sizeof(int));
  dgbtrf_(&la, &la, &kl, &ku, AB, &lab, ipiv, &info);

  /* LU for tridiagonal matrix  (can replace dgbtrf_) */
  // ierr = dgbtrftridiag(&la, &la, &kl, &ku, AB, &lab, ipiv, &info);
    
    set_GB_operator_colMajor_poisson1D(AB,&lab, &la, &kv);
    set_dense_RHS_DBC_1D(RHS, &la, &T0, &T1);
    clock_t begin = clock();
    info= LAPACKE_dgbsv(LAPACK_COL_MAJOR, la, kl, ku, NRHS, AB, lab, ipiv, RHS, la);
    clock_t end = clock();

    double diff= (double) (end-begin)/ CLOCKS_PER_SEC;
    printf("The execution time is: %f\n ", diff); 

  // write_GB_operator_colMajor_poisson1D(AB, &lab, &la, "LU.dat");
  
  /* Solution (Triangular) */
  if (info==0){
    //dgbtrs_("N", &la, &kl, &ku, &NRHS, AB, &lab, ipiv, RHS, &la, &info);
    if (info!=0){printf("\n INFO DGBTRS = %d\n",info);}
  }else{
    printf("\n INFO = %d\n",info);
  }

  /* It can also be solved with dgbsv */
  // TODO : use dgbsv

  write_xy(RHS, X, &la, "SOL.dat");

  /* Relative forward error */
  temp = cblas_ddot(la, RHS, 1, RHS,1);
  temp = sqrt(temp);
  cblas_daxpy(la, -1.0, RHS, 1, EX_SOL, 1);
  relres = cblas_ddot(la, EX_SOL, 1, EX_SOL,1);
  relres = sqrt(relres);
  relres = relres / temp;
  
  printf("\nThe relative forward error is relres = %e\n",relres);
  // TODO : Compute relative norm of the residual
  
  printf("\nThe relative forward error is relres = %e\n",relres);

  free(RHS);
  free(EX_SOL);
  free(X);
  free(AB);
  printf("\n\n--------- End -----------\n");
}
