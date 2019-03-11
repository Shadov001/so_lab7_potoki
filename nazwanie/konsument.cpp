
#include "includes.h"

int main(int argc, char *argv[])
 {


    if (argc != 2)
	{
        	cout << "[Konsument]: Blad, za malo argumentow " << endl;
        	exit(1);
    	}


    int numer = atoi(argv[1]);

    if (numer <= 0)
	{
        	cout << "[Konsument]: Numer musi byc dodatni" << endl;
        	exit(2);
    	}


    char nazwa_pliku[10];
    //sprintf(nazwa_pliku, "./konsumenci/wyjscie%d", numer);
    sprintf(nazwa_pliku, "./wyjscie%d", numer);


    FILE *wyjscie;
    if((wyjscie=fopen(nazwa_pliku, "w"))==NULL) {
        perror("[Konsument]: Nie moge otworzyc pliku wyjscie do zapisu! /konsument\n");
        exit(1);
    }
//   sleep(2);
    const char *potok = "/tmp/POTOK";

if (access(potok, F_OK) == -1)
{

	//while(access(potok,F_OK)==-1)
	//{
	//   if (access(potok, F_OK) == -1){
	std::cout << "[Konsument]: Brak dostepu do potoku! Nie ma?" << std::endl;
	exit(2);
    	//	cout<<"[Konsument]: Brak dospepu do potoku oczekiwanie"<<endl;
	//	sleep(5);
	//}
}

    int sprawdz_potok;
    //sprawdz_potok = open("POTOK", O_RDONLY | O_NONBLOCK);


    if((sprawdz_potok=open(potok, O_RDONLY | O_NONBLOCK)) == -1) {
        perror("[Konsument]: Otwarcie potoku do odczytu\n");
        exit(1);
    }



    //odczytanie z potoku i zapis do pliku wyjsciowego
        char bufor;
    int uchwyt_pot;

    while(1)
    {
        ssize_t retval=read(sprawdz_potok,&bufor,sizeof(bufor));


        if(retval<=0)
        {

            if(retval==-1 && errno != 11)
            {
                perror("[Konsument]: Read error");
                exit(6);
            }

		if(retval == 0) break;

        }
        else
        {
            printf("[Konsument]: Odczytane dane to: %c /konsument%d \n", bufor,atoi(argv[1]));
            fputc(bufor, wyjscie);
            fflush(wyjscie);


        }



    }

//zamykanie pliku wejsciowego
    if(fclose(wyjscie)==EOF)
    {
        printf("[Konsument]: Blad zamykania wyjscia /konsument\n");
        exit(-1);
    }

    close(sprawdz_potok);
    unlink(potok);
    return 0;



}
