#include "includes.h"




int main(int argc, char *argv[]) {

    if (argc != 2)
	{
        	cout << "[Producent]: Za malo argumentow" << endl;
        	exit(1);
    	}

    int numer = atoi(argv[1]);

    if (numer <= 0)
	{
        	cout << "[Producent]: Numer musi byc dodatni" << endl;
        	exit(2);
    	}


    char nazwa_pliku[30];
    sprintf(nazwa_pliku, "./wejscie%d", numer);

//kojarzenie zmiennej zplikiem "wejscie"
    FILE *wejscie;

	if((wejscie=fopen(nazwa_pliku, "r"))==NULL) 
	{
        	if((wejscie=fopen(nazwa_pliku, "w"))) 
			{
            			char literka = 0;

            			for(int b=0;b<10;b++)
				{
                			literka = static_cast<char>('A' + (rand() % 26));
                			fputc(literka, wejscie);
				}

            			fclose(wejscie);
        		}
	}


    if((wejscie=fopen(nazwa_pliku, "r"))==NULL)
	{
        	perror(nazwa_pliku);
        	exit(1);
    	}

    if(sizeof(nazwa_pliku)>(PIPE_BUF*8))
    	{
        	perror("[Producent]: Plik jest za duzy");
        	exit(-2);
    	}

//zapis zawartosci pliku wejsciowego do potoki znak po znaku
    char znak;

    while(znak != EOF)
    {
        znak = fgetc(wejscie);
        if(znak != EOF)
        {
            //zapis bufora do potoku
            int uchwyt_mac;
            uchwyt_mac=write(1 ,&znak,sizeof(char));
            if (uchwyt_mac==-1)
            {
                perror("[Producent]: Blad przy zapisie");
                exit(EXIT_FAILURE);
            }
        }
    }

//zamykanie pliku wejsciowego
    if(fclose(wejscie)==EOF)
    {
        perror("[Producent]: Blad zamykania wejscia");
        exit(-1);
    }

    return 0;


}
