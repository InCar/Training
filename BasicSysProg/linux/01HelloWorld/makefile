MAKEFILE=makefile

all: ./hello.o
	cc hello.o  -o hello
	@echo "PROGRAM MAKED"
./hello: hello.c 
	cc -c ./hello.c
clean:  
	rm -f *.o hello
