CC = gcc
CFLAGS = -Wall
TARGET = myprog

$(TARGET): main.c areaOfRectangle.c areaOfTriangle.c areaOfCircle.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
