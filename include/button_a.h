/**
 * @file    button_a.h
 * @author  Ricardo e Vagner
 * @brief   Task para o botão A
 * @version 0.1
 * @date    2025-06-25
 */
#ifndef BUTTON_A_H
#define BUTTON_A_H


/**
 * @brief Task do botão A
 * Inicializa GPIO do botão
 * Monitora transições de estado do,botão :
 * - quando pressionado:
 *   - envia mensagem que foi pressionado para o display via pilha;
 *   - aguarda o LED estar disponível(MUTEX) para acessá-lo e manda uma nova mensagem para o display via pilha;
 * - quando solto:
 *   - libera o acesso ao LED(MUTEX);
 *   - Manda uma mensagem para o display via pilha
 */
void button_a_task();


#endif // BUTTON_A_H