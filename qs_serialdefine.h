/**
 * @file qs_serialdefine.h
 * @author Fernando Morani
 * @date 30 Mar 2022
 * @brief Defines to be used for managing serial device
 *
 */
#ifndef QS_SERIALDEFINE_H
#define QS_SERIALDEFINE_H

#define    QS_SERIAL_BAUD_2400      2400
#define    QS_SERIAL_BAUD_4800      4800
#define    QS_SERIAL_BAUD_9600      9600
#define    QS_SERIAL_BAUD_19200     19200
#define    QS_SERIAL_BAUD_38400     38400
#define    QS_SERIAL_BAUD_57600     57600
#define    QS_SERIAL_BAUD_115200    115200
#define    QS_SERIAL_BAUD_END       7       // Max Baud rate avail

#define    QS_SERIAL_WRITE_TIMEOUT              1000

#define    QS_SERIAL_RES_READ_TMT               111     // 1ms Transfer time + 100ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_READ_FW_READ_TMT           112     // 2ms Transfer time + 100ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_READ_REV_READ_TMT          112     // 2ms Transfer time + 100ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_READ_DEV_READ_TMT          112     // 2ms Transfer time + 100ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_READ_BOOT_READ_TMT         112     // 2ms Transfer time + 100ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_RESET_READ_TMT             1112     // 2ms Transfer time + 100ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_ERASE_READ_TMT             312     // 2ms Transfer time + 300ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_READ_FLASH_READ_TMT        312     // 2ms Transfer time + 300ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_WRITE_FLASH_READ_TMT       312     // 2ms Transfer time + 300ms Target Elab Time + 10ms General latency time
#define    QS_SERIAL_START_FW_UP_READ_TMT       112     // 2ms Transfer time + 100ms Target Elab Time + 10ms General latency time


#define    QS_SERIAL_MAX_BUF_LEN    512
#endif // QS_SERIALDEFINE_H
