import random
from asyncio import Condition

from random import randint, shuffle
from threading import Thread

from Fila import Fila
from Pessoa import Pessoa
from Salao import Salao

# --------------- constantes ------------------------
N_ATRACOES = 3  # número que representa o número de atrações existentes (>1)
N_PESSOAS = 10  # número que representa o número total de pessoas que irão ingressar na atração (>0)
N_VAGAS = 4  # número máximo de pessoas simultâneas em uma única atração
PERMANENCIA = 5  # tempo de cada pessoa na atração
MAX_INTERVALO = 2  # um número que representa o intervalo máximo de tempo entre a chegada de duas pessoas quaisquer na fila (>0)
SEMENTE = 1  # número inteiro que representa a semente a ser utilizada para o gerador de números aleatórios (>=0)
UNID_TEMPO = 3  # número que representa o tempo, em milissegundos, correspondente a uma unidade de tempo na simulação (>0)
# --------------- constantes ------------------------


# --------------- variáveis globais ------------------------
fila = Fila()
salao = Salao(N_VAGAS)
# --------------- variáveis globais ------------------------


# responsável por avalizar se a pessoa tem condições de entrar no evento
def pode_entrar(pessoa):
    return ((fila.primeiro() == pessoa and salao.experiencia == pessoa.experiencia and not salao.cheio()) or
            (fila.primeiro() == pessoa and salao.vazio()))


def processa_pessoa(pessoa: Pessoa):
    global fila, salao

    # entra na fila
    fila.entrar(pessoa)

    # trava caso não esteja apto a passar
    while not pode_entrar(pessoa):
        fila.barrar_pessoa()

    # caso o salão esteja vazio, muda a experiência
    if salao.vazio() and salao.experiencia != pessoa.experiencia:
        salao.experiencia = pessoa.experiencia


    salao.entrar(pessoa)                    # entra no salão
    fila.sair()                             # sai da fila
    fila.liberar_um_lugar_salao()           # libera um lugar na fila
    pessoa.curtir_experiencia(PERMANENCIA)  # fica dentro do salão por um determinado tempo
    salao.sair(pessoa)                      # sai do salão

    # caso seja a última pessoa a sair, pausa o evento
    if salao.vazio():
        print(f'[NASA] Pausando a experiencia {salao.experiencia}.')

    fila.liberar_um_lugar_salao()   # avisa aos que estão na fila que liberou um lugar no salão


def NasaExperiences():
    print("[NASA] Simulacao iniciada.")

    # cria as experiências e atribui a um objeto do tipo pessoa
    experiencias = ["AT-" + str(i) for i in range(N_ATRACOES)]
    pessoas = [Pessoa("Pessoa " + str(i + 1), experiencias[randint(0, N_ATRACOES - 1)]) for i in range(N_PESSOAS)]
    shuffle(pessoas) # embaralha

    # cria as threads das pessoas
    threads_pessoas = [Thread(target=processa_pessoa, args=(pessoas[i],)) for i in range(N_PESSOAS)]
    for tp in threads_pessoas:
        tp.start()

    for tp in threads_pessoas:
        tp.join()

    print("[NASA] Simulacao finalizada.")


if __name__ == "__main__":
    NasaExperiences()
    random.seed(SEMENTE)