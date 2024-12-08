import random
import sys
from asyncio import Condition, sleep

from random import randint, shuffle
from threading import Thread
from time import time, sleep

from Fila import Fila
from Pessoa import Pessoa
from Salao import Salao

# --------------- constantes ------------------------
N_ATRACOES = 3  # número que representa o número de atrações existentes (>1)
N_PESSOAS = 6  # número que representa o número total de pessoas que irão ingressar na atração (>0)
N_VAGAS = 3  # número máximo de pessoas simultâneas em uma única atração
PERMANENCIA = 10  # tempo de cada pessoa na atração
MAX_INTERVALO = 4  # um número que representa o intervalo máximo de tempo entre a chegada de duas pessoas quaisquer na fila (>0)
SEMENTE = 1  # número inteiro que representa a semente a ser utilizada para o gerador de números aleatórios (>=0)
UNID_TEMPO = 2  # número que representa o tempo, em milissegundos, correspondente a uma unidade de tempo na simulação (>0)
# --------------- constantes ------------------------


# --------------- variáveis globais ------------------------
fila = Fila()                   # objeto global da fila
salao = Salao(N_VAGAS)          # objeto global do salão (onde ocorre o evento)
estatiscas_exp = dict()         # guarda as estatístidas de cada experiência str : [int, int]
tempo_total_simulacao = 0       # armazeno o tempo decorrido da simulação
tempo_total_de_pausa_exp = 0    # total de tempo de simulação pausada
tempo_parcial_de_pausa_exp = 0  # utilitário para auxiliar no cálculo total de total de tempo de simulação pausada
# --------------- variáveis globais ------------------------


# responsável por avalizar se a pessoa tem condições de entrar no evento
def pode_entrar(pessoa):
    return ((fila.primeiro() == pessoa and salao.experiencia == pessoa.experiencia and not salao.cheio()) or
            (fila.primeiro() == pessoa and salao.vazio()))


# thread que cuida do processamento de uma pessoa (1 thread - 1 pessoa)
def processa_pessoa(pessoa: Pessoa):
    global fila, salao, estatiscas_exp, tempo_total_de_pausa_exp, tempo_parcial_de_pausa_exp

    # entra na fila
    fila.entrar(pessoa)

    # trava caso não esteja apto a passar
    tempo_espera = time()
    while not pode_entrar(pessoa):
        fila.barrar_pessoa()    # caso não tenho condições de entrar, fica na fila
    tempo_espera = time() - tempo_espera    # captura o tempo de espera
    estatiscas_exp[pessoa.experiencia][1] += tempo_espera # adiciona o tempo de espera na estatística

    # muda a experiência caso o salão esteja vazio e tenha uma experiência diferente
    if salao.vazio() and salao.experiencia != pessoa.experiencia:
        salao.experiencia = pessoa.experiencia

    # salão vazio indica que a experiência esteja pausada
    # o segundo argumento garante que a primeiro pessoa a entrar não suje a medição
    if salao.vazio() and tempo_parcial_de_pausa_exp != 0.0:
        tempo_total_de_pausa_exp += time() - tempo_parcial_de_pausa_exp

    salao.entrar(pessoa)                    # entra no salão
    fila.sair()                             # sai da fila
    fila.liberar_um_lugar_salao()           # libera um lugar na fila
    pessoa.curtir_experiencia(PERMANENCIA)  # fica dentro do salão por um determinado tempo
    salao.sair(pessoa)                      # sai do salão

    # o último a sair (e fila vazia) pausa o evento e inicia o cronômetro de simulação pausada
    if salao.vazio() and fila.vazia():
        print(f'[NASA] Pausando a experiencia {salao.experiencia}.')
        tempo_parcial_de_pausa_exp = time()

    # avisa aos que estão na fila que liberou um lugar no salão
    fila.liberar_um_lugar_salao()


def NasaExperiences():
    global estatiscas_exp, tempo_total_simulacao

    print("[NASA] Simulacao iniciada.")

    # cria as experiências e atribui a um objeto do tipo pessoa
    experiencias = ["AT-" + str(i + 1) for i in range(N_ATRACOES)]
    for experiencia in experiencias:
        estatiscas_exp[experiencia] = [0, 0]

    # criação de uma lista de pessoas
    pessoas = list()
    for i in range(N_PESSOAS):
        n_exp = randint(0, N_ATRACOES - 1)
        estatiscas_exp[experiencias[n_exp]][0] += 1
        pessoas.append(Pessoa("Pessoa " + str(i + 1), experiencias[n_exp]))

    [print(pessoas[i]) for i in range(N_PESSOAS)] # retirar esta linha

    # cria as threads das pessoas
    threads_pessoas = [Thread(target=processa_pessoa, args=(pessoas[i],)) for i in range(N_PESSOAS)]

    tempo_total_simulacao = time()
    for tp in threads_pessoas:
        tp.start()
        sleep(randint(1, MAX_INTERVALO))

    for tp in threads_pessoas:
        tp.join()
    tempo_total_simulacao = time() - tempo_total_simulacao

    print("[NASA] Simulacao finalizada.")

    print("\nTempo médio de espera:")
    for e in experiencias:
        if estatiscas_exp[e][0] != 0:
            print(f'Experiência {e}: {estatiscas_exp[e][1] / estatiscas_exp[e][0]: .2f}')
        else:
            print(f'Experiência {e}: 0.00')

    print(f'\nTaxa de ocupacao: {(tempo_total_simulacao - tempo_total_de_pausa_exp) / tempo_total_simulacao: .2f}')

if __name__ == "__main__":
    # global N_ATRACOES, N_PESSOAS, N_VAGAS, PERMANENCIA, MAX_INTERVALO, SEMENTE, UNID_TEMPO

    # if len(sys.argv) < 8:
    #     print(
    #         "Uso: python3 nasa.py <N_ATRACOES> <N_PESSOAS> <N_VAGAS> <PERMANENCIA> <MAX_INTERVALO> <SEMENTE> <UNID_TEMPO>")
    #     sys.exit(1)
    #
    # # Leitura dos argumentos da linha de comando
    # N_ATRACOES = int(sys.argv[1])
    # N_PESSOAS = int(sys.argv[2])
    # N_VAGAS = int(sys.argv[3])
    # PERMANENCIA = int(sys.argv[4])
    # MAX_INTERVALO = int(sys.argv[5])
    # SEMENTE = int(sys.argv[6])
    # UNID_TEMPO = sys.argv[7]

    PERMANENCIA = PERMANENCIA * UNID_TEMPO
    MAX_INTERVALO = MAX_INTERVALO * UNID_TEMPO

    random.seed(SEMENTE)
    NasaExperiences()
