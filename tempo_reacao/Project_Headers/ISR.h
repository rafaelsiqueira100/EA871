/*!
 * @file ISR.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo ISR.c
 * @author Wu Shin Ting
 * @date 05/03/2022
 */

#ifndef ISR_H_
#define ISR_H_

typedef enum estado_tag {
	INICIO,
	PREPARA_AUDITIVO,
	ESPERA_ESTIMULO_AUDITIVO,
	ESPERA_REACAO_AUDITIVA,
	RESULTADO,
	LEITURA,
	PREPARA_INICIO
} tipo_estado;

/*
/*!
 * @brief Le o estado do aplicativo
 *
tipo_estado ISR_LeEstado ();

/*!
 * @brief Atualiza o estado do aplicativo
 * @param[in] novo estado
 *
void ISR_escreveEstado (tipo_estado novo);

/*!
 * @brief Inicializa buffers circulares para o aplicativo
 *
void ISR_inicializaBC ();

/*!
 * @brief Extrai do buffer de entrada uma linha de caracteres.
 * @param[out] string contendo a linha terminada com '\0'.
 *
void ISR_extraiString (char *string);

/*!
 * @brief Envia 10 vezes uma string para Terminal. A forma de onda eh espelhada no pino 2 do header H5
 * @param[in] string a ser enviado para Terminal via buffer circular
 /
void ISR_EnviaString10x (char *string);

/*!
 * @brief Envia uma string para Terminal. A forma de onda eh espelhada no pino 2 do header H5
 * @param[in] string a ser enviado para Terminal via buffer circular
 /
void ISR_EnviaString (char *string);

/*!
 * @brief Envia caracteres de controle para realinhamento das linhas no Terminal
 /
void ISR_Realinhamento();

/*!
 * @brief Verificar se o buffer de saida esta vazio
 * @return estado vazio do buffer
 /
uint8_t ISR_BufferSaidaVazio ();
*/
/*!
 * @brief Envia uma string para Terminal. A forma de onda eh espelhada no pino 2 do header H5
 * @param[in] string a ser enviado para Terminal via buffer circular
 */
void ISR_EnviaString (char *string);
/*!
 * @brief Extrai do buffer de entrada uma linha de caracteres.
 * @param[out] string contendo a linha terminada com '\0'.
 */
void ISR_extraiString (char *string);
void PORTA_IRQHandler ();
/*!
 * @brief Le o estado do aplicativo
 */
tipo_estado ISR_LeEstado();
/*!
 * @brief Atualiza o estado do aplicativo
 * @param[in] novo estado
 */
void ISR_escreveEstado(tipo_estado estado);
/*!
 * @brief Verifica se o buffer de saída está vazio
 * @param[out] Retorna 1 se o Buffer de saída está vazio, 0 caso contrário
 */

uint8_t ISR_BufferSaidaVazio();

#endif /* ISR_H_ */
