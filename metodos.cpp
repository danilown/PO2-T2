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

//x0^3 - 2 * x0 * x1 + x1^2
double* gradiente_gen(std::string funcao, double* x, int num_vars, double erro) {

	double lambda;
	double beta;

	double* atual_x = (double*) malloc(num_vars * sizeof(double));
	memcpy(atual_x, x, num_vars * sizeof(double));
	double* prox_x;
	double* prox_direcao;
	double* grad = get_gradiente(funcao,atual_x,num_vars,erro);
	double* direcao;

	double** hessiana = get_hessiana(funcao,atual_x,num_vars,erro);

	double norma2 = get_norma2(grad,num_vars);

	while (norma2 > erro) {

		direcao = get_const_mult_vetor(-1,grad,num_vars);

		for (int i=0;i<num_vars;i++) {

			lambda = lambda_gradiente(grad, direcao, hessiana, num_vars);

			double* lambda_direcao = get_const_mult_vetor(lambda,direcao,num_vars);

			prox_x = get_soma_vetor(atual_x, lambda_direcao,num_vars);

			grad = get_gradiente(funcao,prox_x,num_vars,erro);

			if (i<num_vars-1) {

				beta = beta_gradiente(grad, direcao, hessiana, num_vars);

				double* oposto_gradiente = get_const_mult_vetor(-1,grad,num_vars);
				double* beta_direcao = get_const_mult_vetor(beta,direcao,num_vars);

				prox_direcao = get_soma_vetor(oposto_gradiente, beta_direcao,num_vars);

				free(direcao);
				direcao = prox_direcao;

			}

			hessiana = get_hessiana(funcao,prox_x,num_vars,erro);
			free(atual_x);
			atual_x = prox_x;
		}

		norma2 = get_norma2(grad,num_vars);

	}

	return atual_x;

}

//x0^3 - x0^2 + 2 * x1^2 - 2*x1
double* fletcher_reeves(std::string funcao, double* x, int num_vars, double erro) {

	double lambda;
	double beta;

	double* atual_x = (double*) malloc(num_vars * sizeof(double));
	memcpy(atual_x, x, num_vars * sizeof(double));
	double* prox_x;
	double* prox_direcao;
	double* prox_grad;
	double* grad = get_gradiente(funcao,atual_x,num_vars,erro);
	double* direcao;

	double norma2 = get_norma2(grad,num_vars);

	while (norma2 > erro) {

		direcao = get_const_mult_vetor(-1,grad,num_vars);

		for (int i=0;i<num_vars;i++) {

			std::string lambda_func = transforma_string(funcao, atual_x, direcao, 0);

			for (int j = 1; j < num_vars; j++ )
				lambda_func = transforma_string(lambda_func, atual_x, direcao, j);

			exprtk::expression<double> func = prepara_func(atual_x, num_vars, lambda_func);

			lambda = min_newton(func, atual_x, num_vars, atual_x[num_vars], DBL_MAX, erro);

			printf("\nLambda em %d: %lf\n",i,lambda);

			double* lambda_direcao = get_const_mult_vetor(lambda,direcao,num_vars);

			prox_x = get_soma_vetor(atual_x, lambda_direcao,num_vars);
			printf("Proximo x:\n");
			printaVetor(prox_x,num_vars);

			prox_grad = get_gradiente(funcao,prox_x,num_vars,erro);
			printf("\nNovo gradiente:\n");
			printaVetor(prox_grad,num_vars);

			if (i<num_vars-1) {

				beta = beta_f_r(grad, prox_grad, num_vars);

				printf("\nBeta em %d: %lf\n",i,beta);

				double* oposto_gradiente = get_const_mult_vetor(-1,prox_grad,num_vars);
				double* beta_direcao = get_const_mult_vetor(beta,direcao,num_vars);

				prox_direcao = get_soma_vetor(oposto_gradiente, beta_direcao,num_vars);
				printf("Proxima direcao:\n");
				printaVetor(prox_direcao,num_vars);

				free(direcao);
				direcao = prox_direcao;

			}

			free(atual_x);
			atual_x = prox_x;
			free(grad);
			grad = prox_grad;
		}

		norma2 = get_norma2(grad,num_vars);

	}

	return atual_x;

}