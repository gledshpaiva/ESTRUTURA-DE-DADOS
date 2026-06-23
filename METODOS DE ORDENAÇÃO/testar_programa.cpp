#define TESTE_AUTOMATICO
#include "departamentos.cpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace std;

void exigir(bool condicao, const string& mensagem) {
    if (!condicao) {
        throw runtime_error(mensagem);
    }
}

void testarArquivoTexto() {
    vector<Departamento> departamentos = carregarDepartamentos("entrada_departamentos.txt");

    exigir(departamentos.size() == 10, "O arquivo TXT deveria carregar 10 departamentos.");
    exigir(departamentos[0].codigo == "001", "O primeiro codigo deveria ser 001.");
    exigir(departamentos[0].nome == "FINANCEIRO", "O primeiro departamento deveria ser FINANCEIRO.");
    exigir(departamentos[0].palavrasChave[4] == "LUCRO", "A quinta palavra de FINANCEIRO deveria ser LUCRO.");

    for (const Departamento& departamento : departamentos) {
        const Departamento* encontradoPorNome = buscarPorDepartamento(departamentos, departamento.nome);
        exigir(encontradoPorNome != nullptr, "Departamento existente nao encontrado: " + departamento.nome);
        exigir(encontradoPorNome->codigo == departamento.codigo, "Codigo incorreto para: " + departamento.nome);

        for (const string& palavra : departamento.palavrasChave) {
            const Departamento* encontradoPorPalavra = buscarPorPalavraChave(departamentos, palavra);
            exigir(encontradoPorPalavra != nullptr, "Palavra-chave existente nao encontrada: " + palavra);
            exigir(encontradoPorPalavra->nome == departamento.nome, "Departamento incorreto para palavra: " + palavra);
        }
    }

    exigir(buscarPorDepartamento(departamentos, "RH") == nullptr, "Departamento inexistente nao deveria ser encontrado.");
    exigir(buscarPorPalavraChave(departamentos, "NAOEXISTE") == nullptr, "Palavra inexistente nao deveria ser encontrada.");

    exigir(buscarPorDepartamento(departamentos, "financeiro") != nullptr, "Pesquisa deveria ignorar maiusculas/minusculas.");
    exigir(buscarPorPalavraChave(departamentos, "lucro") != nullptr, "Palavra-chave deveria ignorar maiusculas/minusculas.");
    exigir(buscarPorDepartamento(departamentos, "JURIDICO") != nullptr, "Pesquisa sem acento deveria encontrar JURIDICO.");
    exigir(buscarPorPalavraChave(departamentos, "SAIDA") != nullptr, "Palavra-chave sem acento deveria encontrar SAIDA.");
}

void testarArquivoWord() {
    vector<Departamento> departamentos = carregarDepartamentos("entrada_departamentos.docx");
    exigir(departamentos.size() == 10, "O arquivo DOCX deveria carregar 10 departamentos.");
    exigir(buscarPorDepartamento(departamentos, "FINANCEIRO") != nullptr, "DOCX deveria encontrar FINANCEIRO.");
    exigir(buscarPorPalavraChave(departamentos, "LUCRO") != nullptr, "DOCX deveria encontrar LUCRO.");
}

void testarArquivoInexistente() {
    bool lancouErro = false;

    try {
        carregarDepartamentos("arquivo_inexistente.txt");
    } catch (const exception&) {
        lancouErro = true;
    }

    exigir(lancouErro, "Arquivo inexistente deveria gerar erro controlado.");
}

int main() {
    try {
        testarArquivoTexto();
        testarArquivoWord();
        testarArquivoInexistente();
    } catch (const exception& erro) {
        cerr << "FALHOU: " << erro.what() << "\n";
        return EXIT_FAILURE;
    }

    cout << "Todos os testes automaticos em C++ passaram.\n";
    return EXIT_SUCCESS;
}
