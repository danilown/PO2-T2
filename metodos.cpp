#include "metodos.hpp"

double* coord_ciclicas(std::string funcao, double *x, int num_vars, double err) {
	double *vetor_resu = (double*) malloc(num_vars * sizeof(double));

	/* alocando vetor que salvara os valores originais de x */
	double* x_tmp = (double*) malloc(num_vars * sizeof(double));
	/* alocando vetor que salvara os valores da iteracao anterior */
	double* x_ant = (double*) malloc(num_vars * sizeof(double));
	/* alocando vetor direcao zerado */
	double* d = (double*) calloc(num_vars, sizeof(double));
	/* salvando os valores originais de x */
	memcpy(x_tmp, x, num_vars * sizeof(double));

	while (true) {
		/* salvando os valores da iteracao anterior de x */
		memcpy(x_ant, x, num_vars * sizeof(double));
		for (int i = 0; i < num_vars; i++) {
			/* 'ativando' a direcao i */
			d[i] = 1;
			
			/* determinando o ponto de minimo da funcao com relacao a lambda */
			double lambda = min_lambda(funcao, x, d, num_vars, i, err);
			//getchar();

			x[i] += lambda; 

			/* 'desativamento' da direcao k */
			d[i] = 0;
		}
		/* se a norma dois for menor que o erro, termina */
		if (get_norma2(get_sub_vetor(x, x_ant, num_vars), num_vars) < err)
			break;
	}
	/* copiando o valor de mínimo para retorno */
	memcpy(vetor_resu, x, num_vars * sizeof(double));
	/* restaurando os valores originais de *x */
	memcpy(x, x_tmp, num_vars * sizeof(double));

	/* desalocando */
	free(x_tmp);
	free(x_ant);
	free(d);

	return vetor_resu;
}