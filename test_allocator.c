/*
������: ��� ������ ������� syscall ����������� ��������� ������. ������ ���� ���������������� ������� malloc, calloc, realloc, free.
� �������� �������� ������� ����� ������������, ��������, brk ��� ��������� mmap
������, ��� ���� ������� �� ���, ����� �� ���������� ����� ������ ��������, � ����� - ������ � �� ����������� �����, ���� ����� �������� ������. ��� ������������� mmap �� ����� �������� �������� ������ ���� ����� ������������ ��������� ���� - ������ ���������� �������� ����� ���������.
syscall �������� � unistd.h
���������� ��������� ���� ������� ��� shared ���������� (gcc -fPIC -shared) � �������� libc ������������� ���������� ��� ������ ���������� ��������� LD_PRELOAD (export LD_PRELOAD=filename)

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
	byte* mem2 = malloc(N*2*sizeof(byte));		//2� ���� ������
	for (byte i = 0; i < N*2; i++) 
	{
		mem2[i] = i+N;
	}
	for (byte i = 0; i < N*2; i++) 
	{
		if (mem2[i] != i+N) 
			printf("Malloc error!\n");	
	}
	
	mem = realloc(mem, N*2*sizeof(byte));		//������������ ������ � 2 ��� ������
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
	

	mem = realloc(mem, (N-10) * sizeof(int));		//����� ������������ ������� ��������
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

