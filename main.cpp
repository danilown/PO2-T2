#include "avaliador.hpp"
#include <cmath> 
#include <algorithm>
#include <cfloat>
#include <iostream>

using namespace std;

/* deriva primeiro em relacao a A e depois deriva de novo com relacao a B*/
double get_second_derivativeN(exprtk::expression<double> funcao, double *x, int var_indexA, int var_indexB, double h, double err) {
	/* f1 = f(x[var_indexA]+h, x[var_indexB]+h);
	 * f2 = f(x[var_indexA]+h, x[var_indexB]-h);
	 * f3 = f(x[var_indexA]-h, x[var_indexB]+h);
	 * f4 = f(x[var_indexA]-h, x[var_indexB]-h);
	 */
	double f1, f2, f3, f4;
	double p, q;

	/* F1 */
	x[var_indexA] += h; 
	x[var_indexB] += h;
	f1 = funcao.value();
	/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
	x[var_indexA] -= h; 
	x[var_indexB] -= h;

	/* F2 */
	x[var_indexA] += h; 
	x[var_indexB] -= h;
	f2 = funcao.value();
	/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
	x[var_indexA] -= h; 
	x[var_indexB] += h;

	/* F3 */
	x[var_indexA] -= h; 
	x[var_indexB] += h;
	f3 = funcao.value();
	/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
	x[var_indexA] += h; 
	x[var_indexB] -= h;

	/* F4 */
	x[var_indexA] -= h; 
	x[var_indexB] -= h;
	f4 = funcao.value();
	/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
	x[var_indexA] += h; 
	x[var_indexB] += h;

	p = (f1 - f2 - f3 + f4) / (4 * h * h);

	for (int i = 0; i < 10; i++) {
		q = p;
		h = h / 2;

		/* F1 */
		x[var_indexA] += h; 
		x[var_indexB] += h;
		f1 = funcao.value();
		/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
		x[var_indexA] -= h; 
		x[var_indexB] -= h;

		/* F2 */
		x[var_indexA] += h; 
		x[var_indexB] -= h;
		f2 = funcao.value();
		/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
		x[var_indexA] -= h; 
		x[var_indexB] += h;

		/* F3 */
		x[var_indexA] -= h; 
		x[var_indexB] += h;
		f3 = funcao.value();
		/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
		x[var_indexA] += h; 
		x[var_indexB] -= h;

		/* F4 */
		x[var_indexA] -= h; 
		x[var_indexB] -= h;
		f4 = funcao.value();
		/* restaura o valor anterior de x[var_indexA] e x[var_indexB] */
		x[var_indexA] += h; 
		x[var_indexB] += h;

		p = (f1 - f2 - f3 + f4) / (4 * h * h);

		if (std::abs(p - q) < err)
			break;
	}
	
	return p;
} 

double get_first_derivativeN(exprtk::expression<double> funcao, double *x, int var_index, double h, double err) {
	double p, q;

	x[var_index] += h;
	double func_mais_h = funcao.value();
	/* restaura o valor anterior de x */
	x[var_index] -= h;

	x[var_index] -= h;
	double func_menos_h = funcao.value();
	/* restaura o valor anterior de x */
	x[var_index] += h;

	p = (func_mais_h - func_menos_h) / (2 * h);
	for (int i = 0; i < 10; i++) {
		q = p;
		h = h / 2;
		x[var_index] += h;
		double func_mais_h = funcao.value();
		/* restaura o valor anterior de x */
		x[var_index] -= h;

		x[var_index] -= h;
		double func_menos_h = funcao.value();
		/* restaura o valor anterior de x */
		x[var_index] += h;

		p = (func_mais_h - func_menos_h) / (2 * h);

		if (std::abs(p - q) < err)
			break;
	}

	return p;
}

double get_second_derivativeN(exprtk::expression<double> funcao, double *x, int var_index, double h, double err) {
	double p, q;

	double func_x = funcao.value();

	x[var_index] += 2 * h;
	double func_mais_h = funcao.value();
	/* restaura o valor anterior de x */
	x[var_index] -= 2 * h;

	x[var_index] -= 2 * h;
	double func_menos_h = funcao.value();
	/* restaura o valor anterior de x */
	x[var_index] += 2 * h;

	p = (func_mais_h - 2 * func_x + func_menos_h) / std::pow((2 * h), 2);
	for (int i = 0; i < 10; i++) {
		q = p;
		h = h / 2;

		double func_x = funcao.value();

		x[var_index] += 2 * h;
		double func_mais_h = funcao.value();
		/* restaura o valor anterior de x */
		x[var_index] -= 2 * h;

		x[var_index] -= 2 * h;
		double func_menos_h = funcao.value();
		/* restaura o valor anterior de x */
		x[var_index] += 2 * h;

		p = (func_mais_h - 2 * func_x + func_menos_h) / std::pow((2 * h), 2);

		if (std::abs(p - q) < err)
			break;
	}

	return p;
}

double get_norma2 (double *vetor, int tamamnho) {
	double soma = 0;

	for (int i = 0; i < tamamnho; i++)
		soma += std::pow(vetor[i], 2);

	return std::sqrt(soma);
}
double* get_sub_vetor (double *vetorA, double *vetorB, int tamanho) {
	double* vetor_resu = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_resu[i] = vetorA[i] - vetorB[i];

	return vetor_resu;
}

double* get_gradiente(std::string funcao, double *x, int num_vars, double err) {
	double* gradiente = (double*) malloc(num_vars * sizeof(double));

	exprtk::expression<double> func = prepara_func(x, num_vars, funcao);

	for (int i = 0; i < num_vars; i++)
		gradiente[i] = get_first_derivativeN(func, x, i, 1, err);

	return gradiente;
}

double** get_hessiana(std::string funcao, double *x, int num_vars, double err) {
	double** hessiana = (double**) malloc(num_vars * sizeof(double*));
	for (int i = 0; i < num_vars; i++)
		hessiana[i] = (double*) malloc(num_vars * sizeof(double));


	exprtk::expression<double> func = prepara_func(x, num_vars, funcao);

	for (int i = 0; i < num_vars; i++) {
		for (int j = i; j < num_vars; j++)
			hessiana[i][j] = get_second_derivativeN(func, x, i, j, 1, 0.1);
	}

	for (int i = 1; i < num_vars; i++) {
		for (int j = 0; j < i; j++)
			hessiana[i][j] = hessiana[j][i];
	}

	return hessiana;
}

void retro_subtituicao(double** matriz_A, int ordem, double* vetor_X, double* vetor_B){
  int i, j;

  for(i = ordem - 1; i >= 0; i--){
    vetor_X[i] = vetor_B[i];
    for(j = i + 1; j < ordem; j++){
      vetor_X[i] -= vetor_X[j] * matriz_A[i][j];
    }
    vetor_X[i] = vetor_X[i] / matriz_A[i][i];
  }
}

void gauss_simples(double** matriz_A, int ordem, double* vetor_X, double* vetor_B){
  int i, j, k;
  double coeficiente;

  for(i = 0;i < ordem - 1 ; i++){ // triangularização da matriz_A
    for(j = i + 1; j < ordem; j++){
      coeficiente = matriz_A[j][i] / matriz_A[i][i];
      for(k = i; k < ordem; k++)
        matriz_A[j][k] -= coeficiente * matriz_A[i][k];
      vetor_B[j] -= coeficiente * vetor_B[i];
    }
  }
  retro_subtituicao(matriz_A, ordem, vetor_X, vetor_B);
}

double* get_const_mult_vetor (double constante, double *vetor, int tamanho) {
	double* vetor_resu = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_resu[i] = constante * vetor[i];

	return vetor_resu;
}

double* newton(std::string funcao, double *x, int num_vars, double err) {
	double *vetor_resu = (double*) malloc(num_vars * sizeof(double));

	/* alocando vetor que salvara os valores originais de x */
	double* x_tmp = (double*) malloc(num_vars * sizeof(double));
	/* vetor direcao */
	double* d;
	/* vetor solucao do sistema */
	double* w = (double*) malloc(num_vars * sizeof(double));
	/* alocando vetor que salvara os valores da iteracao anterior */
	double* x_ant = (double*) malloc(num_vars * sizeof(double));
	/* salvando os valores originais de x */
	memcpy(x_tmp, x, num_vars * sizeof(double));

	/* calcula o gradiente no ponto */
	double* gradiente = get_gradiente(funcao, x, num_vars, err);
	
	while (true) {
		/* salvando os valores da iteracao anterior de x */
		memcpy(x_ant, x, num_vars * sizeof(double));

		double** hessiana = get_hessiana(funcao, x, num_vars, err);

		/* d = -gradiente */
		d = get_const_mult_vetor(-1, gradiente, num_vars);
		
		gauss_simples(hessiana, num_vars, w, d);

		/* xk+1 = xk + w (nova aproximacao) */
		for (int i = 0; i < num_vars; i++)
			x[i] += w[i];


		double* dif = get_sub_vetor(x, x_ant, num_vars);
		if ((get_norma2(gradiente, num_vars) < err) || (get_norma2(dif, num_vars) < err)) {
			free(d);
			free(gradiente);
			free(hessiana);
			free(dif);
			break;
		}

		/* liberando o vetor d, o gradiente e a hessiana (serao realocados na proxima iteracao) */
		free(d);
		free(gradiente);
		free(hessiana);

		/* calculando o gradiente no novo ponto */
		gradiente = get_gradiente(funcao, x, num_vars, err);

	}
	
	/* copiando o valor de mínimo para retorno */
	memcpy(vetor_resu, x, num_vars * sizeof(double));
	/* restaurando os valores originais de *x */
	memcpy(x, x_tmp, num_vars * sizeof(double));

	/* desalocando */
	free(x_tmp);
	free(w);
	free(x_ant);

	return vetor_resu;
}

double** matriz_mult(double** matriz_A, int a_linhas, int a_colunas, double** matriz_B, int b_linhas, int b_colunas) {
	if (a_colunas != b_linhas) {
		printf("Essas matrizes nao podem ser multiplicadas!\n");
		exit(EXIT_FAILURE);
	}

	double** matriz_resu = (double**) calloc(a_linhas, sizeof(double*));
	for (int i = 0; i < a_linhas; i++)
		matriz_resu[i] = (double*) calloc(b_colunas, sizeof(double));

	for(int i = 0; i < a_linhas; i++)
    	for(int j = 0; j < b_colunas; j++)
        	for(int k = 0; k < a_colunas; k++)
                matriz_resu[i][j] += matriz_A[i][k] * matriz_B[k][j];

    return matriz_resu;
}

int main() {
	int n_linhasA = 2;
	int n_colunasA = 2;
	int n_linhasB = 2;
	int n_colunasB = 1;

	double** a = (double**) malloc(n_linhasA * sizeof(double*));
	for (int i = 0; i < n_linhasA; i++)
		a[i] = (double*) malloc(n_colunasA * sizeof(double));


	double** b = (double**) malloc(n_linhasB * sizeof(double*));
	for (int i = 0; i < n_linhasB; i++)
		b[i] = (double*) malloc(n_colunasB * sizeof(double));

	cout << endl << "Enter elements of matrix 1:" << endl;
    for(int i = 0; i < n_linhasA; i++)
        for(int j = 0; j < n_colunasA; j++)
        {
            cout << "Enter element a" << i + 1 << j + 1 << " : ";
            cin >> a[i][j];
        }

   cout << endl << "Enter elements of matrix 2:" << endl;
    for(int i = 0; i < n_linhasB; i++)
        for(int j = 0; j < n_colunasB; j++)
        {
            cout << "Enter element a" << i + 1 << j + 1 << " : ";
            cin >> b[i][j];
        }

    double** resu = matriz_mult(a, n_linhasA, n_colunasA, b, n_linhasB, n_colunasB);
	printf("Matriz multiplicada: \n");
	for (int i = 0 ; i < n_linhasA; i++) {
		for (int j = 0; j < n_colunasB; j++)
			printf("%lf ", resu[i][j]);
		printf("\n");
	}

	return 0;
}