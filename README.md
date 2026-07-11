Alternative firmware for LC-Tech ESP8266 relay board v2.3.

A precompiled firmware hex file is provided.

Building with build.sh requires SDCC.

Flashing requires a Nuvoton Nu-Link or a clone equivalent, and [nuvoprog](https://github.com/erincandescent/nuvoprog). Use nuvoprog along the lines of:
`nuvoprog program -t n76e003 -a ./firmware.hex -c @config.json
