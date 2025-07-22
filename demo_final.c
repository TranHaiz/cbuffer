#include "cbuffer.h"

#define DEBUG
#define VER_2
#define TRUE  1
#define FALSE 0
#define FAIL  -1

#define OVERFLOW_CHECK(cb) do   \
{                               \
    if(cb->writer == cb->size)  \
    {                           \
        cb->overflow == TRUE;   \
    }                           \
}                               \
while(0)

void refesh(cbuffer_t* cb)
{
    if(cb->writer >= cb->size)
    {
        cb->writer = 0;
    }
    if(cb->reader >= cb->size)
    {
        cb->reader = 0;
    }
}


//DEBUG LOG
#ifdef DEBUG

#define print(x) printf("%d\n", x)
#define DEBUG_LOG(msg)    fprintf(stderr, "[DEBUG] %s(): %s (line %d)\n", \
                                  __func__, msg, __LINE__)
                                
#else
    #define DEBUG
#endif



/**
 * JUST BASIC, CHUA XET DIEU KIEN ACTIVE,... 
 * CHU Y 1 SO DIEU KIEN KHAC
*/
void cb_init(cbuffer_t *cb, void *buf, uint32_t size)
{
  if(cb == NULL || buf == 0)
  {
    DEBUG_LOG("INIT NULL ?");
    return;
  }
  cb->reader   = 0;
  cb->writer   = 0;
  cb->overflow = 0;
  cb->data     = buf;
  cb->size     = size;
  cb->active   = TRUE;
}
void cb_clear(cbuffer_t *cb)
{
    cb->reader   = 0;
    cb->writer   = 0;
    cb->overflow = 0;
}

#ifdef VER_1
uint32_t cb_read(cbuffer_t *cb, void *buf, uint32_t nbytes)
{
  uint32_t read_index;
  uint8_t* data_read  = buf;
  uint8_t* stop       = buf + nbytes;

  return read_index;
}
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

#ifdef VER_2

void overflow_check(cbuffer_t *cb)
{
    if(cb->writer == cb->size - 1)
    {
        cb->overflow = TRUE; //SET OVERFLOW
    }
}
void reset(cbuffer_t *cb)
{
    if(cb->writer == cb->size)
    {
        cb->writer = 0; 
    }
}

uint32_t cb_read(cbuffer_t *cb, void *buf, uint32_t nbytes)
{
  uint32_t r_index;
  uint8_t* data_read  = buf;
  uint8_t* stop       = buf + nbytes;

  if(!cb->active || cb == 0 || buf == 0)
  {
    DEBUG_LOG("Can't write");
    return FAIL;
  }

  for(r_index = 0; r_index < nbytes; r_index++)
  {
    if(cb_data_count(cb) == 0)
    {
      DEBUG_LOG("Khong doc duoc nua r");
      break;
    }
    else
    {
      if(data_read == NULL)
      {
        break;
        DEBUG_LOG("Dung doc");
      }

      data_read[r_index] = cb->data[cb->reader];
      cb->reader ++;
      reset(cb);
    }
  }
  
  return r_index;
}

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
    if(cb_space_count(cb) == 0)
    {
      DEBUG_LOG("Het xang");
      break;
    }
    else
    {
      cb->data[cb->writer] = w_buffer[w_index];
      cb->writer ++; //increase writter
      reset(cb);
    }
  }

  cb->overflow += nbytes - w_index;

  return w_index;
}
#endif

uint32_t cb_data_count(cbuffer_t *cb)
{
    if(cb->reader <= cb->writer)
    {
        return (cb->writer - cb->reader);
    }
    else
    {
        return (cb->size - (cb->reader + cb->writer));
    }
}
uint32_t cb_space_count(cbuffer_t *cb)
{
    return (cb->size - cb_data_count(cb) - 1);
}



#ifdef DEBUG

uint8_t data[5] = {1, 2, 3, 4, 5};
void print_arr(uint8_t* data,uint32_t size)
{
    for(int i = 0; i < size; i++)
    {
        printf("[%d]=%d\n",i , data[i]);
    }
}

int main()
{   
    cbuffer_t cb_new;
    cb_init(&cb_new, data, 5);  //r=w=0
    print_arr(data, 5);

    uint8_t buff[6] = {6, 7, 8, 9, 10, 11};

    uint8_t check_write = cb_write(&cb_new, buff, 6);
    print_arr(data, 6); //
    print(check_write); //print 4
    print(cb_new.writer);
    print(cb_new.overflow); //print 2

    check_write = cb_write(&cb_new, buff, 6);
    print_arr(data, 6); //
    print(check_write); //print 4
    print(cb_new.writer);
    print(cb_new.overflow); //print 8

    return 0;
}
#endif