from threading import Lock, Condition

from Pessoa import Pessoa


class Fila:
    def __init__(self):
        self._pessoas = list()
        self._lock = Lock()
        self._condition_lock = Condition(self._lock)

    # entra no final da fila
    def entrar(self, pessoa: Pessoa):
        with self._lock:
            self._pessoas.append(pessoa)
            print(f'[{pessoa}] Aguardando na fila.')

    # sai do inicio da fila
    def sair(self):
        with self._lock:
            self._pessoas.pop(0)

    # mantém a pessoa (thread) na fila
    def barrar_pessoa(self):
        with self._lock:
            self._condition_lock.wait()

    def liberar_um_lugar_salao(self):
        with self._lock:
            self._condition_lock.notify_all()

    def primeiro(self):
        with self._lock:
            return self._pessoas[0]

    def vazia(self):
        with self._lock:
            return len(self._pessoas) == 0