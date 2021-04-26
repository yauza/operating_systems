simple:
	gcc -Wall main.c -o main

clean:
	rm -f *.o main
	rm -f pomiar_zad_2.txt

test:
	make clean
	make simple
	./main a a.txt
	./main b a.txt
	./main c a.txt
