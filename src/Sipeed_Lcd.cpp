#include "Sipeed_Lcd.h"
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMono12pt7b.h"
#include "Fonts/FreeMono18pt7b.h"
#include "Fonts/FreeMono24pt7b.h"
#include "lcd.h"
#include "utility/debug.h"

Sipeed_Lcd::Sipeed_Lcd(uint16_t w, uint16_t h, SPIClass& spi, int8_t dc_pin, int8_t rst_pin, uint8_t dma_ch) : Sipeed_ST7789(w, h, spi, dc_pin, rst_pin, dma_ch) {}

void Sipeed_Lcd::begin() {
  Sipeed_ST7789::begin();
  setRotation(1);
  fillScreen(0);

  // Init the back-light LED PWM
//  ledcSetup(BLK_PWM_CHANNEL, 44100, 8);
//  ledcAttachPin(TFT_BL, BLK_PWM_CHANNEL);
//  ledcWrite(BLK_PWM_CHANNEL, 80);
}

void Sipeed_Lcd::sleep() {
//  startWrite();
//  writecommand(ILI9341_SLPIN); // Software reset
//  endWrite();
}

void Sipeed_Lcd::wakeup() {
//  startWrite();
//  writecommand(ILI9341_SLPOUT);
//  endWrite();
}

void Sipeed_Lcd::setBrightness(uint8_t brightness) {
//  ledcWrite(BLK_PWM_CHANNEL, brightness);
}

void Sipeed_Lcd::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data) {
//  bool swap = getSwapBytes();
//  setSwapBytes(true);
//  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
//  setSwapBytes(swap);
  drawImage(x0, y0, w, h, (uint16_t *)data);
}

void Sipeed_Lcd::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data) {
//  bool swap = getSwapBytes();  
//  setSwapBytes(true);
//  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data);
//  setSwapBytes(swap);
  drawImage(x0, y0, w, h, data);
}

void Sipeed_Lcd::progressBar(int x, int y, int w, int h, uint8_t val) {
  drawRect(x, y, w, h, 0x09F1);
  fillRect(x + 1, y + 1, w * (((float)val) / 100.0), h - 1, 0x09F1);
}

#include "utility/qrcode.h"
void Sipeed_Lcd::qrcode(const char *string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {

  // Create the QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  qrcode_initText(&qrcode, qrcodeData, version, 0, string);

  // Top quiet zone
  uint8_t thickness = width / qrcode.size;
  uint16_t lineLength = qrcode.size * thickness;
  uint8_t xOffset = x + (width-lineLength)/2;
  uint8_t yOffset = y + (width-lineLength)/2;
  fillRect(x, y, width, width, TFT_WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      uint8_t q = qrcode_getModule(&qrcode, x, y);
      if (q) fillRect(x * thickness + xOffset, y * thickness + yOffset, thickness, thickness, TFT_BLACK);
    }
  }
}

void Sipeed_Lcd::qrcode(const String &string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  qrcode(buffer, x, y, width, version);
}

void Sipeed_Lcd::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent) {
//  bool swap = getSwapBytes();
//  setSwapBytes(true);
//  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, data, transparent);
//  setSwapBytes(swap);
  drawImage(x0, y0, w, h, (uint16_t *)data);
}

void Sipeed_Lcd::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data) {
//  bool swap = getSwapBytes();
//  setSwapBytes(true);
//  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (const uint16_t*)data);
//  setSwapBytes(swap);
  drawImage(x0, y0, w, h, (uint16_t *)data);
}

void Sipeed_Lcd::drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data) {
//  bool swap = getSwapBytes();
//  setSwapBytes(true);
//  pushImage((int32_t)x0, (int32_t)y0, (uint32_t)w, (uint32_t)h, (uint16_t*)data);
//  setSwapBytes(swap);
  drawImage(x0, y0, w, h, (uint16_t *)data);
}

/***************************************************************************************
** Function name:           drawString (with or without user defined font)
** Description :            draw string with padding if it is defined
***************************************************************************************/
// Without font number, uses font set by setTextFont()
int16_t Sipeed_Lcd::drawString(const String& string, int32_t poX, int32_t poY)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return drawString(buffer, poX, poY, textfont);
}
// With font number
int16_t Sipeed_Lcd::drawString(const String& string, int32_t poX, int32_t poY, uint8_t font)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return drawString(buffer, poX, poY, font);
}

// Without font number, uses font set by setTextFont()
int16_t Sipeed_Lcd::drawString(const char *string, int32_t poX, int32_t poY)
{
  return drawString(string, poX, poY, textfont);
}

// With font number. Note: font number is over-ridden if a smooth font is loaded
int16_t Sipeed_Lcd::drawString(const char *string, int32_t poX, int32_t poY, uint8_t font)
{
  setCursor(poX, poY + 8);
  switch(font) {
    case 1:
      setFont(NULL);
      break;
    case 2:
      setFont(NULL);
      break;
    case 3:
      setFont(&FreeMono9pt7b);
      break;
    case 4:
      setFont(&FreeMono12pt7b);
      break;
    case 5:
      setFont(&FreeMono18pt7b);
      break;
    case 6:
      setFont(&FreeMono24pt7b);
      break;
    default:
      setFont(NULL);
      break;
  }
  print(string);

return 0;
}


/***************************************************************************************
** Function name:           drawCentreString (deprecated, use setTextDatum())
** Descriptions:            draw string centred on dX
***************************************************************************************/
int16_t Sipeed_Lcd::drawCentreString(const String& string, int32_t dX, int32_t poY, uint8_t font)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return drawCentreString(buffer, dX, poY, font);
}

int16_t Sipeed_Lcd::drawCentreString(const char *string, int32_t dX, int32_t poY, uint8_t font)
{
  uint8_t tempdatum = textdatum;
  int32_t sumX = 0;
  textdatum = TC_DATUM;
  sumX = drawString(string, dX, poY, font);
  textdatum = tempdatum;
  return sumX;
}


/***************************************************************************************
** Function name:           drawRightString (deprecated, use setTextDatum())
** Descriptions:            draw string right justified to dX
***************************************************************************************/
int16_t Sipeed_Lcd::drawRightString(const String& string, int32_t dX, int32_t poY, uint8_t font)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return drawRightString(buffer, dX, poY, font);
}

int16_t Sipeed_Lcd::drawRightString(const char *string, int32_t dX, int32_t poY, uint8_t font)
{
  uint8_t tempdatum = textdatum;
  int16_t sumX = 0;
  textdatum = TR_DATUM;
  sumX = drawString(string, dX, poY, font);
  textdatum = tempdatum;
  return sumX;
}

/***************************************************************************************
** Function name:           textWidth
** Description:             Return the width in pixels of a string in a given font
***************************************************************************************/
int16_t Sipeed_Lcd::textWidth(const String& string)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return textWidth(buffer, textfont);
}

int16_t Sipeed_Lcd::textWidth(const String& string, uint8_t font)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return textWidth(buffer, font);
}

int16_t Sipeed_Lcd::textWidth(const char *string)
{
  return textWidth(string, textfont);
}

int16_t Sipeed_Lcd::textWidth(const char *string, uint8_t font)
{
  int16_t dX = 0;
  int16_t dY = 0;
  uint16_t w = 0;
  uint16_t h = 0;

  getTextBounds(string, getCursorX(), getCursorY(), &dX, &dY, &w, &h);
//  return w * textsize;
  return w;
}

/*
 * JPEG
 */

//#include "rom/tjpgd.h"

#define jpgColor(c)                                                            \
  (((uint16_t)(((uint8_t *)(c))[0] & 0xF8) << 8) |                             \
   ((uint16_t)(((uint8_t *)(c))[1] & 0xFC) << 3) |                             \
   ((((uint8_t *)(c))[2] & 0xF8) >> 3))

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
const char *jd_errors[] = {"Succeeded",
                           "Interrupted by output function",
                           "Device error or wrong termination of input stream",
                           "Insufficient memory pool for the image",
                           "Insufficient stream input buffer",
                           "Parameter error",
                           "Data format error",
                           "Right format but not supported",
                           "Not supported JPEG standard"};
#endif

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t maxWidth;
  uint16_t maxHeight;
  uint16_t offX;
  uint16_t offY;
  jpeg_div_t scale;
  const void *src;
  size_t len;
  size_t index;
  Sipeed_Lcd *tft;
  uint16_t outWidth;
  uint16_t outHeight;
} jpg_file_decoder_t;

static uint32_t jpgReadFile(JDEC *decoder, uint8_t *buf, uint32_t len) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  File *file = (File *)jpeg->src;
  if (buf) {
    return file->read(buf, len);
  } else {
//TODO:
//    file->seek(len, SeekCur);
  }
  return len;
}

static uint32_t jpgRead(JDEC *decoder, uint8_t *buf, uint32_t len) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  if (buf) {
    memcpy(buf, (const uint8_t *)jpeg->src + jpeg->index, len);
  }
  jpeg->index += len;
  return len;
}

static uint32_t jpgWrite(JDEC *decoder, void *bitmap, JRECT *rect) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  uint16_t x = rect->left;
  uint16_t y = rect->top;
  uint16_t w = rect->right + 1 - x;
  uint16_t h = rect->bottom + 1 - y;
  uint16_t oL = 0, oR = 0;
  uint8_t *data = (uint8_t *)bitmap;

  if (rect->right < jpeg->offX) {
    return 1;
  }
  if (rect->left >= (jpeg->offX + jpeg->outWidth)) {
    return 1;
  }
  if (rect->bottom < jpeg->offY) {
    return 1;
  }
  if (rect->top >= (jpeg->offY + jpeg->outHeight)) {
    return 1;
  }
  if (rect->top < jpeg->offY) {
    uint16_t linesToSkip = jpeg->offY - rect->top;
    data += linesToSkip * w * 3;
    h -= linesToSkip;
    y += linesToSkip;
  }
  if (rect->bottom >= (jpeg->offY + jpeg->outHeight)) {
    uint16_t linesToSkip = (rect->bottom + 1) - (jpeg->offY + jpeg->outHeight);
    h -= linesToSkip;
  }
  if (rect->left < jpeg->offX) {
    oL = jpeg->offX - rect->left;
  }
  if (rect->right >= (jpeg->offX + jpeg->outWidth)) {
    oR = (rect->right + 1) - (jpeg->offX + jpeg->outWidth);
  }

  uint16_t pixBuf[32];
  uint8_t pixIndex = 0;
  uint16_t line;

  jpeg->tft->startWrite();
  // jpeg->tft->setAddrWindow(x - jpeg->offX + jpeg->x + oL, y - jpeg->offY +
  // jpeg->y, w - (oL + oR), h);
  jpeg->tft->setWindow(x - jpeg->offX + jpeg->x + oL,
                       y - jpeg->offY + jpeg->y,
                       x - jpeg->offX + jpeg->x + oL + w - (oL + oR) - 1,
                       y - jpeg->offY + jpeg->y + h - 1);

  while (h--) {
    data += 3 * oL;
    line = w - (oL + oR);
    while (line--) {
      pixBuf[pixIndex++] = jpgColor(data);
      data += 3;
      if (pixIndex == 32) {
        jpeg->tft->writePixels(pixBuf, 32);
        // SPI.writePixels((uint8_t *)pixBuf, 64);
        pixIndex = 0;
      }
    }
    data += 3 * oR;
  }
  if (pixIndex) {
    jpeg->tft->writePixels(pixBuf, pixIndex);
    // SPI.writePixels((uint8_t *)pixBuf, pixIndex * 2);
  }
  jpeg->tft->endWrite();
  return 1;
}

static bool jpgDecode(jpg_file_decoder_t *jpeg,
                      uint32_t (*reader)(JDEC *, uint8_t *, uint32_t)) {
  static uint8_t work[3100];
  JDEC decoder;

  JRESULT jres = jd_prepare(&decoder, reader, work, 3100, jpeg);
  if (jres != JDR_OK) {
    log_e1("jd_prepare failed! %s", jd_errors[jres]);
    return false;
  }

  uint16_t jpgWidth = decoder.width / (1 << (uint8_t)(jpeg->scale));
  uint16_t jpgHeight = decoder.height / (1 << (uint8_t)(jpeg->scale));

  if (jpeg->offX >= jpgWidth || jpeg->offY >= jpgHeight) {
    log_e("Offset Outside of JPEG size");
    return false;
  }

  size_t jpgMaxWidth = jpgWidth - jpeg->offX;
  size_t jpgMaxHeight = jpgHeight - jpeg->offY;

  jpeg->outWidth =
      (jpgMaxWidth > jpeg->maxWidth) ? jpeg->maxWidth : jpgMaxWidth;
  jpeg->outHeight =
      (jpgMaxHeight > jpeg->maxHeight) ? jpeg->maxHeight : jpgMaxHeight;

  jres = jd_decomp(&decoder, jpgWrite, (uint8_t)jpeg->scale);
  if (jres != JDR_OK) {
    log_e1("jd_decomp failed! %s", jd_errors[jres]);
    return false;
  }

  return true;
}

void Sipeed_Lcd::drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x,
                        uint16_t y, uint16_t maxWidth, uint16_t maxHeight,
                        uint16_t offX, uint16_t offY, jpeg_div_t scale) {
  if ((x + maxWidth) > width() || (y + maxHeight) > height()) {
    log_e("Bad dimensions given");
    return;
  }

  jpg_file_decoder_t jpeg;

  if (!maxWidth) {
    maxWidth = width() - x;
  }
  if (!maxHeight) {
    maxHeight = height() - y;
  }

  jpeg.src = jpg_data;
  jpeg.len = jpg_len;
  jpeg.index = 0;
  jpeg.x = x;
  jpeg.y = y;
  jpeg.maxWidth = maxWidth;
  jpeg.maxHeight = maxHeight;
  jpeg.offX = offX;
  jpeg.offY = offY;
  jpeg.scale = scale;
  jpeg.tft = this;

  jpgDecode(&jpeg, jpgRead);
}

void Sipeed_Lcd::drawJpgFile(SDClass &fs, const char *path, uint16_t x, uint16_t y,
                            uint16_t maxWidth, uint16_t maxHeight, uint16_t offX,
                            uint16_t offY, jpeg_div_t scale) {
  if ((x + maxWidth) > width() || (y + maxHeight) > height()) {
    log_e("Bad dimensions given");
    return;
  }

  File file = fs.open(path);
  if (!file) {
    log_e("Failed to open file for reading");
    log_e(file);
    return;
  }

  jpg_file_decoder_t jpeg;

  if (!maxWidth) {
    maxWidth = width() - x;
  }
  if (!maxHeight) {
    maxHeight = height() - y;
  }

  jpeg.src = &file;
  jpeg.len = file.size();
  jpeg.index = 0;
  jpeg.x = x;
  jpeg.y = y;
  jpeg.maxWidth = maxWidth;
  jpeg.maxHeight = maxHeight;
  jpeg.offX = offX;
  jpeg.offY = offY;
  jpeg.scale = scale;
  jpeg.tft = this;

  jpgDecode(&jpeg, jpgReadFile);

  file.close();
}

