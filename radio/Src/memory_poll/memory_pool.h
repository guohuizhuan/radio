#ifndef  __MEMORY_POOL_H__
#define  __MEMORY_POOL_H__

#ifdef   __cplusplus
#define  MEMORY_POOL_BEGIN        extern "C" {
#define  MEMORY_POOL_END          }
#else
#define  MEMORY_POOL_BEGIN      
#define  MEMORY_POOL_END       
#endif

MEMORY_POOL_BEGIN

#define  MEMORY_POOL_BLOCK_SIZE        32
#define  MEMORY_POOL_BLOCK_CNT         4


/*
* @brief 内存池初始化
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int memory_pool_init();


MEMORY_POOL_END


#endif