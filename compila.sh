#!/bin/bash
# cria o arquivo objeto para o avaliador
#g++ -c avaliador.cpp

# cria o arquivo objeto para common
g++ -c common.cpp 

# cria o arquivo objeto para os metodos
g++ -c metodos.cpp

# cria o arquivo objeto para a interface
g++ interface.cpp `pkg-config --cflags --libs gtk+-3.0` -o t2po2 metodos.o avaliador.o common.o