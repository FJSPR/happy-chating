main = server.o cmd_parse.o mySQL.o mysocket.o
server:$(main)
	@gcc $(main) -o server -lsqlite3
server.o:server.c 
	@gcc server.c -c
cmd_parse.o:cmd_parse.c cmd_parse.h
	@gcc cmd_parse.c -c
mySQL.o:mySQL.c mySQL.h
	@gcc mySQL.c -c
mysocket.o:mysocket.c mysocket.h
	@gcc mysocket.c -c
.PHONY: clean cleanall
clean:
	@rm *.o
cleanall:
	@rm *.o  server
