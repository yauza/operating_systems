simple:
	@gcc -Wall main.c -o main
	@gcc -Wall exec.c -o exec

clean:
	@rm -f *.o main exec

test_ignore:
	@make clean
	@make simple
	@./main fork ignore
	@./main exec ignore

test_handler:
	@make clean
	@make simple
	@./main fork handler
	@./main exec handler

test_pending:
	@make clean
	@make simple
	@./main fork pending
	@./main exec pending

test_mask:
	@make clean
	@make simple
	@./main fork mask
	@./main exec mask
