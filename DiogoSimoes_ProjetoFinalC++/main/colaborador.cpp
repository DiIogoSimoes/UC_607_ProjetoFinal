#include "colaborador.h"
#include <ctime>

// Implementação centralizada das funções de data

bool ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

bool validar_data(int d, int m, int a) {
    if (a < 1900 || a > 2100) return false;
    if (m < 1 || m > 12) return false;

    int diasMes[] = { 31, ehBissexto(a) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (d < 1 || d > diasMes[m - 1]) return false;

    return true;
}

bool eh_fim_de_semana(int d, int m, int a) {
    std::tm t = {};
    t.tm_mday = d;
    t.tm_mon = m - 1;
    t.tm_year = a - 1900;
    std::mktime(&t); // Normaliza a data
    // 0 = Domingo, 6 = Sábado
    return (t.tm_wday == 0 || t.tm_wday == 6);
}