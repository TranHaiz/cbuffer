/**
 * @file       <file_name>.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    major.minor.patch
 * @date       2025-07-211
 * @author     Dai Tran
 * @author     Hai Tran
 * @author     Thanh Le  
 * @author     Han Pham
 *           
 * @brief      <A brief description of the content of the file>
 *             
 * @note          
 * @example    example_file_1.c
 *             Example_1 description
 * @example    example_file_2.c
 *             Example_2 description
 */

/* Includes ----------------------------------------------------------- */
#include <stdint.h>
#include <string.h>

#include "cbuffer.h"
/* Private defines ---------------------------------------------------- */
#define DEBUG
#define CB_WRITE_VER_1
#define FAIL -1

#ifdef DEBUG
    #define DEBUG_LOG(msg)    fprintf(stderr, "[DEBUG] %s(): %s (line %d)\n", \
                                    __func__, msg, __LINE__)
#endif

/* Private enumerate/structure ---------------------------------------- */
/**
 * @brief <enum descriptiton>
 */


/**
 * @brief <structure descriptiton>
 */




/* Private macros ----------------------------------------------------- */
/**
 * @brief  <macro description>
 *
 * @param[in]     <param_name>  <param_despcription>
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */

     
//#define INITIALIZE_WRITER_READER_OVERFLOW
/* Public variables --------------------------------------------------- */


/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */
/**
 * @brief      Overflow check when write
 *
 * @param[in]  cb  Pointer to cbuffer_t varriable
 *
 * @attention  This funstion must be used before write data
 *
 * @return  None
 */
void overflow_check(cbuffer_t *cb);
/**
 * @brief      Reset writer if out of size
 *
 * @param[in]  cb  Pointer to cbuffer_t varriable
 *
 * @attention  This funstion must be used after write and increase writer
 *
 * @return  None
 */
void reset_writer(cbuffer_t *cb);


/* Function definitions ----------------------------------------------- */
void cb_init(cbuffer_t *cb, void *buf, uint32_t size)
{
  cb->data = buf; // data storage of cbuffer
  cb->writer = 0; // Set writer to 0
  cb->reader = 0; // Set reader to 0
  cb->overflow = 0; // Set overflow to 0
  cb->size = size; // Set input size 
  cb->active = TRUE; // Set the status of cbuffer to active (true)   
}

void cb_clear(cbuffer_t *cb)
{

}

uint32_t cb_read(cbuffer_t *cb, void *buf, uint32_t nbytes)
{ 
}
#ifdef CB_WRITE_VER_1
uint32_t cb_write(cbuffer_t *cb, void *buf, uint32_t nbytes)
{
    uint32_t w_index;        //Write index
    uint8_t* w_buffer = buf; //Write buffer

    if(!cb->active || cb == 0 || buf == 0)
    {
        DEBUG_LOG("Can't write");
        return FAIL;
    }

    for(w_index = 0; w_index < nbytes; w_index ++)
    {
        if(cb->overflow && (cb->writer < cb->reader))
        {
            overflow_check(cb);
            cb->data[cb->writer] = w_buffer[w_index];
            cb->writer ++; //increase writter
            reset_writer(cb); 
            DEBUG_LOG("Written");
        }
        else if(!cb->overflow)
        {
            overflow_check(cb);
            cb->data[cb->writer] = w_buffer[w_index];
            cb->writer ++; //increase writter
            reset_writer(cb);
            DEBUG_LOG("Written");
        }
        else
        {
            DEBUG_LOG("OUT");
            break;
        }
    }

    return w_index;
}
#endif
uint32_t cb_data_count(cbuffer_t *cb)
{
  if(cb->reader <= cb->writer)
  {
    return cb->writer - cb->reader;
  }
  else
  {
    return cb->writer + cb->size - cb->reader;
  }
}

uint32_t cb_space_count(cbuffer_t *cb)
{
  return cb->size - cb_data_count(cb) - 1;
}
/* Private definitions ----------------------------------------------- */
void overflow_check(cbuffer_t *cb)
{
    if(cb->writer == cb->size - 1)
    {
        cb->overflow = TRUE; //SET OVERFLOW
    }
}
void reset_writer(cbuffer_t *cb)
{
    if(cb->writer == cb->size)
    {
        cb->writer = 0;
    }
}


#ifdef DEBUG
int main()
{

    return 0;
}
#endif

/* End of file -------------------------------------------------------- */