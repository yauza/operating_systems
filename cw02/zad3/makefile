simple:
	gcc -Wall main.c -o main -lm
	# adding -lm to make math.h library work

clean:
	rm -f *.o main
	rm -f pomiar_zad_3.txt
	rm -f al.txt bl.txt cl.txt
	rm -f as.txt bs.txt cs.txt

test:
	make clean
	make simple
	./main