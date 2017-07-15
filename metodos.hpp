#ifndef METODOS
#define METODOS

#include "avaliador.hpp"
#include "common.hpp"
#include <cfloat>

/**
 * @brief      Calcula o ponto de minimo da funcao dada pela string pelo metodo das Coordenadas ciclicas
 *
 * @param[in]  funcao    string contendo a funcao
 * @param      x         vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao)
 * @param[in]  num_vars  Numero de variaveis de dicisao
 * @param[in]  err       Intervalo de incerteza tolerado para determinacao do ponto de minimo
 *
 * @return     vetor contendo os pontos de minimo.
 */
double* coord_ciclicas(std::string funcao, double *x, int num_vars, double err);

/**
 * @brief      Calcula o ponto de minimo da funcao dada pela string pelo metodo de Hooke Jeeves
 *
 * @param[in]  funcao    string contendo a funcao
 * @param      x         vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao)
 * @param[in]  num_vars  Numero de variaveis de dicisao
 * @param[in]  err       Intervalo de incerteza tolerado para determinacao do ponto de minimo
 *
 * @return     vetor contendo os pontos de minimo.
 */
double* hooke_jeeves(std::string funcao, double *x, int num_vars, double err);

/**
 * @brief      Calcula o ponto de minimo da funcao dada pela string pelo metodo do Passo Descendente
 *
 * @param[in]  funcao    string contendo a funcao
 * @param      x         vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao)
 * @param[in]  num_vars  Numero de variaveis de dicisao
 * @param[in]  err       Intervalo de incerteza tolerado para determinacao do ponto de minimo
 *
 * @return     vetor contendo os pontos de minimo.
 */
double* passo_descendente(std::string funcao, double *x, int num_vars, double err);

/**
 * @brief      Calcula o ponto de minimo da funcao dada pela string pelo metodo de Newton.
 *
 * @param[in]  funcao    string contendo a funcao
 * @param      x         vetor das variaveis de decisao (deve ser mandado por referencia pois eh associado a expressao)
 * @param[in]  num_vars  Numero de variaveis de dicisao
 * @param[in]  err       Intervalo de incerteza tolerado para determinacao do ponto de minimo
 *
 * @return     vetor contendo os pontos de minimo.
 */
double* newton(std::string funcao, double *x, int num_vars, double err);

#endif