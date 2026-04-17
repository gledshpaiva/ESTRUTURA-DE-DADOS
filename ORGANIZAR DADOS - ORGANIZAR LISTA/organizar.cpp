#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream entrada("arquivo.txt");
    ofstream saida("arquivo_organizado.txt");

    char linha[100];
    char nomes[100][50];
    char datas[100][3][7];
    char nomeAux[50];
    char dataAux[7];
    int total = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int p = 0;
    int posNome = 0;
    int ano1 = 0;
    int ano2 = 0;
    int mes1 = 0;
    int mes2 = 0;
    int dia1 = 0;
    int dia2 = 0;
    bool trocar = false;

    if (!entrada)
    {
        cout << "Nao foi possivel abrir o arquivo.txt\n";
        return 1;
    }

    if (!saida)
    {
        cout << "Nao foi possivel criar o arquivo_organizado.txt\n";
        return 1;
    }

    for (total = 0; total < 100; total++)
    {
        if (!entrada.getline(linha, 100))
        {
            break;
        }

        posNome = 0;

        for (i = 0; linha[i] != '\0'; i++)
        {
            if (linha[i] >= '0' && linha[i] <= '9')
            {
                break;
            }

            nomes[total][posNome] = linha[i];
            posNome++;
        }

        nomes[total][posNome] = '\0';

        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 6; k++)
            {
                datas[total][j][k] = linha[i];
                i++;
            }

            datas[total][j][6] = '\0';
        }

        for (j = 0; j < 2; j++)
        {
            for (k = j + 1; k < 3; k++)
            {
                ano1 = (datas[total][j][4] - '0') * 10 + (datas[total][j][5] - '0');
                ano2 = (datas[total][k][4] - '0') * 10 + (datas[total][k][5] - '0');

                if (ano1 <= 26)
                {
                    ano1 = ano1 + 2000;
                }
                else
                {
                    ano1 = ano1 + 1900;
                }

                if (ano2 <= 26)
                {
                    ano2 = ano2 + 2000;
                }
                else
                {
                    ano2 = ano2 + 1900;
                }

                mes1 = (datas[total][j][2] - '0') * 10 + (datas[total][j][3] - '0');
                mes2 = (datas[total][k][2] - '0') * 10 + (datas[total][k][3] - '0');
                dia1 = (datas[total][j][0] - '0') * 10 + (datas[total][j][1] - '0');
                dia2 = (datas[total][k][0] - '0') * 10 + (datas[total][k][1] - '0');

                trocar = false;

                if (ano2 < ano1)
                {
                    trocar = true;
                }
                else
                {
                    if (ano2 == ano1)
                    {
                        if (mes2 < mes1)
                        {
                            trocar = true;
                        }
                        else
                        {
                            if (mes2 == mes1)
                            {
                                if (dia2 < dia1)
                                {
                                    trocar = true;
                                }
                            }
                        }
                    }
                }

                if (trocar)
                {
                    for (p = 0; p < 7; p++)
                    {
                        dataAux[p] = datas[total][j][p];
                        datas[total][j][p] = datas[total][k][p];
                        datas[total][k][p] = dataAux[p];
                    }
                }
            }
        }
    }

    entrada.close();

    for (i = 0; i < total - 1; i++)
    {
        for (j = i + 1; j < total; j++)
        {
            trocar = false;

            for (k = 0; nomes[i][k] != '\0' || nomes[j][k] != '\0'; k++)
            {
                if (nomes[j][k] < nomes[i][k])
                {
                    trocar = true;
                    break;
                }

                if (nomes[j][k] > nomes[i][k])
                {
                    break;
                }
            }

            if (trocar)
            {
                for (k = 0; k < 50; k++)
                {
                    nomeAux[k] = nomes[i][k];
                    nomes[i][k] = nomes[j][k];
                    nomes[j][k] = nomeAux[k];
                }

                for (k = 0; k < 3; k++)
                {
                    for (p = 0; p < 7; p++)
                    {
                        dataAux[p] = datas[i][k][p];
                        datas[i][k][p] = datas[j][k][p];
                        datas[j][k][p] = dataAux[p];
                    }
                }
            }
        }
    }

    for (i = 0; i < total; i++)
    {
        saida << nomes[i] << "," << datas[i][0] << "," << datas[i][1] << "," << datas[i][2];

        if (i < total - 1)
        {
            saida << "\n";
        }
    }

    saida.close();

    cout << "Arquivo organizado com sucesso.\n";

    return 0;
}
