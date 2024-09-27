all: batTime

batTime: *.c
	cc -o batTime `pkg-config --cflags --libs libnotify`  -g *.c

clean: 
	rm -f batTime

re: clean all
# End of file
