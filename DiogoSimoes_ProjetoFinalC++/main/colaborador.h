#pragma once
#ifndef COLABORADOR_H
#define COLABORADOR_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// Estruturas de dados
struct Formacao {
    string nome_curso;
    string data_conclusao;
};

struct Nota {
    string texto;
    string data;
};

struct Colaborador {
    int id;
    string nome;
    string departamento;
    // Data (YYYY-MM-DD) -> Tipo ('F' ou 'X')
    map<string, char> marcacoes;
    vector<Formacao> formacoes;
    vector<Nota> notas;
};

// Declaração de funções auxiliares de data (para serem usadas em todo o projeto)
bool ehBissexto(int ano);
bool validar_data(int d, int m, int a);
bool eh_fim_de_semana(int d, int m, int a);

#endif