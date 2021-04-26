simple:
	@gcc -Wall sender.c -o sender
	@gcc -Wall catcher.c -o catcher

clean:
	@rm -f *.o sender catcher

test_kill:
	@echo "***TESTING KILL***"
	@./catcher kill&
	@echo "Catcher is running..."
	@./sender `pidof catcher` 10 kill

test_sigrt:
	@echo "***TESTING SIGRT***"
	@./catcher sigrt&
	@echo "Catcher is running..."
	@./sender `pidof catcher` 10 sigrt

test_sigqueue:
	@echo "***TESTING SIGQUEUE***"
	@./catcher sigqueue&
	@echo "Catcher is running..."
	@./sender `pidof catcher` 10 sigqueue
