simple:
	@gcc -Wall main.c -lpthread -lm -o main

clean:
	@rm -f main
	@rm -f res_saturn.ascii.pgm res_venus1.ascii.pgm

clean_time:
	@rm -f times.txt

block:
	@make clean
	@make clean_time
	@make simple
	@./main 16 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 16 block venus1.ascii.pgm res_venus1.ascii.pgm

numbers:
	@make clean
	@make clean_time
	@make simple
	@./main 16 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 16 numbers venus1.ascii.pgm res_venus1.ascii.pgm

all:
	@make clean
	@make clean_time
	@make simple
	@./main 1 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 1 numbers venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 2 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 2 numbers venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 4 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 4 numbers venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 8 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 8 numbers venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 16 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 16 numbers venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 32 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 32 numbers venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 100 numbers saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 100 numbers venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 1 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 1 block venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 2 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 2 block venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 4 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 4 block venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 8 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 8 block venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 16 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 16 block venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 32 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 32 block venus1.ascii.pgm res_venus1.ascii.pgm
	@./main 100 block saturn.ascii.pgm res_saturn.ascii.pgm
	@./main 100 block venus1.ascii.pgm res_venus1.ascii.pgm
