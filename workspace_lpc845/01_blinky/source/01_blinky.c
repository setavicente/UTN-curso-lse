/*
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    01_blinky.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#define SEGMENTO
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {
	GPIO_PortInit(GPIO, 1);///base y puerto
	gpio_pin_config_t out_config={ .outputLogic = 1, .pinDirection =kGPIO_DigitalOutput};///outconfig para settear como salida el pin
	GPIO_PinInit(GPIO, 1, 0, &out_config);//Puntero a la estructura del enum de los pines

    while(1) {
    	GPIO_PinWrite(GPIO, 1, 0, !GPIO_PinRead(GPIO, 1, 0));//base, puerto, pin, lo que queres escribir(lees el pin y negas el )
    	for ( uint32_t i= 0; i < 500000; i++);



    }
   return 0 ;
}
