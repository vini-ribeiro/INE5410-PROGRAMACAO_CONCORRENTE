to_run: lista_1

lista_1: lista_introd_c.o exercicio_1.o exercicio_2.o
	gcc -o lista_1 lista_introd_c.o exercicio_1.o exercicio_2.o

lista_introd_c.o: lista_introd_c.c exercicio_1.h exercicio_2.h
	gcc -o lista_introd_c.o lista_introd_c.c -c -W -Wall -pedantic

exercicio_clean_all:
	rm -rf *.o *~ lista_1 lista_1_debugg1.o: exercicio_1.c exercicio_1.h
	gcc -o exercicio_1.o exercicio_1.c -c -W -Wall -pedantic

exercicio_2.o: exercicio_2.c exercicio_2.h
	gcc -o exercicio_2.o exercicio_2.c -c -W -Wall -pedantic


to_debugg: lista_1_debugg

lista_1_debugg: lista_introd_c_debugg.o exercicio_1_debugg.o exercicio_2_debugg.o
	gcc -o lista_1_debugg lista_introd_c.o exercicio_1.o exercicio_2.o

lista_introd_c_debugg.o: lista_introd_c.c exercicio_1.h exercicio_2.h
	gcc -g -o lista_introd_c.o lista_introd_c.c -c -W -Wall -pedantic

exercicio_1_debugg.o: exercicio_1.c exercicio_1.h
	gcc -g -o exercicio_1.o exercicio_1.c -c -W -Wall -pedantic

exercicio_2_debugg.o: exercicio_2.c exercicio_2.h
	gcc -g -o exercicio_2.o exercicio_2.c -c -W -Wall -pedantic

clean_trash:
	rm -rf *.o *~

clean_all:
	rm -rf *.o *~ lista_1 lista_1_debugg