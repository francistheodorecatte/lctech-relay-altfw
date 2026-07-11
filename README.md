Alternative firmware for LC-Tech ESP8266 relay board v2.3.

A precompiled firmware hex file is provided.

Building with build.sh requires SDCC.

Flashing requires a Nuvoton Nu-Link or a clone equivalent, and [nuvoprog](https://github.com/erincandescent/nuvoprog).

Dump stock firmware bin (if not locked; locked chips require a [workaround](https://github.com/erincandescent/nuvoprog/issues/3)):

`nuvoprog read -t n76e003 stock_firmware.hex -v`


Then read the device config back from the dumped stock firmware:

`nuvoprog config decode -t n76e003 -i stock_firmware.hex`


And finally flash the custom firmware:

`nuvoprog program -t n76e003 -a ./firmware.hex -c @config.json`


An example ESPHome config for the ESP-01 will be coming at some point... soon?
