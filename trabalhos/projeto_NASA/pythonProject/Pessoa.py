from time import sleep


class Pessoa:
    def __init__(self, nome: str, exp: str):
        self._nome = nome
        self._experiencia = exp

    # --------------- acesso aos atributos ---------------
    @property
    def nome(self):
        return self._nome

    @nome.setter
    def nome(self, nome):
        self._nome = nome

    @property
    def experiencia(self):
        return self._experiencia

    @experiencia.setter
    def experiencia(self, experiencia):
        self._experiencia = experiencia

    # --------------- acesso aos atributos ---------------

    # --------------- funções auxiliares ---------------
    @staticmethod
    def curtir_experiencia(tempo: float):
        sleep(tempo)

    def __eq__(self, other):
        if not isinstance(other, Pessoa):
            return NotImplemented
        return self._nome == other._nome and self._experiencia == other._experiencia

    def __ne__(self, other):
        if not isinstance(other, Pessoa):
            return NotImplemented
        return not self.__eq__(other)

    def __str__(self):
        return f'{self._nome} / {self._experiencia}'

    # --------------- funções auxiliares ---------------