#include "ui.h"
#include <iostream>
#include <algorithm>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

#undef max
#undef min

using namespace std;

void pausa()
{
    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool confirma(const string& msg)
{
    cout << msg << " (s/n): ";
    char r;
    cin >> r;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return (r == 's' || r == 'S');
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}

string toLower(string s)
{
    transform
    (
        s.begin(),
        s.end(),
        s.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    return s;
}

int lerInteiroIntervalo(const string& msg, int min, int max) {
    int valor;
    while (true) {
        cout << msg;
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpar buffer
                return valor;
            }
            else {
                cout << ">> Erro: O valor deve ser entre " << min << " e " << max << ".\n";
            }
        }
        else {
            cout << ">> Erro: Entrada invalida (nao e numero).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}