#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_swm.h"
#include "fsl_power.h"
#include "fsl_adc.h"

// Canal de ADC para el potenciometro
#define ADC_POT_CH		0
// Pin para el LED azul
#define LED_BLUE	1



int main(void) {

	static uint16_t i200ms = 0;
	static uint16_t i2000ms = 0;


	// Inicializacion LED
	BOARD_BootClockFRO24M();

	// Estructura de configuracion de salida
	gpio_pin_config_t out_config = { kGPIO_DigitalOutput, 1 };

	// Habilito el puerto 1
	GPIO_PortInit(GPIO, 1);
	// Configuro LED como salida
	GPIO_PinInit(GPIO, 1, LED_BLUE, &out_config);




	// Inicializacion de clock
	BOARD_BootClockFRO30M();//seteo en 30Mhz el FRO(free running oscillator, es el oscilador del sistema)(sin esto va por default a 18 Mhz)
    BOARD_InitDebugConsole();

    // Activo clock de matriz de conmutacion
    CLOCK_EnableClock(kCLOCK_Swm);
    // Configuro la funcion de ADC en el canal del potenciometro
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);
    // Desactivo clock de matriz de conmutacion (esto es innecesario)
    CLOCK_DisableClock(kCLOCK_Swm);

    // Elijo clock desde el FRO con divisor de 1(prescaler) (como mencione antes 30MHz) (es l max.)
    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1);

    // Prendo el ADC
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    // Obtengo frecuencia deseada y calibro ADC
	uint32_t frequency = CLOCK_GetFreq(kCLOCK_Fro) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
	ADC_DoSelfCalibration(ADC0, frequency);//me aseguro de que el adc labure con clk de 30 nMhz si o si
	// Configuracion por defecto del ADC (Synchronous Mode, Clk Divider 1, Low Power Mode true, High Voltage Range)
	adc_config_t adc_config;
	ADC_GetDefaultConfig(&adc_config);
    // Habilito el ADC
	ADC_Init(ADC0, &adc_config);
	// Configuracion para las conversiones
	adc_conv_seq_config_t adc_sequence = {
		.channelMask = 1 << ADC_POT_CH,							// Elijo el canal del potenciometro. Si quiero x ej canal 3 y canal 4: hago | 1 << 3 | 1 << 4
		.triggerMask = 0,										// No hay trigger por hardware (lo disparamos nosotros x software, x eso esta en 0)
		.triggerPolarity = kADC_TriggerPolarityPositiveEdge,	// Flanco ascendente
		.enableSyncBypass = false,								// Sin bypass de trigger
		.interruptMode = kADC_InterruptForEachConversion		// Interrupciones para cada conversion (ahora no hay interrupcion ya q estamos en polling, puedo hacer que la interrupcion sea x cada seq. o por cada canal)
	};

	// Configuro y habilito secuencia A
	ADC_SetConvSeqAConfig(ADC0, &adc_sequence);
	ADC_EnableConvSeqA(ADC0, true);


	// Configuro SysTick para 1 ms (inversa de 1000)
	SysTick_Config(SystemCoreClock / 1000);//por lo que divida aca, indica cada cuanto tiempo tendre una interrupcion

    while(1) {


    	// Informacion (channel, valor, etc) de la lectura de adc la guardo en adc_info
    	adc_result_info_t adc_info;
    	// Inicio conversion
    	ADC_DoSoftwareTriggerConvSeqA(ADC0);
    	// Espero a terminar la conversion
    	while(!ADC_GetChannelConversionResult(ADC0, ADC_POT_CH, &adc_info));//esto define el polling, espero constantemente en esta linea hasta realizar la conversion. una vez hecha la conversion el código sigue.

    	if (adc_info.result <= 2047){

    		// Verifico si el SysTick se disparo 200 veces
    		if(i200ms == 200) {
    			// Reinicio el contador
    			i200ms = 0;
    			// Conmuto el LED
    			GPIO_PinWrite(GPIO, 1, LED_BLUE, !GPIO_PinRead(GPIO, 1, LED_BLUE));
    		}
    	}


		if (adc_info.result >= 2047){ //adc_info.result tiene el valor de adc

			// Verifico si el SysTick se disparo 2000 veces
			if(i2000ms == 2000) {
				// Reinicio el contador
				i2000ms = 0;
				// Conmuto el LED
				GPIO_PinWrite(GPIO, 1, LED_BLUE, !GPIO_PinRead(GPIO, 1, LED_BLUE));
			}



    	}


		// Muestro el valor de adc
    	PRINTF(
			"El resultado del canal %ld en el PIO0_7 dio %d\n",
			adc_info.channelNumber, //adc_info.channelNumber tiene el valor de canal del adc usado
			adc_info.result); //adc_info.result tiene el valor de adc

    return 0;

}
}




void SysTick_Handler(void) {
	// Variable para contar interrupciones
	static uint16_t i200ms = 0;
	static uint16_t i2000ms = 0;

	// Incremento contador, sumando interrupciones, recordando que una interrupcion = 1mS
	i200ms++;
	i2000ms++;

}





