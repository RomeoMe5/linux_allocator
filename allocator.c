#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// память - разделяемый ресурс
pthread_mutex_t mem_mutex;
//глобальный указатель на начало спсика блоков
static memBlock_t* base = NULL;
// @TODO: хранить глобальный указатель на хвост списка

//выравнивание блока
inline static size_t getSize(size_t size) 
{
	return size - (size%BORDER) + BORDER;
}

//поиск блока памяти
static memBlock_t* findBlk(size_t size) 
{
	memBlock_t* tmp = base;
	
	while ((tmp!=NULL)
    {
		if(((tmp->size) >= size)&&(tmp->free==true)) 
			break;
		tmp = tmp->next;
	}
	
	if (tmp!=NULL) 
	{
		tmp->free = false;
	}
	
	return tmp;
}

// поиск блока с заданным адресом в памяти
static memBlock_t* getBlkByAddr(void* ptr) 
{
	memBlock_t* blk = base;
	
	while ((blk!=NULL) && (blk != MEMtoBLK(ptr))) 
	{
		blk = blk->next;
	}
	
	return blk;
}

//передвигаем указатель break
static memBlock_t* getMemSpace(size_t size) 
{
	memBlock_t* blk= sbrk(0); 				// текущий адрес break 
	if (sbrk(BLK_SIZE + size) == ((void*)-1)) 	//если нет - то NULL
	{
		return NULL;
	}
	blk->size = size;
	blk->next = NULL;
	blk->free = false;

	if (base != NULL) //если список не пуст - в конец
	{
		memBlock_t* tmp = base;
		while (tmp->next != NULL) 
		{
			tmp = tmp->next;
		}
		tmp->next = blk;
	} 
	else 
	{
		base = blk; 	// иначе - в начало
	}

	return blk;		// текущий блок
}

void* malloc(size_t size)
{
#if DEBUG
	puts("Malloc called");
	fflush(stdout);
#endif

	pthread_mutex_lock(&mem_mutex);	//блокируем ресурс

	size = getSize(size);				//выравняли размер
	memBlock_t* blk = findBlk(size);	//поиск свободных подходящих блоков
	if (blk==NULL) 
	{
		// Подходящий блок не найден
		blk = getMemSpace(size);		
		
		if (blk==NULL) 			// памяти нет
		{
			errno= ENOMEM;		//код ошибки
			pthread_mutex_unlock(&mem_mutex);
			return NULL;
		}
	}

#if DEBUG
	printf("blk=%p base=%p ret=%p blk_size=%lu\n", blk, base, BLKtoMEM(blk), BLK_SIZE);
#endif
	
	pthread_mutex_unlock(&mem_mutex);
	return BLKtoMEM(blk);
}

void* calloc(size_t number, size_t size) 
{
#if DEBUG
	puts("Calloc called");
	fflush(stdout);
#endif
	
	size = getSize(size*number);	//выравнивание
	void* mem = malloc(size);		
	
	if (mem!=NULL) 					//выделили память
	{
		memset(mem, 0, size);		//заполнили ее нулями
	}
	
	return (void*)mem;				//вернули указательна выделенную паямть
}

void free(void* ptr) 
{	
#if DEBUG
	puts("Free called");
	fflush(stdout);
#endif

	memBlock_t* blk = base;
	if (ptr!=NULL) 
	{
		pthread_mutex_lock(&mem_mutex);
		while (blk!=NULL) 
		{
			if (blk == MEMtoBLK(ptr)) 	// нашли нужный блок и установили статус "свободный"
			{
				blk->free = true;
				break;
			}
			blk = blk->next;
		}
		pthread_mutex_unlock(&mem_mutex);
	}
}

void* realloc(void* ptr, size_t size) 
{
#if DEBUG
	puts("Realloc called");
	fflush(stdout);
#endif
	
	size = getSize(size);		//выравняли размер
	if (ptr==NULL) 				// пустоту не пеервыделяем
	{
		return malloc(size);
	} 
	else 
	{
		pthread_mutex_lock(&mem_mutex);
		memBlock_t* blk = getBlkByAddr(ptr);
		if (blk==NULL)							//некорректный адрес
		{
			pthread_mutex_unlock(&mem_mutex);
			return NULL;
		}
		else if (blk->size >= size) 		// если в этом блоке достаточно места
			{
				pthread_mutex_unlock(&mem_mutex);
				return BLKtoMEM(blk);
			} 
			else // если нет - выделяем новую память
			{
				void* tmpPtr = malloc(size);
				if (tmpPtr==NULL) 	//выделенеи памяти не удалось
				{
					pthread_mutex_unlock(&mem_mutex);	
					return NULL;
				} 
				else 
				{
					memcpy(tmpPtr, ptr, (MEMtoBLK(ptr))->size); // копируем содердимое старого блока в новое место
					pthread_mutex_unlock(&mem_mutex);
					free(ptr);									//освободили старый блок
					return tmpPtr;
				}
			}
	}
}


