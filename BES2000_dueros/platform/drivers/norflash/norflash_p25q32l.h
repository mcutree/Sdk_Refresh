#ifndef NORFLASH_P25Q32L_H
#define NORFLASH_P25Q32L_H

#include "plat_types.h"

/* bytes */
#define P25Q32L_PAGE_SIZE (256)
#define P25Q32L_SECTOR_SIZE (4096)
#define P25Q32L_BLOCK_SIZE (32*1024)
#define P25Q32L_TOTAL_SIZE (4*1024*1024)

/* device cmd : FIXME fixed for P25Q32L */
#define P25Q32L_CMD_ID 0x9F
#define P25Q32L_CMD_WRITE_ENABLE 0x06
#define P25Q32L_CMD_PAGE_PROGRAM 0x02
#define P25Q32L_CMD_QUAD_PAGE_PROGRAM 0x32
#define P25Q32L_CMD_BLOCK_ERASE_32K 0x52
#define P25Q32L_CMD_BLOCK_ERASE_64K 0xD8
#define P25Q32L_CMD_BLOCK_ERASE P25Q32L_CMD_BLOCK_ERASE_32K
//#define P25Q32L_CMD_BLOCK_ERASE P25Q32L_CMD_BLOCK_ERASE_64K
#define P25Q32L_CMD_SECTOR_ERASE 0x20
#define P25Q32L_CMD_CHIP_ERASE 0x60
#define P25Q32L_CMD_READ_STATUS_S0_S7 0x05
#define P25Q32L_CMD_READ_STATUS_S8_S15 0x35

#define P25Q32L_CMD_WRITE_STATUS_S0_S7 0x01
#define P25Q32L_CMD_WRITE_STATUS_S8_S15 0x31
#define P25Q32L_CMD_WRITE_STATUS_S0_S7_S8_S15 0x01

#define P25Q32L_CMD_RESET_EN 0x66
#define P25Q32L_CMD_RESET 0x99

#define P25Q32L_CMD_FAST_QUAD_READ 0xEB
#define P25Q32L_CMD_FAST_DUAL_READ 0xBB
#define P25Q32L_CMD_STANDARD_READ 0x03
#define P25Q32L_CMD_STANDARD_FAST_READ 0x0B
#define P25Q32L_CMD_DEEP_POWER_DOWN 0xB9
#define P25Q32L_CMD_RELEASE_FROM_DP 0xAB
#define P25Q32L_CMD_HIGH_PERFORMANCE 0xA3

/* device register */
/* status register _S0_S7*/
#define P25Q32L_WIP_BIT_SHIFT 0
#define P25Q32L_WIP_BIT_MASK ((0x1)<<P25Q32L_WIP_BIT_SHIFT)
#define P25Q32L_WEL_BIT_SHIFT 1
#define P25Q32L_WEL_BIT_MASK ((0x1)<<P25Q32L_WEL_BIT_SHIFT)
/* status register _S7_S15*/
#define P25Q32L_QE_BIT_SHIFT 1
#define P25Q32L_QE_BIT_MASK ((0x1)<<P25Q32L_QE_BIT_SHIFT)


#endif /* NORFLASH_P25Q32L_H */
