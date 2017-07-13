#include <cmath> 

/**
 * @brief      Calcula o valor da derivada com relacao a uma coordenada dada
 *
 * @param[in]  funcao     The funcao
 * @param      x          vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao e ja deve estar inicializado)
 * @param[in]  var_index  Indice da coordenada que sera derivada (var_index = 0 >> derivada com relacao a coordenada 0)
 * @param[in]  h          distancia inicial de aproximacao dos pontos
 * @param[in]  err        Erro tolerado para o valor da derivada
 *
 * @return     Valor da derivada com relacao a coordenada dada.
 */
double get_first_derivativeN(exprtk::expression<double> funcao, double *x, int var_index, double h, double err);