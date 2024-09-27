all: batTime

batTime: *.c
	cc -o batTime *.c

clean: 
	rm -f batTime

re: clean all
# End of file
