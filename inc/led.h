#ifndef KEPLER_LED_H
#define KEPLER_LED_H

inline void
led_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->BSRR = GPIO_Pin;
}

inline void
led_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->BSRR = (uint32_t) GPIO_Pin << 16;
}

inline void
led_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->BSRR = (uint32_t) GPIO_Pin << (16 * ((GPIOx->ODR & GPIO_Pin) == GPIO_Pin));
}


#endif
