# Chip8-Emulator

A Chip8 emulator written for experience purpose

## How to use
- Download the executable from [Release](https://github.com/letiendat198/Chip8-Emulator/releases/latest)
- Place the rom you want to load as **rom.ch8** in the same folder as the executable

## Keyboard Mapping
<pre>
Keypad                   Keyboard
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+
</pre>

## Known Issues
- Disp.wait test not passing (So things will load too fast instead of supposed 60Hz)
- No sound
- Some games will break or behave weirdly

## TODO
- Fix Disp.wait
- Add sound
- Add support for SCHIP and XO-CHIP