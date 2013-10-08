CC=clang
TARGET=rdpscan

default: *.c
	gcc *.c -o $(TARGET) -I. `pkg-config --static --cflags --libs openssl` -pthread

clean:
	rm -f *.o $(TARGET)

