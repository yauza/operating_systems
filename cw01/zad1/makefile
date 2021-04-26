simple:
	gcc -Wall lib.c main.c -o main

static:
	make clean
	gcc -Wall -c lib.c -o lib.o
	ar rcs liblib.a lib.o
	#gcc -Wall main.c -llib -L. -o main

shared:
	make clean
	gcc -Wall -fPIC -shared lib.c -o liblib.so
	#gcc -Wall main.c -L. -Wl,-rpath=. -llib -o main

clean:
	rm -f *.o *.a *.so main
	rm -f *temp.txt


