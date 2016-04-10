.PHONY:all
all:tcp_client tcp_sever
tcp_client:tcp_client.c
	gcc -o $@ $^
tcp_sever:tcp_sever.c
	 gcc -o $@ $^ -lpthread -D_FUN3
.PHONY:clean
clean:
	rm -f tcp_client tcp_sever
