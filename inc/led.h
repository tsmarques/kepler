#ifndef KEPLER_LED_H
#define KEPLER_LED_H

#define LED_PORT(LED_COLOR) LED_##LED_COLOR##_GPIO_Port
#define LED_PIN(LED_COLOR) LED_##LED_COLOR##_Pin

//! Macro to switch led on based on colour name
#define led_on(LED_COLOR) LED_PORT(LED_COLOR)->BSRR = LED_PIN(LED_COLOR)

//! Macro to switch led off based on colour name
#define led_off(LED_COLOR) LED_PORT(LED_COLOR)->BSRR = LED_PIN(LED_COLOR) << 16

//! Macro to toggle a led based on colour name
#define led_toggle(LED_COLOR) {\
    LED_PORT(LED_COLOR)->BSRR = (uint32_t)LED_PIN(LED_COLOR) \
    << (16 * ((LED_PORT(LED_COLOR)->ODR & LED_PIN(LED_COLOR)) == LED_PIN(LED_COLOR)));\
}


#endif
