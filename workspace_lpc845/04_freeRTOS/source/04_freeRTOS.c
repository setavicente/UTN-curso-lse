#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

// GPIO del LED azul
#define LED_BLUE	1

/**
 * @brief Tarea que hace parpadear el LED
 */
void task_blink(void *params) { //esto es una funcion, no una tarea

	while(1) {
		// Conmuto el LED
		GPIO_PinWrite(GPIO, 1, LED_BLUE, !GPIO_PinRead(GPIO, 1, LED_BLUE));
		// Bloqueo la tarea por medio segundo (500 ticks)
		vTaskDelay(500);
	}
}

/**
 * @brief Programa principal
 */
int main(void) {
	// Clock del sistema de 30 MHz
	BOARD_BootClockFRO30M();
	// Inicializo el puerto
	gpio_pin_config_t out_config = { kGPIO_DigitalOutput, 1 };
	GPIO_PortInit(GPIO, 1);
	GPIO_PinInit(GPIO, 1, LED_BLUE, &out_config);

	xTaskCreate( //CON ESTO SE CREA LA TAREA
		task_blink,					// Callback de la tarea
		"Blinky",					// Nombre
		configMINIMAL_STACK_SIZE,	// Stack reservado, CON ESTA MACRO SON 64 BYTES
		NULL,						// Sin parametros
		tskIDLE_PRIORITY + 1UL,		// Prioridad
		NULL						// Sin handler
	);

	vTaskStartScheduler();

    while(1);// NUNCA SE LLEGA A ESTO XQ CON vTaskStartScheduler(); EL SIST OPERATIVO TOMA EL CONTROL
    return 0;
}
