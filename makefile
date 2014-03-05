FILES = ABcastSem.c
TARGET= ABcastSem
distsum: $(FILES)
	 gcc -g -Wall -o $(TARGET) $(FILES) -lpthread

