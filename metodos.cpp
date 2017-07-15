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
		double* dif = get_sub_vetor(x, x_ant, num_vars);
		/* se a norma dois for menor que o erro, termina */
		if (get_norma2(dif, num_vars) < err) {
			free(dif);
			break;
		}
		free(dif);
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

double* hooke_jeeves(std::string funcao, double *x, int num_vars, double err) {
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
		/* vetor diferenca entre x^k+1 e xk (x^k+1 - xk) */
		double* dif = get_sub_vetor(x, x_ant, num_vars);

		/* se a norma dois for menor que o erro, termina */
		if (get_norma2(dif, num_vars) < err) {
			free(dif);
			break;
		}

		else {
			/* substituindo x0 por x0 + lambda * dif */
			std::string lambda_func = transforma_string(funcao, x, dif, 0);
			/* substituindo xi por xi + lambda * dif na funcao ja substituida */
			for (int i = 1; i < num_vars; i++ )
				lambda_func = transforma_string(lambda_func, x, dif, i);
			/* faz o parse da nova expressao */
			exprtk::expression<double> func = prepara_func(x, num_vars, lambda_func);

			/* minimiza a funcao com relacao a lambda */
			double lambda = min_newton(func, x, num_vars, x[num_vars], DBL_MAX, err);
			/* nova aproximacao */
			for (int i = 0; i < num_vars; i++)
				x[i] += lambda * d[i];

			
		}
		free(dif);
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

double* passo_descendente(std::string funcao, double *x, int num_vars, double err) {
	double *vetor_resu = (double*) malloc(num_vars * sizeof(double));

	/* alocando vetor que salvara os valores originais de x */
	double* x_tmp = (double*) malloc(num_vars * sizeof(double));
	/* vetor direcao */
	double* d;
	/* salvando os valores originais de x */
	memcpy(x_tmp, x, num_vars * sizeof(double));

	/* calcula o gradiente no ponto */
	double* gradiente = get_gradiente(funcao, x, num_vars, err);
	
	while (get_norma2(gradiente, num_vars) > err) {
		/* d = -gradiente */
		d = get_const_mult_vetor(-1, gradiente, num_vars);
		
		/* substituindo x0 por x0 + lambda * d */
		std::string lambda_func = transforma_string(funcao, x, d, 0);
		/* substituindo xi por xi + lambda * dif na funcao ja substituida */
			for (int i = 1; i < num_vars; i++ )
				lambda_func = transforma_string(lambda_func, x, d, i);
			/* faz o parse da nova expressao */
			exprtk::expression<double> func = prepara_func(x, num_vars, lambda_func);

		/* minimiza a funcao com relacao a lambda */
		double lambda = min_newton(func, x, num_vars, x[num_vars], DBL_MAX, err);
		/* xk+1 = xk + lambda * d (nova aproximacao) */
		for (int i = 0; i < num_vars; i++)
			x[i] += lambda * d[i];

		/* liberando o vetor d e o gradiente (serao realocados na proxima iteracao) */
		free(d);
		free(gradiente);

		/* calculando o gradiente no novo ponto */
		gradiente = get_gradiente(funcao, x, num_vars, err);

	}
	/* copiando o valor de mínimo para retorno */
	memcpy(vetor_resu, x, num_vars * sizeof(double));
	/* restaurando os valores originais de *x */
	memcpy(x, x_tmp, num_vars * sizeof(double));

	/* desalocando */
	free(x_tmp);

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