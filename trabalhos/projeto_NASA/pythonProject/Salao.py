from threading import Lock

from Pessoa import Pessoa


class Salao:
    def __init__(self, capacidade: int):
        self.lock = Lock()
        self._pessoas = list()
        self._experiencia = None
        self._capacidade = capacidade

    # --------------- funções para o fluxo de pessoas ---------------
    def vazio(self):
        with self.lock:
            return len(self._pessoas) == 0

    def cheio(self):
        with self.lock:
            return len(self._pessoas) == self._capacidade

    def entrar(self, pessoa: Pessoa):
        with self.lock:
            self._pessoas.append(pessoa)
            print(f'[{pessoa}] Entrou na NASA Experiences (quantidade = {len(self._pessoas)}).')

    def sair(self, pessoa: Pessoa):
        with self.lock:
            self._pessoas.remove(pessoa)
            print(f'[{pessoa}] Saiu da NASA Experiences (quantidade = {len(self._pessoas)}).')

    # --------------- funções para o fluxo de pessoas ---------------

    # --------------- acesso aos atributos ---------------
    @property
    def experiencia(self):
        return self._experiencia

    @experiencia.setter
    def experiencia(self, experiencia):
        with self.lock:
            self._experiencia = experiencia
            print(f'[NASA] Iniciando a experiencia {experiencia}.')

    @property
    def capacidade(self):
        return self._capacidade

    # --------------- acesso aos atributos ---------------