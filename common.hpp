#include <cmath> 
#include <algorithm>
#include <stdlib.h>
#include "avaliador.hpp"


/**
 * @brief      Calcula o valor da derivada primeira com relacao a uma coordenada dada
 *
 * @param[in]  funcao     A funcao a ser avaliada
 * @param      x          vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao e ja deve estar inicializado)
 * @param[in]  var_index  Indice da coordenada que sera derivada (ex.: var_index = 0 significa derivada com relacao a coordenada x0)
 * @param[in]  h          distancia inicial de aproximacao dos pontos
 * @param[in]  err        Erro tolerado para o valor da derivada
 *
 * @return     Valor da derivada primeira com relacao a coordenada dada.
 */
double get_first_derivativeN(exprtk::expression<double> funcao, double *x, int var_index, double h, double err);

/**
 * @brief      Calcula o valor da derivada segunda com relacao a uma coordenada dada
 *
 * @param[in]  funcao     A funcao a ser avaliada
 * @param      x          vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao e ja deve estar inicializado)
 * @param[in]  var_index  Indice da coordenada que sera derivada (ex.: var_index = 0 significa derivada com relacao a coordenada x0)
 * @param[in]  h          distancia inicial de aproximacao dos pontos
 * @param[in]  err        Erro tolerado para o valor da derivada
 *
 * @return     Valor da derivada segunda com relacao a coordenada dada.
 */
double get_second_derivativeN(exprtk::expression<double> funcao, double *x, int var_index, double h, double err) ;


/**
 * @brief      Minimizacao monovariavel. Determina o ponto de minimo da funcao com relacao a uma variavel num intervalo dado. 
 * 			   O chute inicial eh dado por lower_bound.
 *
 * @param[in]  funcao       A funcao a ser avaliada
 * @param      x            vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao)
 * @param[in]  var_index    Indice da coordenada que sera derivada (ex.: var_index = 0 significa minimiza a funcao com relacao a coordenada x0)
 * @param[in]  lower_bound  Limite inferior do intervalo
 * @param[in]  upper_bound  Limite superior do intervalo
 * @param[in]  err          Intervalo de incerteza tolerado para determinacao do ponto de minimo
 *
 * @return     O ponto de minimo da funcao com relacao a componente var_index da funcao.
 */
double min_newton(exprtk::expression<double> funcao, double *x, int var_index, double lower_bound, double upper_bound, double err);

/**
 * @brief      Retorna o valor da norma 2 de um dado vetor
 *
 * @param      vetor     O vetor a ser calculada a norma
 * @param[in]  tamamnho  O tamanho do vetor
 *
 * @return     O valor da norma 2 do vetor.
 */
double get_norma2 (double *vetor, int tamamnho);

/**
 * @brief      Determina o vetor resultante da soma de dois vetores
 *
 * @param      vetorA   O vetorA
 * @param      vetorB   O vetorB
 * @param[in]  tamanho  O tamanho dos vetores (devem ser iguais);
 *
 * @return     O vetor soma resultante.
 */
double* get_soma_vetor (double *vetorA, double *vetorB, int tamanho);