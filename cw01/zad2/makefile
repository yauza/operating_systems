static:
	make clean
	gcc -Wall -c lib.c -o lib.o
	ar rcs liblib.a lib.o
	gcc -Wall main.c -llib -L. -o main

shared:
	make clean
	gcc -Wall -fPIC -shared lib.c -o liblib.so
	gcc -Wall main.c -L. -Wl,-rpath=. -llib -o main

clean:
	rm -f *.o *.a *.so main
	rm -f *temp.txt
	rm -f raport2.txt

test_small:
	./main 5 merge_files as.txt:bs.txt bs.txt:as.txt as.txt:as.txt bs.txt:bs.txt as.txt:bs.txt

test_medium:
	./main 5 merge_files am.txt:bm.txt bm.txt:am.txt am.txt:am.txt bm.txt:bm.txt am.txt:bm.txt

test_big:
	./main 5 merge_files ab.txt:bb.txt bb.txt:ab.txt ab.txt:ab.txt bb.txt:bb.txt ab.txt:bb.txt

test_blocks1:
	./main 5 create_table 5 remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4

test_blocks:
	./main 5 create_table 5 merge_files ab.txt:bb.txt bb.txt:ab.txt ab.txt:ab.txt bb.txt:bb.txt ab.txt:bb.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 \
	create_table 5 merge_files ab.txt:bb.txt bb.txt:ab.txt ab.txt:ab.txt bb.txt:bb.txt ab.txt:bb.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4

test_remove_blocks_rows:
	./main 2 create_table 2 merge_files as.txt:bs.txt bs.txt:bs.txt remove_row 0 1 remove_row 0 2 remove_block 1