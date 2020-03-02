#ifndef _SIPEED_LCD_H
#define _SIPEED_LCD_H

#include <Sipeed_ST7789.h>
#include "lcd.h"
#include "SD.h"
#include "utility/tjpgd.h"

  typedef enum {
    JPEG_DIV_NONE,
    JPEG_DIV_2,
    JPEG_DIV_4,
    JPEG_DIV_8,
    JPEG_DIV_MAX
  } jpeg_div_t;

//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline


// New color definitions use for all my libraries
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFC9F

// Next is a special 16 bit colour value that encodes to 8 bits
// and will then decode back to the same 16 bit value.
// Convenient for 8 bit and 16 bit transparent sprites.
#define TFT_TRANSPARENT 0x0120

class Sipeed_Lcd : public Sipeed_ST7789 {
public:
    Sipeed_Lcd(uint16_t w, uint16_t h, SPIClass& spi, int8_t dc_pin, int8_t rst_pin, uint8_t dma_ch);

//    ~Sipeed_Lcd(void);

  void begin();
  void sleep();
  void wakeup();
  void setBrightness(uint8_t brightness);
  void clearDisplay(uint32_t color=TFT_BLACK) { fillScreen(color); }
  void clear(uint32_t color=TFT_BLACK) { fillScreen(color); }

  void setTextFont(uint8_t font) {
    textfont = font;
  }

  inline void setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    lcd_set_area(x1, y1,x2, y2);
  }
  inline void writePixel(uint16_t color) {
//    SPI.write16(color);
    tft_write_half(&color, 1);
  }
  inline void writePixels(uint16_t * colors, uint32_t len){
//    SPI.writePixels((uint8_t*)colors , len * 2);
    tft_write_half(colors, len);
  }

           // Handle char arrays
  int16_t  drawString(const char *string, int32_t poX, int32_t poY, uint8_t font),
           drawString(const char *string, int32_t poX, int32_t poY),
           drawCentreString(const char *string, int32_t dX, int32_t poY, uint8_t font), // Deprecated, use setTextDatum() and drawString()
           drawRightString(const char *string, int32_t dX, int32_t poY, uint8_t font),  // Deprecated, use setTextDatum() and drawString()

           // Handle String type
           drawString(const String& string, int32_t poX, int32_t poY, uint8_t font),
           drawString(const String& string, int32_t poX, int32_t poY),
           drawCentreString(const String& string, int32_t dX, int32_t poY, uint8_t font), // Deprecated, use setTextDatum() and drawString()
           drawRightString(const String& string, int32_t dX, int32_t poY, uint8_t font);  // Deprecated, use setTextDatum() and drawString()

  int16_t  textWidth(const char *string, uint8_t font),
           textWidth(const char *string),
           textWidth(const String& string, uint8_t font),
           textWidth(const String& string),
           fontHeight(int16_t font),
           fontHeight(void);

  uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining);
  uint16_t decodeUTF8(uint8_t c);

  uint8_t  textfont,  // Current selected font
//           textsize,  // Current font size multiplier
           textdatum, // Text reference datum
           rotation;  // Display rotation (0-3)

  int16_t _xpivot;   // x pivot point coordinate
  int16_t _ypivot;   // x pivot point coordinate

  uint8_t  decoderState = 0;   // UTF8 decoder state
  uint16_t decoderBuffer;      // Unicode code-point buffer

      void progressBar(int x, int y, int w, int h, uint8_t val);

//      #define setFont setFreeFont

      void qrcode(const char *string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);
      void qrcode(const String &string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);

      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent);

      void drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x = 0,
                  uint16_t y = 0, uint16_t maxWidth = 0, uint16_t maxHeight = 0,
                  uint16_t offX = 0, uint16_t offY = 0,
                  jpeg_div_t scale = JPEG_DIV_NONE);

      void drawJpg(SDClass &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
                    uint16_t maxWidth = 0, uint16_t maxHeight = 0,
                    uint16_t offX = 0, uint16_t offY = 0,
                    jpeg_div_t scale = JPEG_DIV_NONE);

      void drawJpgFile(SDClass &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
                    uint16_t maxWidth = 0, uint16_t maxHeight = 0,
                    uint16_t offX = 0, uint16_t offY = 0,
                    jpeg_div_t scale = JPEG_DIV_NONE);

 private:
};

#endif
