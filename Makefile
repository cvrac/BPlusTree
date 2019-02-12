OBJS = bplus_tree.o scans_management.o path.o util.o
HEADERS = include
BFLIB = src/BF_64.a

all: bt1 bt2 bt3 bt4

CC 	= gcc
FLAGS	= -c -Iinclude

bt1: $(OBJS) main1.o
	$(CC) -o bt1 $(OBJS) main1.o $(BFLIB)

bt2: $(OBJS) main2.o
	$(CC) -o bt2 $(OBJS) main2.o $(BFLIB)

bt3: $(OBJS) main3.o
	$(CC) -o bt3 $(OBJS) main3.o $(BFLIB)

bt4: $(OBJS) main4.o
	$(CC) -o bt4 $(OBJS) main4.o $(BFLIB)


bplus_tree.o: src/bplus_tree.c
	$(CC) $(FLAGS) src/bplus_tree.c

scans_management.o: src/scans_management.c
	$(CC) $(FLAGS) src/scans_management.c

path.o: src/path.c
	$(CC) $(FLAGS) src/path.c

util.o: src/util.c
	$(CC) $(FLAGS) src/util.c

main1.o: tests/main1.c
	$(CC) $(FLAGS) tests/main1.c

main2.o: tests/main2.c
	$(CC) $(FLAGS) tests/main2.c

main3.o: tests/main3.c
	$(CC) $(FLAGS) tests/main3.c

main4.o: tests/main4.c
	$(CC) $(FLAGS) tests/main4.c

clean:
	rm -f bt1 bt2 bt3 bt4 $(OBJS) main1.o main2.o main3.o main4.o
	rm -f DNAME-AGE EMP-AGE EMP-NAME EMP-SAL
