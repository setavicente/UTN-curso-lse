#include "board.h"
#include "fsl_sctimer.h"
#include "fsl_swm.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_swm.h"
#include "fsl_debug_console.h"
#include "fsl_adc.h"
#include "fsl_power.h"



// Frecuencia de PWM
#define PWM_FREQ	50
#define ADC_POT_CH		0

/**
 * @brief Programa principal
 */
int main(void) {

	// Variable para guardar el duty
	uint32_t duty;// duty debe ir de 5%-10%(dutycycle de 1ms a 2ms) para una freq PWM de 50 Hz(cuyo periodo es 20mS) para que el SC90 se mueva


	// Inicializacion de clock
	BOARD_BootClockFRO30M();//seteo en 30Mhz el FRO(free running oscillator, es el oscilador del sistema)(sin esto va por default a 18 Mhz)
    BOARD_InitDebugConsole();


    // Configuro la funcion de ADC en el canal del potenciometro
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);
    // Elijo clock desde el FRO con divisor de 1(prescaler) (como mencione antes 30MHz)
	CLOCK_Select(kADC_Clk_From_Fro);
	CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1);

	// Prendo el ADC
	POWER_DisablePD(kPDRUNCFG_PD_ADC0);

	// Obtengo frecuencia deseada y calibro ADC
	uint32_t frequency = CLOCK_GetFreq(kCLOCK_Fro) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
	ADC_DoSelfCalibration(ADC0, frequency);//me aseguro de que el adc labure con clk de 30 Mhz si o si
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







	// Conecto la salida 4 del SCT a un pin random de salida, al cual irá conectado el motor
    CLOCK_EnableClock(kCLOCK_Swm);// activo clk matriz de conmutacion
    SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT4, kSWM_PortPin_P1_0);
    CLOCK_DisableClock(kCLOCK_Swm);

    // Elijo el clock para el Timer
    uint32_t sctimer_clock = CLOCK_GetFreq(kCLOCK_Fro);
    // Configuracion del SCT Timer
    sctimer_config_t sctimer_config;
    SCTIMER_GetDefaultConfig(&sctimer_config);
    SCTIMER_Init(SCT0, &sctimer_config);

    // Configuro el PWM
    sctimer_pwm_signal_param_t pwm_config = {
		.output = kSCTIMER_Out_4,		// Salida del Timer
		.level = kSCTIMER_HighTrue,		// Logica positiva
		.dutyCyclePercent = duty		// duty: la variable que va a ir cambiando segun el valor de adc
    };

    // Variable para guardar el evento al que se asigna el PWM
    uint32_t event;
    // Inicializo el PWM
    SCTIMER_SetupPwm(
		SCT0,
		&pwm_config,
		kSCTIMER_CenterAlignedPwm,
		PWM_FREQ,						//freq PWM = 50Hz
		sctimer_clock,					//defino mi clk de base
		&event
	);
    // Inicializo el Timer
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);







    while (1) {




    	// Resultado de conversion adc
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



    	duty = (adc_info.result*5/4095)+5;// con esta linea hago el mapeo para que el valor de adc de 0-4095 quede "asignado" a un valor de duty del 5-10



    	// Verifico que este entre 0 y 100
    	if(duty < 100 && duty > 0){

    		// Actualizo el ancho de pulso
    		SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_4, duty, event);

    	}
    }
    return 0;
}
