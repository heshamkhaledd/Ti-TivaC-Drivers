/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1


#define mainREGION_1_SIZE	7201
#define mainREGION_2_SIZE	29905
#define mainREGION_3_SIZE	6407
#define SIZE 10
#define ROW SIZE
#define COL SIZE


xTaskHandle matrix_handle, readQueue_handle;
QueueHandle_t Queue_1 = NULL;

/*-----------------------------------------------------------*/

static void  prvInitialiseHeap( void );

void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );


static void prvSaveTraceFile( void );

/*-----------------------------------------------------------*/


StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdTRUE;

/*-----------------------------------------------------------*/

/* Create a struct to hold the data*/
typedef struct
{
	double** c;
}message;


/* Declaring an instance of the struct */
message data;



static void matrix_task(void *pvParameters)
{
	(void)pvParameters;

	int i;
	double** a = (double**)pvPortMalloc(ROW * sizeof(double*));
	for (i = 0; i < ROW; i++) a[i] = (double*)pvPortMalloc(COL * sizeof(double));
	double** b = (double**)pvPortMalloc(ROW * sizeof(double*));
	for (i = 0; i < ROW; i++) b[i] = (double*)pvPortMalloc(COL * sizeof(double));
	double** c = (double**)pvPortMalloc(ROW * sizeof(double*));
	for (i = 0; i < ROW; i++) c[i] = (double*)pvPortMalloc(COL * sizeof(double));

	double sum = 0.0;
	int j, k, l;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			a[i][j] = 1.5;
			b[i][j] = 2.6;
		}
	}

	while (1) {

		long simulationdelay;
		for (simulationdelay = 0; simulationdelay < 1000000000; simulationdelay++)
			;
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				c[i][j] = 0.0;
			}
		}

		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				sum = 0.0;
				for (k = 0; k < SIZE; k++) {
					for (l = 0; l < 10; l++) {
						sum = sum + a[i][k] * b[k][j];
					}
				}
				c[i][j] = sum;
			}
		}


		/* Store the data of double** c in the created instance of the struct */
		data.c = (double**)pvPortMalloc(ROW * sizeof(double*));
		for (i = 0; i < ROW; i++) data.c[i] = \
			(double*)pvPortMalloc(COL * sizeof(double));
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				data.c[i][j] = c[i][j];
				c[i][j] = 0.0;
			}
		}

		/* Pushing the Matrix inside the Queue*/
		xQueueSend (Queue_1,(void *) &data, (TickType_t)0);
		printf("Data sent to Queue!\n");
		fflush(stdout);
		vTaskDelay(100);
	}
}


static void readQueue_task(void* pvParameters)
{
	(void)pvParameters;

	message recv;
	int i, j;

	while (1) {

		/* Check if the Queue isn't Empty by using xQueuePeek function */
		if ((xQueuePeek(Queue_1, &recv, (TickType_t)10)) != pdFALSE) 
		{
			
			/* If true, Recieve the data and store it inside the recv instance*/
			xQueueReceive(Queue_1, &recv, (TickType_t) 10);
			printf("Data Received from Queue!\n");
			fflush(stdout);

			for (i = 0; i < SIZE; i++) {
				for (j = 0; j < SIZE; j++) {
					printf("Queue Element[%d][%d]: %f\n",i,j, (recv.c[i][j]));
				}
			}
		}
		else {
			printf("Queue is empty!\n");
			fflush(stdout);
		}

		vTaskDelay(100);

	}
		
}


int main( void )
{
	prvInitialiseHeap();

	vTraceEnable( TRC_START );


	/* Create a queue and store it's handle in Queue_1 */
	Queue_1 = xQueueCreate(10, sizeof(message*));

	if (Queue_1 == NULL) {
		printf("Queue wasn't created!\n");
		fflush(stdout);
	}
	else {
		printf("Queue created succesfully!\n");
		fflush(stdout);
	}


	xTaskCreate(matrix_task,"Matrix", 1000, NULL, 3, &matrix_handle);
	xTaskCreate(readQueue_task, "Reader", 1000, NULL, 2, &readQueue_handle);

	vTaskStartScheduler();
	for (;;);
	return 0;
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{

	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{


	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		/* Call the idle task processing used by the full demo.  The simple
		blinky demo does not use the idle task hook. */
		vFullDemoIdleFunction();
	}
	#endif
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;
	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{

	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		vFullDemoTickHookFunction();
	}
	#endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */

}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{

}
/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
static BaseType_t xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	( void ) ulLine;
	( void ) pcFileName;

	printf( "ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError() );

 	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if( xPrinted == pdFALSE )
		{
			xPrinted = pdTRUE;
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
			}
		}

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
			__asm{ NOP };
			__asm{ NOP };
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

static void prvSaveTraceFile( void )
{
FILE* pxOutputFile;

	fopen_s( &pxOutputFile, "Trace.dump", "wb");

	if( pxOutputFile != NULL )
	{
		fwrite( RecorderDataPtr, sizeof( RecorderDataType ), 1, pxOutputFile );
		fclose( pxOutputFile );
		printf( "\r\nTrace output saved to Trace.dump\r\n" );
	}
	else
	{
		printf( "\r\nFailed to create trace dump file\r\n" );
	}
}
/*-----------------------------------------------------------*/

static void  prvInitialiseHeap( void )
{

static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
const HeapRegion_t xHeapRegions[] =
{
	/* Start address with dummy offsets						Size */
	{ ucHeap + 1,											mainREGION_1_SIZE },
	{ ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
	{ ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
	{ NULL, 0 }
};

	/* Sanity check that the sizes and offsets defined actually fit into the
	array. */
	configASSERT( ( ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE ) < configTOTAL_HEAP_SIZE );

	/* Prevent compiler warnings when configASSERT() is not defined. */
	( void ) ulAdditionalOffset;

	vPortDefineHeapRegions( xHeapRegions );
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

