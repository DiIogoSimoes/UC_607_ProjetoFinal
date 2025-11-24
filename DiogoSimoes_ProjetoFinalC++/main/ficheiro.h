#pragma once
#ifndef FICHEIRO_H
#define FICHEIRO_H

#include <string>
#include <vector>
#include "colaborador.h"

std::string cesarCifra(const std::string& s, int desloc);

void gravarDados(const std::string& caminho, const std::vector<Colaborador>& lista, int nextId);
bool carregarDados(const std::string& caminho, std::vector<Colaborador>& lista, int& nextId);
void imprimirCalendario(const Colaborador& c, int mes, int ano);

#endif