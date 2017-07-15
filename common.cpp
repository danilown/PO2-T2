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
		if ((std::abs(derivada_primera) < err) || ((std::abs(x_prox - x_atual) / std::max(x_prox, 1.)) < err))
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
	double* vetor_resu = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_resu[i] = vetorA[i] + vetorB[i];

	return vetor_resu;
}

double* get_sub_vetor (double *vetorA, double *vetorB, int tamanho) {
	double* vetor_resu = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_resu[i] = vetorA[i] - vetorB[i];

	return vetor_resu;
}

double* get_const_mult_vetor (double constante, double *vetor, int tamanho) {
	double* vetor_resu = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_resu[i] = constante * vetor[i];

	return vetor_resu;
}

std::string transforma_string (std::string funcao, double *x, double *d, int var_index) {
  std::string nova_funcao;

  /* monta o nome da variavel a ser procurada pela string */
  std::string var_name = "x";
  var_name += std::to_string(var_index);

  while (funcao.length() > 0) {
  	/* se nao existe mais ocorrencia da variavel na funcao, copia o resto da funcao*/
    if (funcao.find(var_name) == std::string::npos) {
      nova_funcao += funcao;
      break;
    }
    /* copia a parte da funcao que nao possui a variavel a ser substituida */
    nova_funcao += funcao.substr(0, funcao.find(var_name)); 
    /* remove a parte ja inserida na nova string da string original (para possibilitar encontrar as proxima ocorrencias da 
     * variavel a ser substituida) */
    funcao.erase(0,funcao.find(var_name));
    /* Fazendo a substituicao da variavel */
    nova_funcao += "( ";
    nova_funcao += var_name;
    nova_funcao += " + ";
    nova_funcao += "lambda ";
    nova_funcao += "* ";
    nova_funcao += std::to_string(d[var_index]);
    nova_funcao += " )";
    funcao.erase(0,var_name.length());
  }

  return nova_funcao;
}

double min_lambda (std::string funcao, double *x, double *d, int num_vars, int var_index, double err) {
	std::string lambda_func = transforma_string(funcao, x, d, var_index);
	exprtk::expression<double> func = prepara_func(x, num_vars, lambda_func);

	/* minimiza a funcao com relacao a lambda 'campo fantasma' do vetor x original */
	return min_newton(func, x, num_vars, x[num_vars], DBL_MAX, err);
}

double* get_gradiente(std::string funcao, double *x, int num_vars, double err) {
	double* gradiente = (double*) malloc(num_vars * sizeof(double));

	exprtk::expression<double> func = prepara_func(x, num_vars, funcao);

	for (int i = 0; i < num_vars; i++)
		gradiente[i] = get_first_derivativeN(func, x, i, 1, err);

	return gradiente;
}