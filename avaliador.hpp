#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string>
#include "exprtk.hpp"

/**
 * @brief      prepara a funcao recebida como string para que possa ser futuramente avaliada
 * 			   a funcao deve ser em funcao de x0, x1, ..., xn , do contrario, nao vai funcionar
 *
 * @param      x                  vetor das variaveis de decisao (deve ser mandado por referencia para se associar a 
 * 								  expressao e deve ter tamanho num_vars + 1 (ultimo campo eh o lamda))
 * @param[in]  num_vars           Number variaveis de decisao do problema
 * @param[in]  expression_string  A string que contem a expression 
 *
 * @return     expressao compilada pronta para ser avaliada
 */
exprtk::expression<double> prepara_func(double *x, int num_vars, std::string expression_string);

/* 
EXEMPLO DE USO DA FUNCAO prepara_func:
int main() {
	int num_vars = 3;
	double x[num_vars];
	exprtk::expression<double> expression_t = prepara_func(x, num_vars, "x0 + x1 + x2");


	x[0] = 2;
	x[1] = 3;
	x[2] = 4;
	printf("Deu isso ai: %lf\n", expression_t.value());

	return 0;
}
 */