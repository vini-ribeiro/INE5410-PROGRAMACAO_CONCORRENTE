from time import sleep
from random import randint
from threading import Thread, Semaphore

def produtor():
  global buffer
  for i in range(10):
    sleep(randint(0,2))           # fica um tempo produzindo...
    item = 'item ' + str(i)
    # verifica se há lugar no buffer
    sem_produtor.acquire()  # decrementa o semáforo do produtor, ou seja, indica que irá produzir uma unidade
    buffer.append(item)
    print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sem_consumidor.release()  # incrementa o semáforo do consumidor, ou seja, há uma unidade disponível

def consumidor():
  global buffer
  for i in range(10):
    # aguarda que haja um item para consumir
    sem_consumidor.acquire()  #
    item = buffer.pop(0)
    print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sem_produtor.release()
    sleep(randint(0,2))         # fica um tempo consumindo...

buffer = []
tam_buffer = 3
# cria semáforos. Como teremos apenas um produtor e um consumidor, não precisamos de um mutex para controlar o acesso ao buffer
sem_produtor = Semaphore(tam_buffer)
sem_consumidor = Semaphore(0)
produtor = Thread(target=produtor)
consumidor = Thread(target=consumidor) 
produtor.start()
consumidor.start()
produtor.join()
consumidor.join()