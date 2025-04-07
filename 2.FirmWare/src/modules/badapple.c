
/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <dfs_fs.h>
#include "drv_gc9a01.h"

#define DBG_TAG "app.badapple"
//#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define FILE_PATH "/sdcard/badapple16.bin" 

static rt_thread_t tid;

static rt_uint8_t readbuf[115200];
ALIGN(RT_ALIGN_SIZE)

void gc9a01_lcd_read(void) 
{
    int fd;
    ssize_t result;
    
    rt_thread_mdelay(500);

    fd = open(FILE_PATH, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("Error: Open file %s failed!\n", FILE_PATH);
        return;
    }

    lcd_fill(0, 0, 240, 240, WHITE);
    lcd_address_set(0, 0, 239, 239);	

    for(; ;)
    {    
        result = read(fd, readbuf, sizeof(readbuf));
        
        if (result == 0)
            lseek(fd, 0, SEEK_SET);
        
        if(result < 0)
        {
            LOG_E("Read file %s failed\n", FILE_PATH);
            close(fd);
            break;
        }
        
        lcd_show_image(0, 0, 240, 240 , readbuf);
    }
}

int gc9a01_lcd_init(void)
{
    tid = rt_thread_create("gc9a01_lcd_read", gc9a01_lcd_read, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX - 2, 20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create gc9a01_lcd_read thread err!");
        rt_thread_delete(tid);
    }

    return RT_EOK;
}
INIT_APP_EXPORT(gc9a01_lcd_init);





