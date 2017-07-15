#include "metodos.hpp"
#include "avaliador.hpp"
#include "exprtk.hpp"
#include <math.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <stddef.h>
#include <gtk/gtk.h>

/*Conserta a rotina to_string*/
namespace patch{

    template < typename T > std::string to_string( const T& n ){

        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#define MAIOR_DIMENSAO 10

/*Variáveis de Acesso ao Arquivo*/
GtkBuilder *builder;
/*Variáveis de Acesso à Janela*/
GtkWidget *window;
/*Variáveis de Acesso aos Elementos de Entrada*/
GtkWidget *comboMetodos;
GtkWidget *campoFuncao;
GtkWidget *spinnerDim;
GtkWidget *spinnerErro;
GtkWidget *botaoOk;
/*Variáveis de Acesso aos Elementos de Chute*/
GtkWidget *layoutChuteGeral;
GtkWidget *layoutChutes;
GtkWidget *botaoOk2;
/*Variáveis de Acesso aos Elementos de Resposta*/
GtkWidget *layoutRespostaGeral;
GtkWidget *layoutRespostas;
GtkWidget *layoutRespostaFuncao;
GtkWidget *funcaoOtima;

/*Variáveis de Controle da Entrada Dinâmica*/
GtkWidget *layoutCampos[MAIOR_DIMENSAO/2];
GtkWidget *chutes[MAIOR_DIMENSAO];
/*Variáveis de Controle da Saída Dinâmica*/
GtkWidget *layoutCampos2[MAIOR_DIMENSAO/2];
GtkWidget *resultado[MAIOR_DIMENSAO];

/*Variáveis de Configuração de Spinners*/
GtkAdjustment *adjustment;

/*Variáveis das Janelas de Aviso*/
GtkWidget *funcaoNaoPreenchida;
GtkWidget *funcaoInvalida;
GtkWidget *campoNaoPreenchido;
GtkWidget *campoInvalido;

/*Variáveis Adquiridas da Interface*/
int metodo;
std::string funcao;
int dimensao;
double erro;
double valorChute[MAIOR_DIMENSAO+1];

/*Variável Adquirida do Cálculo*/
double* resposta;

gboolean escondeJanela(GtkWidget *widget, GdkEvent *event, gpointer data){

    gtk_widget_hide(widget);
    return TRUE;
}

void limpaCriaPainel(GtkWidget *painelPrincipal,GtkWidget *paineisAuxiliares[]) {


	/*Limpa o Painel Com os Campos para que Eles Não se Acumulem*/
	GList *itensPainel, *iterador;

	itensPainel = gtk_container_get_children(GTK_CONTAINER(painelPrincipal));

	for(iterador = itensPainel; iterador != NULL; iterador = g_list_next(iterador))
  		gtk_widget_destroy(GTK_WIDGET(iterador->data));

	g_list_free(itensPainel);
	/*************************************************************************/

	/*Inicia os Layouts que Conterão os Campos, e os Inclui no Layout Principal*/	
	for (int i=0;i<MAIOR_DIMENSAO/2;i++) {

		paineisAuxiliares[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		gtk_box_pack_start(GTK_BOX(painelPrincipal),paineisAuxiliares[i],0,0,0);
	}
	/*************************************************************************/
}

void preenchePaineis(GtkWidget *paineisPreenchidos[],GtkWidget *camposInseridos[],bool editavel) {

	int index = 0;

	/*Preenche Os Layouts do Campo com Labels para Identificar os Campos e os Campos em Si, Dependedendo da Dimensão do Problema*/
	for (int i=0;i<ceil(((float)dimensao)/2);i++) {

		std::string label="x[" + patch::to_string(index) + "]: ";
		
		GtkWidget *temp = gtk_label_new(&label[0]);
		
		gtk_box_pack_start(GTK_BOX(paineisPreenchidos[i]),temp,false,false,0);

		camposInseridos[index] = gtk_entry_new();

		if (!editavel) {

			gtk_widget_set_can_focus(camposInseridos[index],false);
		}

		gtk_box_pack_start(GTK_BOX(paineisPreenchidos[i]),camposInseridos[index],false,false,2);

		index++;

		if (2*(i+1) <= dimensao) {

			label="x[" + patch::to_string(index) + "]: ";
			GtkWidget *temp2 = gtk_label_new(&label[0]);

			gtk_box_pack_start(GTK_BOX(paineisPreenchidos[i]),temp2,false,false,0);

			camposInseridos[index] = gtk_entry_new();

			if (!editavel) {

				gtk_widget_set_can_focus(camposInseridos[index],false);
			}

			gtk_box_pack_start(GTK_BOX(paineisPreenchidos[i]),camposInseridos[index],false,false,2);

			index++;

		}
	}
	/*************************************************************************/

}

bool camposPreenchidos () {

	for (int i=0;i<dimensao; i++) {

		if (gtk_entry_get_text_length(GTK_ENTRY(chutes[i])) == 0) {

			return false;
		}
	}

	return true;
}

bool camposInvalidos () {

	for (int i=0;i<dimensao; i++) {

		std::string aux = gtk_entry_get_text(GTK_ENTRY(chutes[i]));
		char* aux2;

		if (aux.find(".") != std::string::npos) {

			return true;
		}

		valorChute[i] = std::strtod(&aux[0],&aux2);
		
		if (valorChute[i] == 0 && aux2 == aux) {

			return true;
		}

		else if (errno == ERANGE){

			errno = 0;
            return true;
        }

	}

	return false;
}

//(x0 - 2)^4 + (x0 - 2 * x1)^2

void chamaMetodo () {

	switch (metodo) {

		case 0: {

			resposta = coord_ciclicas(funcao, valorChute, dimensao, erro);
			break;
		}

		case 1: {

			resposta = hooke_jeeves(funcao, valorChute, dimensao, erro);
			break;
		}

		case 2: {

			resposta = passo_descendente(funcao, valorChute, dimensao, erro);
			break;
		}

		case 3: {

			resposta = newton(funcao, valorChute, dimensao, erro);
			break;
		}

		case 4: {

			g_print("Gradiente Generalizado\n");
			break;
		}

		case 5: {

			g_print("Fletcher & Reeves\n");
			break;
		}
	}
}

void preencheResposta () {

	for (int i=0;i<dimensao;i++) {

		//g_print("Preenchido Campo %d\n",i);
		gtk_entry_set_text(GTK_ENTRY(resultado[i]),(patch::to_string(resposta[i])).c_str());
	}
}

static void entradaPronta (GtkWidget* widget, gpointer data) {

	int precisao = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinnerErro));

	/*Variáveis de Suporte ás Operações Realizadas Abaixo*/
	dimensao = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinnerDim));
	erro = pow(10,-precisao);
	funcao = gtk_entry_get_text(GTK_ENTRY(campoFuncao));
	/*************************************************************************/

	/*Limpa o Painel Com os Campos para que Eles Não se Acumulem e Inicia os Layouts que Conterão os Campos, e os Inclui no Layout Principal*/
	limpaCriaPainel(layoutChutes,layoutCampos);
	/*************************************************************************/

	/*Checa se o Usuário Preenheu o Campo da Função, se Sim, Prossegue, Caso Contrário Para*/
	if (funcao.empty()) {

		gtk_widget_show(funcaoNaoPreenchida);
		return;
	}
	/*************************************************************************/
	
	/*Checa se a Função do Usuário Possui ao Menos em x[0]*/
	if (funcao.find("x0")==std::string::npos) {

		gtk_widget_show(funcaoInvalida);
		return;
	}
	/*************************************************************************/

	/*Preenche Os Layouts do Campo com Labels para Identificar os Campos e os Campos em Si, Dependedendo da Dimensão do Problema*/
	preenchePaineis(layoutCampos,chutes,true);
	/*************************************************************************/

	/*Esconde alguns Painéis, Mostra Outros para Manter a Consitência do Programa e Força os Campos a Serem Mostrados*/
	gtk_widget_set_visible(layoutChuteGeral,true);
	gtk_widget_set_visible(layoutRespostaGeral,false);
	gtk_widget_set_visible(layoutRespostaFuncao,false);
	gtk_window_resize(GTK_WINDOW(window),10,10);
	gtk_widget_show_all(layoutChutes);
	/*************************************************************************/
}

static void chutesProntos (GtkWidget* widget, gpointer data) {

	/*Variáveis de Suporte ás Operações Realizadas Abaixo*/
	metodo = gtk_combo_box_get_active(GTK_COMBO_BOX(comboMetodos));
	/*************************************************************************/

	/*Limpa o Painel Com os Campos para que Eles Não se Acumulem e Inicia os Layouts que Conterão os Campos, e os Inclui no Layout Principal*/
	limpaCriaPainel(layoutRespostas,layoutCampos2);
	/*************************************************************************/

	/*Checa se Algum dos Campos do Chute Inicial Não Foi Preenchido*/
	if (!camposPreenchidos()) {

		gtk_widget_set_visible(layoutRespostaGeral,false);
		gtk_widget_set_visible(layoutRespostaFuncao,false);
		gtk_widget_show(campoNaoPreenchido);
		return;
	}
	/*************************************************************************/

	/*Checa se Algum dos Campos do Chute Inicial é Inválido, ou Seja, Se Não é um Número*/
	if (camposInvalidos()) {

		gtk_widget_set_visible(layoutRespostaGeral,false);
		gtk_widget_set_visible(layoutRespostaFuncao,false);
		gtk_widget_show(campoInvalido);
		return;

	}
	/*************************************************************************/

	/*Checa o Método Escolhido e Passa para Ele os Dados Necesários para o Cálculo*/
	chamaMetodo();
	/*************************************************************************/

	/*Preenche Os Layouts do Campo com Labels para Identificar os Campos e os Campos em Si, Dependedendo da Dimensão do Problema*/
	preenchePaineis(layoutCampos2,resultado,false);
	/*************************************************************************/

	/*Preenche os Campos de Resposta com os Valores Obtidos do Cálculo do Método*/
	preencheResposta();
	/*************************************************************************/

	/*Preenche o Campo do Valor da Função no X Ótimo Obtido no Cálculo*/
	double valorFuncDouble = (prepara_func(resposta,dimensao,funcao)).value();
	const char* valorFuncString = (patch::to_string(valorFuncDouble)).c_str();
	gtk_entry_set_text(GTK_ENTRY(funcaoOtima),valorFuncString);
	/*************************************************************************/

	gtk_widget_set_visible(layoutRespostaGeral,true);
	gtk_widget_set_visible(layoutRespostaFuncao,true);
	gtk_widget_show_all(layoutRespostas);
}

void preparaEntradas () {

	/*Adquirida Instância do Combo Box de Métodos*/
	comboMetodos = GTK_WIDGET(gtk_builder_get_object(builder, "comboMetodos"));
	/*---------------------------------------------------------------------------------------*/

	/*Adquirida Instância do Campo de Entrada de Função*/
	campoFuncao = GTK_WIDGET(gtk_builder_get_object(builder, "entradaFuncao"));
	/*---------------------------------------------------------------------------------------*/

	/*Configurações do Spinner de Dimensão*/
	adjustment = gtk_adjustment_new (2, 2, MAIOR_DIMENSAO, 1, 0.1, 0.0);
	spinnerDim = GTK_WIDGET(gtk_builder_get_object(builder, "spinDim"));
	gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(spinnerDim),GTK_ADJUSTMENT(adjustment));
	/*---------------------------------------------------------------------------------------*/

	/*Configurações do Spinner de Erro*/
	adjustment = gtk_adjustment_new (1, 1, 6, 1, 0.1, 0.0);
	spinnerErro = GTK_WIDGET(gtk_builder_get_object(builder, "spinErro"));
	gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(spinnerErro),GTK_ADJUSTMENT(adjustment));
	/*---------------------------------------------------------------------------------------*/

	/*Configurações do Botão OK da entrada*/
	botaoOk = GTK_WIDGET(gtk_builder_get_object(builder, "botaoEntrada"));
	g_signal_connect(botaoOk, "clicked", G_CALLBACK(entradaPronta), NULL);
	/*---------------------------------------------------------------------------------------*/
}

void preparaChutes () {

	/*Adquirida Instância do Layout de Chute*/
	layoutChuteGeral = GTK_WIDGET(gtk_builder_get_object(builder, "layoutChute"));
	/*---------------------------------------------------------------------------------------*/

	/*Adquirida Instância do Layout dos Campos de Inserção*/
	layoutChutes = GTK_WIDGET(gtk_builder_get_object(builder, "layoutCamposChute"));
	/*---------------------------------------------------------------------------------------*/

	/*Configurações do Botão OK da entrada*/
	botaoOk2 = GTK_WIDGET(gtk_builder_get_object(builder, "botaoChute"));
	g_signal_connect(botaoOk2, "clicked", G_CALLBACK(chutesProntos), NULL);
	/*---------------------------------------------------------------------------------------*/
}

void preparaRespostas () {

	/*Adquirida Instância do Layout de Resposta*/
	layoutRespostaGeral = GTK_WIDGET(gtk_builder_get_object(builder, "layoutResultado"));
	/*---------------------------------------------------------------------------------------*/

	/*Adquirida Instância do Layout da Função no Ponto Ótimo*/
	layoutRespostaFuncao = GTK_WIDGET(gtk_builder_get_object(builder, "layoutResultado2"));
	/*---------------------------------------------------------------------------------------*/

	/*Adquirida Instância do Layout dos Campos de Resposta*/
	layoutRespostas = GTK_WIDGET(gtk_builder_get_object(builder, "layoutCamposResposta"));
	/*---------------------------------------------------------------------------------------*/

	/*Adquirida Instância do Campo de Valor da Função no Ponto Ótimo*/
	funcaoOtima = GTK_WIDGET(gtk_builder_get_object(builder, "entradaFuncaoOtima"));
	/*---------------------------------------------------------------------------------------*/
}

void preparaAvisos () {

	/*Criada Janela de Aviso Para Função Não Preenchida*/
	funcaoNaoPreenchida = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE, "Função Não Preenchida !\nAperte Esc para sair automaticamente.");
	gtk_window_set_accept_focus(GTK_WINDOW(funcaoNaoPreenchida),false);
	g_signal_connect(funcaoNaoPreenchida, "delete_event", G_CALLBACK(escondeJanela), NULL);
	/*---------------------------------------------------------------------------------------*/

	/*Criada Janela de Aviso Para Função Inválida*/
	funcaoInvalida = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE, "Função Inválida !\nÍndice do Vetor x Deve Começar em 0!\nAperte Esc para sair automaticamente.");
	gtk_window_set_accept_focus(GTK_WINDOW(funcaoInvalida),false);
	g_signal_connect(funcaoInvalida, "delete_event", G_CALLBACK(escondeJanela), NULL);
	/*---------------------------------------------------------------------------------------*/

	/*Criada Janela de Aviso Para Campo(s) Vazios*/
	campoNaoPreenchido = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE, "Campo Não Prenchido !\nAperte Esc para sair automaticamente.");
	gtk_window_set_accept_focus(GTK_WINDOW(campoNaoPreenchido),false);
	g_signal_connect(campoNaoPreenchido, "delete_event", G_CALLBACK(escondeJanela), NULL);
	/*---------------------------------------------------------------------------------------*/

	/*Criada Janela de Aviso Para Campo(s) Vazios*/
	campoInvalido = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE, "Campo Inválido !\nApenas Números Reais, Não Infinitos \ne com Parte Decimal Separada por Vírgula !\nAperte Esc para sair automaticamente.");
	gtk_window_set_accept_focus(GTK_WINDOW(campoInvalido),false);
	g_signal_connect(campoInvalido, "delete_event", G_CALLBACK(escondeJanela), NULL);
	/*---------------------------------------------------------------------------------------*/
}

int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv);
 
	builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "interface.glade", NULL);
 
	window = GTK_WIDGET(gtk_builder_get_object(builder, "janela"));
	gtk_builder_connect_signals(builder, NULL);
 
	gtk_window_set_resizable(GTK_WINDOW(window),false);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ALWAYS);

	preparaEntradas();
	preparaChutes();
	preparaRespostas();
	preparaAvisos();

	g_object_unref(builder);
 
	gtk_widget_show(window);
	gtk_main();

	return 0;
}