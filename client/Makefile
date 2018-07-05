main = client.o cmd.o my_function.o
client:$(main)
	@g++ $(main) -o client -lpthread
client.o:client.c
	@g++ client.c -c
cmd.o:cmd.c cmd.h
	@g++ cmd.c -c
my_function.o:my_function.c my_function.h
	@g++ my_function.c -c
.PHONY: clean cleanall
clean:
	@rm *.o
cleanall:
	@rm *.o  client