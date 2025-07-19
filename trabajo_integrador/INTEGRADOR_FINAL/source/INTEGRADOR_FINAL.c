#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_sctimer.h"
#include "fsl_swm.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "fsl_i2c.h"/////<<-------- Hay que agregar esto al integradoe


// Pines
#define SEG_A GPIO, 0 , 11
#define SEG_B GPIO, 0 , 10
#define SEG_C GPIO, 0 , 6
#define SEG_D GPIO, 0 , 14
#define SEG_E GPIO, 0 , 0
#define SEG_F GPIO, 0 , 13
#define SEG_G GPIO, 0 , 15
#define DOT   GPIO, 0 , 1
#define A1    GPIO, 0 , 8
#define A2    GPIO, 0 , 9
/// hasta aca esta todo bien, por lo que que el problema no es de como se configuro el 7 segmentos.
#define USER  GPIO, 0 , 4
#define S1    GPIO, 0 , 16
#define S2    GPIO, 0 , 25
#define BUZZ  GPIO, 0 , 28// Cambiar si difiere<---------Estaba mal el pin del buzzer
#define IR_ON GPIO, 0 , 17
#define IR_READ GPIO, 0 , 19

#define BH_1750_ADDR 0x5c

// SETPOINTS
#define SETPOINT_MIN 27
#define SETPOINT_MAX 75

// ADC
#define ADC_POT_CH 0//////UNA RESPONDE A EL CANAL DE UN POTE

// PWM
#define PWM_FREQ 50///////ESTO DEBERIA SER PARA VARIAR LA INSTENSIDAD DEL LED AZUL ENTRE 0 y 100%

// Recursos FreeRTOS

SemaphoreHandle_t xMutex;
SemaphoreHandle_t xMutexPWM;
QueueHandle_t xQueueLux;

// Variables globales protegidas
uint32_t setpoint = 50;
uint32_t potDuty = 0;
uint8_t mostrarSetpoint = 0;

// PWM
uint32_t pwmEvent;
uint32_t sctimer_clock;

// Tabla segmentos (0=encendido con PNP)
const uint8_t tabla7Seg[10][7] = {
    {0,0,0,0,0,0,1}, {1,0,0,1,1,1,1}, {0,0,1,0,0,1,0},
    {0,0,0,0,1,1,0}, {1,0,0,1,1,0,0}, {0,1,0,0,1,0,0},
    {0,1,0,0,0,0,0}, {0,0,0,1,1,1,1}, {0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0}
};

void mostrarNumero(uint8_t num) {
    GPIO_PinWrite(SEG_A, tabla7Seg[num][0]);
    GPIO_PinWrite(SEG_B, tabla7Seg[num][1]);
    GPIO_PinWrite(SEG_C, tabla7Seg[num][2]);
    GPIO_PinWrite(SEG_D, tabla7Seg[num][3]);
    GPIO_PinWrite(SEG_E, tabla7Seg[num][4]);
    GPIO_PinWrite(SEG_F, tabla7Seg[num][5]);
    GPIO_PinWrite(SEG_G, tabla7Seg[num][6]);
}

///INICIALIZACION DE PERIFERICOS Y PINES
void Task_InitPerifericos(void *pvParameters) {
    BOARD_BootClockFRO30M();
    BOARD_InitDebugConsole();
    GPIO_PortInit(GPIO, 0);
////DIreccion de los pines
    gpio_pin_config_t out_config = {.outputLogic = 1, .pinDirection = kGPIO_DigitalOutput};
    gpio_pin_config_t in_config = {.pinDirection = kGPIO_DigitalInput};
///SENSOR DE IR
    GPIO_PinInit(IR_ON,&out_config);
    GPIO_PinInit(IR_READ,&in_config);
    /////   7 SEGMENTOS
    GPIO_PinInit(SEG_A, &out_config); GPIO_PinInit(SEG_B, &out_config);
    GPIO_PinInit(SEG_C, &out_config); GPIO_PinInit(SEG_D, &out_config);
    GPIO_PinInit(SEG_E, &out_config); GPIO_PinInit(SEG_F, &out_config);
    GPIO_PinInit(SEG_G, &out_config); GPIO_PinInit(DOT, &out_config);
    GPIO_PinInit(A1, &out_config); GPIO_PinInit(A2, &out_config);
///BOTONES
    GPIO_PinInit(USER, &in_config);
    GPIO_PinInit(S1, &in_config);
    GPIO_PinInit(S2, &in_config);
////BUZZER
    GPIO_PinInit(BUZZ, &out_config);
////ADC
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);
    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1);
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    uint32_t freq = CLOCK_GetFreq(kCLOCK_Fro) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
    ADC_DoSelfCalibration(ADC0, freq);
    adc_config_t adc_config;
    ADC_GetDefaultConfig(&adc_config);
    ADC_Init(ADC0, &adc_config);
    adc_conv_seq_config_t adc_seq = {
        .channelMask = (1 << ADC_POT_CH),/////<<<<-------| (1 << ADC_IR_CH)
        .triggerMask = 0, .triggerPolarity = kADC_TriggerPolarityPositiveEdge,
        .enableSyncBypass = false, .interruptMode = kADC_InterruptForEachConversion
    };
    ADC_SetConvSeqAConfig(ADC0, &adc_seq);
    ADC_EnableConvSeqA(ADC0, true);
///PWM, debe estar mal el pin azul
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT4, kSWM_PortPin_P1_0);
    CLOCK_DisableClock(kCLOCK_Swm);
    sctimer_config_t sctimer_config;
    SCTIMER_GetDefaultConfig(&sctimer_config);
    SCTIMER_Init(SCT0, &sctimer_config);
    sctimer_pwm_signal_param_t pwm_config = {
        .output = kSCTIMER_Out_4, .level = kSCTIMER_HighTrue, .dutyCyclePercent = potDuty
    };
    sctimer_clock = CLOCK_GetFreq(kCLOCK_Fro);
    SCTIMER_SetupPwm(SCT0, &pwm_config, kSCTIMER_CenterAlignedPwm, PWM_FREQ, sctimer_clock, &pwmEvent);
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);
///I2C del sensor de luz
    CLOCK_Select(kI2C1_Clk_From_MainClk);
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_27);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_26);
    CLOCK_DisableClock(kCLOCK_Swm);
    // Configuracion de master para el I2C con 400 KHz de clock
    i2c_master_config_t config;
    I2C_MasterGetDefaultConfig(&config);
    config.baudRate_Bps = 400000;
    // Usa el clock del sistema de base para generar el de la comunicacion
    I2C_MasterInit(I2C1, &config, SystemCoreClock);


    ///ESTO ES PARA EL PWM DE LOS 2 ROJO Y AZUL DEL TRICOLOR, ES EL PWM
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT5, kSWM_PortPin_P1_1); // Azul
    SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT6, kSWM_PortPin_P1_2); // Rojo
    CLOCK_DisableClock(kCLOCK_Swm);
    sctimer_pwm_signal_param_t pwm_azul = {
        .output = kSCTIMER_Out_5,
        .level = kSCTIMER_HighTrue,
        .dutyCyclePercent = 0
    };
    sctimer_pwm_signal_param_t pwm_rojo = {
        .output = kSCTIMER_Out_6,
        .level = kSCTIMER_HighTrue,
        .dutyCyclePercent = 0
    };
    SCTIMER_SetupPwm(SCT0, &pwm_azul, kSCTIMER_CenterAlignedPwm, PWM_FREQ, sctimer_clock, NULL);
    SCTIMER_SetupPwm(SCT0, &pwm_rojo, kSCTIMER_CenterAlignedPwm, PWM_FREQ, sctimer_clock, NULL);

    vTaskDelete(NULL);
}





//////Esta supuestamente ya esta corregido
void Task_ADC(void *pvParameters) {
    adc_result_info_t adc_info;
    uint16_t adc_pot = 0;
    while(1) {
        ////Esto es para variar el LED AZUL
        ADC_DoSoftwareTriggerConvSeqA(ADC0);
        while(!ADC_GetChannelConversionResult(ADC0, ADC_POT_CH, &adc_info));
        adc_pot = adc_info.result;
        potDuty = (adc_pot * 100) / 4095;
        SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_4, potDuty, pwmEvent);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void Task_Botones(void *pvParameters) {
    while(1) {
        if (GPIO_PinRead(S1)==0) { xSemaphoreTake(xMutex, portMAX_DELAY); if (setpoint<SETPOINT_MAX) setpoint++; xSemaphoreGive(xMutex); }
        if (GPIO_PinRead(S2)==0) { xSemaphoreTake(xMutex, portMAX_DELAY); if (setpoint>SETPOINT_MIN) setpoint--; xSemaphoreGive(xMutex); }
        if (GPIO_PinRead(USER)==0) { xSemaphoreTake(xMutex, portMAX_DELAY); mostrarSetpoint=!mostrarSetpoint; xSemaphoreGive(xMutex);}
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Task_Display(void *pvParameters) {
    uint32_t valDisplay;
    while(1) {
        if (mostrarSetpoint) {
            xSemaphoreTake(xMutex, portMAX_DELAY);
            valDisplay = setpoint;
            xSemaphoreGive(xMutex);
            GPIO_PinWrite(DOT, 0);
        } else {
            xQueuePeek(xQueueLux, &valDisplay, 0);
            GPIO_PinWrite(DOT, 1);
        }
        if (valDisplay > 99) valDisplay = 99;

        uint8_t decenas = valDisplay / 10;
        uint8_t unidades = valDisplay % 10;

        GPIO_PinWrite(A1,0); GPIO_PinWrite(A2,1);
        mostrarNumero(decenas);
        vTaskDelay(pdMS_TO_TICKS(5));

        GPIO_PinWrite(A1,1); GPIO_PinWrite(A2,0);
        mostrarNumero(unidades);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void Task_Lux(void *pvParameters) {
    // Inicialización I2C para BH1750
    CLOCK_Select(kI2C1_Clk_From_MainClk);
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_27);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_26);
    CLOCK_DisableClock(kCLOCK_Swm);

    i2c_master_config_t config;
    I2C_MasterGetDefaultConfig(&config);
    config.baudRate_Bps = 400000;

    // Encender sensor (power on y modo continuo)
    if (I2C_MasterStart(I2C1, BH_1750_ADDR, kI2C_Write) == kStatus_Success) {
        uint8_t cmd = 0x01;  // Power on
        I2C_MasterWriteBlocking(I2C1, &cmd, 1, kI2C_TransferDefaultFlag);
        I2C_MasterStop(I2C1);
    }

    vTaskDelay(pdMS_TO_TICKS(10));  // Delay entre comandos

    if (I2C_MasterStart(I2C1, BH_1750_ADDR, kI2C_Write) == kStatus_Success) {
        uint8_t cmd = 0x10;  // Continuous H-Resolution Mode
        I2C_MasterWriteBlocking(I2C1, &cmd, 1, kI2C_TransferDefaultFlag);
        I2C_MasterStop(I2C1);
    }

    uint8_t res[2] = {0};
    uint16_t lux = 0;

    while (1) {
        if (I2C_MasterStart(I2C1, BH_1750_ADDR, kI2C_Read) == kStatus_Success) {
            I2C_MasterReadBlocking(I2C1, res, 2, kI2C_TransferDefaultFlag);
            I2C_MasterStop(I2C1);

            lux = ((res[0] << 8) | res[1]) / 1.2f;
            uint32_t lux_percent = (lux * 100) / 30000;
            if (lux_percent > 100) lux_percent = 100;
            // Enviar lux a la cola
            xQueueSend(xQueueLux, &lux_percent, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void Task_ControlLED_Blue(void *pvParameters) {
    uint32_t lux_percent = 0;
    uint32_t dutyRed = 0;
    uint32_t dutyBlue = 0;

    while (1) {
        // Obtenemos la última lectura de luz
        if (xQueuePeek(xQueueLux, &lux_percent, 0) == pdPASS) {
            xSemaphoreTake(xMutex, portMAX_DELAY);
            int32_t delta = (int32_t)setpoint - (int32_t)lux_percent;////<<---- Si no anda agregar absolute.
            xSemaphoreGive(xMutex);

            if (delta > 0) {
                // Está por debajo → LED AZUL más brillante
                dutyBlue= delta * 100/75 ;
                dutyRed = 0;
                // Actualizar los PWM
                xSemaphoreTake(xMutexPWM, portMAX_DELAY);
                SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_5, dutyBlue, pwmEvent);  // LED azul
                SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_6, dutyRed, pwmEvent);   // LED rojo
                xSemaphoreGive(xMutexPWM);

            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


void Task_ControlLED_Red(void *pvParameters) {
    uint32_t lux_percent = 0;
    uint32_t dutyRed = 0;
    uint32_t dutyBlue = 0;

    while (1) {
        // Obtenemos la última lectura de luz
        if (xQueuePeek(xQueueLux, &lux_percent, 0) == pdPASS) {
            xSemaphoreTake(xMutex, portMAX_DELAY);
            int32_t delta2 = (int32_t)lux_percent - (int32_t)setpoint;////<<---- Si no anda agregar absolute.
            xSemaphoreGive(xMutex);


            if (delta2 > 0) {
                // Está por debajo → LED AZUL más brillante
                dutyBlue= 0;
                dutyRed = delta2 * 100/ 75 ;

                // Actualizar los PWM
                xSemaphoreTake(xMutexPWM, portMAX_DELAY);
                SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_5, dutyBlue, pwmEvent);  // LED azul
                SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_6, dutyRed, pwmEvent);   // LED rojo
                xSemaphoreGive(xMutexPWM);

            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Task_IR(void *pvParameters){
    GPIO_PinWrite(IR_ON, 1);
    while(1){
    uint8_t IR_VALUE = GPIO_PinRead(IR_READ);
    if(IR_VALUE==1)
    {
    GPIO_PinWrite(BUZZ,1);
    }
    else
    {
    GPIO_PinWrite(BUZZ,0);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
}


// ========== Main ==========


int main(void) {
    xMutex = xSemaphoreCreateMutex();
    xMutexPWM = xSemaphoreCreateMutex();
    xQueueLux = xQueueCreate(5, sizeof(uint32_t));
    xTaskCreate(Task_InitPerifericos, "PIN", 100, NULL, 3, NULL);
    xTaskCreate(Task_IR, "IR-BUZZ", 100, NULL, 1, NULL);
    xTaskCreate(Task_ADC, "ADC", 100, NULL, 1, NULL);
    xTaskCreate(Task_Botones, "BOT", 100, NULL, 1, NULL);
    xTaskCreate(Task_Display, "DISP", 100, NULL, 1, NULL);
    xTaskCreate(Task_ControlLED_Blue, "BLUE", 100, NULL, 1, NULL);
    xTaskCreate(Task_ControlLED_Red, "RED", 100, NULL, 1, NULL);
    vTaskStartScheduler();
    for(;;);
}
