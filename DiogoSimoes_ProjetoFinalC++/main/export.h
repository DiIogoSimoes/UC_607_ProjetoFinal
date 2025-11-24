#pragma once
#include <vector>
#include <string>
#include "colaborador.h"

// Note: A função tem "CSV" no nome, mas na verdade exporta um relatório TXT.

// Exporta o calendário de um colaborador (Formato Vertical)
bool exportarCalendarioColaboradorCSV(const Colaborador& c, int mes, int ano, const std::string& caminho);

// Exporta o mapa de marcações de um departamento (Formato Lista Vertical)
bool exportarDepartamentoCSV(const std::vector<Colaborador>& lista, const std::string& departamento, int mes, int ano, const std::string& caminho);
