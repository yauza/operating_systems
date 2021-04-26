static:
	gcc -Wall -c lib.c -o lib.o
	ar rcs liblib.a lib.o
	gcc -Wall main.c -llib -L. -o main

shared:
	gcc -Wall -fPIC -shared lib.c -o liblib.so
	gcc -Wall main.c -L. -Wl,-rpath=. -llib -o main

dynamic:
	gcc -Wall -fPIC -shared lib.c -o liblib.so
	gcc -Wall main.c -ldl -D DYNAMIC -o main

clean:
	rm -f *.o *.a *.so main
	rm -f *temp.txt
	rm -f raport3a.txt

clean_without_report:
	rm -f *.o *.a *.so main
	rm -f *temp.txt

test_small:
	./main 5 merge_files as.txt:bs.txt bs.txt:as.txt as.txt:as.txt bs.txt:bs.txt as.txt:bs.txt

test_medium:
	./main 5 merge_files am.txt:bm.txt bm.txt:am.txt am.txt:am.txt bm.txt:bm.txt am.txt:bm.txt

test_big:
	./main 5 merge_files ab.txt:bb.txt bb.txt:ab.txt ab.txt:ab.txt bb.txt:bb.txt ab.txt:bb.txt

test_blocks:
	./main 5 create_table 5 merge_files ab.txt:bb.txt bb.txt:ab.txt ab.txt:ab.txt bb.txt:bb.txt ab.txt:bb.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 \
	create_table 5 merge_files ab.txt:bb.txt bb.txt:ab.txt ab.txt:ab.txt bb.txt:bb.txt ab.txt:bb.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4

test_static:
	make clean_without_report
	make static
	make test_small
	make test_medium
	make test_big
	make test_blocks

test_dynamic:
	make clean_without_report
	make dynamic
	make test_small
	make test_medium
	make test_big
	make test_blocks

test_shared:
	make clean_without_report
	make shared
	make test_small
	make test_medium
	make test_big
	make test_blocks

test_all:
	echo "\n   STATIC   \n---------------\n" >> raport3a.txt
	make test_static
	printf "\n   DYNAMIC   \n---------------\n" >> raport3a.txt
	make test_dynamic
	printf "\n   SHARED   \n---------------\n" >> raport3a.txt
	make test_shared