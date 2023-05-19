/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
/**
 * @file main.c
 * @brief Este projeto demonstra o uso de um buffer circular para compatibilizar
 * a velocidade de dois dispositivos. Ao digitar uma das seguintes palavras em maiusculas 		
 * VERDE, VINHO, VIOLETA, VERMELHO, VIRIDIANO, são "ecoadas" a palavra digitada 10x no Terminal 
 * @author Beatriz Moura 
 * @author Rafael Siqueira
 * @date 19/05/2023
 *
 */
#include "stdlib.h"
#include "string.h"
//#include "UART.h"
#include "ISR.h"
#include "SIM.h"
//#include "buffer_circular.h"
#include "util.h"
#include "derivative.h" /* include peripheral declarations */

/*!
 * @brief Programa que identifica o bit de paridade
 * para números binários, hexadecimais e decimais
 */
int main(void)
{		
	/*
	 * Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);
	
	/*
	 * Configura fonte de pulsos para contadores de TPMs
	 */
	SIM_setaTPMSRC (0b01);
	
	/*
	 * Habilitar sinais de relogio e alocar os pinos
	 * Inicializar os pinos de monitoramento via analisador logico
	 */
	TPM0TPM1_PTA4PTE21_config_basica ();
	GPIO_H5Pin2_config_basica ();
	 
	/*
	 *  Configurar TPM0 e TPM1
	 */
	TPM_config_especifica (0, 65535, 0b1111, 0, 0, 0, 0, 0, 0b111);  // (65535*128)/20971520 = 0.3999
	TPM_config_especifica (1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b111);  // (65535*128)/20971520 = 0.3999

	TPM_CH_config_especifica (0, 1, 0b0010, 0);                //IC na borda de descida
	TPM_CH_config_especifica (0, 2, 0b0110, 0);				   //OC reseta saida em 0 ao igualar
	
	TPM_CH_config_especifica (1, 1, 0b0110, 0);				   //OC reseta saida em 0 ao igualar
	
	/*
	 * Configurar o pino H5Pin2
	 */
	GPIO_H5Pin2_config_especifica();
	
	/*
	 * Habilitar interrupcoes NVIC
	 */
	TPM_habilitaNVICIRQ(17, 1);
	
	/*
	 * Habilitar interrupcoes dos canais
	 */
	TPM_habilitaInterrupCH(0, 1);
	
	ISR_escreveEstado(INICIO);
	for(;;) {	   
		switch(ISR_LeEstado()){
			case INICIO:
				ISR_escreveEstado(PREPARA_AUDITIVO);	
				GPIO_reativaSwitchesNVICInterrupt ();
				break;
			case PREPARA_AUDITIVO:		
				GPIO_desativaSwitchesNVICInterrupt ();
				GPIO_ativaConLCD();
				GPIO_initLCD();
				GPIO_escreveStringLCD(, "Teste Auditivo\0");
				ISR_escreveEstado(ESPERA_ESTIMULO_AUDITIVO);	
				break;
			case ESPERA_ESTIMULO_AUDITIVO:
				break;
			case ESPERA_REACAO_AUDITIVA:
				ISR_escreveEstado(RESULTADO);	
				GPIO_reativaSwitchesNVICInterrupt ();
				break;
			case RESULTADO:
				//ISR_escreveEstado(RESULTADO);	
				break;
			case LEITURA: 
				GPIO_desativaSwitchesNVICInterrupt ();
				ISR_escreveEstado(LEITURA);	
				break;
			case PREPARA_INICIO: 
				ISR_escreveEstado(INICIO);//talvez este estado seja definido por interrupção	
				break;
		}
	}
	return 0;
}
