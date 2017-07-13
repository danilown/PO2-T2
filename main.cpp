#include "avaliador.hpp"

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


int main() {
	int num_vars = 2;
	double x[num_vars];
	exprtk::expression<double> expression_t = prepara_func(x, num_vars, "2x0^2 + x1");


	x[0] = 2;
	x[1] = 3;
	printf("Deu isso ai: %lf\n", expression_t.value());
	printf("Derivada da funcao no ponto x eh: %lf\n", get_first_derivativeN(expression_t, x, 1, 0.1, 0.001));
	return 0;
}