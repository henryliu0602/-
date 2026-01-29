/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <unistd.h>
#include "queue.h"       // cubemx 队列的头文件
#include "my_freertos.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern UART_HandleTypeDef huart1;

TaskHandle_t xHandle1, xHandle2, xHandle3;    //  task1,2,3 的任务句柄
TaskHandle_t SenderHandle_1,SenderHandle_2, SenderHandle1 , SenderHandle2 , ReceiverHandle ;  //  发送和接收队列的句柄
QueueHandle_t queue_handle;
xQueueHandle xQueue1 ,xQueue2 ;               //  创建两个队列的句柄
xQueueSetHandle xQueueSet ;                   //  创建队列集合   使用队列集合需要先在FreeRTOSConfig.h 中定义宏 configUSE_QUEUE_SETS 并将其赋值为 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
uint32_t a ,b,c ,d,e,value1,value2;
uint8_t flag=0;
void vTask2(void *pvParameters) ;
void vTask3(void *pvParameters) ;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const char * s1 = "task1 controller running..." ;
const char * s2 = "task2 controller running..." ;
const char * s3 = "task3 controller running..." ;

uint32_t priority ;

typedef enum {                  //  传感器枚举类型
  humidity_sensor,         //  温度传感器
  pressure_sensor          //  压力传感器
}DataSource_t;

typedef struct {
  uint8_t ucValue ;    // 数据
  DataSource_t sDataSource ;  // 传感器类型
}Data_t ;

static const Data_t xStructsToSend[2] =   //  初始化数据
  {
     {77,humidity_sensor},
     {63,pressure_sensor}
  } ;

void vTask1(void *pvParameters) {

  for (;;) {
    ++a ;
    value1 ++ ;
//  printf("%s\r\n",(char *)pvParameters);
//	osDelay(10);
    for (int i =0; i <500 ;i++){}
/*  if(value1 > 7000)
{
    flag = 1;
    vTaskDelete(NULL);                    // 删除任务  使用函数前需要将 FreeRTOSConfig.h 中的宏 INCLUDE_vTaskDelete 赋值为 1
    vTaskSuspend(NULL);                   // 挂起任务  操作自身 参数填NULL
   vTaskDelay(pdMS_TO_TICKS(100));        // 将任务从就绪态 转到 阻塞态, pdMS_TO_TICKS()宏的作用是将括号中的毫秒转成对应的tick数
}  */
//    vTaskPrioritySet(NULL,3) ;              // 优先级设置
//    priority = uxTaskPriorityGet(xHandle1);   // 获取优先级
  }
}

void vTask2(void *pvParameters) {
  for (;;) {
  ++b;
//  printf("%s\r\n",(char *)pvParameters);
//	osDelay(10);
//   vTaskDelay(pdMS_TO_TICKS(100));
  }
}
void vTask3(void *pvParameters) {
  for (;;) {
  ++c;
//  printf("%s\r\n",(char *)pvParameters);
//	osDelay(10);
  /*  for (int i =0; i <500 ;i++){}
  if(flag == 1)
{  value2 ++ ;
   if(value2 > 10000)
{
    vTaskResume(xHandle1);    //  唤醒任务
    flag = 0 ;
}
}    */
//    vTaskDelay(pdMS_TO_TICKS(100));

  }
}

// 在每次执行任务切换的时候会执行这个函数，中断回调函数
void vApplicationTickHook(void) {     //  钩子函数   对应将宏 configUSE_TICK_HOOK 置为 1
  e++ ;
}

//  RTOS要求在任何状态下都要有任务运行，在其他所有任务都不在就绪态时，这个空闲任务（IDLE）执行，调用该函数
void vApplicationIdleHook( void ) {   //  钩子函数   对应将宏 configUSE_IDLE_HOOK 置为 1
  d++ ;
}

void SenderTask (void * pvParameters) {
  BaseType_t qStatus;
  const TickType_t wait_time = pdMS_TO_TICKS(200);
  while(1) {
    qStatus = xQueueSend(queue_handle,pvParameters, wait_time) ;
   if (qStatus == pdPASS) {

   }
    for (int i =0; i <50000 ;i++){}
  }
}

void SenderTask1(void *pvParameters) {           //  发送任务函数
  uint32_t value = 5;
  BaseType_t qStatus;
  while(1) {
   qStatus = xQueueSend(queue_handle,&value,0) ;     //  向队列中发送数据   这里的句柄传的就是在主函数中创建队列得到的句柄，得以对队列进行具体操作
    if (qStatus != pdTRUE) {
      printf("Sender Task Error\r\n");
    }
    for( int i = 0 ;i < 100000 ; i++ ){}
  }
}

void SenderTask2(void *pvParameters) {           //  发送任务函数
  uint32_t value = 6;
  BaseType_t qStatus;
  while(1) {
    qStatus = xQueueSend(queue_handle,&value,0) ;     //  向队列中发送数据   这里的句柄传的就是在主函数中创建队列得到的句柄，得以对队列进行具体操作
    if (qStatus != pdTRUE) {
      printf("Sender Task Error\r\n");
    }
    for( int i = 0 ;i < 100000 ; i++ ){}
  }
}

void ReceiverTask(void *pvParameters) {          //  接收任务函数
  Data_t xReceiveStructure ;
  const TickType_t wait_time = pdMS_TO_TICKS(100);     //  用宏来设定最大延时时间
  BaseType_t qStatus;
  while(1) {
    qStatus = xQueueReceive(queue_handle,&xReceiveStructure ,wait_time);  //  从队列中接收数据
    if (qStatus == pdTRUE) {
      if (xReceiveStructure .sDataSource ==humidity_sensor) {
        printf("humidity sensor value:%d \r\n",xReceiveStructure.ucValue );
      }
      else if (xReceiveStructure .sDataSource ==pressure_sensor) {
        printf("pressure sensor value:%d \r\n",xReceiveStructure.ucValue );
      }
    }
    else {
      printf("Receiver Error\r\n");
      for( int i = 0 ;i < 100000 ; i++ ){}
    }
  }
}

void SenderTask_1(void *pvParameters) {
  const  TickType_t xBlockTime = pdMS_TO_TICKS(100);
  const char * const msg ="messege from SenderTask_1\r\n " ;

  while(1) {
  vTaskDelay(xBlockTime);
    xQueueSend(xQueue1 ,&msg ,0) ;
  }
}

void SenderTask_2(void *pvParameters) {
  const  TickType_t xBlockTime = pdMS_TO_TICKS(100);
  const char * const msg ="messege from SenderTask_2\r\n " ;

  while(1) {
    vTaskDelay(xBlockTime);
    xQueueSend(xQueue2 ,&msg ,0) ;
  }
}

void ReceiveTask_1(void *pvParameters) {
  QueueHandle_t xQueueThatContainsData ;
  char *pcReicievedString ;
  while(1) {                         //  从队列中接收字符
    xQueueThatContainsData = (QueueHandle_t)xQueueSelectFromSet(xQueueSet,portMAX_DELAY) ;          //  检查队列集中所有成员（队列或信号量）的状态，返回第一个被检测到有数据/事件的成员
    xQueueReceive(xQueueThatContainsData,&pcReicievedString ,0) ;                    // 接收的是被选择的句柄的数据
    printf("%s\n\r",pcReicievedString);

  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
 // xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xHandle1);
 // xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xHandle2);
 // xTaskCreate(vTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 1, &xHandle3);

  // queue_handle = xQueueCreate(5, sizeof(Data_t));      // 创建队列函数  参数是队列的长度 以及 单个元素的大小  队列创建成功之后会得到一个任务句柄
  xQueue1 = xQueueCreate(1,sizeof(char *));
  xQueue2 = xQueueCreate(1,sizeof(char *));

  //  任务集合适用于需要从多个数据来源同步接收处理数据，可以避免通过轮询检查任务状态的方式，把数据放到一个队列里，提高处理效率
  xQueueSet = xQueueCreateSet(1*2) ;   // 创建队列集合，每条队列长度为 1，一共有两条队列   需要先定义宏configUSE_QUEUE_SETS 并赋值为 1
  xQueueAddToSet(xQueue1,xQueueSet) ;     //  将两条队列分别加入队列集合
  xQueueAddToSet(xQueue2,xQueueSet) ;

  // xTaskCreate(SenderTask1,"Sender1",512,NULL,1,&SenderHandle1);    // 创建任务
  // xTaskCreate(SenderTask2,"Sender2",512,NULL,1,&SenderHandle2);    // 创建任务

//  xTaskCreate(SenderTask,"Sender_1",512,(void *)&(xStructsToSend[0]),2,&SenderHandle_1);    // 传入的 (void *)&(xStructsToSend[0]) 就是传给Task函数的参数
//  xTaskCreate(SenderTask,"Sender_2",512,(void *)&(xStructsToSend[1]),2,&SenderHandle_2);
//  xTaskCreate(ReceiverTask,"Receiver",1024,NULL,2,&ReceiverHandle);

    xTaskCreate(SenderTask_1,"sender1",512,NULL,1,NULL);
    xTaskCreate(SenderTask_2,"sender2",512,NULL,1,NULL);
    xTaskCreate(ReceiveTask_1,"receiver",1024,NULL,2,NULL);

    vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Init scheduler */
  //osKernelInitialize();  /* Call init function for freertos objects (in cmsis_os2.c) */
  //MX_FREERTOS_Init();

  /* Start scheduler */
  //osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
