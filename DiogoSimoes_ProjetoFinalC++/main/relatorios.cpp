#include "relatorios.h" 
#include "colaborador.h" 
#include <iostream>
#include <iomanip>
#include <map>
#include <limits>
#include <algorithm>
#include <string>

using namespace std;

// Funcao auxiliar: Conta as marcações (Férias e Faltas) de um colaborador para um dado mês/ano e para o ano completo.
static void contarFeriasFaltas(const Colaborador& c, int mes, int ano, int& feriasMes, int& faltasMes, int& feriasAno) {
    feriasMes = 0;
    faltasMes = 0;
    feriasAno = 0;

    // Itera sobre todas as marcações do colaborador (map<string data, char tipo>)
    for (const auto& p : c.marcacoes) {
        // Extrai Ano (4 dígitos) e Mês (2 dígitos) da chave no formato "YYYY-MM-DD"
        int y = stoi(p.first.substr(0, 4));
        int m = stoi(p.first.substr(5, 2));
        char t = p.second; // Tipo de marcação ('F' ou 'X')

        // 1. Contagem do Mês/Ano específico
        if (y == ano && m == mes) {
            if (t == 'F') ++feriasMes;
            else if (t == 'X') ++faltasMes;
        }

        // 2. Contagem de Férias do Ano completo (para o dashboard)
        if (y == ano && t == 'F') {
            ++feriasAno;
        }
    }
}

// Gera um relatório detalhado das marcações (Férias e Faltas) de cada colaborador no mês/ano fornecido.
void relatorioMensal(const vector<Colaborador>& lista, int mes, int ano) {
    cout << "\nRelatorio mensal " << mes << "/" << ano << "\n";

    // Configuração do cabeçalho com largura fixa (left-justified)
    cout << left << setw(6) << "ID" << setw(25) << "Nome" << setw(15) << "Dept" << setw(8) << "Ferias" << setw(8) << "Faltas" << "\n";
    cout << string(70, '-') << "\n";

    for (const auto& c : lista) {
        int f, x, fa;
        // Chama a função auxiliar para obter as contagens
        contarFeriasFaltas(c, mes, ano, f, x, fa);

        // Imprime os dados formatados
        cout << left << setw(6) << c.id << setw(25) << c.nome << setw(15) << c.departamento << setw(8) << f << setw(8) << x << "\n";
    }

    // Aguarda a interação do utilizador
    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Gera estatísticas agregadas por departamento, somando Férias e Faltas no mês/ano.
void estatisticasDepartamento(const vector<Colaborador>& lista, int mes, int ano) {
    // Mapa para armazenar as estatísticas por departamento: 
    map<string, pair<int, int>> stats;

    // 1. Agrega dados de todos os colaboradores no mapa 'stats'
    for (const auto& c : lista) {
        for (const auto& p : c.marcacoes) {
            int y = stoi(p.first.substr(0, 4));
            int m = stoi(p.first.substr(5, 2));

            // Filtra apenas as marcações do mês e ano fornecidos
            if (y == ano && m == mes) {
                if (p.second == 'F') stats[c.departamento].first++;  // Incrementa Férias
                else if (p.second == 'X') stats[c.departamento].second++; // Incrementa Faltas
            }
        }
    }

    cout << "\nEstatisticas por departamento " << mes << "/" << ano << "\n";
    // Configuração do cabeçalho
    cout << left << setw(20) << "Departamento" << setw(10) << "Ferias" << setw(10) << "Faltas" << "\n";
    cout << string(45, '-') << "\n";

    string worst;
    int worstCount = -1;

    // 2. Imprime o mapa e identifica o departamento com mais ausências
    for (const auto& kv : stats) {
        int total = kv.second.first + kv.second.second; // Soma Férias + Faltas

        // Imprime os dados formatados
        cout << left << setw(20) << kv.first << setw(10) << kv.second.first << setw(10) << kv.second.second << "\n";

        // Lógica para encontrar o departamento com o maior total de ausências
        if (total > worstCount) {
            worstCount = total;
            worst = kv.first;
        }
    }

    // 3. Exibe o resultado do departamento com mais ausências
    if (worstCount >= 0) {
        cout << "\nDepartamento com mais ausencias: " << worst << " (" << worstCount << " dias)\n";
    }
    else {
        cout << "\nNenhuma ausencia registada neste mes.\n";
    }

    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Gera um dashboard resumido que mostra o saldo de férias restantes para o ano.
void dashboardResumido(const vector<Colaborador>& lista, int mes, int ano, int dias_ano) {
    cout << "\nDashboard resumido " << mes << "/" << ano << "\n";
    // Configuração do cabeçalho
    cout << left << setw(25) << "Nome" << setw(8) << "Ferias" << setw(8) << "Faltas" << setw(14) << "Dias restantes" << "\n";
    cout << string(60, '-') << "\n";

    int totalFeriasAno = 0;
    for (const auto& c : lista) {
        int f, x, fa;
        // A função auxiliar é crucial aqui, pois 'fa' retorna as férias tiradas no ANO
        contarFeriasFaltas(c, mes, ano, f, x, fa);

        totalFeriasAno += fa; // Soma para o total geral de férias anuais

        int restantes = dias_ano - fa; // dias_ano é o total de dias de férias disponíveis 
        if (restantes < 0) restantes = 0; // Garante que o saldo não é negativo

        // Imprime os dados formatados
        // Coluna 1: Férias do Mês (f) | Coluna 2: Faltas do Mês (x) | Coluna 3: Saldo Anual (restantes)
        cout << left << setw(25) << c.nome << setw(8) << f << setw(8) << x << setw(14) << restantes << "\n";
    }

    cout << "\nTotal de ferias no ano: " << totalFeriasAno << "\n";
    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}