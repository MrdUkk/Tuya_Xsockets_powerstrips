/********************************************************************************************************
 * @file    app_ui.h
 *
 * @brief   This is the header file for app_ui
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
#ifndef _APP_UI_H_
#define _APP_UI_H_

/**********************************************************************
 * CONSTANT
 */


/**********************************************************************
 * TYPEDEFS
 */
enum {
    APP_STATE_NORMAL,
	APP_STATE_RELEASE,
    APP_FACTORY_NEW_SET_CHECK,
    APP_FACTORY_NEW_DOING
};


/**********************************************************************
 * FUNCTIONS
 */
void cmdOnOff_off(u8 ep);
void cmdOnOff_on(u8 ep);
void cmdOnOff_toggle(u8 ep);

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime);
void light_blink_stop(void);

void led_on();
void led_off();

void app_key_handler(void);

#endif	/* _APP_UI_H_ */
