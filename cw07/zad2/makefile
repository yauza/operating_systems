simple:
	@gcc -Wall main.c -lpthread -lrt -o main
	@gcc -Wall cook.c -lpthread -lrt -o cook
	@gcc -Wall delivery.c -lpthread -lrt -o delivery

clean:
	@rm -f main cook delivery

all:
	@make simple
	@./main