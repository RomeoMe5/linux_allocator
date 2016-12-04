#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdint.h>

// размер структуры
#define BLK_SIZE (sizeof (memBlock_t))   
// выравнивание до 8 байт
#define BORDER 8						 
//Преобразование адресов м-у адресом блока и памяти
#define BLKtoMEM(blk) (void*)((char*)blk + BLK_SIZE)		
#define MEMtoBLK(ptr) (memBlock_t*)((char*)ptr - BLK_SIZE)

//отладка включена
#define DEBUG

// Структура для зранения информации о блоке памяти
struct memBlock_t {
	size_t size;				// размер блока
	struct _memBlock_t *next;	// указательна следующий
	bool free;					// состояние блока (свободен/выделен)
};

/*Функции для реализации*/
void *malloc(size_t size);		
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif // ALLOCATOR_H
