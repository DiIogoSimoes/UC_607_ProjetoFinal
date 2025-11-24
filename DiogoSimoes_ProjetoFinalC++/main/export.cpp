#include "export.h"
#include "colaborador.h" 
#include "ui.h" 
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm> 

using namespace std;

// --- FUNÇÕES AUXILIARES DE DATA ---

static string obterNomeMes(int mes) {
    string nomes[] = {
        "Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho",
        "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"
    };
    if (mes >= 1 && mes <= 12) {
        return nomes[mes - 1];
    }
    return "Mes Invalido";
}

static bool ehBissextoExport(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

static int diasNoMesExport(int mes, int ano) {
    int diasMes[] = { 31, (ehBissextoExport(ano) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (mes < 1 || mes > 12) return 0;
    return diasMes[mes - 1];
}

// --- FUNÇÕES DE EXPORTAÇÃO ---

// 1. Exporta o calendário individual (Vertical) - MANTIDO
bool exportarCalendarioColaboradorCSV(const Colaborador& c, int mes, int ano, const string& caminho) {
    ofstream ofs(caminho);
    if (!ofs) return false;

    int dias = diasNoMesExport(mes, ano);

    ofs << "MAPA DE FERIAS INDIVIDUAL\n";
    ofs << "Colaborador: " << c.nome << "\n";
    ofs << "Data: " << obterNomeMes(mes) << "/" << ano << "\n\n";

    ofs << left << setw(5) << "DIA" << setw(10) << "ESTADO" << "DESCRICAO\n";
    ofs << "--------------------------------\n";

    for (int d = 1; d <= dias; ++d) {
        string key = to_string(ano) + "-" + (mes < 10 ? "0" : "") + to_string(mes) + "-" + (d < 10 ? "0" : "") + to_string(d);
        char mark = '.';
        auto it = c.marcacoes.find(key);
        if (it != c.marcacoes.end())
            mark = it->second;

        string desc;
        if (mark == 'F') desc = "Ferias";
        else if (mark == 'X') desc = "Falta";
        else if (mark == '.') desc = "Normal";
        else desc = "Indefinido";

        ofs << left << setw(5) << d << setw(10) << mark << desc << "\n";
    }

    return true;
}

// 2. Exporta o mapa do departamento (LISTA VERTICAL COMPLETA) - CORRIGIDO
bool exportarDepartamentoCSV(const vector<Colaborador>& lista, const string& departamento, int mes, int ano, const string& caminho) {
    ofstream ofs(caminho);
    if (!ofs) return false;

    int dias = diasNoMesExport(mes, ano);

    // Título e metadados
    ofs << "MAPA DE MARCACOES POR DEPARTAMENTO (CALENDARIO VERTICAL)\n";
    ofs << "Departamento: " << (departamento.empty() ? "GERAL" : departamento) << "\n";
    ofs << "Mes/Ano: " << obterNomeMes(mes) << "/" << ano << "\n\n";

    // Cabeçalho da Tabela Vertical de Detalhe
    // Alterei 'DATA' para 'DIA' e ajustei o espaço do dia para 5
    ofs << left << setw(5) << "ID" << setw(20) << "COLABORADOR" << setw(5) << "DIA" << setw(10) << "ESTADO" << "DESCRICAO\n";
    ofs << "-----------------------------------------------------\n";

    // Dados
    for (const auto& c : lista) {
        // Filtro por departamento
        if (!departamento.empty() && toLower(c.departamento) != toLower(departamento)) continue;

        for (int d = 1; d <= dias; ++d) {
            string key = to_string(ano) + "-" + (mes < 10 ? "0" : "") + to_string(mes) + "-" + (d < 10 ? "0" : "") + to_string(d);
            char mark = '.'; // Por defeito, é um dia normal

            auto it = c.marcacoes.find(key);
            if (it != c.marcacoes.end()) {
                mark = it->second; // Marca real (F, X ou .)
            }

            string desc;
            if (mark == 'F') desc = "Ferias";
            else if (mark == 'X') desc = "Falta";
            else if (mark == '.') desc = "Normal";

            // Formato Tabela Visual Vertical: ID | Nome | DIA | Status
            ofs << left << setw(5) << c.id
                << setw(20) << c.nome.substr(0, 18)
                << setw(5) << d // Apenas o número do dia
                << setw(10) << mark
                << desc << "\n";
        }
        // Linha separadora entre colaboradores
        ofs << "-----------------------------------------------------\n";
    }

    return true;
}