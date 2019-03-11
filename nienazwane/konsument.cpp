#include "includes.h"

int main(int argc, char *argv[]) {

    if (argc != 2)
	{
        	cout << "[Konsument]: Za malo argumentow  " << endl;
        	exit(1);
	}

    int numer = atoi(argv[1]);

    if (numer <= 0)
	{
        	cout << "[Konsument]: Numer musi byc dodatni" << endl;
       	 	exit(2);
    	}


    char nazwa_pliku[30];
    sprintf(nazwa_pliku, "./wyjscie%d", numer);

    FILE *wyjscie;
    if((wyjscie=fopen(nazwa_pliku, "w"))==NULL) {
        perror("[Konsument]: Nie moge otworzyc pliku wyjscie do zapisu\n");
        exit(1);
    }

	sleep(2);
    //odczytanie z potoku i zapis do pliku wyjsciowego
    char bufor;
    int uchwyt_pot;
    while((uchwyt_pot=read(0,&bufor,sizeof(bufor))) > 0)
    {
        printf("[Konsument]: Odczytane dane: %c  numer konsumenta: %d\n",bufor,numer);

        if(fputc(bufor, wyjscie) == EOF)
        {
            perror("[Konsument]: Blad zapisuje do pliku\n");
            exit(-10);
        }

        if (uchwyt_pot==-1)
        {
            perror("[Konsument]: Blad przy czytaniu.\n");
            exit(EXIT_FAILURE);
        }
    }

//zamykanie pliku wejsciowego
    if(fclose(wyjscie)==EOF)
    {
        printf("[Konsument]: Blad zamykania wyjscia\n");
        exit(-1);
    }

    return 0;





}
