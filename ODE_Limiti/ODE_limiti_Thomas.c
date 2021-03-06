/*
* The information in this file is
* Copyright(c) 2012, Gabriele Colosimo, Andrea Nascetti <gabriele.colosimo, andrea.nascetti>@uniroma1.it
* and is subject to the terms and conditions of the
* GNU Lesser General Public License Version 2.1
* The license text is available from
* http://www.gnu.org/licenses/lgpl.html
*/

/*
 * Soluzione del problema differenziale lineare ai limiti
 * 
 * y'' = p(x)y' + q(x)y -r(x)       a < x < b
 * y(a) = alpha
 * y(b) = beta
 * 
 * con il metodo lineare alle differenze finite
 */ 

#include <stdio.h>
#include <math.h>

double p(double x);
double q(double x);
double r(double x);

void STAMPA_MATRICE(int righe, int colonne, double A[righe][colonne]);
void STAMPA_VETTORE(int num, double a[num]);
void ZEROS_VETTORE(int num, double V[num]);

double SOL_ESATTA(double x);
void THOMAS(int num, double A_diag[num], double A_sup[num], double A_inf[num], double B[num], double Ris[num]);

int main()
{
	int i = 0, j = 0;
	int N = 0;                    /* Numero dei nodi interni */
	double a = 0., b = 0.;        /* Estremi intervallo di integrazione */
	double alfa = 0., beta = 0.;
	double h = 0.;                /* Passo di integrazione */
	
	/* Lettura parametri di input */
	printf("Intervallo inferiore a: ");
	scanf("%lf", &a);
	printf("Intervallo superiore b: ");
	scanf("%lf", &b);
	printf("Numero nodi interni N: ");
	scanf("%d", &N);
	printf("Valore alfa: ");
	scanf("%lf", &alfa);
	printf("Valore beta: ");
	scanf("%lf", &beta);	
	
	/* Calcolo del passo di integrazione e del vettore dei nodi */
	h = (b - a)/(double)(N + 1.);
	printf("Il passo di integrazione h = % .3lg\n", h);
	double Xi[N];
	ZEROS_VETTORE(N, Xi);

	for(i=1; i <= N; i++)
		Xi[i-1] = a + (double)i * h;

	STAMPA_VETTORE(N, Xi);
	printf("\n CALCOLO DELLE FUNZIONI NEI NODI\n");
	printf("%3s%10s%10s%10s%10s\n", "i",  "Xi",  "p(Xi)",  "q(Xi)",  "r(Xi)");
	for(i = 0; i < N; i++)
		printf("%3d% 10.5lf% 10.5lf% 10.5lf% 10.5lf\n", i, Xi[i], p(Xi[i]), q(Xi[i]), r(Xi[i]));
	/* Costruzione della matrice tridiagonale A usando 3 vettori 
	 * Costruzione del termine noto B */
	double A_diag[N];
	double A_inf[N]; /* Lungo N-1, si mette N per utilizzare un solo ciclo nella assegnazione */
	double A_sup[N]; /* Lungo N-1, si mette N per utilizzare un solo ciclo nella assegnazione */
	double B[N];

	ZEROS_VETTORE(N, A_diag);
	ZEROS_VETTORE(N, A_inf);
	ZEROS_VETTORE(N, A_sup);
	ZEROS_VETTORE(N, B);
	
	printf("\n MATRICE A TRIDIAGONALE\n");
	for(i = 0; i < N; i++)
	{
		A_diag[i] = 2. + pow(h, 2.) * q(Xi[i]);
		A_sup[i] = -1. + 0.5 * h * p(Xi[i]);
		A_inf[i] = -1. - 0.5 * h * p(Xi[i]);
		B[i] = pow(h, 2.) * r(Xi[i]);
	}/* for i */
	
	B[0]   += (1. + 0.5 * h * p(Xi[0])) * alfa;	
	B[N-1] += (1. - 0.5 * h * p(Xi[N-1])) * beta;

	/* Stampa matrice A */
	for(i=0; i < N; i++)
	{
		for(j=0; j < N; j++)
		{
			if(j == i)
				printf("% 10.4lf ", A_diag[i]);
			else if(j == i-1)
				printf("% 10.4lf ", A_inf[i]);
			else if(j == i + 1)
				printf("% 10.4lf ", A_sup[i]);
			else 
				printf("% 10.4lf ", 0.);
		}/* for j */
		printf("\n");
	}/* for i */
	
	double X[N];
	ZEROS_VETTORE(N, X);			

	THOMAS(N, A_diag, A_sup, A_inf, B, X);		
	/* Vettore risultati */
	printf("%5s%15s%15s%17s\n", "Xi", "Yi", "Y(xi)", "|Y(xi) - Yi|");
	for(i=0; i < N; i++)
		printf("%.5lf%15.7lf%15.7lf%16.7le\n", Xi[i], X[i], SOL_ESATTA(Xi[i]), fabs(SOL_ESATTA(Xi[i]) - X[i]));
	
	return 0;
} /* main */

double p(double x)
{
	return 2./x;	
}

double q(double x)
{
	return -2./pow(x, 2.);	
}

double r(double x)
{
	return -sin(log(x))/x;
}

void STAMPA_VETTORE(int num, double a[num])
{
	int i;
	
	for(i=0; i < num; i++)
	{
		printf("a[%3d] = % 10.5lf\n", i, a[i]);
	} 
}

void ZEROS_VETTORE(int num, double V[num])
{
	int i;
	
	for(i = 0; i < num; i++)
	{
			V[i] = 0.;
	}	
}

void STAMPA_MATRICE(int righe, int colonne, double A[righe][colonne])
{
	int i, j;
	
	for(i=0; i < righe; i++)
	{
		for(j = 0; j < colonne; j++)
		{
			printf("% 10.5lf ", A[i][j]);
		}
		printf("\n");
	} 
}

double SOL_ESATTA(double x)
{
	double C = cos(log(2.)) + sin(log(2.));
	
	double a = 0.5 * x * (4. - x);
	double b = x*(1. - x)*C;
	double d =  0.5 * pow(x, 2.)*(cos(log(x)) + sin(log(x)));
	
	return a - b - d;
}/* SOL_ESATTA */

void THOMAS(int num, double A_diag[num], double A_sup[num], double A_inf[num], double B[num], double X[num])
{
	int i;
	double alfa[num];      // Vettore alfa matrice L 
	double u[num], v[num]; // Vettori U e V matrice U
	double y[num];         // Vettore soluzione Y=UX

	ZEROS_VETTORE(num, alfa);
	ZEROS_VETTORE(num, u);
	ZEROS_VETTORE(num, v);
	ZEROS_VETTORE(num, y);
	
	/* Troviamo i componenti delle due matrici L e U
	 * che vogliamo utilizzare per fattorizzare la matrice A
	 * come A = LU
	 */
	 
	u[0] = A_diag[0];
	for(i=0; i < num-1; i++)
	{
		v[i] = A_sup[i];
	}
	
	printf("\n VETTORI DIAGONALI DELLE MATRICE L e U\n");
	for(i=1; i < num; i++)
	{
		alfa[i] = A_inf[i]/u[i-1];
		u[i] = A_diag[i] - alfa[i] * v[i-1];
	} /* for i */
	printf("%3s%10s%10s%10s\n", "i", "alfa", "u", "v"); 
	
	for(i=0; i < num; i++)
		printf("%3d% 10.5lf% 10.5lf% 10.5lf\n", i, alfa[i], u[i], v[i]);
	
	/* Soluzione del sistema triangolare inferiore LY = B (dove Y = UX) con
	 * il metodo della sostituzione in avanti */
	y[0] = B[0];
	for(i=1; i < num; i++)
	{
		y[i] = B[i] - alfa[i] * y[i-1];
	}
	
	printf("\n SOLUZIONE Y DEL SISTEMA TRIANGOLARE INFERIORE LY=B\n");
	printf("%3s%10s\n", "i", "y(i)");
	for(i=0; i < num; i++)
	{
		printf("%3d%10.5lf\n", i, y[i]);
	}
	
	/* Soluzione del sistema triangolare superiore UX = Y con
	 * il metodo della sostituzione all'indietro */
	X[num-1] = y[num-1] / u[num-1];
	for(i=num-2; i >= 0; i--)
	{
		X[i] = (y[i] - v[i] * X[i+1]) / u[i];
	}/* for i */
		
}/* THOMAS */


