CMP = gcc

TARGET = invert

$(TARGET): invert.o reverse.o
	$(CMP) -o $@ $^

invert.o: invert.c reverse.h
	$(CMP) -c $<

reverse.o: reverse.c reverse.h
	$(CMP) -c $<

clean:
	rm -f *.o
