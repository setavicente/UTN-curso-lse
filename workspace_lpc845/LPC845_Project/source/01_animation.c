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
#define SEG_A GPIO, 0 , 11
#define SEG_F GPIO, 0 , 13
#define SEG_E GPIO, 0 , 0
#define SEG_D GPIO, 0 , 14
#define SEG_C GPIO, 0 , 6
#define SEG_B GPIO, 0 , 10
#define A1 GPIO,0,8
#define USER GPIO,0,4
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

	GPIO_PortInit(GPIO, 0);///base y puerto

	gpio_pin_config_t out_config={ .outputLogic = 1, .pinDirection =kGPIO_DigitalOutput};///outconfig para settear como salida el pin
	gpio_pin_config_t in_config={ .pinDirection =kGPIO_DigitalInput};

	GPIO_PinInit(A1 , &out_config);

	GPIO_PinInit(USER , &in_config);


	GPIO_PinInit(SEG_B , &out_config);//Puntero a la estructura del enum de los pines
	GPIO_PinInit(SEG_F , &out_config);
	GPIO_PinInit(SEG_E , &out_config);
	GPIO_PinInit(SEG_D , &out_config);
	GPIO_PinInit(SEG_C , &out_config);
	GPIO_PinInit(SEG_A , &out_config);




    while(1) {
    	int boton = GPIO_PinRead(USER);
    	if(boton==0){
    	GPIO_PinWrite(A1, 0);

    	GPIO_PinWrite(SEG_B, 0);//base, puerto, pin, lo que queres escribir(lees el pin y negas el estado)
    	for ( uint32_t i= 0; i < 50000; i++);
    	GPIO_PinWrite(SEG_B, 1);
    	GPIO_PinWrite(SEG_F, 0);
    	for ( uint32_t i= 0; i < 50000; i++);
    	GPIO_PinWrite(SEG_F, 1);
    	GPIO_PinWrite(SEG_E, 0);
    	for ( uint32_t i= 0; i < 50000; i++);
    	GPIO_PinWrite(SEG_E, 1);
    	GPIO_PinWrite(SEG_D, 0);
    	for ( uint32_t i= 0; i < 50000; i++);
    	GPIO_PinWrite(SEG_D, 1);
    	GPIO_PinWrite(SEG_C, 0);
    	for ( uint32_t i= 0; i < 50000; i++);
    	GPIO_PinWrite(SEG_C, 1);
    	GPIO_PinWrite(SEG_A, 0);
    	for ( uint32_t i= 0; i < 50000; i++);
    	GPIO_PinWrite(SEG_A, 1);
    	}
    	else {
    	GPIO_PinWrite(A1, 1);
    	}
    }
   return 0 ;
}
