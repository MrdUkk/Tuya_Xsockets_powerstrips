# About
This a replacement firmware for ZigBee Tuya-ecosystem based socket power strips thats harware is based on
Tuya ZT3L or ZTU models chips. Those modules contains TLSR8258 with 1M/512kB FLASH (Telink Semi TLSR8x series microcontrollers).


# Description
This is a PoC firmware written from Telink's sampleSwitch SDK example. I was starting from the
ground learning of ZigBee Telink Stack and TLSR8 microcontroller while digging available source code internals.
This process was very interesting and took me 3 days of work.
The documentation to SDK is rather poor and there are practically only few source code examples without description but with a bugs.
I provide currently only source files. no makefiles. Builded using Telink IoT Studio IDE with SDK V3.7.1.4.
Need to learn additionally how to properly setup new project in Telinks way without unnessary references.

My firmware exposes standard ZigBee ZCL clusters Basic/Identify/OnOff/Scenes/Groups.
Physical button on strip momentary push toggles on/off between all 5 sockets. and holding it pressed for five
seconds initialize ZigBee network pairing.
On Basic cluster command 0x0000 (reset_fact_default) supported resetting saved in flash configuration and restarting microcontroller.
Because i needed to support OTA firmware updates i need to set unique MANUFACTURER ID 
or update process can potentially interfere with already existed device on a market.
I can not allocate it via standard ZigBee Alliance way so decided to set it some random upper 16bit value.
Observed interested behavior in current consumption: original firmware draws 21mA in idle condition and
my firmware only 12mA.
when all five sockets are ON - current draw peaked at 340mA @ 5V.



# Supported models:
ts011f tz3000_gdyjfvgm

Any other model can be added easy by modifying board_8258.h file assigning proper GPIOs 
and modifying sampleSwitchEpCfg.c when number of sockets differs from 5 like in my device.

After firmware is flashed device will be ZED (ZigBee End Device) and seen as Manufacturer = Wholesome Software, and model = 5SockPowerStrip-1.
Leaving manufacturer and model id original is a no-way to go because Home Assistant/other will use Tuya ecosystem quirks.



# TODO:
- Implement 2 steps OTA firmware so anyone from Home Assistant can do upgrade from Tuya flawlessly
- Investigate Groups/Scenes ZCL clusters. Possibly it was not needed for power sockets
- Implement zcl static array structure sizes automatic calculation at compile time
- Investigate why globally allocated structures are needed by SDK, may be some RAM can be saved 
- Code polishing and bug hunting

