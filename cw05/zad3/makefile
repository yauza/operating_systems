simple:
	@gcc -Wall producer.c -o producer
	@gcc -Wall consumer.c -o consumer
	@gcc -Wall main.c -o main

clean:
	@rm -f fifo producer main consumer

test:
	@make clean
	@make simple
	@./main
