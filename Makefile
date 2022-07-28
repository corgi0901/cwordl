TARGET=cwordl

$(TARGET) : *.c
	gcc *.c -o $(TARGET)
clean:
	rm $(TARGET)
