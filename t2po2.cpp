#include <gtk/gtk.h>
#include <math.h>
#include <string>
#include <sstream>

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
std::string funcao;
/*Variáveis de Acesso aos Elementos de Chute*/
GtkWidget *layoutChuteGeral;
GtkWidget *layoutChutes;
GtkWidget *botaoOk2;
/*Variáveis de Acesso aos Elementos de Resposta*/
GtkWidget *layoutRespostaGeral;
GtkWidget *layoutRespostaFuncao;
GtkWidget *layoutRespostas;
GtkWidget *funcaoOtima;

/*Variáveis de Controle da Entrada Dinâmica*/
GtkWidget *layoutCampos[MAIOR_DIMENSAO/2];
GtkWidget *chutes[MAIOR_DIMENSAO];
int dimensao;
/*Variáveis de Controle da Saída Dinâmica*/
GtkWidget *layoutCampos2[MAIOR_DIMENSAO/2];
GtkWidget *resultado[MAIOR_DIMENSAO];

/*Variáveis de Configuração de Spinners*/
GtkAdjustment *adjustment;

/*Variáveis das Janelas de Aviso*/
GtkWidget *funcaoNaoPreenchida;
GtkWidget *funcaoInvalida;
GtkWidget *campoNaoPreenchido;

gboolean escondeJanela(GtkWidget *widget, GdkEvent *event, gpointer data){

    gtk_widget_hide(widget);
    return TRUE;
}

static void entradaPronta (GtkWidget* widget, gpointer data) {

	/*Variáveis de Suporte ás Operações Realizadas Abaixo*/
	int index = 0;

	dimensao = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinnerDim));

	int precisao = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinnerErro));
	double erro = pow(10,-precisao);
	/*************************************************************************/

	/*Limpa o Painel Com os Campos para que Eles Não se Acumulem*/
	GList *children, *iter;

	children = gtk_container_get_children(GTK_CONTAINER(layoutChutes));

	for(iter = children; iter != NULL; iter = g_list_next(iter))
  		gtk_widget_destroy(GTK_WIDGET(iter->data));

	g_list_free(children);
	/*************************************************************************/

	/*Inicia os Layouts que Conterão os Campos, e os Inclui no Layout Principal*/	
	for (int i=0;i<MAIOR_DIMENSAO/2;i++) {

		layoutCampos[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		gtk_box_pack_start(GTK_BOX(layoutChutes),layoutCampos[i],0,0,0);
	}
	/*************************************************************************/

	/*Checa se o Usuário Preenheu o Campo da Função, se Sim, Prossegue, Caso Contrário Para*/
	funcao = gtk_entry_get_text(GTK_ENTRY(campoFuncao));

	if (funcao.empty()) {

		gtk_widget_show(funcaoNaoPreenchida);
		return;
	}
	/*************************************************************************/
	
	/*Checa se a Função do Usuário Possui ao Menos em x[0]*/
	if (funcao.find("x[0]")==std::string::npos) {

		gtk_widget_show(funcaoInvalida);
		return;
	}
	/*************************************************************************/

	/*Preenche Os Layouts do Campo com Labels para Identificar os Campos e os Campos em Si, Dependedendo da Dimensão do Problema*/
	for (int i=0;i<ceil(((float)dimensao)/2);i++) {

		std::string label="x[" + patch::to_string(index) + "]: ";
		
		GtkWidget *temp = gtk_label_new(&label[0]);
		
		gtk_box_pack_start(GTK_BOX(layoutCampos[i]),temp,false,false,0);

		chutes[index] = gtk_entry_new();

		gtk_box_pack_start(GTK_BOX(layoutCampos[i]),chutes[index],false,false,2);

		index++;

		if (2*(i+1) <= dimensao) {

			label="x[" + patch::to_string(index) + "]: ";
			GtkWidget *temp2 = gtk_label_new(&label[0]);

			gtk_box_pack_start(GTK_BOX(layoutCampos[i]),temp2,false,false,0);

			chutes[index] = gtk_entry_new();

			gtk_box_pack_start(GTK_BOX(layoutCampos[i]),chutes[index],false,false,2);

			index++;

		}
	}
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

	int index = 0;

	int metodo = gtk_combo_box_get_active(GTK_COMBO_BOX(comboMetodos));

	GList *children, *iter;

	children = gtk_container_get_children(GTK_CONTAINER(layoutRespostas));

	for(iter = children; iter != NULL; iter = g_list_next(iter))
  		gtk_widget_destroy(GTK_WIDGET(iter->data));

	g_list_free(children);

	for (int i=0;i<MAIOR_DIMENSAO/2;i++) {

		layoutCampos2[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		gtk_box_pack_start(GTK_BOX(layoutRespostas),layoutCampos2[i],0,0,0);
	}

	for (int i=0;i<ceil(((float)dimensao)/2);i++) {

		std::string label="x[" + patch::to_string(index) + "]: ";
		
		GtkWidget *temp = gtk_label_new(&label[0]);
		
		gtk_box_pack_start(GTK_BOX(layoutCampos2[i]),temp,false,false,0);

		resultado[index] = gtk_entry_new();

		gtk_widget_set_can_focus(resultado[index],false);
		gtk_box_pack_start(GTK_BOX(layoutCampos2[i]),resultado[index],false,false,2);

		index++;

		if (2*(i+1) <= dimensao) {

			label="x[" + patch::to_string(index) + "]: ";
			GtkWidget *temp = gtk_label_new(&label[0]);

			gtk_box_pack_start(GTK_BOX(layoutCampos2[i]),temp,false,false,0);

			resultado[index] = gtk_entry_new();

			gtk_widget_set_can_focus(resultado[index],false);
			gtk_box_pack_start(GTK_BOX(layoutCampos2[i]),resultado[index],false,false,2);

			index++;

		}
	}

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
}

int main(int argc, char *argv[]){

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