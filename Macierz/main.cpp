
//Celem dzialania programu jest przeanalizowanie tekstowego pliku wejsciowego, a nastepnie wyliczenie wyznacznika macierzy, macierzy odwrotnej (przy uzyciu macierzy transponowanej, a nastepnie jej rozwiazanie.
//The purpose of this program is to analyze text input file and calculate the determinant of matrix, create inverted matrix (using transported matrix) and find a solution to it


#include "stdafx.h"
#include "matrix.h"
#include <stdio.h>


#define FILENAME argv[1]  //argv[1]-umozliwia uruchamianie z konsoli
#define _DEBUGMAIN_	   //wyswietla wyniki dzialania poszczegolnych funkcji 



void ReadData( FILE* fin, double** pMatrix, double * b, int nDim );// pliku, ktory zostal wczesniej otwary, macierzy wczesniej 
//wykreowanej b - wektor macierz, nDim - wyrazu wolnego

int main( int argc, const char* argv[] )
{
	if( argc != 2 )
	{
		printf( "Nalezy podac nazwe programu i nazwe pliku tekstowego z macierza\n" );
	}
	FILE* fin;
	

	if( (fin = fopen( FILENAME, "r" )) == NULL ) //jesli nie znajdzie pliku zwraca NULL
	{
		perror( "ERROR- nie mogê otworzyæ pliku" );
	}

	int nDim;//jeden wymiar macierzy

	fscanf( fin, "%d", &nDim );// trzeba podac adres nDim dzieki czemu funkcja przypisuje wartosc

#ifdef _DEBUGMAIN_


	printf( "Wymiar macierzy: \n" );
	printf( "%dx%d\n", nDim,nDim );
#endif

	double** pMatrix = NULL;
	double* pFreeWords = NULL;
	double det;	//wyznacznik

	if( !CreateVector( &pFreeWords, nDim ) )
	{
		perror( "Nie udalo sie utworzyc tablicy wyrazow wolnych" );
	}

	if( !CreateMatrix( &pMatrix, nDim ) )
	{
		perror( "Nie udalo sie utworzyc macierzy" );
	}

	ReadData( fin, pMatrix, pFreeWords, nDim );

#ifdef _DEBUGMAIN_
	printf( "MACIERZ ORYGINAL: \n" );
	PrintMatrix( pMatrix, nDim );


	printf( "\nWyrazy wolne: \n" );
	for( int i = 0; i < nDim; i++ )
	{
		printf( "[%d] = %f\n", i, pFreeWords[i] );
	}
#endif

	det = Det( pMatrix, nDim );	//przypisanie zmiennej det wyznacznika macierzy pMatrix

#ifdef _DEBUGMAIN_
	printf( "\n" );
	printf( "Wyznacznik: %f\n", det );
#endif
	
	double** pInv = NULL;	 //tworzenie miejsca dla macierzy odwroconej
	if( det<1e-12 )//!!!!!  //nie mozna utworzyc macierzy odwroconej jesli wyznacznik jest rowny zero
	{
		if( !CreateMatrix( &pInv, nDim ) )
		{
			perror( "Nie udalo sie utworzyc macierzy" );
		}
		InverseMatrix( pInv, pMatrix, nDim, det );

#ifdef _DEBUGMAIN_
		printf( "\nMACIERZ ODWROCONA: \n" );
		PrintMatrix( pInv, nDim );
#endif

		double* pRes = NULL;//wektor rozwiazan

		if( !CreateVector( &pRes, nDim ) )
		{
			perror( "Nie udalo sie utworzyc wektora rozwiazan" );
		}

		LayoutEqu( pInv, pFreeWords, pRes, nDim );

#ifdef _DEBUGMAIN_	
		printf( "\nROZWIAZANIA: \n" );

		for( int i = 0; i < nDim; i++ )
		{
			printf( "x[%d] = %f\n", i, pRes[i] );
		}
#endif
		

	}
	else
	{
		printf( "Macierz odwrotna nie istnieje, poniewaz wyznacznik wynosi 0" );
	}



	DeleteMatrix( &pMatrix, nDim );
	DeleteMatrix( &pInv, nDim ); //!!!
	free( pFreeWords );
	
	fclose( fin );
	return 0;
}

void ReadData( FILE* fin, double** pMatrix, double * b, int nDim )// podaj za pomoca wskaznika na wskazniki- na kolumne i wiersze, dwa for
{
	double** pM = pMatrix;
	double* pF = b;
	
	for( int i = 0; i < nDim; i++, pM++, pF++ )
	{
		double* pColumn = *pM;	 //za kazdym zwiekszeniem pM ponownie trzeba to zadeklarowac - zmiana wiersza
		for( int j = 0; j < nDim; j++, pColumn++ )
		{
			fscanf( fin, "%lf", pColumn );

		}

		fscanf( fin, "%lf", pF );
	}

}
