#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"

/* TODO: insert other include files here. */

#define SEG_A 	GPIO, 0 , 11  	//defino base, puerto y num de GPIO
#define SEG_F 	GPIO, 0 , 13	//defino base, puerto y num de GPIO
#define SEG_E 	GPIO, 0 , 0		//defino base, puerto y num de GPIO
#define SEG_D 	GPIO, 0 , 14	//defino base, puerto y num de GPIO
#define SEG_C 	GPIO, 0 , 6		//defino base, puerto y num de GPIO
#define SEG_B 	GPIO, 0 , 10	//defino base, puerto y num de GPIO
#define A1 		GPIO,0,8		//defino base, puerto y num de GPIO
#define USER 	GPIO,0,4		//defino base, puerto y num de GPIO
#define S1 		GPIO,0,16		//defino base, puerto y num de GPIO
#define S2 		GPIO,0,25		//defino base, puerto y num de GPIO

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
void task_init(void *params) { //esto es una funcion, no una tarea

	GPIO_PortInit(GPIO, 0);///inicializo base y puerto

	gpio_pin_config_t out_config={ .outputLogic = 1, .pinDirection =kGPIO_DigitalOutput};///outconfig para settear como salida el pin
	gpio_pin_config_t in_config={ .pinDirection =kGPIO_DigitalInput};//inconfig para settear como entrada un pin


	GPIO_PinInit(A1 , &out_config);//Puntero a la estructura del enum de los pines

	GPIO_PinInit(USER , &in_config);	//Puntero a la estructura del enum de los pines
	GPIO_PinInit(S1 , &in_config);		//Puntero a la estructura del enum de los pines
	GPIO_PinInit(S2 , &in_config);		//Puntero a la estructura del enum de los pines


	GPIO_PinInit(SEG_B , &out_config);//Puntero a la estructura del enum de los pines
	GPIO_PinInit(SEG_F , &out_config);
	GPIO_PinInit(SEG_E , &out_config);
	GPIO_PinInit(SEG_D , &out_config);
	GPIO_PinInit(SEG_C , &out_config);
	GPIO_PinInit(SEG_A , &out_config);



	}



void task_read_s1_s2(void *params) { //esto es una funcion, no una tarea

	int boton1 = GPIO_PinRead(S1);
	int boton2 = GPIO_PinRead(S2);

}

void task_user(void *params) { //esto es una funcion, no una tarea

	int user = GPIO_PinRead(USER);


}

void task_1(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 1:
	GPIO_PinWrite(SEG_B, 1);
	GPIO_PinWrite(SEG_C, 1);

}

void task_2(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 2:
	GPIO_PinWrite(SEG_A, 1);
	GPIO_PinWrite(SEG_B, 1);
	GPIO_PinWrite(SEG_G, 1);
	GPIO_PinWrite(SEG_E, 1);
	GPIO_PinWrite(SEG_D, 1);
}
void task_3(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 3:
	GPIO_PinWrite(SEG_A, 1);
	GPIO_PinWrite(SEG_B, 1);
	GPIO_PinWrite(SEG_G, 1);
	GPIO_PinWrite(SEG_C, 1);
	GPIO_PinWrite(SEG_D, 1);
}
void task_4(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 4:
	GPIO_PinWrite(SEG_B, 1);
	GPIO_PinWrite(SEG_C, 1);
	GPIO_PinWrite(SEG_G, 1);
	GPIO_PinWrite(SEG_F, 1);

}
void task_5(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 5:
	GPIO_PinWrite(SEG_A, 1);
	GPIO_PinWrite(SEG_F, 1);
	GPIO_PinWrite(SEG_G, 1);
	GPIO_PinWrite(SEG_C, 1);
	GPIO_PinWrite(SEG_D, 1);
}

void task_6(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 6:
	GPIO_PinWrite(SEG_A, 1);
	GPIO_PinWrite(SEG_F, 1);
	GPIO_PinWrite(SEG_D, 1);
	GPIO_PinWrite(SEG_E, 1);
	GPIO_PinWrite(SEG_C, 1);
	GPIO_PinWrite(SEG_G, 1);
}

void task_7(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 7:
	GPIO_PinWrite(SEG_A, 1);
	GPIO_PinWrite(SEG_B, 1);
	GPIO_PinWrite(SEG_C, 1);

}
void task_8(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 8:
	GPIO_PinWrite(SEG_A, 1);
	GPIO_PinWrite(SEG_B, 1);
	GPIO_PinWrite(SEG_C, 1);
	GPIO_PinWrite(SEG_E, 1);
	GPIO_PinWrite(SEG_D, 1);
	GPIO_PinWrite(SEG_F, 1);
	GPIO_PinWrite(SEG_G, 1);
}

void task_9(void *params) { //esto es una funcion, no una tarea

	GPIO_PinWrite(A1, 0); //habilito el display 1 (el circuito tiene un PNP que se satura con "0" en base)

	//dibujo el 9:
	GPIO_PinWrite(SEG_A, 1);
	GPIO_PinWrite(SEG_B, 1);
	GPIO_PinWrite(SEG_C, 1);
	GPIO_PinWrite(SEG_F, 1);
	GPIO_PinWrite(SEG_G, 1);
}



int main(void) {

	BOARD_BootClockFRO30M(); //clock a 30 mhz


	xTaskCreate( //CON ESTO SE CREA LA TAREA
			task_init,					// Callback de la tarea
			"init",						// Nombre
			configMINIMAL_STACK_SIZE,	// Stack reservado, CON ESTA MACRO SON 64 BYTES
			NULL,						// Sin parametros
			tskIDLE_PRIORITY + 3UL,		// Prioridad
			NULL						// Sin handler
		);

	xTaskCreate( //CON ESTO SE CREA LA TAREA
				task_read_s1_s2,					// Callback de la tarea
				"read_s1_s2",					// Nombre
				configMINIMAL_STACK_SIZE,	// Stack reservado, CON ESTA MACRO SON 64 BYTES
				NULL,						// Sin parametros
				tskIDLE_PRIORITY + 1UL,		// Prioridad
				NULL						// Sin handler
			);
	xTaskCreate( //CON ESTO SE CREA LA TAREA
				task_user,					// Callback de la tarea
				"user_reset",					// Nombre
				configMINIMAL_STACK_SIZE,	// Stack reservado, CON ESTA MACRO SON 64 BYTES
				NULL,						// Sin parametros
				tskIDLE_PRIORITY + 2UL,		// Prioridad
				NULL						// Sin handler
			);



    }




