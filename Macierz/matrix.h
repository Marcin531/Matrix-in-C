#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

int CreateMatrix( double*** pTab, int nSize ); // 0 - error 1 - ok  0 tablica ma byc tworzona pierwszym sposobem z wykladu
int CreateVector( double** pTab, int nSize );//tworzy tablice wyrazow wolnych
void DeleteMatrix( double*** pTab, int nSize );	//po wyjsciu z tej funkcji wskaznik tablicy jest NULL'em
void InverseMatrix( double ** pInv, double** pTab, int nSize, double det );	// pTab- macierz, det- wyznacznik
double Det( double ** pTab, int nSize ); // rozwiazanie wzgledem 0-go wiersza, liczy wyznacznik z podanej macierzy- 
//liczyc rekurecyjnie jesli zejdzie do rozmiaru dwa to liczyc juz mozna normalnie
void LayoutEqu( double ** pInv, double* pV, double* pRes, int nSize );// pInv- macierz odwrotna, pV-wektor wyrazow wolny, pRes- wyjscio
//wy wektor rozwiazan
void PrintMatrix( double** pTab, int nSize );	//drukuje macierz wierszami



void TransMatrix( double ** pTab, int nDim ); // transofowanie macierzy, dziala na tablicy pTab i w niej dokonuje transfomowania  



#endif
