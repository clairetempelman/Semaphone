all: write.o control.o
	gcc -o write write.o
	gcc -o control control.o

write.o: write.c semaphone.h
	gcc -c write.c

control.o: control.c semaphone.h
	gcc -c control.c

clean:
	rm *.o
	rm *~
