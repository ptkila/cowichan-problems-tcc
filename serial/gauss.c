#include <stdio.h>
#include <stdlib.h>

static float* matrix;
static float* target;
static float* solution;

void set_values_matrix(int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            matrix[i*size + j] = (float) rand();
        }
    }
}

void gauss(int size) {

    int i, j, k;
    float aux = 0.0;
    /* loop for the generation of upper triangular matrix*/

    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(j > i) {
                aux = A[j][i]/A[i][i];
                for(k = 0; k < size; k++) {
                    A[j][k] = A[j][k] - aux*A[i][k];
                }
            }
        }
    }
    x[n]=A[n][n+1]/A[n][n];
    /* this loop is for backward substitution*/
    for(i=n-1; i>=1; i--)
    {
        sum=0;
        for(j=i+1; j<=n; j++)
        {
            sum=sum+A[i][j]*x[j];
        }
        x[i]=(A[i][n+1]-sum)/A[i][i];
    }

}

int main(int argc, char* argv)
{

    if (argc == 3) {

        int i,j,k,n;
        float A[20][20],c,x[10],sum=0.0;
        srand (time(NULL));
        int size = atoi(argv[1]);
        int print = atoi(argv[2]);
        int i;

    /*
    printf("\nEnter the order of matrix: ");
    scanf("%d",&n);
    printf("\nEnter the elements of augmented matrix row-wise:\n\n");
    for(i=1; i<=n; i++)
    {
        for(j=1; j<=(n+1); j++)
        {
            printf("A[%d][%d] : ", i,j);
            scanf("%f",&A[i][j]);
        }
    }
    */

    matrix = (float*) malloc(sizeof(float) * size * size);
    set_values_matrix(size);
    gauss(size);
    
    if (print == 1) {
        for(i = 0; i < size; i++) {
            printf("\nx%d=%f\t",i,x[i]); /* x1, x2, x3 are the required solutions*/
        }
    }
} else {

    printf("programa <tamanho> <printar?>\n");

}

return 0;
}