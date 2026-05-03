/** library for the Velleman VMA209 extension board which covers all standard io components + an interface for communicating over usb
 * Author: Bauwen De ROn + KdG + Bronnen KdG
 * The library is equiped with wioth debugging features, to enable those enable __VMA209_LIB_DEBUG__ in the precompiler
*/

#ifndef _VMA209_GLOBAL_H
#define _VMA209_GLABAL_H
#include "adc.h"
#include "button.h"
#include "buzzer.h"
#include "display.h"
#include "led.h"
#include "usart.h"

#define VMA209_LIB_ERROR 0
#define VMA209_LIB_SUCCES 1

unsigned char initVma209();

#endif
