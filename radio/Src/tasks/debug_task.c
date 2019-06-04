#include "debug_task.h" 

void StartDefaultTask(void const * argument)
{
    uint8_t cnt;
    uint8_t cmd[5];
    uint8_t id;
    int rc;
    uint32_t freq;
  
    log_debug("waiting for cmd...\r\n");

    while (1) {

        cnt = SEGGER_RTT_Read(0,cmd,5);
        if (cnt >= 3) {      
            switch(cmd[0])
            {
                case 'id':
                rc = tea5767_id(&id);   
                break;
    case 'a':
     rc = tea5767_init();   
     break; 
    case 'u':
    rc = tea5767_search_up(SEARCH_STOP_LEVEL_10, &freq);   
    break;
    case 'd':
    rc = tea5767_search_down(SEARCH_STOP_LEVEL_10, &freq);   
    break;   
    
    case 's':
    rc = tea5767_set_cur_freq((cmd[1]-'0')*10000000+(cmd[2]-'0')*1000000+(cmd[3]-'0')*100000);   
    break;
    case 'g':
    rc = tea5767_get_cur_freq(&freq);   
    break;  
    case 'o':
    rc =tea5767_mute_on();
    break;
     case 'f':
    rc =tea5767_mute_off();
    break;
    
    case 'O':
    rc =tea5767_stereo_on();
    break;
     case 'F':
    rc =tea5767_stereo_off();
    break;
    
    case 'c':
    rc = tea5767_cancle_search();   
    break;  
    default :
    rc =-1;
    } 
    if(rc == -1){
    log_error("excute error.\r\n");
    }else{
     log_debug("excute ok.\r\n");
    }
    }
    osDelay(100);
  }