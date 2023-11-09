## A Simple Arduino example on how to draw on a WaveShare 5.83inch 648x480 (Epd5in83b V2) E-Paper with an ESP8266 using adafruit GFX    

![waveshare e-paper display](https://github.com/tomekness/waveshare_Epd5in83b-adafruitGFX/blob/main/img3.jpg?raw=true)


//Based on the esp8266-waveshare-epd drawing example and the adafruit GFX canvas example 

to make a long story short: I just wanted to use the adafruit GFX drawing features on a Waveshare ePaper display controlled by an ESP8266 (Node MCU). I ended up using the canvas feature (first draw onto a virtual canvas and then send the finished drawing to the display). On the way I ran into a lot of storage issues ... so I had to write directly into the display, therefor I had to make two private functions from the esp8266_waveshare lib public available in my Arduino sketch, but I didn't want to mess with the lib, so I copy&pasted »EPD_5IN83B_V2_SendCommand« and »EPD_5IN83B_V2_SendData« directly into the sketch : / (little ugly. but works)

adafruit GFX Library ---> https://github.com/adafruit/Adafruit-GFX-Library
<br>
waveshare ---> https://www.waveshare.com/wiki/E-Paper_ESP8266_Driver_Board



////////////////////////////// be advised

*For E-paper displays that support partial refresh, please note that you cannot refresh them with the 
partial refresh mode all the time. After refreshing partially several times, you need to fully refresh EPD once. 
Otherwise, the display effect will be abnormal, which cannot be repaired!*

*Note that the screen cannot be powered on for a long time. When the screen is not refreshed, 
please set the screen to sleep mode or power off it. Otherwise, the screen will remain in a 
high voltage state for a long time, which will damage the e-Paper and cannot be repaired!*

[5.83inch e-Paper HAT (B) Manual](https://www.waveshare.com/wiki/5.83inch_e-Paper_HAT_(B)_Manual)

////////////////////////////// how does the ePaper work

*For 5.83inch e-Paper (B), a red-black-white e-paper, we need to divide one picture into two pictures, 
that is, one is black and white, and the other one is red and white. 
As there is a register controlling the black and white color when transmitting, 
then another one controls the red and white color. One byte controls 8 pixels in these two registers.*

[5.83inch e-Paper HAT (B) Manual](https://www.waveshare.com/wiki/5.83inch_e-Paper_HAT_(B)_Manual)

