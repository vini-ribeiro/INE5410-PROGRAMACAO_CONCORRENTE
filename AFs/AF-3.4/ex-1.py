import threading
from time import sleep
from random import randint
from threading import Thread, Lock, Condition

def produtor():
    global buffer
    for i in range(10):
        sleep(randint(0,2))           # fica um tempo produzindo...
        item = 'item ' + str(i)
        with lock:
            while len(buffer) == tam_buffer:
                print('>>> Buffer cheio. Produtor ira aguardar.')
                lugar_no_buffer.wait()    # aguarda que haja lugar no buffer
            buffer.append(item)
            print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
            item_no_buffer.notify()

def consumidor():
    global buffer
    for i in range(10):
        with lock:
            while len(buffer) == 0:
                print('>>> Buffer vazio. Consumidor ira aguardar.')
                item_no_buffer.wait()   # aguarda que haja um item para consumir
            item = buffer.pop(0)
            print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
            lugar_no_buffer.notify()
        sleep(randint(0,2))         # fica um tempo consumindo...

buffer = []
tam_buffer = 5
lock = Lock()
lugar_no_buffer = Condition(lock)
item_no_buffer = Condition(lock)

threads_produtoras = []  # lista de threads produtoras
threads_consumidoras = []  # lista de threads consumidoras

n_threads = 2
for i in range(n_threads):
    threads_produtoras.append(Thread(target=produtor))
    threads_consumidoras.append(Thread(target=consumidor))
    threads_produtoras[i].start()
    threads_consumidoras[i].start()

for i in range(len(threads_produtoras)):
    threads_produtoras[i].join()
    threads_consumidoras[i].join()