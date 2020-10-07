server: server.c server.h myString.c myString.h linkedList.c linkedList.h
	gcc --std=c18 -o server server.c myString.c linkedList.c

client: client.c client.h myString.c myString.h
	gcc --std=c18 -o client client.c myString.c

clean:
	rm *.o server client
