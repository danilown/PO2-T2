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

/*fn get_second_derivative1(func: &Fn(f64) -> f64, x: f64, mut h: f64, err: f64) -> f64 {
    let mut p: f64;
    let mut q: f64;

    p = (func(x + 2. * h) - 2. * func(x) + func(x - 2. * h)) / (2. * h).powi(2);
    for _ in 0..10 {
        q = p;
        h = h / 2.;
        p = (func(x + 2. * h) - 2. * func(x) + func(x - 2. * h)) / (2. * h).powi(2);
        if (p - q).abs() < err {
            break;
        }
    }
    p
}*/ 
