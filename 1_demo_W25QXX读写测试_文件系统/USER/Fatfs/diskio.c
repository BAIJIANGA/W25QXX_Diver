/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "USER_Hardware.h"
#include "USER_BSP.h"
/* Definitions of physical drive number for each drive */
#define DEV_flash		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_SD_Card		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	// DSTATUS stat;
	// int result;
    uint32_t w25_id = 1;
	switch (pdrv) 
    {
	case DEV_flash :
        // printf("|aaaa\r\n");
        // uint32_t KK = 1;
         W25Qxx_ReadID(&W25Qxx_handler,&w25_id);
         //printf("id : %x\r\n",w25_id);
//        if(w25_id != 0x000000 && w25_id != 0xFFFFFF)
//        {
            return RES_OK;
//        }
        
        return STA_NOINIT;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
//	DSTATUS stat;
	int result;

	switch (pdrv) 
    {
        case DEV_flash :
            
            result = disk_status(pdrv);
            if(result == RES_OK)
            {
                return RES_OK;
            }
            break;
        case DEV_SD_Card : 
            result = disk_status(pdrv);
            if(result == RES_OK)
            {
                return RES_OK;
            }
            break;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

	switch (pdrv) 
    {
        case DEV_flash :
            // translate the arguments here
            //读扇区
            //printf("读出 %d 扇区\r\n",sector/4096);
            W25Qxx_Sector_ReadData(&W25Qxx_handler,sector, buff, count);
            // for(int a = 0;a <4096;a++)
            // {
            //     printf("%d ",buff[a]);
            // }
            // printf("\r\n");
//            result = RAM_disk_read(buff, sector, count);
            res = RES_OK;
            return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;

	switch (pdrv) 
    {
        case DEV_flash :
        //printf("写入 %d 扇区\r\n",sector/4096);
        //W25Qxx_SectorErase(&W25Qxx_handler1,sector<<12);
        //SPI_FLASH_BufferWrite(&W25Qxx_handler1,(uint8_t*)buff,sector<<12,count<<12);
            W25Qxx_SectorWrite(&W25Qxx_handler,sector, (uint8_t*)buff, count);
            res = RES_OK;
            return res;
        

	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) 
    {
	    case DEV_flash :
            switch(cmd)
            {
                //返回扇区个数
                case GET_SECTOR_COUNT:
                        *(DWORD *)buff = 1024; //此次试验是w25q128，全部用于文件系统，共16MB字节，即16777216字节
                break;
                //返回扇区大小
                case GET_SECTOR_SIZE:
                        *(WORD *)buff = 4096 ;
                break;
                //返回擦除扇区的最小个数
                case GET_BLOCK_SIZE:
                        *(WORD *)buff = 1 ;   //每次擦除一个扇区
                break;
			}
		    res = RES_OK;
            return res;


    }
	return RES_PARERR;
}
DWORD get_fattime (void)
{
    return HAL_OK;
}
