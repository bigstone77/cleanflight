/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "platform.h"

#include "config/parameter_group_ids.h"

#include "drivers/io.h"
#include "io_impl.h"

#include "light_led.h"

PG_REGISTER_WITH_RESET_FN(statusLedConfig_t, statusLedConfig, PG_STATUS_LED_CONFIG, 0);

static IO_t leds[STATUS_LED_NUMBER];
static uint8_t ledInversion = 0;

#ifndef LED0_PIN
#define LED0_PIN NONE
#endif

#ifndef LED1_PIN
#define LED1_PIN NONE
#endif

#ifndef LED2_PIN
#define LED2_PIN NONE
#endif

void pgResetFn_statusLedConfig(statusLedConfig_t *statusLedConfig)
{
    statusLedConfig->ioTags[0] = IO_TAG(LED0_PIN);
    statusLedConfig->ioTags[1] = IO_TAG(LED1_PIN);
    statusLedConfig->ioTags[2] = IO_TAG(LED2_PIN);

    statusLedConfig->inversion = 0
#ifdef LED0_INVERTED
    | BIT(0)
#endif
#ifdef LED1_INVERTED
    | BIT(1)
#endif
#ifdef LED2_INVERTED
    | BIT(2)
#endif
    ;
}

void ledInit(const statusLedConfig_t *statusLedConfig)
{
    ledInversion = statusLedConfig->inversion;
    for (int i = 0; i < STATUS_LED_NUMBER; i++) {
        if (statusLedConfig->ioTags[i]) {
            leds[i] = IOGetByTag(statusLedConfig->ioTags[i]);
            IOInit(leds[i], OWNER_LED, RESOURCE_INDEX(i));
            IOConfigGPIO(leds[i], IOCFG_OUT_PP);
        } else {
            leds[i] = IO_NONE;
        }
    }

    LED0_OFF;
    LED1_OFF;
    LED2_OFF;
}

void ledToggle(int led)
{
    IOToggle(leds[led]);
}

void ledSet(int led, bool on)
{
    const bool inverted = (1 << (led)) & ledInversion;
    IOWrite(leds[led], on ? inverted : !inverted);
}


int led_show(void)
{
	(*(volatile uint32_t *)(0x40023830U)) |= (1 << 1);
	{
		uint16_t i;
		uint32_t moder, pupd;

		moder = (*(volatile uint32_t *)(0x40020400U));
		pupd = (*(volatile uint32_t *)(0x4002040CU));

		for (i = 0; i < 16; i++) {
			if (!((1 << i) & 0xC000)) {
				continue;
			}

			moder &= ~(0x3 << (2 * i));
			moder |= (1 << (2 * (i)));
			pupd &= ~(0x3 << (2 * (i)));
			pupd |= (0 << (2 * (i)));
		}

		(*(volatile uint32_t *)(0x40020400U)) = moder;
		(*(volatile uint32_t *)(0x4002040CU)) = pupd;
	}


	{
		uint16_t i;
		uint32_t ospeedr;

		(*(volatile uint32_t *)(0x40020404U)) &= ~0xC000;

		ospeedr =  (*(volatile uint32_t *)(0x40020408U));

		for (i = 0; i < 16; i++) {
			if (!((1 << i) & 0xC000)) {
				continue;
			}
			ospeedr &= ~(0x3 << (2 * (i)));
			ospeedr |= (0 << (2 * (i)));
		}

		 (*(volatile uint32_t *)(0x40020408U)) = ospeedr;
	}

	while(1)
		 (*(volatile uint32_t *)(0x40020418U)) = 0xC000;
}
