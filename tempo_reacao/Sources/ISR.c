/*
 * ISR.c
 *
 */
#include "derivative.h"
//#include "buffer_circular.h"
#include "ISR.h"
#include "stdlib.h"
#define TAM_MAX 100
static uint8_t flag;
static BufferCirc_type bufferE;				//buffer de entrada
static BufferCirc_type bufferS;	//buffer de saida Terminal 

void PORTA_IRQHandler () {
	//Reativa PIT 	
	//GPIOE_PTOR |= GPIO_PTOR_PTTO (GPIO_PIN(20));

	switch (estado) {
		case ESPERA_ESTIMULO_AUDITIVO:
			
			/*pode-se habilitar a interrupção TOF do canal do buzzer (TPM1_CH1) com
			TPM1_C1V==0 (sem som) para contar um tempo (m*PeríodoTPM1+ resíduo). O resíduo
			corresponde ao intervalo de tempo que o contador precisa para gerar o primeiro evento de
			estouro.*/
			
			
			ISR_escreveEstado(ESPERA_REACAO_AUDITIVA);
			break;
		case RESULTADO:
			/*if (PORTA_ISFR & PORT_ISFR_ISF(GPIO_PIN(5))) {
				hor[1]++;
				if (hor[1] >= 60) hor[1] = 0;
				estado = INCREMENTE_MINUTO;
				
				//Limpar o estado de solicitacao
				PORTA_ISFR |= PORT_ISFR_ISF(GPIO_PIN(5)); 
			} */
			
			ISR_escreveEstado(LEITURA);
			break;
		default:
			return;
	}
	
}
void FTM0_IRQHandler () {
	static uint16_t counter;
	static uint8_t vezes=0;
	uint16_t valor1, valor2;

	if (TPM0_STATUS & TPM_STATUS_CH1F_MASK) {
		valor1 = TPM0_C1V;
		valor2 = TPM1_CNT + 5; 
		if (vezes == 0) {
			TPM_CH_config_especifica (1, 1, 0b0111, valor2); // set o canal TPM1_CH1 (H5Pin3, led R) com contagem em TPM1_CNT+5
			GPIOE_PSOR = (1 << 22);					// ativa o led G
			/*!
			 * ativar interrupcao do TPM0_CH2
			 */
			TPM0_C2SC |= TPM_CnSC_CHF_MASK; 	    // baixar bandeira (w1c)
			TPM0_C2SC |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do TPM0_CH2
			TPM0_C2V = TPM_CnV_VAL(valor1);   		// seta o valor de match em TPM0_CH2
			counter = 0;							//  contador de ciclos intervalo
			vezes++;
		} else if (vezes == 1) {
			/*!
			 * desativa interrupcao do TPM0_CH2
			 */
			TPM0_C2SC &= ~TPM_CnSC_CHIE_MASK; 	    // desabilitar a interrupcao do TPM0_C2
			vezes = 0;
			GPIOE_PCOR = (1 << 22);			    	// apaga o led G
			TPM_CH_config_especifica (1, 1, 0b0110, valor2); //clear o canal TPM1_CH1 (H5Pin3, led R) com contagem em TPM1_CNT+5
		}

		TPM0_C1SC |= TPM_CnSC_CHF_MASK;     	// limpar solicitacao da chave     
	} else if (TPM0_STATUS & TPM_STATUS_CH2F_MASK) {
		counter++;								// incrementar counter
		GPIOE_PTOR = (1 << 22);			    	// alternar o sinal indicador (led G) de periodo do TPM0
		TPM0_C2SC |= TPM_CnSC_CHF_MASK;     	// limpar solicitacao de Output Compare    
	} 
}

void ISR_inicializaBC () {
	/*!
	 * Inicializa um buffer circular de entrada
	 */
	BC_init (&bufferE, TAM_MAX);

	/*!
	 * Inicializa o buffer circular de saida
	 */
	BC_init(&bufferS, TAM_MAX);
}
uint8_t ISR_BufferSaidaVazio(){
	return BC_isEmpty(&bufferS);
}

void ISR_EnviaString (char *string) {
	uint8_t i;
	
	while (BC_push( &bufferS, string[0])==-1);
	UART0_C2 |= UART0_C2_TIE_MASK;
	i=1;
	while (string[i] != '\0') {
		while (BC_push( &bufferS, string[i])==-1);
		i++;
	}
}

void ISR_extraiString (char *string) {
	//Entrada de uma nova string
	uint8_t i=0;
	//string = malloc(TAM_MAX);
	BC_pop (&bufferE, string);
	while (string[i] != '\0') {
		BC_pop (&bufferE, &(string[++i]));				
	}
}
tipo_estado ISR_LeEstado () {
	return flag;
}
void ISR_escreveEstado (uint8_t novo) {
	flag = novo;
	return;
}
