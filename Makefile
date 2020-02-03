CC = gcc
CFLAGS = -g
Objects = CFS.o cfs_fun.o fun.o 

run : $(Objects)
	$(CC) $(CFLAGS) $(Objects) -o CFS
	
cfs_fun : $(Objects)
	$(CC) $(CFLAGS) cfs_fun.c -o cfs_fun.o -c
cfs_fun.o: cfs_fun.h

fun : $(Objects)
	$(CC) $(CFLAGS) fun.c -o fun.o -c
fun.o : fun.h struct.h

CFS : $(Objects)
	$(CC) $(CFLAGS) CFS.c -o CFS.o -c


clean :
	rm CFS