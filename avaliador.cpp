#include "avaliador.hpp"

exprtk::expression<double> prepara_func(double *x, int num_vars, std::string expression_string) {

	exprtk::symbol_table<double> symbol_table;
	exprtk::expression<double>  expression;
	exprtk::parser<double> parser;

	/* informa ao parser que as variaveis sao x0, x1, ... xn */
	for (int i = 0; i < num_vars; i++) {
		/* construcao da string de nomes de variaveis. ex.: "x" + "0" = x0 */
		std::string var_name = "x";
		var_name += std::to_string(i);
		/* adiciona a string a tabela de simbolos */
		symbol_table.add_variable(var_name, x[i]);
	}
	/* adicionando o lambda a tabela de simbolos e ligando-o ao 'indice fantasma' do vetor x*/
	symbol_table.add_variable("lambda", x[num_vars]);
	/* adicionando as demais constantes */
	symbol_table.add_constants();

	/* informando a expressao a sofre o parse quais simbolos sao variaveis */
	expression.register_symbol_table(symbol_table);

	/* compilando a expressao para que possa ser avaliada futuramente */
	parser.compile(expression_string,expression);

	/* retorna a expresso pronta para ser avaliada */
	return expression;
}
