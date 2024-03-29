/**********************************************/
/* lib_poisson1D.c                            */
/* Numerical library developed to solve 1D    */ 
/* Poisson problem (Heat equation)            */
/**********************************************/
#include "lib_poisson1D.h"

void set_GB_operator_colMajor_poisson1D(double* AB, int *lab, int *la, int *kv){// lab : nb ligne, la: nb colomne,

  int i, j, k;
  for (j=0;j<(*la);j++){
    k = j*(*lab);
    if (*kv>=0){
      for (i=0;i< *kv;i++){
	AB[k+i]=0.0;
      }
    }
    AB[k+ *kv]=-1.0;
    AB[k+ *kv+1]=2.0;
    AB[k+ *kv+2]=-1.0;
  }
  AB[0]=0.0;
  if (*kv == 1) {AB[1]=0;}
  
  AB[(*lab)*(*la)-1]=0.0;

}

void set_GB_operator_colMajor_poisson1D_Id(double* AB, int *lab, int *la, int *kv){

int i, j, k;
  for (j=0;j<(*la);j++){
    k = j*(*lab);
    if (*kv>=0){
      for (i=0;i< *kv;i++){
	AB[k+i]=0.0;
      }
    }
    AB[k+ *kv]=0.0;
    AB[k+ *kv+1]=1.0;
    AB[k+ *kv+2]=0.0;
  }
  AB[1]=0.0;
  AB[(*lab)*(*la)-1]=0.0;

}

void set_dense_RHS_DBC_1D(double* RHS, int* la, double* BC0, double* BC1){
int j;
  RHS[0]= *BC0;
  RHS[(*la)-1]= *BC1;
  for (j=1;j<(*la)-1;j++){
    RHS[j]=0.0;
  }
}  

void set_analytical_solution_DBC_1D(double* EX_SOL, double* X, int* la, double* BC0, double* BC1){
int j;
  double h, DELTA_T;
  DELTA_T=(*BC1)-(*BC0);
  for (j=0;j<(*la);j++){
    EX_SOL[j] = (*BC0) + X[j]*DELTA_T;
  }
}  

void set_grid_points_1D(double* x, int* la){
  int j;
  double h;
  h=1.0/(1.0*((*la)+1));
  for (j=0;j<(*la);j++){
    x[j]=(j+1)*h;
  }
}

void write_GB_operator_rowMajor_poisson1D(double* AB, int* lab, int* la, char* filename){
  FILE * file;
  int ii,jj;
  file = fopen(filename, "w");
  //Numbering from 1 to la
  if (file != NULL){
    for (ii=0;ii<(*lab);ii++){
      for (jj=0;jj<(*la);jj++){
	fprintf(file,"%lf\t",AB[ii*(*la)+jj]);
      }
      fprintf(file,"\n");
    }
    fclose(file);
  }
  else{
    perror(filename);
  }
}

void write_GB_operator_colMajor_poisson1D(double* AB, int* lab, int* la, char* filename){
  FILE * file;
  int ii,jj;
  file = fopen(filename, "w");
  //Numbering from 1 to la
  if (file != NULL){
    for (ii=0;ii<(*la);ii++){
      for (jj=0;jj<(*lab);jj++){
	fprintf(file,"%lf\t",AB[ii*(*lab)+jj]);
      }
      fprintf(file,"\n");
    }
    fclose(file);
  }
  else{
    perror(filename);
  }
}

void write_GB2AIJ_operator_poisson1D(double* AB, int* la, char* filename){
  FILE * file;
  int jj;
  file = fopen(filename, "w");
  //Numbering from 1 to la
  if (file != NULL){
    for (jj=1;jj<(*la);jj++){
      fprintf(file,"%d\t%d\t%lf\n",jj,jj+1,AB[(*la)+jj]);
    }
    for (jj=0;jj<(*la);jj++){
      fprintf(file,"%d\t%d\t%lf\n",jj+1,jj+1,AB[2*(*la)+jj]);
    }
    for (jj=0;jj<(*la)-1;jj++){
      fprintf(file,"%d\t%d\t%lf\n",jj+2,jj+1,AB[3*(*la)+jj]);
    }
    fclose(file);
  }
  else{
    perror(filename);
  }
}

void write_vec(double* vec, int* la, char* filename){
  int jj;
  FILE * file;
  file = fopen(filename, "w");
  // Numbering from 1 to la
  if (file != NULL){
    for (jj=0;jj<(*la);jj++){
      fprintf(file,"%lf\n",vec[jj]);
    }
    fclose(file);
  }
  else{
    perror(filename);
  } 
}  

void write_xy(double* vec, double* x, int* la, char* filename){
  int jj;
  FILE * file;
  file = fopen(filename, "w");
  // Numbering from 1 to la
  if (file != NULL){
    for (jj=0;jj<(*la);jj++){
      fprintf(file,"%lf\t%lf\n",x[jj],vec[jj]);
    }
    fclose(file);
  }
  else{
    perror(filename);
  } 
}  

void LU_Facto(double* AB, int *lab, int *la, int *kv){
    int i, j, k, k1 = 3;


      if (*kv>=0){
        k1 = 4;
        for (i=0;i< *kv;i++){
            AB[i]=0.0;
        }
      }
      AB[*kv+2]/=AB[*kv+1];
    

    for (j=1;j<(*la);j++){
      k = j*(*lab);
      if (*kv>=0){
        for (i=0;i< *kv;i++){
            AB[k+i]=0.0;
        }
      }

      printf("kv+2 = %lf\n",AB[(k-3)+ *kv+2]);

      AB[k+ *kv+1]-=AB[k+ *kv]*AB[(k-k1)+ *kv+2];
      AB[k+ *kv+2]/=AB[k+ *kv+1];
    }

  }


void eig_poisson1D(double* eigval, int *la){


}

double eigmax_poisson1D(int *la){
  return 0;
}

double eigmin_poisson1D(int *la){
  return 0;
}

double richardson_alpha_opt(int *la){
  return 0;
}

void richardson_alpha(double *AB, double *RHS, double *X, double *alpha_rich, int *lab, int *la,int *ku, int*kl, double *tol, int *maxit, double *resvec, int *nbite){

cblas_dgbmv(CblasColMajor, CblasConjNoTrans, *la, *la, *kl, *ku, 1.0, AB+1, *lab, X, 1, 0.0, resvec, 1) ;
cblas_dscal(*la,-1.0, resvec, 1);
cblas_daxpy(*la, 1.0, RHS, 1, resvec, 1);

double result = cblas_dnrm2(*la, resvec, 1) / cblas_dnrm2(*la, RHS, 1) ;

while (result > (*tol) && *nbite < *maxit)
{

cblas_daxpby (*la, *alpha_rich, resvec, 1, 1.0, X, 1) ;

cblas_dgbmv (CblasColMajor, CblasConjNoTrans, *la, *la, *kl, *ku, 1.0, AB, *lab, X, 1, 0.0, resvec, 1);
cblas_dscal(*la,-1.0,resvec, 1);
cblas_daxpy (*la, 1.0, RHS, 1, resvec, 1);

*nbite += 1;
result = cblas_dnrm2(*la, resvec, 1) / cblas_dnrm2 (*la, RHS, 1);

}
printf("\n\n\n iter = %d",*nbite);

}

void extract_MB_jacobi_tridiag(double *AB, double *MB, int *lab, int *la,int *ku, int*kl, int *kv){

}

void extract_MB_gauss_seidel_tridiag(double *AB, double *MB, int *lab, int *la,int *ku, int*kl, int *kv){

}

void richardson_MB(double *AB, double *RHS, double *X, double *MB, int *lab, int *la,int *ku, int*kl, double *tol, int *maxit, double *resvec, int *nbite){

}

int indexABCol(int i, int j, int *lab){
  return j*(*lab)+i;
}
int dgbtrftridiag(int *la, int*n, int *kl, int *ku, double *AB, int *lab, int *ipiv, int *info){
  return *info;
}
