/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                         (c) Copyright 1992-1999, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*                                       80x86/80x88 Specific code
*                                          LARGE MEMORY MODEL
*
*                                          VC++6.0
*
* File         : OS_CPU.H
* By           : Jean J. Labrosse
  Changed by   : luyl Email?? luyl@uestc.edu.cn
*********************************************************************************************************
*/
#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                  */ 
typedef unsigned int OS_CPU_SR;                /* Define size of CPU status register (PSW = 32 bits) */ 

#define BYTE           INT8S                     /* Define data types for backward compatibility ...   */
#define UBYTE          INT8U                     /* ... to uC/OS V1.xx.  Not actually needed for ...   */
#define WORD           INT16S                    /* ... uC/OS-II.                                      */
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U


/* 
*********************************************************************************************************
*                              Intel 80x86 (Real-Mode, Large Model)
*
* Method #1:  ???????ü򵥵ĵ?һ?ַ???,????һ??ȫ?ֱ?????ͨ??????ȫ?ֱ??????????ж?
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if 
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.  You MUST change the constant in OS_CPU_A.ASM, function 
*             OSIntCtxSw() from 8 to 10.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/
extern BOOLEAN FlagEn;
#define  OS_CRITICAL_METHOD    1

#if      OS_CRITICAL_METHOD == 1
#define  OS_ENTER_CRITICAL()  FlagEn=0                   /* ??ֹ??ʱ?? ????                      */
#define  OS_EXIT_CRITICAL()   FlagEn=1				//??????ʱ??????                /* Enable  interrupts                        */
#endif

#if      OS_CRITICAL_METHOD == 2
#define  OS_ENTER_CRITICAL()  _asm {PUSHF; CLI}            /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   _asm  POPF                   /* Enable  interrupts                        */
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  (cpu_sr = OSCPUSaveSR())    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   (OSCPURestoreSR(cpu_sr))    /* Enable  interrupts                        */
#endif

/*
*********************************************************************************************************
*                           Intel 80x86 (Real-Mode, Large Model) Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                       /* Stack grows from HIGH to LOW memory on 80x86  */


	
#define OS_TASK_SW() OSCtxSw()		//????ģʽ?²????ж?


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/



#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
OS_CPU_SR  OSCPUSaveSR(void);
void       OSCPURestoreSR(OS_CPU_SR cpu_sr);
#endif