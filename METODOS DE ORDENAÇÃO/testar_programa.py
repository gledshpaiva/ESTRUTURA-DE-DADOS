import subprocess
from pathlib import Path


BASE = Path(__file__).resolve().parent
EXE = BASE / "departamentos.exe"
ENTRADA = "entrada_departamentos.txt"
DADOS = BASE / ENTRADA


def sem_acentos(texto):
    trocas = str.maketrans(
        "ÁÀÂÃÄÅÉÈÊËÍÌÎÏÓÒÔÕÖÚÙÛÜÇáàâãäåéèêëíìîïóòôõöúùûüç",
        "AAAAAAEEEEIIIIOOOOOUUUUCaaaaaaeeeeiiiiooooouuuuc",
    )
    return texto.translate(trocas)


def executar(entrada):
    processo = subprocess.run(
        [str(EXE)],
        input=entrada.encode("utf-8"),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        cwd=BASE,
        timeout=20,
    )
    return processo.returncode, processo.stdout.decode("utf-8", errors="replace"), processo.stderr.decode("utf-8", errors="replace")


def falhar(mensagem, saida="", erro=""):
    print(f"FALHOU: {mensagem}")
    if saida:
        print("--- STDOUT ---")
        print(saida)
    if erro:
        print("--- STDERR ---")
        print(erro)
    raise SystemExit(1)


linhas = [linha.strip().split() for linha in DADOS.read_text(encoding="utf-8").splitlines() if linha.strip()]

total = 0

for partes in linhas:
    codigo, departamento, *palavras = partes

    entrada = f"{ENTRADA}\n2\n{departamento}\n0\n"
    retorno, saida, erro = executar(entrada)
    total += 1
    if retorno != 0 or f"Codigo identificador: {codigo}" not in saida:
        falhar(f"departamento {departamento} deveria retornar codigo {codigo}", saida, erro)

    entrada_sem_acento = f"{ENTRADA}\n2\n{sem_acentos(departamento)}\n0\n"
    retorno, saida, erro = executar(entrada_sem_acento)
    total += 1
    if retorno != 0 or f"Codigo identificador: {codigo}" not in saida:
        falhar(f"departamento sem acento {departamento} deveria retornar codigo {codigo}", saida, erro)

    for palavra in palavras:
        entrada = f"{ENTRADA}\n1\n{palavra}\n0\n"
        retorno, saida, erro = executar(entrada)
        total += 1
        if retorno != 0 or f"Departamento: {departamento}" not in saida:
            falhar(f"palavra-chave {palavra} deveria retornar departamento {departamento}", saida, erro)

        entrada_sem_acento = f"{ENTRADA}\n1\n{sem_acentos(palavra)}\n0\n"
        retorno, saida, erro = executar(entrada_sem_acento)
        total += 1
        if retorno != 0 or f"Departamento: {departamento}" not in saida:
            falhar(f"palavra-chave sem acento {palavra} deveria retornar departamento {departamento}", saida, erro)

retorno, saida, erro = executar(f"{ENTRADA}\n1\nNAOEXISTE\n2\nRH\n9\n0\n")
total += 1
if retorno != 0 or "Palavra-chave nao encontrada." not in saida or "Departamento nao encontrado." not in saida or "Opcao invalida." not in saida:
    falhar("casos inexistentes e opcao invalida deveriam ser tratados", saida, erro)

retorno, saida, erro = executar("arquivo_inexistente.txt\n")
total += 1
if retorno == 0 or "Erro: Nao foi possivel abrir o arquivo informado." not in saida:
    falhar("arquivo inexistente deveria retornar erro controlado", saida, erro)

print(f"Todos os {total} testes automaticos passaram.")
