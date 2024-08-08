#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#define DELAY_US    20000

/*Data input*/
#define STAT_REG 0x64 //lê os estados do KBC
#define OUT_BUF 0x60 //recebe e lê os scancodes
#define KBC_IRQ 1 //IRQ line do KBC
#define COMMAND_KBC 0X64 //envia comandos para o KBC
#define ARGUMENTS_KBC 0X60 //envia argumentos de comandos do KBC

/*Status Register*/
#define PARITY BIT(7) //erro:dados inválidos
#define TIMEOUT BIT(6) //erro:dados inválidos
#define AUX BIT(5) // dados do mouse
#define INH BIT(4) //0 se o keyboard estiver inhibited
#define IBF BIT(1) //nao ecreve comandos nem argumentos
#define OBF BIT(0) //dados disponíveis para leitura

/*Scancodes*/
#define BREAKCODE_ESC 0X81 //breakcode da tecla ESC
#define TWO_BYTE_SCAN 0XE0 //primeiro byte dos scancodes de 2 bytes
#define MAKECODE_ESC 0X01 //makecode da tecla ESC

/*KBC commands*/
#define READ_COMMAND 0X20 
#define WRITE_COMMAND 0X60
///-----------------------------
#define CHECK_KBC 0XAA //autoteste
#define OK_CHECK_KBC 0X55 //retorna 0x55 se OK
#define ERROR_CHECK_KBC 0XFC //retorna 0xFC se erro
///--------------------------------------------------
#define CHECK_KEYBOARD 0XAB //verifica a interface
#define OK_CHECK_KEYBOARD 0X00 //retorna 0 se OK
///-------------------------------------------------
#define DISABLE_KBD 0XAD
#define ENABLE_KBD 0XAE

#endif  /* _LCOM_I8254_H */
