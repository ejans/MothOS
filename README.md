# Important Notes:
* ESP32 S3 with 512kb RAM and 4MB Flash Required if building DIY MothSynths
* When compiling sketch, select PARTITION SCHEME / HUGE APP from Arduino IDE tools menu, otherwise the app wont fit on your esp32

# Arduino:
* Get arduino appimage from here: https://www.arduino.cc/en/software
* In download location make file executable:
>$ chmod +x arduino-ide\*.AppImage
* Install python3-serial:
>$ sudo apt install python3-serial
* Run arduino:
>$ ./arduino-ide_2.3.4_Linux_64bit.AppImage
* Open MothOS.ino inside repo
* Select board: ESP32S3 Dev Module
* Install esp32 core (Popup on right lower corner)
* Install required libraries: Tools => Manage Libraries
    * Search for Keypad, U8g2, and install
* Enable USB CDC (Serial active & reporgramming without boot button): Tools => USB CDC on boot: Enable
* Set Partition Scheme to Huge App: Tools => Partition Scheme => Huge APP
* Verify
* Upload

# Own samples:
* To create your own samples use the following link: https://mothsynth.com/sampleconverter.html

# MothSynth
MothSynth is an opensource, ultracheap music making platform based on a few components DIY enthusiasts might find in their drawers. Available in pre-assembled, PCB schematic and DIY module forms. Scroll down for GitHub and Schematics.

There are a few ways to get your hands dirty with MothSynth, build one yourself from modules easily obtainable from Amazon or AliExpress, manufacture your own PCB, or purchase one from our store.

MothOS, the default Arduino compatible software, currently supports 4 tracks, 2 drum machine banks, 10 instrumets and 5 effects (delay, lowpass, phaser, reverb and overdrive), but who knows where developers will take it! Check out our GitHub and Schematics below!

To upload your own samples to MothSynth, clone the GitHub repo, convert your samples with sample converter and replace .h files in /Samples directory of MothOS Arduino Project.

Sample Converter, pre-assembled boards, schematics and usage info: (https://mothsynth.com/).


# DIY Instructions:
If you're building your own MothSynth, You'll need the following commontly available modules, you'll just need to modify pin assignments in the MothOS project:
- ESP32S3 Dev Board
- Max98357 Breakout Board
- Arduino Compatible Keypad
- 4 LEDs + Required Resistors

![](https://www.mothsynth.com/images/easy.jpg)

# PCB Schematic
![](https://www.mothsynth.com/images/schematic.jpg)
