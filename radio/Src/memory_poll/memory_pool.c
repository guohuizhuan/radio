#include "cmsis_os.h"
#include "memory_pool.h"

static osPoolId memory_pool_id;
typedef struct
{
    uint8_t buffer[MEMORY_POOL_BLOCK_SIZE];
}memory_pool_block_t;

/*
* @brief 内存池初始化
* @param 无
* @param
* @return > 0：内存池句柄 其他：失败
* @note
*/
int memory_pool_init()
{


    return (int)memory_pool_id;

}

/*
* @brief 从内存池中分配一块内存
* @param size 期望分配的内存大小
* @param
* @return > 0：分配的内存块地址 -1：失败
* @note
*/
void *memory_pool_alloc(uint32_t size)
{
    void *mem;
    
    mem = osPoolAlloc(
    osPoolDef(memory_pool,MEMORY_POOL_BLOCK_CNT,memory_pool_block_t);
    memory_pool_id = osPoolCreate (osPool(memory_pool));
    if (memory_pool_id == NULL) {
        return -1;
    }

    return 0;

}