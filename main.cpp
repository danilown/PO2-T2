#include "avaliador.hpp"
#include <cmath> 
#include <algorithm>
#include <cfloat>

double* get_sub_vetor (double *vetorA, double *vetorB, int tamanho) {
	double* vetor_resu = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_resu[i] = vetorA[i] - vetorB[i];

	return vetor_resu;
}

double get_norma2 (double *vetor, int tamamnho) {
	double soma = 0;

	for (int i = 0; i < tamamnho; i++)
		soma += std::pow(vetor[i], 2);

	return std::sqrt(soma);
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

	p = (func_mais_h - 2 * func_x + func_menos_h) / pow((2 * h), 2);
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

		p = (func_mais_h - 2 * func_x + func_menos_h) / pow((2 * h), 2);

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

std::string transforma_string (std::string funcao, double *x, double *d, int var_index) {
  std::string nova_funcao;

  std::string var_name = "x";
  var_name += std::to_string(var_index);

  while (funcao.length() > 0) {
    if (funcao.find(var_name) == std::string::npos) {
      nova_funcao += funcao;
      break;
    }
    nova_funcao += funcao.substr(0, funcao.find(var_name)); 
    funcao.erase(0,funcao.find(var_name));
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

double* get_const_mult_vetor (double constante, double *vetor, int tamanho) {
	double* vetor_resu = (double*) malloc(tamanho * sizeof(double));

	for (int i = 0; i < tamanho; i++)
		vetor_resu[i] = constante * vetor[i];

	return vetor_resu;
}

double* get_gradiente(std::string funcao, double *x, int num_vars, double err){
	double* gradiente = (double*) malloc(num_vars * sizeof(double));

	exprtk::expression<double> func = prepara_func(x, num_vars, funcao);

	for (int i = 0; i < num_vars; i++)
		gradiente[i] = get_first_derivativeN(func, x, i, 1, err);

	return gradiente;
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

int main() {
	int num_vars = 2;
	double x[num_vars + 1];
	x[0] = 0;
	x[1] = 3;
	double *resu = passo_descendente("(x0 - 2)^4 + (x0 - 2x1)^2", x, num_vars, 0.1);
	for (int i = 0; i < num_vars; i++)
		printf("%lf\n", resu[i]);
	printf("originais:\n");
	for (int i = 0; i < num_vars; i++)
		printf("%lf\n", x[i]);
	return 0;
}