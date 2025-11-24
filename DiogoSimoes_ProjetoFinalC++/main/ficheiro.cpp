#include "ficheiro.h" 
#include "colaborador.h" // Importante para ver ehBissexto
#include <fstream> 
#include <iomanip> 
#include <iostream> 
#include <sstream> 
#include <cctype> 
#include <ctime> 

using namespace std;

// Implementa a cifra de César para cifrar e decifrar strings
string cesarCifra(const string& s, int desloc) {
    string r = s;
    for (char& ch : r) { // Itera sobre cada caractere
        unsigned char u = static_cast<unsigned char>(ch);
        if (isalpha(u)) { // Verifica se é uma letra
            bool up = isupper(u);
            char base = up ? 'A' : 'a';
            ch = static_cast<char>(base + ((u - base + desloc + 26) % 26));
        }
    }
    return r;
}

// Grava os dados da lista de colaboradores e o próximo ID num ficheiro
void gravarDados(const string& caminho, const vector<Colaborador>& lista, int nextId)
{
    ofstream ofs(caminho);
    if (!ofs) {
        cout << "Erro: nao foi possivel gravar ficheiro.\n";
        return;
    }

    for (const auto& c : lista) {
        ofs << "===COLAB===\n";
        ofs << "ID:" << c.id << "\n";
        ofs << "NOME:" << cesarCifra(c.nome, 4) << "\n";
        ofs << "DEPT:" << cesarCifra(c.departamento, 4) << "\n";

        for (auto& p : c.marcacoes) {
            ofs << "MARC:" << p.first << ";" << p.second << "\n";
        }
        for (auto& f : c.formacoes) {
            ofs << "FORM:" << cesarCifra(f.nome_curso, 4) << ";" << f.data_conclusao << "\n";
        }
        for (auto& n : c.notas) {
            ofs << "NOTA:" << cesarCifra(n.texto, 4) << ";" << n.data << "\n";
        }
        ofs << "===END===\n";
    }
    ofs << "===META===\n";
    ofs << "NEXTID:" << nextId << "\n";
}

// Carrega os dados de colaboradores e o próximo ID de um ficheiro
bool carregarDados(const string& caminho, vector<Colaborador>& lista, int& nextId)
{
    ifstream ifs(caminho);
    if (!ifs) return false;

    string linha;
    Colaborador cur;
    bool inColab = false;
    nextId = 1;

    while (getline(ifs, linha)) {
        if (linha == "===COLAB===") {
            cur = Colaborador{};
            inColab = true;
            continue;
        }
        if (linha == "===END===") {
            if (cur.id == 0) cur.id = nextId++;
            lista.push_back(cur);
            inColab = false;
            continue;
        }
        if (linha.rfind("ID:", 0) == 0) {
            cur.id = stoi(linha.substr(3));
            if (cur.id >= nextId) nextId = cur.id + 1;
            continue;
        }
        if (linha.rfind("NOME:", 0) == 0) {
            cur.nome = cesarCifra(linha.substr(5), -4);
            continue;
        }
        if (linha.rfind("DEPT:", 0) == 0) {
            cur.departamento = cesarCifra(linha.substr(5), -4);
            continue;
        }
        if (linha.rfind("MARC:", 0) == 0) {
            string rest = linha.substr(5);
            size_t p = rest.find(';');
            if (p != string::npos) {
                string data = rest.substr(0, p);
                char tipo = rest[p + 1];
                cur.marcacoes[data] = tipo;
            }
            continue;
        }
        if (linha.rfind("FORM:", 0) == 0) {
            string rest = linha.substr(5);
            size_t p = rest.find(';');
            if (p != string::npos) {
                string curso = cesarCifra(rest.substr(0, p), -4);
                string data = rest.substr(p + 1);
                cur.formacoes.push_back({ curso, data });
            }
            continue;
        }
        if (linha.rfind("NOTA:", 0) == 0) {
            string rest = linha.substr(5);
            size_t p = rest.find(';');
            if (p != string::npos) {
                string texto = cesarCifra(rest.substr(0, p), -4);
                string data = rest.substr(p + 1);
                cur.notas.push_back({ texto, data });
            }
            continue;
        }
        if (linha == "===META===") {
            while (getline(ifs, linha)) {
                if (linha.rfind("NEXTID:", 0) == 0) {
                    nextId = stoi(linha.substr(7));
                    break;
                }
            }
            continue;
        }
    }
    return true;
}

// Imprime um calendário mensal
void imprimirCalendario(const Colaborador& c, int mes, int ano) {
    cout << "\nCalendario de " << c.nome << " (" << mes << "/" << ano << ")\n";
    cout << "Su Mo Tu We Th Fr Sa\n";

    std::tm first = {};
    first.tm_mday = 1;
    first.tm_mon = mes - 1;
    first.tm_year = ano - 1900;
    std::mktime(&first);

    int start = first.tm_wday;

    int diasMes[] = { 31, ehBissexto(ano) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int dias = diasMes[mes - 1];
    int col = 0;

    for (int i = 0; i < start; i++) {
        cout << "   ";
        col++;
    }

    for (int d = 1; d <= dias; d++) {
        string key = to_string(ano) + "-" + (mes < 10 ? "0" : "") + to_string(mes) + "-" + (d < 10 ? "0" : "") + to_string(d);

        char marca = ' ';
        auto it = c.marcacoes.find(key);
        if (it != c.marcacoes.end()) marca = it->second;

        bool fimsem;
        std::tm t = {};
        t.tm_mday = d;
        t.tm_mon = mes - 1;
        t.tm_year = ano - 1900;
        std::mktime(&t);
        fimsem = (t.tm_wday == 0 || t.tm_wday == 6);

        if (fimsem) {
            cout << setw(2) << d << (marca == ' ' ? '*' : marca);
        }
        else {
            cout << setw(2) << d << (marca == ' ' ? ' ' : marca);
        }

        cout << " ";
        col++;
        if (col % 7 == 0) cout << "\n";
    }
    cout << "\nLegenda: F=Ferias | X=Falta | *=Fim de Semana\n";
}