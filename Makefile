TARGET=cwordl

$(TARGET) : *.c
	gcc *.c -o $(TARGET) -std=c11
clean:
	rm $(TARGET)
