#ifndef RECUP_FICHIER
#define RECUP_FICHIER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T_MEMOIRE 5000

int syntaxe(char mot[]);
int calcul_adr(char etiquette[][100],  char etiq[], int ligne);
int etiquette_colle(char str[]);
int code_inst(char instr[]);
int recup_fichier(int argc, char* argv[]);

#endif   
