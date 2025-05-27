#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_swm.h"
#include "fsl_power.h"
#include "fsl_adc.h"

// Canal de ADC para el potenciometro
#define ADC_POT_CH		0

/*
 * @brief   Application entry point.
 */
int main(void) {

	// Inicializacion de clock
	BOARD_BootClockFRO30M();//seteo en 30Mhz el FRO(free running oscillator, es el oscilador del sistema)(sin esto va por default a 18 Mhz)
    BOARD_InitDebugConsole();

    // Activo clock de matriz de conmutacion
    CLOCK_EnableClock(kCLOCK_Swm);
    // Configuro la funcion de ADC en el canal del potenciometro
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);
    // Desactivo clock de matriz de conmutacion (esto es innecesario)
    CLOCK_DisableClock(kCLOCK_Swm);

    // Elijo clock desde el FRO con divisor de 1(prescaler) (como mencione antes 30MHz)
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

    while(1) {
    	// Resultado de conversion
    	adc_result_info_t adc_info;
    	// Inicio conversion
    	ADC_DoSoftwareTriggerConvSeqA(ADC0);
    	// Espero a terminar la conversion
    	while(!ADC_GetChannelConversionResult(ADC0, ADC_POT_CH, &adc_info));//esto define el polling, espero constantemente en esta linea hasta realizar la conversion. una vez hecha la conversion el código sigue.
    	// Muestro resultado
    	PRINTF(
			"El resultado del canal %ld en el PIO0_7 dio %d\n",
			adc_info.channelNumber,
			adc_info.result //funcion para exponer el resultado de la conversion
    	);
    }
    return 0;
}
