TARGET=cwordle

$(TARGET) : *.c
	gcc *.c -o $(TARGET) -std=c11 -lncurses
clean:
	rm $(TARGET)
