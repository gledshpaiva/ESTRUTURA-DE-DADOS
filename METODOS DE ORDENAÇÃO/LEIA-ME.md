# LEIA-ME - Programa de Pesquisa de Departamentos

## Objetivo

Programa feito em C++ para ler um arquivo com dados de departamentos e permitir duas pesquisas:

1. Informar uma palavra-chave e descobrir o departamento correspondente.
2. Informar um departamento e exibir o codigo identificador e suas cinco palavras-chave.

Formato esperado em cada linha do arquivo:

```text
CODIGO DEPARTAMENTO PALAVRA1 PALAVRA2 PALAVRA3 PALAVRA4 PALAVRA5
```

Exemplo:

```text
001 FINANCEIRO INVESTIMENTO RENDA GASTOS DESPESAS LUCRO
002 CONTABILIDADE BALANCO BALANCETE RAZAO ENTRADA SAIDA
```

## Arquivos do projeto

| Arquivo | Funcao |
|---|---|
| `departamentos.cpp` | Codigo-fonte principal em C++. |
| `testar_programa.cpp` | Testes automaticos em C++. |
| `departamentos.exe` | Executavel do programa principal. |
| `testar_programa.exe` | Executavel dos testes automaticos, gerado na compilacao. |
| `ARQUIVO PARA IMPORTACAO NO PROGRAMA.docx` | Arquivo Word original com os dados dos departamentos. |
| `entrada_departamentos.docx` | Copia do arquivo Word com nome simples para teste. |
| `entrada_departamentos.txt` | Versao em texto dos mesmos dados. |

## Como compilar o programa

No PowerShell, dentro da pasta do projeto:

```powershell
g++ departamentos.cpp -o departamentos.exe -std=c++11 -static -static-libgcc -static-libstdc++
```

Se o terminal ja estava aberto antes da instalacao do WinLibs, feche e abra novamente para o comando `g++` entrar no PATH.

## Como executar

```powershell
.\departamentos.exe
```

O programa pede o caminho do arquivo:

```text
Informe o caminho do arquivo Word (.docx) ou texto (.txt):
```

Voce pode informar:

```text
entrada_departamentos.txt
```

ou:

```text
entrada_departamentos.docx
```

Depois o menu e exibido:

```text
===== MENU =====
1 - Pesquisar palavra-chave
2 - Pesquisar departamento
0 - Sair
Opcao:
```

## Como compilar os testes

Os testes tambem estao em C++:

```powershell
g++ testar_programa.cpp -o testar_programa.exe -std=c++11 -static -static-libgcc -static-libstdc++
```

## Como executar os testes

```powershell
.\testar_programa.exe
```

Saida esperada:

```text
Todos os testes automaticos em C++ passaram.
```

## Estrutura de dados usada

O programa usa uma `struct` chamada `Departamento`:

```cpp
struct Departamento {
    string codigo;
    string nome;
    array<string, 5> palavrasChave;
};
```

Os departamentos sao guardados em:

```cpp
vector<Departamento>
```

Cada departamento tem:

- `codigo`: codigo identificador.
- `nome`: nome do departamento.
- `palavrasChave`: cinco palavras-chave.

## Funcionamento geral

1. O usuario informa um arquivo `.docx` ou `.txt`.
2. O programa le as linhas do arquivo.
3. Cada linha e transformada em uma `struct Departamento`.
4. Os departamentos sao armazenados em um `vector`.
5. O menu permite pesquisar por palavra-chave ou por departamento.
6. O resultado encontrado e exibido na tela.

## Principais funcoes

| Funcao | O que faz |
|---|---|
| `trim` | Remove espacos extras do inicio e fim do texto. |
| `normalizarEspacos` | Troca varias separacoes por um espaco simples. |
| `removerAcentosUTF8` | Remove acentos para facilitar a pesquisa. |
| `chavePesquisa` | Prepara texto para comparacao, removendo acentos e usando maiusculas. |
| `lerLinhasTexto` | Le linhas de arquivo `.txt` ou extrai texto de `.docx`. |
| `carregarDepartamentos` | Converte as linhas em estruturas `Departamento`. |
| `buscarPorPalavraChave` | Retorna o departamento ligado a uma palavra-chave. |
| `buscarPorDepartamento` | Retorna o departamento pelo nome. |
| `pesquisarPalavraChave` | Interage com o usuario para pesquisar uma palavra-chave. |
| `pesquisarDepartamento` | Interage com o usuario para pesquisar um departamento. |

## Testes realizados

O arquivo `testar_programa.cpp` valida:

- carregamento do arquivo `.txt`;
- carregamento do arquivo `.docx`;
- pesquisa por departamento;
- pesquisa por palavra-chave;
- pesquisa com letras maiusculas/minusculas;
- pesquisa sem acento;
- tratamento de arquivo inexistente;
- retorno vazio para departamento ou palavra-chave inexistente.

## Observacoes

- O projeto esta todo em C++.
- Cada linha deve ter um codigo, um departamento e cinco palavras-chave.
- O nome do departamento deve estar em uma unica palavra.
- As palavras-chave tambem devem ser palavras simples.
- Para ler `.docx`, o programa C++ chama o PowerShell apenas para extrair internamente o arquivo Word, pois `.docx` e um arquivo compactado.
