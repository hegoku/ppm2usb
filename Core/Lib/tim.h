#ifndef TIM_H
#define TIM_H

#include "stm32f103x6.h"

void tim_enable(TIM_TypeDef *tim);
void tim_disable(TIM_TypeDef *tim);

void tim_init_timer(TIM_TypeDef *tim, unsigned short counter, unsigned char dir, unsigned short prescaler);

#endif