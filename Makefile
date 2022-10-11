CC = gcc
CFLAGS = `pkg-config --cflags gtk4`
LDLIBS = `pkg-config --libs gtk4`


Qalculator: Qalculator.c
	$(CC) $(CFLAGS) Qalculator.c -o Qalculator.o -lm $(LDLIBS)
clean:
	-rm Qalculator
