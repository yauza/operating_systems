simple:
	gcc -Wall main.c -o main -lm
	# adding -lm to make math.h library work

clean:
	rm -f *.o main
	rm -f pomiar_zad_5.txt
	rm -f b.txt

test:
	make clean
	make simple
	./main a.txt b.txt