all: write.o control.o 
	gcc -o write write.o
	gcc -o control control.o

control.o: control.c control.h 
	gcc -c control.c

write.o: write.c 
	gcc -c write.c 

