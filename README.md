# waveshare_Epd5in83b-adafruitGFX

/////////// WaveShare Epaper Display + ESP8266 +  adafruit GFX ////////////////////////////////////////////
//                                                                                                       //
// drawing on a WaveShare 5.83inch 648x480 (Epd5in83b V2) E-Paper with an ESP8266 using adafruit GFX     //
//                                                                                                       //
// based on the esp8266-waveshare-epd drwawing example and the adafruit GFX canvas example               //
//                                                                                                       //
//////////////////////////////////////////////////////////////////////////// tomek n. | 2023 //////////////


/* to make a long story short: i just wanted to use the adafruit GFX drawing features on a Waveshare ePaper 
dispaly controlled by an ESP8266. I ended up using the canvas feature (first draw on to a virtual canvas and then
send the finished drawing to the display). On the way i run into a lof of storrage issues … so i have to write  
directly into the display, therefor i had to make two private function from the esp8266_waveshre lib public avaiable 
in my arduino sketch, but i didn't want to mess with the lib, so i copy&pasted »EPD_5IN83B_V2_SendCommand« and 
»EPD_5IN83B_V2_SendData« directly into the sktech : / (little ugly. but works)


////////////////////////////// be adviced

/* For E-paper displays that support partial refresh, please note that you cannot refresh them with the 
partial refresh mode all the time. After refreshing partially several times, you need to fully refresh EPD once. 
Otherwise, the display effect will be abnormal, which cannot be repaired! */

/* Note that the screen cannot be powered on for a long time. When the screen is not refreshed, 
please set the screen to sleep mode or power off it. Otherwise, the screen will remain in a 
high voltage state for a long time, which will damage the e-Paper and cannot be repaired! */


////////////////////////////// how does the ePaper work

/* For 5.83inch e-Paper (B), a red-black-white e-paper, we need to divide one picture into two pictures, 
that is, one is black and white, and the other one is red and white. 
As there is a register controlling the black and white color when transmitting, 
then another one controls the red and white color. One byte controls 8 pixels in these two registers.*/

