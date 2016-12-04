#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdint.h>

// ������ ���������
#define BLK_SIZE (sizeof (memBlock_t))   
// ������������ �� 8 ����
#define BORDER 8						 
//�������������� ������� �-� ������� ����� � ������
#define BLKtoMEM(blk) (void*)((char*)blk + BLK_SIZE)		
#define MEMtoBLK(ptr) (memBlock_t*)((char*)ptr - BLK_SIZE)

//������� ��������
#define DEBUG

// ��������� ��� �������� ���������� � ����� ������
struct memBlock_t {
	size_t size;				// ������ �����
	struct _memBlock_t *next;	// ����������� ���������
	bool free;					// ��������� ����� (��������/�������)
};

/*������� ��� ����������*/
void *malloc(size_t size);		
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif // ALLOCATOR_H
