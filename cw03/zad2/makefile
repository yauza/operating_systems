simple:
	gcc -Wall lib.c main.c -o main

clean:
	rm -f *.o main
	rm -f *temp.txt time_report.txt

test_small:
	echo "**********\nMałe pliki\n**********\n Mała liczba par:\n" >> time_report.txt
	./main as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt
	echo "\nŚrednia liczba par:\n" >> time_report.txt
	./main as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt \
	bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt
	echo "\nDuża liczba par:\n" >> time_report.txt
	./main as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt \
    bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt \
    bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt \
    bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt \
    bs.txt:as.txt as.txt:bs.txt as.txt:as.txt bs.txt:bs.txt bs.txt:as.txt

test_medium:
	echo "\n\n**********\nŚrednie pliki\n**********\n Mała liczba par:\n" >> time_report.txt
	./main am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt
	echo "\nŚrednia liczba par:\n" >> time_report.txt
	./main am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt \
	bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt
	echo "\nDuża liczba par:\n" >> time_report.txt
	./main am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt \
	bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt \
	bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt \
	bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt \
	bm.txt:am.txt am.txt:bm.txt am.txt:am.txt bm.txt:bm.txt bm.txt:am.txt

test_big:
	echo "\n\n**********\nDuże pliki\n**********\n Mała liczba par:\n" >> time_report.txt
	./main ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt
	echo "\nŚrednia liczba par:\n" >> time_report.txt
	./main ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt \
	bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt
	echo "\nDuża liczba par:\n" >> time_report.txt
	./main ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt \
	bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt \
	bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt \
	bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt \
	bb.txt:ab.txt ab.txt:bb.txt ab.txt:ab.txt bb.txt:bb.txt bb.txt:ab.txt

test_all:
	make clean
	make
	make test_small
	make test_medium
	make test_big

