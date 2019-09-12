/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   AppTask_1TCB;
static  OS_TCB   AppTask_2TCB;
static  OS_TCB   AppTask_3TCB;
static  OS_TCB   AppTask_4TCB;
static  OS_TCB   AppTask_5TCB;
static  OS_TCB   AppTask_6TCB;


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTask_1Stk [ APP_TASK_1_STK_SIZE ];
static  CPU_STK  AppTask_2Stk [ APP_TASK_2_STK_SIZE ];
static  CPU_STK  AppTask_3Stk [ APP_TASK_3_STK_SIZE ];
static  CPU_STK  AppTask_4Stk [ APP_TASK_4_STK_SIZE ];
static  CPU_STK  AppTask_5Stk [ APP_TASK_5_STK_SIZE ];
static  CPU_STK  AppTask_6Stk [ APP_TASK_5_STK_SIZE ];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTask_1  ( void * p_arg );
static  void  AppTask_2  ( void * p_arg );
static  void  AppTask_3  ( void * p_arg );
static  void  AppTask_4  ( void * p_arg );
static  void  AppTask_5  ( void * p_arg );
static  void  AppTask_6  ( void * p_arg );

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;
	
//	
//		SCB->VTOR = FLASH_BASE | 0x10000;//中断向量表偏移
//	
//		__enable_irq();//开所有中断	 


    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
		
		
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    CPU_IntDisMeasMaxCurReset();


    OSTaskCreate((OS_TCB     *)&AppTask_1TCB,                /* Create the _1 task                                */
                 (CPU_CHAR   *)"App Task _1",
                 (OS_TASK_PTR ) AppTask_1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_1_PRIO,
                 (CPU_STK    *)&AppTask_1Stk[0],
                 (CPU_STK_SIZE) APP_TASK_1_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_1_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTask_2TCB,                /* Create the _2 task                                */
                 (CPU_CHAR   *)"App Task _2",
                 (OS_TASK_PTR ) AppTask_2,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_2_PRIO,
                 (CPU_STK    *)&AppTask_2Stk[0],
                 (CPU_STK_SIZE) APP_TASK_2_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_2_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTask_3TCB,                /* Create the _3 task                                */
                 (CPU_CHAR   *)"App Task _3",
                 (OS_TASK_PTR ) AppTask_3,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_3_PRIO,
                 (CPU_STK    *)&AppTask_3Stk[0],
                 (CPU_STK_SIZE) APP_TASK_3_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_3_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
		OSTaskCreate((OS_TCB     *)&AppTask_4TCB,                /* Create the _4 task                                */
                 (CPU_CHAR   *)"App Task _4",
                 (OS_TASK_PTR ) AppTask_4,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_4_PRIO,
                 (CPU_STK    *)&AppTask_4Stk[0],
                 (CPU_STK_SIZE) APP_TASK_4_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_4_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
		
		OSTaskCreate((OS_TCB     *)&AppTask_5TCB,                /* Create the _5 task                                */
                 (CPU_CHAR   *)"App Task _5",
                 (OS_TASK_PTR ) AppTask_5,
                 (void       *) 0, 
                 (OS_PRIO     ) APP_TASK_5_PRIO,
                 (CPU_STK    *)&AppTask_5Stk[0],
                 (CPU_STK_SIZE) APP_TASK_5_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_5_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
								 
	 OSTaskCreate((OS_TCB     *)&AppTask_6TCB,                /* Create the _6 task                                */
                 (CPU_CHAR   *)"App Task _6",
                 (OS_TASK_PTR ) AppTask_6,
                 (void       *) 0, 
                 (OS_PRIO     ) APP_TASK_6_PRIO,
                 (CPU_STK    *)&AppTask_6Stk[0],
                 (CPU_STK_SIZE) APP_TASK_6_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_6_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
								 
								 
		OSTaskDel ( & AppTaskStartTCB, & err );
		
		
}


/*
*********************************************************************************************************
*                                          1 TASK
*********************************************************************************************************
*/

static  void  AppTask_1 ( void * p_arg )//wifi启动、通讯
{
    OS_ERR      err;
   (void)p_arg;
    while (DEF_TRUE) 
		{   
      OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );	
			macLED1_ON();			
			//Link_start();		
    }			
}


/*
*********************************************************************************************************
*                                          2 TASK
*********************************************************************************************************
*/

static  void  AppTask_2 ( void * p_arg )//实时数据上传
{
   OS_ERR      err;
   (void)p_arg;	
	while(1)
	{
		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );
		//real_time_data();	
	}	
}


/*
*********************************************************************************************************
*                                          3 TASK
*********************************************************************************************************
*/

static  void  AppTask_3 ( void * p_arg )//加减速转向
{
  OS_ERR      err;
	extern uint8_t Speed_cache_flag;
	extern uint8_t cw;
	(void)p_arg;
	
	while(1)
	{
//		if(Speed_cache_flag == 1)
//		{
//			Speed_cache_flag = 0;
//			Speed_cache();
//			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );
//			EXTI->IMR |= HUOER_INT_EXTI_LINE;//开启中断
//			EXTI->IMR |= HUOER1_INT_EXTI_LINE;//开启中断
//		}
		OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
	
	}
		
}

/*
*********************************************************************************************************
*                                          4 TASK
*********************************************************************************************************
*/

static  void  AppTask_4 ( void * p_arg )//电量监测
{
  OS_ERR      err;
	(void)p_arg;
	
	while(1)
	{
		//Electricity_monitoring();//电量监测函数
		OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );
	}
		
}


/*
*********************************************************************************************************
*                                          5 TASK
*********************************************************************************************************
*/

static  void  AppTask_5 ( void * p_arg )//485通信发送
{
  OS_ERR      err;
	(void)p_arg;
	while(1)
	{
		Battery_tx();
		//Modbus_tx();
		OSTimeDly ( 4000, OS_OPT_TIME_DLY, & err );//延时2s调度	
	}
		
}

/*
*********************************************************************************************************
*                                          6 TASK
*********************************************************************************************************
*/
static  void  AppTask_6 ( void * p_arg )//看门狗
{
  
	OS_ERR      err;
	//IWDG_Config(IWDG_Prescaler_64 ,12500);//独立看门狗设计，时间为64/40*12500 = 20s
	
	(void)p_arg;
	
	while(1)
	{
		OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );//延时20ms调度
		//IWDG_Feed();//喂狗
	}
		
}

