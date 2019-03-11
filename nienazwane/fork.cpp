#include "includes.h"


int main(int argc, char *argv[]) {

    //system("./zmienne.sh");

	int limit = atoi(getenv("LIMIT"));
	int limit1 = atoi(getenv("LIMIT1"));

        cout<<limit<<" "<<limit1<<endl;

        limit = limit - limit1-3;

    if(argc!=3)
    {
        printf("[START]:Zla liczba argumentow (liczba_producentow, liczba_konsumentow)\n");
        exit(-1);
    }

    int liczba_p = atoi(argv[1]), liczba_k = atoi(argv[2]);
    if(!liczba_p)
    {
        printf("[START]: Argument1 musi byc liczba\n");
        exit(4);
    }

    if(liczba_p<=0)
    {
        printf("[START]: Argument1 musi byc liczba dodatnia\n");
        exit(4);
    }

    if(!liczba_k)
    {
        printf("[START]: Argument2 musi byc liczba\n");
        exit(4);
    }

    if(liczba_k<=0)
    {
        printf("[START]: Argument2 musi byc liczba dodatnia\n");
        exit(4);
    }


    if(liczba_p+liczba_k>limit)
    {
        printf("[START]: Przekroczono maksymalna liczbe procesow\n");
        exit(4);
    }

    //tworzenie procesow potomnych
    int i;
    int potok[2]; //deskryptory potoku
    int sprawdz_potok;
    char bufor[5];
    printf("[STERT]: Maksymalna liczba bitow w potoku %d( %d bajtow )\n",PIPE_BUF*8,PIPE_BUF);

    sprawdz_potok = pipe(potok);    //tworzenie potoku PIPE
    if(sprawdz_potok == -1)
    {
        printf("Blad tworzenia potoku: %i (%s)\n",errno,strerror(errno));
        exit(-2);
    }

//tworzenie procesow konsumenta
    for(i=0; i<liczba_k; i++)
    {
        sprintf(bufor, "%d", i+1);

        switch(fork())
        {
            case -1:
                printf("Blad tworzenia konsumenta\n");
                exit(-4);
            case 0:
                if(close(potok[1]) == -1)
                {
                    printf("Blad close (konsument): %i (%s)\n",errno,strerror(errno));
                    exit(-5);
                }

                if(dup2(potok[0], 0)< 0)                                {
                    printf("Blad dup2 konsument):: %i (%s)\n",errno,strerror(errno));
                    exit(-5);
                }

                if(execl("./konsument", "konsument",bufor, NULL)==-1)
                {
                    printf("Blad exec (konsument): %i (%s)\n",errno,strerror(errno));
                    exit(-4);
                }
        }
    }

//tworzenie procesow producentow
    for(i=0; i<liczba_p; i++)
    {
        sprintf(bufor, "%d", i+1);

        switch(fork())
        {
            case -1:
                printf("Blad tworzenia producenta: %i (%s)\n", errno,strerror(errno));
                exit(-4);
            case 0:
                if(dup2(potok[1], 1) < 0) //powielanie deskryptora
                {
                    printf("Blad dup2 (producent):: %i (%s)\n",errno,strerror(errno));
                    exit(-5);
                }
                if(execl("./producent", "producent",bufor,NULL)==-1)
                {
                    printf("Blad exec (producent): %i (%s)\n",errno,strerror(errno));
                    exit(-4);
                }
        }
    }

    close(potok[0]);
    close(potok[1]);

//oczekiwanie na zakonczenie procesow potomnych
    for(i=0;i<liczba_p+liczba_k;i++)
        wait(NULL);

    return 0;
}
