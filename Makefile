objects=main.o

genetic:$(objects)
	cc -o genetic $(objects) -lm

.PHONY:clean

clean:
	rm genetic $(objects)
