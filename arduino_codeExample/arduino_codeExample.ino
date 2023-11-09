

/////////// WaveShare Epaper Display + ESP8266 +  adafruit GFX ////////////////////////////////////////////
//                                                                                                       //
// drawing on a WaveShare 5.83inch 648x480 (Epd5in83b V2) E-Paper with an ESP8266 using adafruit GFX     //
//                                                                                                       //
// based on the esp8266-waveshare-epd drawing example and the adafruit GFX canvas example               //
//                                                                                                       //
//////////////////////////////////////////////////////////////////////////// tomek n. | 2023 //////////////


/* to make a long story short: I just wanted to use the adafruit GFX drawing features on a Waveshare ePaper display 
controlled by an ESP8266 (Node MCU). I ended up using the canvas feature (first draw onto a virtual canvas and 
then send the finished drawing to the display). On the way I ran into a lot of storage issues ... so I had to 
write directly into the display, therefor I had to make two private functions from the  esp8266_waveshare lib public 
available in my Arduino sketch, but I didn't want to mess with the lib, so I copy&pasted »EPD_5IN83B_V2_SendCommand« 
and »EPD_5IN83B_V2_SendData« directly into the sketch : / (little ugly. but works) */


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


/* Includes ------------------------------------------------------------------*/

#include "DEV_Config.h"
#include "EPD.h"
#include <Adafruit_GFX.h>
#include <gfxfont.h>

boolean display_debug = true;

void setup() {

  DEV_Module_Init();  // init pins, etc // this inits the serial as well at 115200

  Serial.begin(115200);  // but just in case



  // first things first

  if (display_debug) Serial.print("--> init - ");
  EPD_5IN83B_V2_Init();
  if (display_debug) Serial.print("--> clear - ");
  EPD_5IN83B_V2_Clear();

  // i use a two color ePaper display
  //

  drawAndSend_BlackLayer();
  drawAndSend_RedLayer();

  if (display_debug) Serial.println("Goto Sleep...\r\n");
  EPD_5IN83B_V2_Sleep();
}

/* The main loop -------------------------------------------------------------*/
void loop() {
}

void drawAndSend_BlackLayer() {

  /////// create adafruit GfX canvas
  GFXcanvas1 canvas(EPD_5IN83B_V2_WIDTH, EPD_5IN83B_V2_HEIGHT);  // 1-bit, 648x480 pixels // canvas for one color (black or read)

  ///////// colors
  // white --> 1
  // black --> 0

  ////// here is where the drawing is happening

  if (display_debug) Serial.println("get Pixel");              // debugging
  if (display_debug) Serial.println(canvas.getPixel(10, 10));  // debugging

  canvas.fillScreen(1);                                        // fill background
  if (display_debug) Serial.println(canvas.getPixel(10, 10));  // debugging

  for (int e = 0; e < 34; e++) {
    canvas.drawLine(e * 10, 0, 0, canvas.height() - e * 10, 0);
  }
   
  canvas.setTextColor(0, 1); // black text, white background
  canvas.setTextSize(2);               // 2X size text
  canvas.setCursor(400,380); // Position
  canvas.print("Hello World");  // Print a message


  ///// done drawing, so send it of to the dispaly
  GFXcanvas1 *_can = &canvas;   // create a pointer to the canvas object // because we are short of storrage
  sendImageToDisplay(0, _can);  // 0 = black layer
}


void drawAndSend_RedLayer() {

  /////// create adafruit GfX canvas
  GFXcanvas1 canvas(EPD_5IN83B_V2_WIDTH, EPD_5IN83B_V2_HEIGHT);  // 1-bit, 648x480 pixels // canvas for one color (black or read)

  ///////// colors
  // white --> 0
  // Red --> 1

  ////// here is where the drawing is happening
  canvas.fillScreen(0);  // fill backgrund;

  for (int e = 0; e < 25; e++) {
    canvas.drawLine( 0, e * 10, canvas.width() - e * 10, 0, 1);
  }

  canvas.setTextColor(1, 0); // Red text, white background
  canvas.setTextSize(2);               // 2X size text
  canvas.setCursor(450,400); // Position
  canvas.print("Hello World");  // Print a message

  ///// done drawing, so send it of to the dispaly
  GFXcanvas1 *_can = &canvas;   // create a pointer to the canvas object // because we are short of storrage
  sendImageToDisplay(1, _can);  // 1 = red layer layer
}


///////////// send the canvas pixel to the epaper display
///// because wir are short of storrage we just pass the pointer to the canvas into this function
///// and directly send the pixel to the display

void sendImageToDisplay(boolean _layer, GFXcanvas1 *_canvas) {

  /////////////////////
  //convert your canvases to Epaper readable Arrays
  // canvas ------------------> 1bit - 648 x 480
  // ePaper Image ------------> 8bit - 81 x 480 (1 byte = 8 bit)

  if (display_debug) Serial.print("--> send black data");

  //which layer should we write to
  if (_layer == 0) {  // black layer
    my_EPD_5IN83B_V2_SendCommand(0x10);
  } else {  // red Layer
    my_EPD_5IN83B_V2_SendCommand(0x13);
  }
  /// write the display bytes
  for (int h = 0; h < EPD_5IN83B_V2_HEIGHT; h++) {
    for (int w = 0; w < EPD_5IN83B_V2_WIDTH / 8; w++) {
      byte pixel_buffer;
      for (int b = 0; b < 8; b++) {  // get 8 pixel and stuff them into one byte
        pixel_buffer = pixel_buffer << 1;
        pixel_buffer += _canvas->getPixel(w * 8 + b, h);  // _canvas is just the pointer thats why the "->" // google »pointer in C« if you want to know more
      }
      my_EPD_5IN83B_V2_SendData(pixel_buffer);  // write the byte to the display
    }
  }

  if (display_debug) Serial.print("--> turn on diplay");
  EPD_5IN83B_V2_TurnOnDisplay();  // done upadating pixel show the image
}

//////////////////////////////////////////////////////////
// just copied the following two function from the esp8266-waveshare-epd library
// to access those two public (from the arduino sktech)

void my_EPD_5IN83B_V2_SendCommand(UBYTE Reg) {
  DEV_Digital_Write(EPD_DC_PIN, 0);
  DEV_Digital_Write(EPD_CS_PIN, 0);
  DEV_SPI_WriteByte(Reg);
  DEV_Digital_Write(EPD_CS_PIN, 1);
}

void my_EPD_5IN83B_V2_SendData(UBYTE Data) {

  DEV_Digital_Write(EPD_CS_PIN, 0);
  DEV_Digital_Write(EPD_DC_PIN, 1);
  DEV_SPI_WriteByte(Data);
  DEV_Digital_Write(EPD_CS_PIN, 1);
}
