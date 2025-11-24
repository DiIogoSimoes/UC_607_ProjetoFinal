#pragma once
#ifndef RELATORIOS_H
#define RELATORIOS_H

#include <vector>
#include "colaborador.h"

void relatorioMensal(const std::vector<Colaborador>& lista, int mes, int ano);
void estatisticasDepartamento(const std::vector<Colaborador>& lista, int mes, int ano);
void dashboardResumido(const std::vector<Colaborador>& lista, int mes, int ano, int dias_ano);

#endif // RELATORIOS_H