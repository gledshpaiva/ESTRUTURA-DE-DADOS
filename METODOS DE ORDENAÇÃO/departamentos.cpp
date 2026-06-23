#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

struct Departamento {
    string codigo;
    string nome;
    array<string, 5> palavrasChave;
};

string trim(const string& texto) {
    size_t inicio = texto.find_first_not_of(" \t\r\n");
    if (inicio == string::npos) return "";
    size_t fim = texto.find_last_not_of(" \t\r\n");
    string resultado = texto.substr(inicio, fim - inicio + 1);
    if (resultado.size() >= 3 &&
        static_cast<unsigned char>(resultado[0]) == 0xEF &&
        static_cast<unsigned char>(resultado[1]) == 0xBB &&
        static_cast<unsigned char>(resultado[2]) == 0xBF) {
        resultado.erase(0, 3);
    }
    return resultado;
}

string normalizarEspacos(const string& texto) {
    string resultado;
    bool emEspaco = false;

    for (unsigned char c : texto) {
        if (isspace(c)) {
            if (!emEspaco) resultado += ' ';
            emEspaco = true;
        } else {
            resultado += static_cast<char>(c);
            emEspaco = false;
        }
    }

    return trim(resultado);
}

string maiusculasASCII(string texto) {
    for (char& c : texto) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return texto;
}

string removerAcentosUTF8(const string& texto) {
    string resultado;

    for (size_t i = 0; i < texto.size(); ++i) {
        unsigned char atual = static_cast<unsigned char>(texto[i]);

        if (atual == 0xC3 && i + 1 < texto.size()) {
            unsigned char proximo = static_cast<unsigned char>(texto[i + 1]);
            switch (proximo) {
                case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85:
                case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5:
                    resultado += 'A';
                    ++i;
                    continue;
                case 0x87: case 0xA7:
                    resultado += 'C';
                    ++i;
                    continue;
                case 0x88: case 0x89: case 0x8A: case 0x8B:
                case 0xA8: case 0xA9: case 0xAA: case 0xAB:
                    resultado += 'E';
                    ++i;
                    continue;
                case 0x8C: case 0x8D: case 0x8E: case 0x8F:
                case 0xAC: case 0xAD: case 0xAE: case 0xAF:
                    resultado += 'I';
                    ++i;
                    continue;
                case 0x92: case 0x93: case 0x94: case 0x95: case 0x96:
                case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6:
                    resultado += 'O';
                    ++i;
                    continue;
                case 0x99: case 0x9A: case 0x9B: case 0x9C:
                case 0xB9: case 0xBA: case 0xBB: case 0xBC:
                    resultado += 'U';
                    ++i;
                    continue;
            }
        }

        resultado += static_cast<char>(atual);
    }

    return resultado;
}

string chavePesquisa(const string& texto) {
    return maiusculasASCII(removerAcentosUTF8(texto));
}

bool terminaCom(const string& texto, const string& sufixo) {
    if (sufixo.size() > texto.size()) return false;
    return equal(sufixo.rbegin(), sufixo.rend(), texto.rbegin(),
                 [](char a, char b) {
                     return tolower(static_cast<unsigned char>(a)) ==
                            tolower(static_cast<unsigned char>(b));
                 });
}

string escaparPowerShell(const string& texto) {
    string saida;
    for (char c : texto) {
        saida += c;
        if (c == '\'') saida += '\'';
    }
    return saida;
}

string base64Encode(const vector<unsigned char>& dados) {
    static const char tabela[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string saida;

    for (size_t i = 0; i < dados.size(); i += 3) {
        int b0 = dados[i];
        int b1 = (i + 1 < dados.size()) ? dados[i + 1] : 0;
        int b2 = (i + 2 < dados.size()) ? dados[i + 2] : 0;

        saida += tabela[(b0 >> 2) & 0x3F];
        saida += tabela[((b0 & 0x03) << 4) | ((b1 >> 4) & 0x0F)];
        saida += (i + 1 < dados.size()) ? tabela[((b1 & 0x0F) << 2) | ((b2 >> 6) & 0x03)] : '=';
        saida += (i + 2 < dados.size()) ? tabela[b2 & 0x3F] : '=';
    }

    return saida;
}

string comandoPowerShellUnicode(const string& script) {
#ifdef _WIN32
    int tamanho = MultiByteToWideChar(CP_UTF8, 0, script.c_str(), -1, nullptr, 0);
    wstring scriptWide;

    if (tamanho > 0) {
        scriptWide.resize(tamanho - 1);
        MultiByteToWideChar(CP_UTF8, 0, script.c_str(), -1, &scriptWide[0], tamanho);
    } else {
        for (unsigned char c : script) scriptWide += static_cast<wchar_t>(c);
    }

    vector<unsigned char> bytes;
    bytes.reserve(scriptWide.size() * 2);
    for (wchar_t c : scriptWide) {
        bytes.push_back(static_cast<unsigned char>(c & 0xFF));
        bytes.push_back(static_cast<unsigned char>((c >> 8) & 0xFF));
    }

    return "powershell -NoProfile -ExecutionPolicy Bypass -EncodedCommand " + base64Encode(bytes);
#else
    return "powershell -NoProfile -ExecutionPolicy Bypass -Command \"" + script + "\"";
#endif
}

string decodificarEntidadesXML(const string& texto) {
    string saida = texto;
    const array<pair<string, string>, 5> entidades = {{
        {"&amp;", "&"},
        {"&lt;", "<"},
        {"&gt;", ">"},
        {"&quot;", "\""},
        {"&apos;", "'"}
    }};

    for (const auto& entidade : entidades) {
        size_t pos = 0;
        while ((pos = saida.find(entidade.first, pos)) != string::npos) {
            saida.replace(pos, entidade.first.size(), entidade.second);
            pos += entidade.second.size();
        }
    }

    return saida;
}

string extrairTextoXMLWord(const string& xml) {
    string texto;
    size_t pos = 0;

    while (pos < xml.size()) {
        if (xml.compare(pos, 4, "<w:t") == 0) {
            size_t inicio = xml.find('>', pos);
            if (inicio == string::npos) break;
            size_t fim = xml.find("</w:t>", inicio);
            if (fim == string::npos) break;
            texto += decodificarEntidadesXML(xml.substr(inicio + 1, fim - inicio - 1));
            pos = fim + 6;
        } else if (xml.compare(pos, 6, "<w:tab") == 0 || xml.compare(pos, 5, "<w:br") == 0) {
            texto += ' ';
            pos = xml.find('>', pos);
            if (pos == string::npos) break;
            ++pos;
        } else if (xml.compare(pos, 6, "</w:p>") == 0) {
            texto += '\n';
            pos += 6;
        } else {
            ++pos;
        }
    }

    return texto;
}

bool extrairDocx(const string& caminhoDocx, const string& destino) {
    string origem = escaparPowerShell(caminhoDocx);
    string pasta = escaparPowerShell(destino);
    string script =
        "$ProgressPreference = 'SilentlyContinue'; "
        "Add-Type -AssemblyName System.IO.Compression.FileSystem; "
        "if (Test-Path -LiteralPath '" + pasta + "') { Remove-Item -LiteralPath '" + pasta + "' -Recurse -Force }; "
        "[System.IO.Compression.ZipFile]::ExtractToDirectory('" + origem + "', '" + pasta + "')";

    return system(comandoPowerShellUnicode(script).c_str()) == 0;
}

void removerPastaTemporaria(const string& pastaTemporaria) {
    string pasta = escaparPowerShell(pastaTemporaria);
    string script = "$ProgressPreference = 'SilentlyContinue'; if (Test-Path -LiteralPath '" + pasta + "') { Remove-Item -LiteralPath '" + pasta + "' -Recurse -Force }";
    system(comandoPowerShellUnicode(script).c_str());
}

string pastaTemporariaSistema() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD tamanho = GetTempPathA(MAX_PATH, buffer);
    if (tamanho > 0 && tamanho < MAX_PATH) {
        string caminho(buffer);
        if (!caminho.empty() && caminho.back() == '\\') caminho.pop_back();
        return caminho;
    }
#else
    const char* tmp = getenv("TMPDIR");
    if (tmp != nullptr && tmp[0] != '\0') return string(tmp);
#endif
    return ".";
}

string identificadorProcesso() {
#ifdef _WIN32
    return to_string(GetCurrentProcessId());
#else
    return "0";
#endif
}

vector<string> lerLinhasTexto(const string& caminhoArquivo) {
    string conteudo;

    if (terminaCom(caminhoArquivo, ".docx")) {
        auto agora = chrono::system_clock::now().time_since_epoch().count();
        string pastaTemporaria = pastaTemporariaSistema() + "\\docx_extraido_" + identificadorProcesso() + "_" + to_string(agora);

        if (!extrairDocx(caminhoArquivo, pastaTemporaria)) {
            throw runtime_error("Nao foi possivel extrair o arquivo DOCX.");
        }

        string documentXml = pastaTemporaria + "\\word\\document.xml";
        ifstream entrada(documentXml, ios::binary);
        if (!entrada) {
            removerPastaTemporaria(pastaTemporaria);
            throw runtime_error("O arquivo word/document.xml nao foi encontrado no DOCX.");
        }

        conteudo.assign((istreambuf_iterator<char>(entrada)), istreambuf_iterator<char>());
        conteudo = extrairTextoXMLWord(conteudo);
        entrada.close();
        removerPastaTemporaria(pastaTemporaria);
    } else {
        ifstream entrada(caminhoArquivo);
        if (!entrada) {
            throw runtime_error("Nao foi possivel abrir o arquivo informado.");
        }
        conteudo.assign((istreambuf_iterator<char>(entrada)), istreambuf_iterator<char>());
    }

    vector<string> linhas;
    string linha;
    stringstream ss(conteudo);
    while (getline(ss, linha)) {
        linha = normalizarEspacos(linha);
        if (!linha.empty()) linhas.push_back(linha);
    }

    return linhas;
}

vector<Departamento> carregarDepartamentos(const string& caminhoArquivo) {
    vector<Departamento> departamentos;
    vector<string> linhas = lerLinhasTexto(caminhoArquivo);

    for (const string& linha : linhas) {
        stringstream ss(linha);
        Departamento departamento;

        ss >> departamento.codigo >> departamento.nome;
        for (string& palavra : departamento.palavrasChave) {
            ss >> palavra;
        }

        if (!departamento.codigo.empty() && !departamento.nome.empty() && !departamento.palavrasChave[4].empty()) {
            departamentos.push_back(departamento);
        }
    }

    return departamentos;
}

const Departamento* buscarPorPalavraChave(const vector<Departamento>& departamentos, const string& palavra) {
    string busca = chavePesquisa(palavra);

    for (const Departamento& departamento : departamentos) {
        for (const string& palavraChave : departamento.palavrasChave) {
            if (chavePesquisa(palavraChave) == busca) {
                return &departamento;
            }
        }
    }

    return nullptr;
}

const Departamento* buscarPorDepartamento(const vector<Departamento>& departamentos, const string& nome) {
    string busca = chavePesquisa(nome);

    for (const Departamento& departamento : departamentos) {
        if (chavePesquisa(departamento.nome) == busca) {
            return &departamento;
        }
    }

    return nullptr;
}

void pesquisarPalavraChave(const vector<Departamento>& departamentos) {
    string palavra;
    cout << "Digite a palavra-chave: ";
    cin >> palavra;

    const Departamento* departamento = buscarPorPalavraChave(departamentos, palavra);
    if (departamento != nullptr) {
        cout << "\nPalavra-chave encontrada.\n";
        cout << "Departamento: " << departamento->nome << "\n";
        return;
    }

    cout << "\nPalavra-chave nao encontrada.\n";
}

void pesquisarDepartamento(const vector<Departamento>& departamentos) {
    string nome;
    cout << "Digite o nome do departamento: ";
    cin >> nome;

    const Departamento* departamento = buscarPorDepartamento(departamentos, nome);
    if (departamento != nullptr) {
        cout << "\nDepartamento encontrado.\n";
        cout << "Codigo identificador: " << departamento->codigo << "\n";
        cout << "Palavras-chave: ";
        for (size_t i = 0; i < departamento->palavrasChave.size(); ++i) {
            cout << departamento->palavrasChave[i];
            if (i + 1 < departamento->palavrasChave.size()) cout << ", ";
        }
        cout << "\n";
        return;
    }

    cout << "\nDepartamento nao encontrado.\n";
}

#ifndef TESTE_AUTOMATICO
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    string caminhoArquivo;
    vector<Departamento> departamentos;

    cout << "Informe o caminho do arquivo Word (.docx) ou texto (.txt): ";
    getline(cin, caminhoArquivo);
    caminhoArquivo = trim(caminhoArquivo);

    try {
        departamentos = carregarDepartamentos(caminhoArquivo);
    } catch (const exception& erro) {
        cout << "Erro: " << erro.what() << "\n";
        return 1;
    }

    if (departamentos.empty()) {
        cout << "Nenhum departamento foi carregado.\n";
        return 1;
    }

    cout << "\n" << departamentos.size() << " departamento(s) carregado(s) com sucesso.\n";

    int opcao = -1;
    while (opcao != 0) {
        cout << "\n===== MENU =====\n";
        cout << "1 - Pesquisar palavra-chave\n";
        cout << "2 - Pesquisar departamento\n";
        cout << "0 - Sair\n";
        cout << "Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                pesquisarPalavraChave(departamentos);
                break;
            case 2:
                pesquisarDepartamento(departamentos);
                break;
            case 0:
                cout << "Programa encerrado.\n";
                break;
            default:
                cout << "Opcao invalida.\n";
                break;
        }
    }

    return 0;
}
#endif
