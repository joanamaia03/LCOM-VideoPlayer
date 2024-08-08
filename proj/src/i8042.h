#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/*Data input*/
#define STAT_REG 0x64 //lê os estados do KBC
#define OUT_BUF 0x60 //recebe e lê os scancodes
#define KBC_IRQ 1 //IRQ line do KBC
#define MOUSE_IRQ 12 //IRQ line do mouse
#define COMMAND_KBC 0X64 //envia comandos para o KBC
#define ARGUMENTS_KBC 0X60 //envia argumentos de comandos do KBC

/*Status Register*/
#define PARITY BIT(7) //erro:dados inválidos
#define TIMEOUT BIT(6) //erro:dados inválidos
#define AUX BIT(5) // dados do mouse
#define INH BIT(4) //0 se o keyboard estiver inhibited
#define IBF BIT(1) //nao ecreve comandos nem argumentos
#define OBF BIT(0) //dados disponíveis para leitura
#define KBC_INT BIT(0) //verifica se os interruptores estão enabled
#define IS_BREAK BIT(7) //verifica se o scancode é um breakcode

/*Scancodes*/
#define BREAKCODE_ESC 0X81 //breakcode da tecla ESC
#define TWO_BYTE_SCAN 0XE0 //primeiro byte dos scancodes de 2 bytes
#define MAKECODE_ESC 0X01 //makecode da tecla ESC
#define BREAKCODE_SPACE 0x39

/*KBC commands*/
#define READ_COMMAND 0X20 
#define WRITE_COMMAND 0X60
#define WRITE_MOUSE 0XD4
///-----------------------------
#define CHECK_KBC 0XAA //autoteste
#define OK_CHECK_KBC 0X55 //retorna 0x55 se OK
#define ERROR_CHECK_KBC 0XFC //retorna 0xFC se erro
///--------------------------------------------------
#define CHECK_KEYBOARD 0XAB //verifica a interface
#define CHECK_MOUSE 0XA9 
#define OK_CHECK_KEYBOARD 0X00 //retorna 0 se OK
#define OK_CHECK_MOUSE 0X00
///-------------------------------------------------
#define DISABLE_KBD 0XAD
#define ENABLE_KBD 0XAE
#define DISABLE_MOUSE 0XA7
#define ENABLE_MOUSE 0XA8
///------------------------------------------------------------------------
#define DIS2 BIT(5) //disable mouse
#define DIS1 BIT(4) //disable keyboard
#define INT2 BIT(1) //enable interrupt on OBF, from mouse
#define INT1 BIT(0) //enable interrupt on OBF, from keyboard
#define FIRST_BYTE BIT(3) //verifica se recebemos o primeiro bit do pacote

/*Keys keyboard*/
#define LEFT_ARROW    0x4b
#define RIGHT_ARROW   0x4d
#define ENTER         0x1c

/*Mouse Commands*/
#define RESET_MOUSE 0XFF
#define RESEND 0XFE //erros de comunicação
#define SET_DEFAULT 0XF6 //valor de default do mouse
#define DISABLE 0XF5 //em stream mode
#define ENABLE 0XF4 //em stream mode
#define SAMPLE_RATE 0XF3 //define a state sampling rate
#define REMOTE_MODE 0XF0 //envia dados apenas a pedido
#define READ_DATA 0XEB //envia pedido de pacote de dados
#define STREAM_MODE 0XEA //envia dados em eventos
#define STAT_REQUEST 0XE9 //configuração do mouse
#define RESOLUTION 0XE8
#define SCAL_ACEL 0XE7 //escala 2:1
#define SCAL_LIN 0XE6 //escala 1:1
#define WRITE_TO_MOUSE 0XD4 //escrever o byte no mouse
#define ACK 0XFA //se esta tudo ok
#define NACK 0XFE //se o byte for invalido
#define ERROR 0XFC //se o byte estiver errado duas vezes consecutivas

#endif  /* _LCOM_I8042_H_ */
