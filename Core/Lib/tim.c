#include "tim.h"

inline void tim_enable(TIM_TypeDef *tim)
{
	tim->CR1 |= TIM_CR1_CEN;
}

inline void tim_disable(TIM_TypeDef *tim)
{
	tim->CR1 = tim->CR1 & ~TIM_CR1_CEN_Msk;
}

void tim_init_timer(TIM_TypeDef *tim, unsigned short counter, unsigned char dir, unsigned short prescaler)
{
	tim_disable(tim);
	tim->ARR = counter-1;
	tim->PSC = prescaler-1;
	tim->CR1 = TIM_CR1_ARPE;
	if (dir) {
		tim->CR1 |= TIM_CR1_DIR;
	} else {
		tim->CR1 = (tim->CR1 & ~TIM_CR1_DIR_Msk);
	}

	tim->SMCR = TIM_SMCR_SMS_Pos;
	tim->EGR = TIM_EGR_UG_Pos;

	tim_enable(tim);
}

void tim_init_pwm_capture(TIM_TypeDef *tim, unsigned short counter)
{
	tim->ARR = counter-1;
	tim->CCMR1 = TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_1;
	tim->CCER = TIM_CCER_CC2P;
	tim->SMCR = TIM_SMCR_SMS_2 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0;
	tim->DIER |= TIM_DIER_CC1IE_Pos;
	tim->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
}