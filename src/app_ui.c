/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
const u32 relays[5] = { RELAY1, RELAY2, RELAY3, RELAY4, RELAY5 };

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

void cmdOnOff_off(u8 ep) {
	zcl_onOffAttr_t *pOnOff = &g_zcl_onOffAttrs[ep-1];
	pOnOff->onOff = false;
	drv_gpio_write(relays[ep-1], FALSE);
}

void cmdOnOff_on(u8 ep) {
	zcl_onOffAttr_t *pOnOff = &g_zcl_onOffAttrs[ep-1];
	pOnOff->onOff = true;
	drv_gpio_write(relays[ep-1], TRUE);
}

void cmdOnOff_toggle(u8 ep) {
	zcl_onOffAttr_t *pOnOff = &g_zcl_onOffAttrs[ep-1];
	pOnOff->onOff = !pOnOff->onOff;
	drv_gpio_write(relays[ep-1], pOnOff->onOff ? TRUE : FALSE);
}

void led_on()
{
    drv_gpio_write(LED_POWER, TRUE);
}

void led_off()
{
    drv_gpio_write(LED_POWER, FALSE);
}


s32 zclLightTimerCb(void *arg)
{
    u32 interval = 0;

    if (g_switchAppCtx.sta == g_switchAppCtx.oriSta) {
        g_switchAppCtx.times--;
        if (g_switchAppCtx.times <= 0) {
            g_switchAppCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_switchAppCtx.sta = !g_switchAppCtx.sta;
    if (g_switchAppCtx.sta) {
    	led_on();
        interval = g_switchAppCtx.ledOnTime;
    } else {
    	led_off();
        interval = g_switchAppCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    g_switchAppCtx.times = times;

    if (!g_switchAppCtx.timerLedEvt) {
        if (g_switchAppCtx.oriSta) {
        	led_off();
            g_switchAppCtx.sta = 0;
            interval = ledOffTime;
        } else {
        	led_on();
            g_switchAppCtx.sta = 1;
            interval = ledOnTime;
        }

        g_switchAppCtx.ledOnTime = ledOnTime;
        g_switchAppCtx.ledOffTime = ledOffTime;
        g_switchAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if (g_switchAppCtx.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&g_switchAppCtx.timerLedEvt);

        g_switchAppCtx.times = 0;
        if (g_switchAppCtx.oriSta) {
        	led_on();
        } else {
        	led_off();
        }
    }
}


/*******************************************************************
 * @brief	Button click detect:
 * 			SW1. keep press button1 5s === factory reset
 * 			SW1. short press button1   === send level step with OnOff command (Up)
 *
 */
void buttonKeepPressed(u8 btNum)
{
    if (btNum == VK_SW1) {
        g_switchAppCtx.state = APP_FACTORY_NEW_DOING;
        g_switchAppCtx.ctn = 0;
        zb_factoryReset();
        light_blink_start(90, 250, 750);
    }
}

void buttonShortPressed(u8 btNum)
{
    if (btNum == VK_SW1) {
        cmdOnOff_toggle(SAMPLE_SWITCH_ENDPOINT);
    }
}

void keyScan_keyPressedCB(kb_data_t *kbEvt)
{
    u8 keyCode = kbEvt->keycode[0];

    if (keyCode == VK_SW1) {
        g_switchAppCtx.keyPressedTime = clock_time();
        g_switchAppCtx.state = APP_FACTORY_NEW_SET_CHECK;
        g_switchAppCtx.ctn++;
        light_blink_start(1, 30, 1);
    }
}

void keyScan_keyReleasedCB(u8 keyCode)
{
    if (keyCode != 0xff) {
    	g_switchAppCtx.keyReleasedTime = clock_time();
    	g_switchAppCtx.state = APP_STATE_RELEASE;
    }
}

void app_key_handler(void)
{
    static u8 valid_keyCode = 0xff;

    if (g_switchAppCtx.state == APP_FACTORY_NEW_SET_CHECK) {
        if (clock_time_exceed(g_switchAppCtx.keyPressedTime, 5 * 1000 * 1000)) {
            buttonKeepPressed(VK_SW1);
        }
    }

    if (g_switchAppCtx.state == APP_STATE_RELEASE) {
        if (clock_time_exceed(g_switchAppCtx.keyReleasedTime, 100 * 1000)) {
        	g_switchAppCtx.state = APP_STATE_NORMAL;
            if (g_switchAppCtx.ctn == 1) {
            	buttonShortPressed(VK_SW1);
            }
            g_switchAppCtx.ctn = 0;
        }
    }

    if (kb_scan_key(0, 1)) {
        if (kb_event.cnt) {
            g_switchAppCtx.keyPressed = 1;
            keyScan_keyPressedCB(&kb_event);
            if (kb_event.cnt == 1) {
                valid_keyCode = kb_event.keycode[0];
            }
        } else {
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_switchAppCtx.keyPressed = 0;
        }
    }
}

#endif  /* __PROJECT_TL_SWITCH__ */
