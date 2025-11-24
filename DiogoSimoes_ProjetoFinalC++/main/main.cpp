#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <ctime>
#include <string>

// Inclusão dos cabeçalhos das estruturas e funcionalidades
#include "colaborador.h"
#include "ficheiro.h"
#include "relatorios.h"
#include "export.h"
#include "ui.h"

using namespace std;

// Variável estática para garantir que cada novo colaborador tenha um ID único
static int nextId = 1;

// --- FUNÇÃO DE VALIDAÇÃO GLOBAL PARA STRINGS (YYYY-MM-DD) ---
bool validarStringData(const string& s) {
    // 1. Verifica o tamanho (tem de ter 10 caracteres: 2023-01-01)
    if (s.length() != 10) return false;

    // 2. Verifica se tem os traços nas posições certas
    if (s[4] != '-' || s[7] != '-') return false;

    // 3. Tenta converter os números e validar a lógica (dias do mês, bissexto, etc)
    try {
        int ano = stoi(s.substr(0, 4));
        int mes = stoi(s.substr(5, 2));
        int dia = stoi(s.substr(8, 2));

        // Usa a função validar_data que já existe no colaborador.cpp
        return validar_data(dia, mes, ano);
    }
    catch (...) {
        return false; // Se der erro na conversão (ex: letras no lugar de números)
    }
}

// Função auxiliar para buscar o ID de um colaborador pelo nome (parcial) ou pelo ID
int buscarIdPorNomeOuId(const vector<Colaborador>& lista, const string& entrada) {
    try {
        int id = stoi(entrada);
        for (const auto& c : lista) {
            if (c.id == id) {
                return id;
            }
        }
    }
    catch (...) {
        // nao era numero
    }
    string q = toLower(entrada);
    for (const auto& c : lista) {
        if (toLower(c.nome).find(q) != string::npos) {
            return c.id;
        }
    }
    return -1; // Retorna -1 se não encontrar
}

// Função auxiliar para procurar e retornar o ponteiro de um colaborador pelo seu ID
Colaborador* procurarPorId(vector<Colaborador>& lista, int id) {
    for (auto& c : lista) {
        if (c.id == id) {
            return &c;
        }
    }
    return nullptr; // Retorna nullptr se não encontrar
}

// Função para exibir a lista de todos os colaboradores com ID, nome e departamento
void listarColaboradores(const vector<Colaborador>& lista) {
    cout << "\nLista de Colaboradores:\n";
    for (const auto& c : lista) {
        cout << "ID: " << c.id << " | Nome: " << c.nome << " | Departamento: " << c.departamento << "\n";
    }
    pausa();
}

// Função para adicionar um novo colaborador à lista
void adicionarColaborador(vector<Colaborador>& lista) {
    Colaborador c;
    cout << "Nome completo: ";
    getline(cin, c.nome);
    cout << "Departamento: ";
    getline(cin, c.departamento);
    // Verifica a existência de colaboradores com nome igual (duplicados)
    for (const auto& x : lista) {
        if (toLower(x.nome) == toLower(c.nome)) {
            cout << "Ja existe um colaborador com esse nome. ";
            if (!confirma("Confirmar duplicacao?")) {
                return;
            }
            break;
        }
    }
    c.id = nextId++; // Atribui o próximo ID disponível
    lista.push_back(c);
    cout << "Colaborador adicionado com ID " << c.id << "\n";
}

// Verifica se há conflito (outro colega do mesmo departamento de férias) na data
bool existeConflito(const vector<Colaborador>& lista, const Colaborador& c, const string& data) {
    for (const auto& o : lista) {
        if (o.id == c.id) {
            continue;
        }
        if (o.departamento == c.departamento) {
            auto it = o.marcacoes.find(data);
            if (it != o.marcacoes.end() && it->second == 'F') { // 'F' = Férias
                return true;
            }
        }
    }
    return false;
}

// Função principal para marcar (Férias 'F' ou Falta 'X') ou desmarcar (modo 5) um dia
void marcarDesmarcar(vector<Colaborador>& lista, int modo) {
    cout << "ID ou parte do nome do colaborador: ";
    string entrada;
    getline(cin, entrada);
    int id = buscarIdPorNomeOuId(lista, entrada);
    if (id == -1) {
        cout << "Colaborador nao encontrado.\n";
        return;
    }
    Colaborador* c = procurarPorId(lista, id);
    if (!c) {
        cout << "Erro.\n";
        return;
    }

    // Leitura e validação da data
    int d, m, a;
    cout << "Dia: ";
    if (!(cin >> d)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida\n";
        return;
    }

    m = lerInteiroIntervalo("Mes (1-12): ", 1, 12);

    cout << "Ano: ";
    cin >> a;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!validar_data(d, m, a)) {
        cout << "Data invalida.\n";
        return;
    }
    if (eh_fim_de_semana(d, m, a)) {
        cout << "Nao e permitido marcar ao fim-de-semana.\n";
        return;
    }
    // Cria a chave no formato YYYY-MM-DD para o mapa
    string key = to_string(a) + "-" + (m < 10 ? "0" : "") + to_string(m) + "-" + (d < 10 ? "0" : "") + to_string(d);
    if (modo == 5) { // Modo de Desmarcar
        if (c->marcacoes.erase(key)) {
            cout << "Marcacao removida.\n";
        }
        else {
            cout << "Nao havia marcacao nessa data.\n";
        }
        return;
    }

    // Modo de Marcar Férias ('F', modo 3) ou Falta ('X', modo 4)
    char simbolo = (modo == 3) ? 'F' : 'X';

    if (simbolo == 'F') { // Verifica conflito apenas para Férias
        if (existeConflito(lista, *c, key)) {
            cout << "Aviso: outro colaborador do mesmo departamento ja tem ferias nesse dia.\n";
            if (!confirma("Deseja prosseguir e marcar mesmo assim?")) {
                cout << "Operacao cancelada.\n";
                return;
            }
        }
    }

    // Verifica se já existe uma marcação e pede confirmação para substituir
    if (c->marcacoes.count(key)) {
        cout << "Ja existe marcacao nesse dia (" << c->marcacoes[key] << "). ";
        if (!confirma("Substituir?")) {
            return;
        }
    }
    c->marcacoes[key] = simbolo;
    cout << "Marcado " << simbolo << " em " << key << ".\n";
}

// Função para pedir o mês/ano e imprimir o calendário de marcações de um colaborador
void verCalendarioColaborador(vector<Colaborador>& lista) {
    cout << "ID ou parte do nome: ";
    string entrada;
    getline(cin, entrada);
    int id = buscarIdPorNomeOuId(lista, entrada);
    if (id == -1) {
        cout << "Colaborador nao encontrado.\n";
        return;
    }
    Colaborador* c = procurarPorId(lista, id);
    if (!c) {
        cout << "Erro.\n";
        return;
    }

    int mes = lerInteiroIntervalo("Mes (1-12): ", 1, 12);
    cout << "Ano: ";
    int ano;
    cin >> ano;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    imprimirCalendario(*c, mes, ano);
    pausa();
}

// Função para buscar um colaborador por ID ou nome e mostrar seus detalhes
void buscarColaborador(const vector<Colaborador>& lista) {
    cout << "ID ou nome: ";
    string entrada;
    getline(cin, entrada);
    int id = -1;
    try {
        id = stoi(entrada);
    }
    catch (...) {
        // nao numero
    }
    const Colaborador* c = nullptr;
    // Lógica de busca por ID ou nome
    if (id != -1) {
        for (const auto& x : lista) {
            if (x.id == id) {
                c = &x;
                break;
            }
        }
    }
    else {
        string q = toLower(entrada);
        for (const auto& x : lista) {
            if (toLower(x.nome).find(q) != string::npos) {
                c = &x;
                break;
            }
        }
    }
    if (!c) {
        cout << "Nao encontrado.\n";
        return;
    }
    // Exibe detalhes e calendário do mês atual
    cout << "ID: " << c->id << "\n";
    cout << "Nome: " << c->nome << "\n";
    cout << "Departamento: " << c->departamento << "\n";
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    int mes = lt->tm_mon + 1;
    int ano = lt->tm_year + 1900;
    imprimirCalendario(*c, mes, ano);
    pausa();
}

// Função para gerir as formações (cursos) de um colaborador específico
void gerirFormacoes(Colaborador& c) {
    while (true) {
        cout << "\n--- FORMACOES / CURSOS DE " << c.nome << " ---\n";
        // Menu de gestão de formações: Listar, Adicionar, Editar, Remover, Voltar
        cout << "1. Listar formacoes\n2. Adicionar formacao\n3. Editar formacao\n4. Remover formacao\n0. Voltar\nEscolha: ";
        int op;
        if (!(cin >> op)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (op == 0) {
            break;
        }
        else if (op == 1) { // Listar formações
            cout << "\nFormacoes:\n";
            int i = 1;
            for (const auto& f : c.formacoes) {
                cout << i++ << ") " << f.nome_curso << " | " << f.data_conclusao << "\n";
            }
            pausa();
        }
        else if (op == 2) { // Adicionar formação
            Formacao f;
            cout << "Nome do curso: ";
            getline(cin, f.nome_curso);

            // Loop para garantir data válida
            while (true) {
                cout << "Data de conclusao (YYYY-MM-DD): ";
                getline(cin, f.data_conclusao);
                if (validarStringData(f.data_conclusao)) {
                    break;
                }
                cout << "ERRO: Data invalida! Use o formato YYYY-MM-DD.\n";
            }

            c.formacoes.push_back(f);
            cout << "Formacao adicionada.\n";
        }
        else if (op == 3) { // Editar formação
            cout << "Indice da formacao a editar: ";
            int idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx < 1 || idx >(int)c.formacoes.size()) {
                cout << "Indice invalido.\n";
                continue;
            }
            Formacao& f = c.formacoes[idx - 1];
            // Permite deixar o campo vazio para manter o valor atual
            cout << "Nome (" << f.nome_curso << "): ";
            string s;
            getline(cin, s);
            if (!s.empty()) {
                f.nome_curso = s;
            }
            cout << "Data (" << f.data_conclusao << "): ";
            getline(cin, s);
            if (!s.empty()) {
                if (validarStringData(s)) {
                    f.data_conclusao = s;
                    cout << "Data alterada com sucesso.\n";
                }
                else {
                    cout << "ERRO: Data invalida. Mantido o valor anterior.\n";
                }
            }
            cout << "Edicao concluida.\n";
        }
        else if (op == 4) { // Remover formação
            cout << "Indice da formacao a remover: ";
            int idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx < 1 || idx >(int)c.formacoes.size()) {
                cout << "Indice invalido.\n";
                continue;
            }
            c.formacoes.erase(c.formacoes.begin() + (idx - 1));
            cout << "Removido.\n";
        }
    }
}

// Função para gerir as notas/observações de um colaborador específico
void gerirNotas(Colaborador& c) {
    while (true) {
        cout << "\n--- NOTAS / OBSERVACOES DE " << c.nome << " ---\n";
        // Menu de gestão de notas: Listar, Adicionar, Editar, Remover, Voltar
        cout << "1. Listar notas\n2. Adicionar nota\n3. Editar nota\n4. Remover nota\n0. Voltar\nEscolha: ";
        int op;
        if (!(cin >> op)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (op == 0) {
            break;
        }
        else if (op == 1) { // Listar notas
            cout << "\nNotas:\n";
            int i = 1;
            for (const auto& n : c.notas) {
                cout << i++ << ") " << n.data << " | " << n.texto << "\n";
            }
            pausa();
        }
        else if (op == 2) { // Adicionar nota
            Nota n;
            cout << "Texto da nota: ";
            getline(cin, n.texto);

            // Loop para garantir data válida
            while (true) {
                cout << "Data (YYYY-MM-DD): ";
                getline(cin, n.data);
                if (validarStringData(n.data)) {
                    break;
                }
                cout << "ERRO: Data invalida! Use o formato YYYY-MM-DD.\n";
            }

            c.notas.push_back(n);
            cout << "Nota adicionada.\n";
        }
        else if (op == 3) { // Editar nota
            cout << "Indice da nota a editar: ";
            int idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx < 1 || idx >(int)c.notas.size()) {
                cout << "Indice invalido.\n";
                continue;
            }
            Nota& n = c.notas[idx - 1];
            // Permite deixar o campo vazio para manter o valor atual
            cout << "Texto (" << n.texto << "): ";
            string s;
            getline(cin, s);
            if (!s.empty()) {
                n.texto = s;
            }
            cout << "Data (" << n.data << "): ";
            getline(cin, s);
            if (!s.empty()) {
                if (validarStringData(s)) {
                    n.data = s;
                    cout << "Data alterada com sucesso.\n";
                }
                else {
                    cout << "ERRO: Data invalida. Mantido o valor anterior.\n";
                }
            }
            cout << "Edicao concluida.\n";
        }
        else if (op == 4) { // Remover nota
            cout << "Indice da nota a remover: ";
            int idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx < 1 || idx >(int)c.notas.size()) {
                cout << "Indice invalido.\n";
                continue;
            }
            c.notas.erase(c.notas.begin() + (idx - 1));
            cout << "Removido.\n";
        }
    }
}

// Função que exibe o menu principal e gere o fluxo da aplicação
void menuPrincipal(vector<Colaborador>& lista, const string& ficheiro) {
    int opc = -1;
    while (opc != 0) {
        clearScreen(); // Limpa o ecrã
        cout << "=============================\n";
        cout << "      RH AVANCADO\n";
        cout << "=============================\n";
        // Opções principais do menu
        cout << "1. Listar colaboradores\n";
        cout << "2. Adicionar colaborador\n";
        cout << "3. Marcar ferias\n";
        cout << "4. Marcar falta\n";
        cout << "5. Desmarcar\n";
        cout << "6. Visualizar calendario do colaborador\n";
        cout << "7. Buscar colaborador por ID ou nome\n";
        cout << "8. Gerir formacoes/cursos de um colaborador\n";
        cout << "9. Gerir notas/observacoes de um colaborador\n";
        cout << "10. Relatorios mensais\n";
        cout << "11. Estatisticas por departamento\n";
        cout << "12. Dashboard resumido\n";
        cout << "13. Exportar Relatorio (TXT)\n";
        cout << "0. Sair\n";
        cout << "-----------------------------\n";
        cout << "Escolha: ";
        // Leitura da opção
        if (!(cin >> opc)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Implementação da lógica de cada opção
        if (opc == 1) {
            listarColaboradores(lista);
        }
        else if (opc == 2) {
            adicionarColaborador(lista);
        }
        else if (opc == 3) {
            marcarDesmarcar(lista, 3); // Marca Férias ('F')
        }
        else if (opc == 4) {
            marcarDesmarcar(lista, 4);
            // Marca Falta ('X')
        }
        else if (opc == 5) {
            marcarDesmarcar(lista, 5); // Desmarca
        }
        else if (opc == 6) {
            verCalendarioColaborador(lista);
            // Visualiza calendário mensal
        }
        else if (opc == 7) {
            buscarColaborador(lista); // Busca e exibe detalhes
        }
        else if (opc == 8) {
            // Encontra e chama a gestão de formações para o colaborador
            cout << "ID ou nome do colaborador: ";
            string s;
            getline(cin, s);
            int id = buscarIdPorNomeOuId(lista, s);
            if (id == -1) {
                cout << "Nao encontrado.\n";
                pausa();
                continue;
            }
            Colaborador* c = procurarPorId(lista, id);
            if (!c) {
                cout << "Erro.\n";
                continue;
            }
            gerirFormacoes(*c);
        }
        else if (opc == 9) {
            // Encontra e chama a gestão de notas para o colaborador
            cout << "ID ou nome do colaborador: ";
            string s;
            getline(cin, s);
            int id = buscarIdPorNomeOuId(lista, s);
            if (id == -1) {
                cout << "Nao encontrado.\n";
                pausa();
                continue;
            }
            Colaborador* c = procurarPorId(lista, id);
            if (!c) {
                cout << "Erro.\n";
                continue;
            }
            gerirNotas(*c);
        }
        else if (opc == 10) {
            // Gera relatório mensal de marcações
            int mes = lerInteiroIntervalo("Mes (1-12): ", 1, 12);
            cout << "Ano: ";
            int ano;
            cin >> ano;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            relatorioMensal(lista, mes, ano);
        }
        else if (opc == 11) {
            // Gera estatísticas de marcações por departamento
            int mes = lerInteiroIntervalo("Mes (1-12): ", 1, 12);
            cout << "Ano: ";
            int ano;
            cin >> ano;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            estatisticasDepartamento(lista, mes, ano);
        }
        else if (opc == 12) {
            // Gera um dashboard resumido de marcações
            int mes = lerInteiroIntervalo("Mes (1-12): ", 1, 12);
            cout << "Ano: ";
            int ano;
            cin >> ano;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            dashboardResumido(lista, mes, ano, 22);
        }
        else if (opc == 13) {
            // Opção de exportação para ficheiro CSV/TXT
            cout << "Exportar: 1) Colaborador 2) Departamento : ";
            int e;
            cin >> e;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (e == 1) {
                // Exporta o calendário de um único colaborador
                cout << "ID ou nome do colaborador: ";
                string s;
                getline(cin, s);
                int id = buscarIdPorNomeOuId(lista, s);
                if (id == -1) {
                    cout << "Nao encontrado.\n";
                    pausa();
                    continue;
                }
                Colaborador* c = procurarPorId(lista, id);
                if (!c) {
                    cout << "Erro.\n";
                    continue;
                }
                int mes = lerInteiroIntervalo("Mes (1-12): ", 1, 12);
                cout << "Ano: ";
                int ano;
                cin >> ano;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Caminho do ficheiro (ex: relatorio.txt): ";
                string caminho;
                getline(cin, caminho);
                if (exportarCalendarioColaboradorCSV(*c, mes, ano, caminho)) {
                    cout << "Exportado com sucesso. Abra no Bloco de Notas!\n";
                }
                else {
                    cout << "Erro ao exportar.\n";
                }
                pausa();
            }
            else if (e == 2) {
                // Exporta o calendário de um departamento ou todos
                cout << "Nome do departamento (ou Enter para todos): ";
                string dept;
                getline(cin, dept);
                int mes = lerInteiroIntervalo("Mes (1-12): ", 1, 12);
                cout << "Ano: ";
                int ano;
                cin >> ano;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Caminho do ficheiro (ex: departamento.txt): ";
                string caminho;
                getline(cin, caminho);
                if (exportarDepartamentoCSV(lista, dept, mes, ano, caminho)) {
                    cout << "Exportado com sucesso. Abra no Bloco de Notas!\n";
                }
                else {
                    cout << "Erro ao exportar.\n";
                }
                pausa();
            }
        }
        else if (opc == 0) {
            gravarDados(ficheiro, lista, nextId);
            // Grava os dados antes de sair
            cout << "Dados guardados. Adeus!\n";
        }
    }
}

// Função principal de entrada do programa
int main() {
    vector<Colaborador> colaboradores;
    const string ficheiro = "data_rh.txt";
    // Tenta carregar os dados existentes do ficheiro
    if (carregarDados(ficheiro, colaboradores, nextId)) {
        cout << "Dados carregados de '" << ficheiro << "'.\n";
    }
    else {
        cout << "Nenhum ficheiro encontrado. Base de dados vazia.\n";
    }
    menuPrincipal(colaboradores, ficheiro); // Inicia o menu principal
    return 0;
}