# LEIA-ME - Programa de Pesquisa de Departamentos

## Objetivo do programa

Este programa foi desenvolvido em C++ para ler um arquivo com dados de departamentos e permitir duas formas de pesquisa:

1. Pesquisar uma palavra-chave e descobrir qual departamento ela representa.
2. Pesquisar um departamento e exibir o codigo identificador e as cinco palavras-chave dele.

O arquivo de entrada possui uma linha para cada departamento. Cada linha deve seguir este formato:

```text
CODIGO DEPARTAMENTO PALAVRA1 PALAVRA2 PALAVRA3 PALAVRA4 PALAVRA5
```

Exemplo:

```text
001 FINANCEIRO INVESTIMENTO RENDA GASTOS DESPESAS LUCRO
002 CONTABILIDADE BALANÇO BALANCETE RAZÃO ENTRADA SAÍDA
```

No exemplo acima:

- `001` e o codigo identificador.
- `FINANCEIRO` e o nome do departamento.
- `INVESTIMENTO`, `RENDA`, `GASTOS`, `DESPESAS` e `LUCRO` sao as cinco palavras-chave.

## Arquivos do projeto

| Arquivo | Funcao |
|---|---|
| `departamentos.cpp` | Codigo-fonte principal do programa em C++. |
| `departamentos.exe` | Executavel gerado apos a compilacao. |
| `ARQUIVO PARA IMPORTAÇÃO NO PROGRAMA.docx` | Arquivo Word original com os dados dos departamentos. |
| `entrada_departamentos.docx` | Copia do arquivo Word com nome mais simples para testes. |
| `entrada_departamentos.txt` | Versao em texto dos mesmos dados, util para testes rapidos. |
| `Documento_Metodos_Ordenacao.docx` | Documento explicando Shell Sort, Heap Sort e Merge Sort. |
| `gerar_documento_ordenacao.py` | Script usado para gerar o documento dos metodos de ordenacao. |

## Como compilar

Para compilar usando o MinGW, abra o terminal na pasta do projeto e execute:

```powershell
$env:PATH = 'C:\MinGW\bin;' + $env:PATH
g++ departamentos.cpp -o departamentos.exe -std=c++11 -static-libgcc -static-libstdc++
```

O comando acima gera o arquivo:

```text
departamentos.exe
```

## Como executar

No terminal, execute:

```powershell
.\departamentos.exe
```

O programa vai pedir o caminho do arquivo de entrada:

```text
Informe o caminho do arquivo Word (.docx) ou texto (.txt):
```

Voce pode digitar, por exemplo:

```text
ARQUIVO PARA IMPORTAÇÃO NO PROGRAMA.docx
```

ou:

```text
entrada_departamentos.txt
```

Depois disso, o programa mostra o menu:

```text
===== MENU =====
1 - Pesquisar palavra-chave
2 - Pesquisar departamento
0 - Sair
Opcao:
```

## Como usar o menu

### Opcao 1 - Pesquisar palavra-chave

Se o usuario escolher a opcao `1`, o programa pede uma palavra-chave.

Exemplo:

```text
Digite a palavra-chave: LUCRO
```

Saida esperada:

```text
Palavra-chave encontrada.
Departamento: FINANCEIRO
```

Isso acontece porque a palavra `LUCRO` pertence ao departamento `FINANCEIRO`.

### Opcao 2 - Pesquisar departamento

Se o usuario escolher a opcao `2`, o programa pede o nome do departamento.

Exemplo:

```text
Digite o nome do departamento: FINANCEIRO
```

Saida esperada:

```text
Departamento encontrado.
Codigo identificador: 001
Palavras-chave: INVESTIMENTO, RENDA, GASTOS, DESPESAS, LUCRO
```

### Opcao 0 - Sair

Se o usuario escolher a opcao `0`, o programa encerra:

```text
Programa encerrado.
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

Essa estrutura guarda as tres partes principais de cada linha do arquivo:

- `codigo`: armazena o codigo identificador, por exemplo `001`.
- `nome`: armazena o nome do departamento, por exemplo `FINANCEIRO`.
- `palavrasChave`: armazena exatamente cinco palavras-chave.

Como a quantidade de palavras-chave e fixa em 5, foi usado:

```cpp
array<string, 5>
```

Todos os departamentos lidos do arquivo sao armazenados em um:

```cpp
vector<Departamento>
```

Ou seja, o programa usa um vetor dinamico para guardar varios departamentos.

## Funcionamento geral do codigo

O funcionamento do programa pode ser resumido nestas etapas:

1. O usuario informa o nome ou caminho do arquivo.
2. O programa verifica se o arquivo e `.docx` ou texto comum.
3. Se for `.docx`, o programa extrai o conteudo interno do Word.
4. Se for `.txt`, o programa le o arquivo diretamente.
5. Cada linha e separada em codigo, departamento e cinco palavras-chave.
6. Os dados sao guardados em uma `struct Departamento`.
7. O menu e exibido para o usuario.
8. O usuario pode pesquisar por palavra-chave ou por departamento.
9. O programa mostra o resultado encontrado ou uma mensagem informando que nao encontrou.

## Principais funcoes do programa

### `trim`

Remove espacos em branco do inicio e do fim de um texto.

Tambem remove o marcador BOM, que pode aparecer no inicio de alguns arquivos de texto.

Exemplo:

```cpp
string trim(const string& texto)
```

Essa funcao evita problemas quando o arquivo ou a entrada do usuario vem com espacos extras.

### `normalizarEspacos`

Transforma varios espacos, tabs ou quebras em apenas um espaco simples.

Exemplo:

```text
001   FINANCEIRO     LUCRO
```

vira:

```text
001 FINANCEIRO LUCRO
```

Isso facilita a separacao dos dados.

### `removerAcentosUTF8`

Remove acentos de letras em UTF-8.

Exemplo:

```text
VIGILÂNCIA
```

vira:

```text
VIGILANCIA
```

Essa funcao permite que o usuario pesquise com ou sem acento.

Por exemplo, se no arquivo estiver:

```text
JURÍDICO
```

o usuario pode digitar:

```text
JURIDICO
```

e o programa ainda encontra o departamento correto.

### `chavePesquisa`

Prepara textos para comparacao durante as pesquisas.

Ela faz duas coisas:

1. Remove acentos.
2. Converte letras para maiusculas.

Assim, as pesquisas ficam mais flexiveis.

Exemplo:

```text
saída
SAIDA
Saida
```

Todas essas formas passam a ser comparadas como:

```text
SAIDA
```

### `terminaCom`

Verifica se o arquivo termina com uma extensao especifica.

O programa usa essa funcao para descobrir se o arquivo informado e `.docx`.

Exemplo:

```cpp
terminaCom(caminhoArquivo, ".docx")
```

### `extrairDocx`

Essa funcao extrai o conteudo do arquivo Word.

Um arquivo `.docx` e, internamente, um arquivo compactado. Dentro dele existe um arquivo chamado:

```text
word/document.xml
```

Esse XML contem o texto do documento.

A funcao `extrairDocx` usa o PowerShell para descompactar o `.docx` em uma pasta temporaria.

### `extrairTextoXMLWord`

Depois que o `.docx` e extraido, essa funcao le o arquivo:

```text
word/document.xml
```

Ela procura as partes de texto do Word, que ficam dentro de tags como:

```xml
<w:t>texto</w:t>
```

Depois, ela monta novamente as linhas de texto para que o programa consiga ler os dados.

### `lerLinhasTexto`

Essa funcao retorna as linhas do arquivo de entrada.

Ela funciona de duas formas:

- Se o arquivo for `.docx`, ela extrai o Word e le o texto interno.
- Se o arquivo for `.txt`, ela le diretamente o conteudo do arquivo.

No final, ela devolve um vetor de linhas:

```cpp
vector<string>
```

### `carregarDepartamentos`

Essa e uma das funcoes mais importantes do programa.

Ela recebe as linhas do arquivo e transforma cada linha em uma `struct Departamento`.

Exemplo de linha:

```text
001 FINANCEIRO INVESTIMENTO RENDA GASTOS DESPESAS LUCRO
```

O programa separa essa linha assim:

```text
codigo: 001
nome: FINANCEIRO
palavras-chave: INVESTIMENTO, RENDA, GASTOS, DESPESAS, LUCRO
```

Depois guarda tudo dentro do vetor de departamentos.

### `pesquisarPalavraChave`

Essa funcao permite pesquisar uma palavra-chave.

Ela percorre todos os departamentos e verifica as cinco palavras-chave de cada um.

Se encontrar a palavra, mostra o departamento correspondente.

Exemplo:

```text
Entrada: LUCRO
Saida: Departamento: FINANCEIRO
```

Se nao encontrar, mostra:

```text
Palavra-chave nao encontrada.
```

### `pesquisarDepartamento`

Essa funcao permite pesquisar pelo nome do departamento.

Se encontrar o departamento, mostra:

- Codigo identificador.
- Cinco palavras-chave.

Exemplo:

```text
Entrada: FINANCEIRO
Saida:
Codigo identificador: 001
Palavras-chave: INVESTIMENTO, RENDA, GASTOS, DESPESAS, LUCRO
```

Se nao encontrar, mostra:

```text
Departamento nao encontrado.
```

### `main`

A funcao `main` controla o programa inteiro.

Ela faz:

1. Configura o console para UTF-8 no Windows.
2. Pede o arquivo ao usuario.
3. Carrega os departamentos.
4. Verifica se algum departamento foi carregado.
5. Mostra o menu.
6. Chama a funcao correta de acordo com a opcao escolhida.
7. Repete o menu ate o usuario escolher `0`.

## Tratamento de acentos

O programa foi ajustado para permitir pesquisas com ou sem acento.

Exemplos que funcionam:

```text
VIGILÂNCIA
VIGILANCIA
```

```text
JURÍDICO
JURIDICO
```

```text
SAÍDA
SAIDA
```

Isso melhora o uso do programa, porque o usuario nao precisa digitar exatamente igual ao arquivo.

## Testes realizados

Foram feitos testes com o arquivo Word original e com o arquivo de texto.

### Teste 1 - Carregar arquivo Word

Arquivo:

```text
ARQUIVO PARA IMPORTAÇÃO NO PROGRAMA.docx
```

Resultado:

```text
10 departamento(s) carregado(s) com sucesso.
```

### Teste 2 - Pesquisar palavra-chave existente

Entrada:

```text
LUCRO
```

Resultado:

```text
Departamento: FINANCEIRO
```

### Teste 3 - Pesquisar departamento existente

Entrada:

```text
FINANCEIRO
```

Resultado:

```text
Codigo identificador: 001
Palavras-chave: INVESTIMENTO, RENDA, GASTOS, DESPESAS, LUCRO
```

### Teste 4 - Pesquisar sem acento

Entrada:

```text
JURIDICO
```

Resultado:

```text
Codigo identificador: 007
Palavras-chave: LEIS, CONTRATO, JURISPRUDÊNCIA, AÇÃO, RECURSO
```

### Teste 5 - Pesquisar palavra inexistente

Entrada:

```text
NAOEXISTE
```

Resultado:

```text
Palavra-chave nao encontrada.
```

### Teste 6 - Pesquisar departamento inexistente

Entrada:

```text
RH
```

Resultado:

```text
Departamento nao encontrado.
```

## Observacoes importantes

- Cada departamento deve ter exatamente cinco palavras-chave.
- Nao devem existir palavras-chave repetidas.
- As palavras-chave nao devem ser compostas.
- O nome do departamento tambem deve estar em uma unica palavra, como `FINANCEIRO`, `CONTABILIDADE` ou `VIGILÂNCIA`.
- O programa aceita arquivos `.docx` e `.txt`.
- Para arquivos `.docx`, o programa usa PowerShell para extrair o conteudo interno do Word.

## Resumo final

O programa le os dados dos departamentos, guarda cada departamento em uma estrutura de dados e permite pesquisas simples pelo menu.

Ele resolve exatamente as duas consultas pedidas no trabalho:

1. Descobrir o departamento a partir de uma palavra-chave.
2. Descobrir o codigo e as palavras-chave a partir do nome do departamento.

