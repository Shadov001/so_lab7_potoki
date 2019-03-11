#include "includes.h"


int main(int argc, char *argv[]) {

    if (argc != 2)
	{
        	cout << "[Producent]: Za malo argumentow " << endl;
        	exit(1);
    	}


    int numer = atoi(argv[1]);

    if (numer <= 0)
	{
        	cout << "[Producent]: Argument musi byc dodatni " << endl;
        	exit(2);
    	}


    char nazwa_pliku[30];
    sprintf(nazwa_pliku, "./wejscie%d", numer);

//kojarzenie zmiennej zplikiem "wejscie"
    FILE *wejscie;
    if((wejscie=fopen(nazwa_pliku, "r"))==NULL) {

        cout<<"[Producent]: Nie znaleziono pliku, tworze plik"<<endl;

        if((wejscie=fopen(nazwa_pliku, "w"))) {
            char literka = 0;

            for(int b=0;b<10;b++){
                literka = static_cast<char>('A' + (rand() % 26));
                fputc(literka, wejscie);

            }
            fclose(wejscie);

        }

        if((wejscie=fopen(nazwa_pliku, "r"))==NULL) {

            perror(nazwa_pliku);
            exit(1);
        }
    }


    const char *potok = "/tmp/POTOK";

    if (access(potok, F_OK) == -1) {
        if (mkfifo(potok, 0660) == -1) //(umask) mkfifo(potok, 0600)
        {
            if (errno != 17) {
                //cout << "tworzenie: " << errno << std::strerror(errno) << endl;
                perror("Mkfifo error");
                exit(3);
            }
        }
    }


    int sprawdz_potok;
    sprawdz_potok = open(potok, O_WRONLY);
    if(sprawdz_potok == -1)
    {
        cout<<"Otwarcie potoku do zapisu: "<< errno<< std::strerror(errno)<<endl;
        exit(1);
    }


    char znak;


    while(znak != EOF)
    {
        //sleep(1);
        znak = fgetc(wejscie);
        if(znak != EOF)
        {
            int uchwyt_mac;
            uchwyt_mac=write(sprawdz_potok ,&znak,sizeof(char));
            if (uchwyt_mac==-1)
            {
                perror("Blad przy zapisie /producent");
                exit(EXIT_FAILURE);
            }
        }
    }



    if(fclose(wejscie)==EOF)
    {
        perror("Blad zamykania wejscia /producent");
        exit(-1);
    }

    close(sprawdz_potok);


}
