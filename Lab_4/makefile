CMP = gcc

TARGET = shm

all: $(TARGET)

$(TARGET): p.o shm.o
	$(CMP) -o $@ $^

p.o: p.c shm.h
	$(CMP) -c $<

shm.o: shm.c shm.h
	$(CMP) -c $<

clean:
	rm -f *.o
