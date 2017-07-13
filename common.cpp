#include "common.hpp"

 /* var_index = com relacao a qual x (0, 1, ..., n) esta sendo derivada a funcao 
  x = vetor ja inicializado de preferencia */
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

		if (abs(p - q) < err)
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

		if (abs(p - q) < err)
			break;
	}

	return p;
}


double min_newton(exprtk::expression<double> funcao, double *x, int var_index, double lower_bound, double upper_bound, double err) {
	double derivada_primera, derivada_segunda, x_tmp, x_atual, x_prox;

	x_atual = lower_bound;

	while (true) {
		x_tmp = x[var_index];
		x[var_index] = x_atual;
		/* calculando a derivada primeira da funcao com relacao a variavel var_index no ponto x_atual */
		derivada_primera = get_first_derivativeN(funcao, x, var_index, 1, err / 100);
		/* calculando a derivada segunda da funcao com relacao a variavel var_index no ponto x_atual */
		derivada_segunda = get_second_derivativeN(funcao, x, var_index, 1, err / 100);
		/* restaurando o valor de x[var_index] */
		x[var_index] = x_tmp;

		/* calculando nova aproximacao */
		x_prox = x_atual - (derivada_primera / derivada_segunda);
        x_tmp = x[var_index];
		x[var_index] = x_prox;
		/* calculando a derivada primeira da funcao com relacao a variavel var_index no ponto x_prox */
		derivada_primera = get_first_derivativeN(funcao, x, var_index, 1, err / 100);
		/* restaurando o valor de x[var_index] */
		x[var_index] = x_tmp;

		/* se a derivada primera da funcao no ponto atual eh menor que o erro aceitavel ou 
           se o modulo da diferenca entre o x anterior e o x atual dividido pelo maior entre 1 e o ponto atual eh menor que o erro aceitavel
        */
		if ((abs(derivada_primera) < err) || ((abs(x_prox - x_atual) / std::max(x_prox, 1.)) < err))
			return x_prox;
		
        x_atual = x_prox;
	}
}

double get_norma2 (double *vetor, int tamamnho) {
	double soma = 0;

	for (int i = 0; i < tamamnho; i++)
		soma += std::pow(vetor[i], 2);

	return std::sqrt(soma);
}	

double* get_soma_vetor (double *vetorA, double *vetorB, int tamanho) {
	double* vetor_soma = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_soma[i] = vetorA[i] + vetorB[i];

	return vetor_soma;
}