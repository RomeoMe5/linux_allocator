test_allocator: test_allocator.o liballocator.so
	gcc -o test_allocator test_allocator.o -L. -lallocator
	
test_allocator.o: test_allocator.c
	gcc -c test.c

liballocator.so: allocator.o
	gcc -shared -o liballocator.so allocator.o
	
allocator.o: allocator.c
	gcc -c -fpic allocator.c
	
clean:
	rm test_allocator allocator.o test_allocator.o liballocator.so
