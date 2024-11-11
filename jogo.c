#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

struct ranks
{
    int rank;
    char nome[10];
    struct ranks* next;
};
CONSOLE_SCREEN_BUFFER_INFO csbi;
int columns, rows;

//Geral:
int certeza(char *texto);
int DimencionarTela();

// Coisas do Menu:
int menu();
void construirmenu(int colunas, int linhas, int selected);
void printmenu(int colunas, char*palavra);

// Coisas do Jogo:
int jogo();
int administrarniveis(int dificuldade);
int construirjogo(int dificuldade, int matriz[dificuldade][dificuldade], int selecionado);
void perguntarnomejogo(char *palavra);

//Coisa dos ranks:
struct ranks* organizar(struct ranks*first, int rank, char * nome);
int rank();
void criarank(int posicao);




int main()
{
    DimencionarTela();
    system("cls");
    int tela = 0;
    while(1)
    {
        tela = menu();
        if (tela == 0)
        {
            return 0;
        }
        else if (tela == 1)
        {
            tela = jogo();
        }else if (tela == 2)
        {
            tela = rank();
        }
    }
}

int menu()
{
    int atualizar = 1;

    int posicao = 3;
    
    int n = 0;

    while (1)
    {
        if (atualizar == 1)
        {
            construirmenu(columns,rows ,posicao);
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();
            switch(n)
            {
                case 72:
                    if (posicao != 3)
                    {
                        posicao+=1;
                        atualizar = 1;
                    }
                break;

                case 80:
                    if (posicao != 1)
                    {
                        posicao-=1;
                        atualizar = 1;
                    }   
                break;

                case 13:
                    if (posicao == 1)
                    {
                        system("cls");
                        return 0;
                    }else if (posicao == 3)
                    {
                        system("cls");
                        return 1;
                    }else // posicao = 2
                    {
                        system("cls");
                        return 2;
                    }
                break;
            }
        }
    }
}
void construirmenu(int colunas, int linhas, int selected)
{
    system("cls");
    for (int i = 0; i < (linhas-8)/2; ++i)
    {
        printf("\n");
    }
    if (selected == 1)
    {
        printmenu(colunas, "MENU\n");
        printmenu(colunas, " ");
        printmenu(colunas, "Play\n");
        printmenu(colunas, "Rank\n");
        printmenu(colunas, "[Sair]\n");
    }
    else if (selected == 2)
    {
        printmenu(colunas, "MENU\n");
        printmenu(colunas, " ");
        printmenu(colunas, "Play\n");
        printmenu(colunas, "[Rank]\n");
        printmenu(colunas, "Sair\n");
    }
    else if (selected == 3)
    {
        printmenu(colunas, "MENU\n");
        printmenu(colunas, " ");
        printmenu(colunas, "[Play]\n");
        printmenu(colunas, "Rank\n");
        printmenu(colunas, "Sair\n");
    }

}

void printmenu(int colunas, char*palavra) // Funcao usada no rank para colocar a palavras no meio da tela
{
    int tamanho = strlen(palavra);
    printf("\n");
    for (int i = 0; i < (colunas - tamanho)/2; ++i)
    {
        printf(" ");
    }
    printf("%s",palavra);
    for (int i = 0; i < (colunas - tamanho)/2; ++i)
    {
        printf(" ");
    }
}

int jogo()
{
    int contador = 1;
    struct ranks* first = 0;
    int temp;
    char ntemp[10];
    char nome[10];
    perguntarnomejogo(nome);
    system("cls");
    int pontos = 0;
    int nivel = 0;
    while (administrarniveis(10+nivel))
    {
        nivel+=1;
        pontos += nivel*10;
    }
    FILE* rank = fopen("ranks.bin", "rb");
    while(fread(ntemp,sizeof(char),10,rank)!= 0)
    {
        fread(&temp,sizeof(int),1,rank);
        first = organizar(first, temp,ntemp);
        contador+=1;
    }
    first = organizar(first, pontos, nome);
    fclose(rank);
    rank = fopen("ranks.bin", "wb");
    for (int i = 0; i < contador; ++i)
    {
        fwrite(first->nome, sizeof(char),10,rank);
        fwrite(&first->rank, sizeof(int),1,rank);
        first = first->next;
    }
    fclose(rank);
}

int administrarniveis(int dificuldade)
{
    int n;
    int atualizar = 1;
    int selecionado = 0;
    int matriz[dificuldade][dificuldade];
    for (int i = 0; i < dificuldade; ++i)
    {
        for (int j = 0; j < dificuldade; ++j)
        {
            matriz[i][j] = rand() % 10;
        }
    }
    while (1)
    {

        if (atualizar == 1)
        {
            construirjogo(dificuldade, matriz, selecionado);
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();
            switch(n)
            {
                // <- 75,-> 77 ,^ 72, \/ 80
                case 77:
                    if (selecionado % dificuldade != dificuldade-1)
                    {
                        selecionado+=1;
                        atualizar = 1;
                    }
                break;

                case 75:
                    if (selecionado % dificuldade != 0)
                    {
                        selecionado-=1;
                        atualizar = 1;
                    }   
                break;

                case 72:
                    if (selecionado >= dificuldade)
                    {
                        selecionado-=dificuldade;
                        atualizar = 1;
                    }
                break;

                case 80:
                    if (selecionado +1 <= dificuldade*dificuldade - dificuldade)
                    {
                        selecionado+=dificuldade;
                        atualizar = 1;
                    }
                break;

                case 's':
                    return 0;
                break;
            }
        }
    }
    return 0;
}

int construirjogo(int dificuldade, int matriz[dificuldade][dificuldade], int selecionado)
{
    // i muda a linha j muda a coluna
    system("cls");
    int tmatrizcima[dificuldade];
    int tmatrizlado[dificuldade];
    int total = 0;
    for (int i = 0; i < dificuldade; ++i)
    {
        for (int j = 0; j < dificuldade; ++j)
        {
            total += matriz[i][j];
        }
        tmatrizlado[i] = total;
        total = 0;
    }
    for (int i = 0; i < dificuldade; ++i)
    {
        for (int j = 0; j < dificuldade; ++j)
        {
            total += matriz[j][i];
        }
        tmatrizcima[i] = total;
        total = 0;
    }
    for (int i = 0; i < (rows - dificuldade) /2 ; ++i)
    {
        printf("\n");
    }
    for (int i = 0; i < dificuldade + 2; ++i)
    {
        for (int j = 0; j < (columns - (dificuldade * 3)-9)/2; ++j)
        {
            printf(" ");
        }
        for (int j = 0; j < dificuldade + 2; ++j)
        {
            if((j == 0 || j == 1) && (i == 0 || i == 1))
            {
                printf("   ");
            }
            else if (j == 0)
            {
                if (tmatrizlado[i - 2] > 9)
                {
                    printf(" %d",tmatrizlado[i - 2]);
                }
                else
                {
                    printf(" %d ",tmatrizlado[i - 2]);
                }
            }
            else if(i == 0)
            {
                if (tmatrizcima[j - 2] > 9)
                {
                    printf(" %d",tmatrizcima[j - 2]);
                }
                else
                {
                    printf(" %d ",tmatrizcima[j - 2]);                    
                }
            }
            else if(i == 1)
            {   
                if (j== 1)
                {
                    printf(" --");
                }
                else
                {
                    printf("---");  
                }
            }
            else if (j == 1)
            {
                printf(" | ");
            }
            else
            {
                if (matriz[i - 2][j - 2] == 0)
                {
                    printf("   ");
                }
                else if ((i - 2)*dificuldade + j - 2 == selecionado)
                {
                    printf("[%d]", matriz[i - 2][j - 2]);  
                }
                else 
                {
                    printf(" %d ", matriz[i - 2][j - 2]);  
                }
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("\n");
    for (int j = 0; j < (columns - (dificuldade * 3)-9)/2; ++j)
    {
        printf(" ");
    }
    printf("Press Enter to erase / S to menu\n");
}
struct ranks* organizar(struct ranks*first, int rank, char * nome)
{
    struct ranks* temp;
    if (first == 0)
    {
        temp = malloc(sizeof(struct ranks));
        temp->rank = rank;
        strcpy(temp->nome, nome);
        temp->next = 0;
        return temp;
    }
    else
    {
        if (first->rank < rank)
        {
            temp = malloc(sizeof(struct ranks));
            temp->rank = rank;
            strcpy(temp->nome, nome);
            temp->next = first;
            return temp;
        }
        else
        {
            first->next = organizar(first->next, rank,nome);
            return first;
        }
    }
}
void perguntarnomejogo(char * palavra)
{
    char temp[100];
    char c = 0;
    int contador = 0;
    system("cls");
    printf("\n");
    printf("\n");
    while(1)
    {
        
        for (int i = 2; i < rows/4; ++i)
        {
            printf("\n");
        }
        printf("    Digite o seu nome : ");
        while((c = getchar()) != EOF && c != '\n' && contador != 100)
        {
            if (contador >= 10)
            {
                contador = 99;
                system("cls");
                printf("\n    Nome muito grande!");
                printf("\n    Insira novamente");
            }
            else
            {
                palavra[contador] = c;
            }
            contador +=1; 
        }
        if (c!= '\n')
        {
            while((c = getchar()) != EOF && c != '\n');
        }
        if (contador != 100)
        {
            palavra[contador] = '\0';
            return;
        }
        contador=0;
    }
    return;
}

int rank()
{
    int posicao = 1;
    int atualizar = 1;
    int n = 0;

    FILE* rank;

    while (1)
    {

        if (atualizar == 1)
        {
            criarank(posicao);
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();
            switch(n)
            {
                case 77:
                    if (posicao != 2)
                    {
                        posicao = 2; 
                        atualizar = 1;
                    }
                break;

                case 75:
                    if (posicao != 1)
                    {
                        posicao = 1; 
                        atualizar = 1;
                    }
                break;

                case 13:
                    if (posicao == 1)
                    {
                        return 0;
                    }
                    else
                    {
                        if (certeza(" Tem certeza que quer apagar os ranks?"))
                        {
                            rank = fopen("ranks.bin", "wb");
                            fwrite(NULL,1,1,rank);
                            atualizar = 1;
                            fclose(rank);
                        }
                        atualizar = 1;
                    }
                    
                break;
            }
        }
    }
}
void criarank(int posicao)
{
    int linha = 3;
    int pontos;
    char palavra[20];
    char numero [10];

    system("cls");
    FILE * rank = fopen("ranks.bin", "rb");
    if (rank == NULL)
    {
        printf("Arquivo de ranks nao encontrado\n");
    }
    printmenu(columns, "RANK\n");
    while(fread(palavra,sizeof(char),10,rank)!= 0)
    {
        linha+=1;
        fread(&pontos,sizeof(int),1,rank);
        strcat(palavra," ");
        sprintf(numero,"%d",pontos);
        strcat(palavra,numero);
        printmenu(columns, palavra);
    }
    while(linha < rows -1)
    {
        linha+=1;
        printf("\n");
    }
    if (posicao == 1)
    {
        printf("      [sair]  apagar");
    }
    else
    {
        printf("       sair  [apagar]");
    }
    fclose(rank);
}

int DimencionarTela()
{
    int tcolumns, trows;

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    // Obter o handle (manipulador) da janela do console
    HWND consoleWindow = GetConsoleWindow();
    
    // Remover o estilo de barra de rolagem horizontal e vertical
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style &= ~WS_VSCROLL;  // Remove a barra de rolagem vertical
    style &= ~WS_HSCROLL;  // Remove a barra de rolagem horizontal
    SetWindowLong(consoleWindow, GWL_STYLE, style);
    
    // Atualizar a janela para aplicar as mudanças
    SetWindowPos(consoleWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

    GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &csbi);
    tcolumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    trows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    if (columns != tcolumns || rows != trows)
    {
        columns = tcolumns;
        rows = trows;
        return 1;
    }    
    return 0;
}

int certeza(char *texto)
{
    int tamanho = strlen(texto);
    int posicao = 1;
    int atualizar = 1;
    int n = 0;

    while(1)
    {

        if (atualizar == 1)
        {
            system("cls");
            for (int i = 0; i < (rows-6)/2; ++i)
            {
                printf("\n");
            }
            for (int i = 0; i < (columns - 50)/2; ++i)
            {
                printf(" ");
            }
            printf("|");
            for (int i = 0; i < 50; ++i)
            {
                printf("-");
            }
            printf("|");
            printf("\n");
            for (int j = 0; j < 5; ++j)
            {
                for (int i = 0; i < (columns - 50)/2; ++i)
                {
                    printf(" ");
                }
                printf("|");

                if (j == 0)
                {
                    for (int k = 0; k < (50 - tamanho) / 2; ++k)
                    {
                        printf(" ");
                    }
                    printf("%s", texto);
                    for (int k = 0; k < (50 - tamanho) / 2; ++k)
                    {
                        printf(" ");
                    }
                }
                else if (j == 3)
                {   
                    if (posicao == 1)
                    {
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                        printf("[Nao]      Sim");
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                    }
                    else
                    {
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                        printf("Nao      [Sim]");
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 50; ++i)
                    {
                        printf(" ");
                    }
                }
                printf("|\n");
            } 
            for (int i = 0; i < (columns - 50)/2; ++i)
            {
                printf(" ");
            }
            printf("|");
            for (int i = 0; i < 50; ++i)
            {
                printf("-");
            }  
            printf("|");
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();
            switch(n)
            {
                case 77:
                    if (posicao != 2)
                    {
                        posicao = 2; 
                        atualizar = 1;
                    }
                break;

                case 75:
                    if (posicao != 1)
                    {
                        posicao = 1; 
                        atualizar = 1;
                    }
                break;

                case 13:
                    if (posicao == 2)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                    
                break;
            }
        }
    }
}