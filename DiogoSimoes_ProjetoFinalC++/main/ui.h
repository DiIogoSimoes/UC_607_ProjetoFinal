#pragma once
#ifndef UI_H
#define UI_H

#include <string>

void pausa();
bool confirma(const std::string& msg);
void clearScreen();
std::string toLower(std::string s);
int lerInteiroIntervalo(const std::string& msg, int min, int max);

#endif // UI_H