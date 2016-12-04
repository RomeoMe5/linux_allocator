/*
Задача: при помощи функции syscall реализовать аллокатор памяти. Должны быть имплементированы функции malloc, calloc, realloc, free.
В качестве основных вызовов можно использовать, например, brk или анонимный mmap
Учтите, что надо следить за тем, какая из выделенной ядром памяти свободна, а какая - занята и не запрашивать новую, если можно обойтись старой. При использовании mmap не стоит выделять страницу памяти если нужно аллоцировать несколько байт - делите полученные страницы между запросами.
syscall объявлен в unistd.h
Полученный результат надо собрать как shared библиотеку (gcc -fPIC -shared) и заменить libc имплементацию аллокатора при помощи переменной окружения LD_PRELOAD (export LD_PRELOAD=filename)

 */


#include <stdio.h>

#include "allocator.h"

#define byte unsigned char
//N 11..100
#define N 100

int main(int argc, char **argv)
{
	//Malloc test
	byte* mem = malloc(N*sizeof(byte));
	for (byte i = 0; i < N; i++) 
	{
		mem[i] = i;
	}
	for (byte i = 0; i < N; i++) 
	{
		if (mem[i] != i) 
			printf("Malloc error!\n");	
	}
	byte* mem2 = malloc(N*2*sizeof(byte));		//2й блок памяти
	for (byte i = 0; i < N*2; i++) 
	{
		mem2[i] = i+N;
	}
	for (byte i = 0; i < N*2; i++) 
	{
		if (mem2[i] != i+N) 
			printf("Malloc error!\n");	
	}
	
	mem = realloc(mem, N*2*sizeof(byte));		//пеервыделили памяти в 2 раз больше
	for (byte i = 0; i < 20; ++i) 
	{
		mem[i] = i;
	}
	for (byte i = 0; i < N*2; i++) 
	{
		if (mem[i] != i) 
			printf("Realloc error!\n");	
	}
	for (byte i = 0; i < N*2; i++) 
	{
		if (mem2[i] != i+N) 
			printf("Realloc error!\n");	
	}
	

	mem = realloc(mem, (N-10) * sizeof(int));		//снова пеервыделили меньший фрагмент
	for (byte i = 0; i < N-10; i++) 
	{
		if (mem[i] != i) 
			printf("Realloc error!\n");	
	}
	for (byte i = 0; i < N*2; i++) 
	{
		if (mem2[i] != i+N) 
			printf("Realloc error!\n");	
	}

	free(mem);	
	for (byte i = 0; i < N*2; i++) 
	{
		if (mem2[i] != i+N) 
			printf("Free error!\n");	
	}
	
	free(mem2);
 	
	return 0;
}

