#include "matrix.h"

//#define _DEBUG_		  //dokladne wyswietlenie etapow liczenia


void Complement( double** pTabOut, double** pTabIn, int nRow, int nCol, int nDim );	//	wejsciowy pyTabI o rozmiarze nDim, pTabO ma
//rozmiar nDim - 1, wycina z macierzy pierwsza kolumnê i daje to co zotalo, w petli w petli wykorzystac instrukcje continue
void ComplMatrix( double ** pTabD, double ** pTab, int nDim );  //liczy macierz dopelnien - tworzy pTabD 


int CreateMatrix( double*** pTab, int nSize )
{
	double** p = *pTab = (double**)malloc( sizeof( double* )*nSize ); 	 //alokacja pamieci na wiersze
																	   //**p oznacza numer wiersza- wskaznik umozliwiajacy przesuwanie sie po wierszach
	if( !(*pTab) )													   //*p oznacza numer kolumny- wskaznik umozliwiajacy przesuwanie sie po kolumnach
	{
		perror( "ERROR-BAD MEMORY ALLOCATION!!!" );
		return 0;
	}

	for( int i = 0; i < nSize; i++, p++ ) // przesuwanie wskaznika p i alokowanie pamieci w wierszu
	{
		*p = (double*)malloc( sizeof( double )*nSize );
//!!!!	
		if( !(*p) )													 
		{
			perror( "ERROR-BAD MEMORY ALLOCATION!!!" );
			return 0;
		}
		memset( *p, 0, sizeof( double )*nSize );
	}
	return 1;
}

//!!!
int CreateVector( double** pTab, int nSize )
{
	*pTab = (double*)malloc( sizeof( double )*nSize );
	if( !(*pTab) )
	{
		perror( "ERROR-BAD MEMORY ALLOCATION!!!" );
		return 0;
	}
	memset( *pTab, 0, sizeof( double )*nSize );
	return 1;

}


void DeleteMatrix( double*** pTab, int nSize )// *** poniewaz przekazujemy oryginal
{
	double ** pRow = *pTab;
	for( int i = 0; i < nSize; i++, pRow++ )
	{
		free( *pRow );//zerowanie kazdego wiersza
	}
	*pTab = NULL;// po wyzerowaniu wskaznik pTab ma wskazywac na null

}

void InverseMatrix( double ** pInv, double** pTab, int nSize, double det )
{
	//double** pRow = pTab;
	//double** pRow2 = pInv;// dwa wskazniki zeby moc sie poruszac po obu tablicach
// compl 
	//inv
	double** pMatrixD;//macierz dopelnien


	if( !CreateMatrix( &pMatrixD, nSize ) )
	{
		perror( "Nie udalo sie utworzyc macierzy dopelnien" );
	}

	ComplMatrix( pMatrixD, pTab, nSize );
	TransMatrix( pMatrixD, nSize );


	for( int i = 0; i < nSize; i++)
	{
		double* pColumn = *pMatrixD++;
		double* pColumn2 = *pInv++;
		for( int j = 0; j < nSize; j++ )
		{
			*pColumn2++ = *pColumn++ / det;  //przypisanie kazdemu wyrazowi pInv wartosci rownej 1/wyznacznik* odpowiadajacy wyraz z macierzy dopelnien
		}
		
		
	}

}

double Det( double ** pTab, int nSize )
{
	int nCol = 0;//kolumna, wzgledem ktorej liczymy det- zmienna wprowadzona aby uniknac MAGICNUMBER
	int nRow = 0;//wiersz, wzgledem ktorego liczymy det-  zmienna wprowadzona aby uniknac MAGICNUMBER
	if( nSize == 1 ) return **pTab;		   //macierz jednowymiarowa- jej wyznacznik jest rowny temu elementowi
	else if( nSize == 2 )
	{
#ifdef	_DEBUG_
		printf( "\n" );
		PrintMatrix( pTab, nSize );
#endif
		return **pTab * pTab[1][1] - pTab[0][1] * pTab[1][0]; //skorzystalem z indeksowania do obliczenia wyznacznika macierzy dwuwymiarowej
	}
	else
	{
		
		double det = 0;
		double* first = *pTab;//zaznaczam pierwszy wyraz
		
		int m = 1; 

		double** pTabO = NULL;	 //tablica pomocnicza w ktorej zapiszemy macierz po wykresleniu danego wiersza i kolumny

		if( !CreateMatrix( &pTabO, nSize - 1 ) )//nSize-1 poniewaz ma rozmiar o jeden mniejszy gdyz wykreslilismy wiersz i kolumne
		{
			perror( "Nie udalo sie utworzyc macierzy" );
		}

		for( int i = 0; i < nSize; i++ )
		{
			Complement( pTabO, pTab, nRow, i, nSize );//tworzenie macierzy po wykresleniu nRow i nCol
			det += *first++*Det( pTabO, nSize - 1 )*m;
			m = -m;
		}

#ifdef	_DEBUG_
		printf( "\n" );
		PrintMatrix( pTab, nSize );
#endif

		DeleteMatrix( &pTabO, (nSize-1) );
		return det;
	}
}

void LayoutEqu( double ** pInv, double* pV, double* pRes, int nSize )
{

	for( int i = 0; i < nSize; i++, pRes++)
	{
		double* pColumn = *pInv++;//wskaznie pierwszego elementu danego wiersza
		double* pFreeWords = pV;  //musimy cofnac sie na poczatek pV
		for( int j = 0; j < nSize; j++ )
		{
			*pRes += (*pColumn)*(*pFreeWords); //mnozymy przez wyrazy wolne i element w danej kolumnie
			pColumn++;
			pFreeWords++;
		}	
			
	}

}

void PrintMatrix( double** pTab, int nSize )
{
	//double** pRow = pTab; // sluzy do zmiany wiersza, zmienne wprowadzone aby nie modyfikowac pTab
	for( int i = 0; i < nSize; i++ )
	{
		double* pColumn = *pTab++;//sluzy do przesuwania sie po kolumnach w danych wierszu

		for( int j = 0; j < nSize; j++, pColumn++ )
		{
			printf( "%lf\t", *pColumn );
		}

		printf( "\n" );
	}
}

void Complement( double** pTabOut, double** pTabIn, int nRow, int nCol, int nDim )
{

	for( int i = 0; i < nDim; i++,pTabIn++  )
	{
		if( i == nRow ) 	 //nRow jest to wiersz do usuniecia, jesli napotka wiersz to przechodzi jedna iteracje
		{
			continue;
		}
		double* pColumn = *pTabIn;
		double* pColumn2 = *pTabOut++;

		for( int j = 0; j < nDim; j++,pColumn++ )
		{
			if( j == nCol )		//nCol jest to kolumna do usuniecia, jesli napotka wiersz to przechodzi jedna iteracj
			{
				continue;
			}
			*pColumn2++ = *pColumn; //przypisanie danemu elementowi tablicy pTabOut elementu tablicy pTabIn
			
		}
	
	}
}

void ComplMatrix( double ** pTabD, double ** pTab, int nDim )
{
	
	double** pTempTab = NULL;//tablica pomocnicza, sa w niej zapisane macierze po wycieciu kolumny
	int m = 1;

	if( !CreateMatrix( &pTempTab, nDim - 1 ) )	 //moze byc zadeklarowana rozmiarem o jeden mniejszym- bo i tak wpisujemy w nia macierz
	{										// z wycietym wierszem i kolumna
		perror( "Nie udalo sie utworzyc macierzy" );
	}


	for( int i = 0; i < nDim; i++,pTabD++ )
	{

		double* pColumn = *pTabD;
		double m = ( i % 2 ) ? -1 : 1;
		for( int j = 0; j < nDim; j++, pColumn++ )
		{
			Complement( pTempTab, pTab, i, j, nDim );

#ifdef _DEBUG_
			printf( "\nMacierz: \n" );
			PrintMatrix( pTempTab, nDim - 1 );
			printf( "\n" );
			printf( "\nWyznacznik : %f", Det( pTempTab, nDim - 1 ) );
#endif
			*pColumn = Det( pTempTab, nDim - 1 )*m;
			m = -m;

		}
		
	}


}

void TransMatrix( double ** pTab, int nDim )
{
	double** pRow = pTab;

	for( int i = 0; i < nDim - 1; i++ )
	{
		double* pColumn = *pRow++ + i+1;
		for( int j = i + 1; j < nDim; j++ )
		{
			double  temp=*pColumn; //pTab[i][j]
			*pColumn++ = pTab[j][i];
			pTab[j][i] = temp;
		}
	}

	
}










/*	   Stara wersja iczenia wyznacznika- jakby co
double Det( double ** pTab, int nSize )
{
double** pTemp = NULL;//tablica pomocnicza, sa w niej zapisane macierze po wycieciu kolumny


  if( !CreateMatrix( &pTemp, nSize ) )
  {
	  perror( "Nie udalo sie utworzyc macierzy" );
  }

  pTemp = pTab;


  int nCol = 0;//kolumna wzgledem ktorej liczymy det
  if( nSize == 1 ) return **pTab;
  else if( nSize == 2 )
  {
#ifdef	_DEBUG_
		printf( "\n" );
		PrintMatrix( pTab, nSize );
#endif
		return pTab[0][0] * pTab[1][1] - pTab[0][1] * pTab[1][0];
	}
	else
	{
		double det = **pTab;
		double temp;




		for( int i = 1; i < nSize; i++ )
		{
			double** pRow = pTab;
			double first = **pRow;
			double* pColumn = *pRow;


			pColumn = *pRow;
			pColumn += i;
			if( first != 0 )temp = *pColumn / first;
			else
				temp = -1;

			for( int j = 0; j < nSize; j++ )
			{
				pColumn = *pRow + i;
				*pColumn -= first*temp;
				if( j != nSize - 1 )
				{
					pRow++;
					first = **pRow;
				}

			}
		}

		double** pTabO = NULL; if( !CreateMatrix( &pTabO, nSize - 1 ) )
		{
			perror( "Nie udalo sie utworzyc macierzy" );
		}

		Complement( pTabO, pTab, 0, 0, nSize );

#ifdef	_DEBUG_
		printf( "\n" );
		PrintMatrix( pTab, nSize );
#endif

		det = det*Det( pTabO, nSize - 1 );
		return det;
	}
}
  */