void quadrado(int ci, int li, int cf, int lf, int cor)
{
    textcolor(cor);
    gotoxy(ci, li);
    printf("%c", 201);
    gotoxy(ci, lf);
    printf("%c", 200);
    gotoxy(cf, li);
    printf("%c", 187);
    gotoxy(cf, lf);
    printf("%c", 188);

    for (int i = ci + 1; i < cf; i++)
    {
        gotoxy(i, li);
        printf("%c", 205);
        gotoxy(i, lf);
        printf("%c", 205);
    }
    for (int i = li + 1; i < lf; i++)
    {
        gotoxy(ci, i);
        printf("%c", 186);
        gotoxy(cf, i);
        printf("%c", 186);
    }
    textcolor(15);
}

void inicioTOP()
{
    gotoxy(2, 2);
    printf("Sistemas Operacionais");
    textcolor(0);
    textbackground(15);
    quadrado(1, 1, 80, 25, 4);
    textcolor(15);
    textbackground(0);
    int i, j, k;
    char frase[23];
    strcpy(frase, "CICLO DE VIDA PROCESSOS\0");
    for (i = 0; i < strlen(frase); i++)
    {
        for (j = strlen(frase); j > i; j--)
        {
            gotoxy(j + 27 + 1, 12);
            printf(" ");
            gotoxy(j + 27, 12);
            for (k = 0; k < 8; k++)
            {
                printf("%c", rand() % 26 + 64);
                gotoxy(j + 27, 12);
            }
            gotoxy(j + 27 + 1, 12);
            printf(" ");
            Sleep(5);
        }
        gotoxy(i + 27 + 1, 12);
        printf(" ");
        gotoxy(i + 27, 12);
        printf("%c", frase[i]);
    }
    while (!kbhit())
    {
        Sleep(500);
        gotoxy(26, 15);
        printf("PRESS ANY KEY TO CONTINUE");
        Sleep(500);
        gotoxy(26, 15);
        printf("                          ");
    }
}