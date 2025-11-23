// Demonstrate that u8g2 library only displays 96 pixels height on Zio OLED
#include <U8g2lib.h>

#define USE_FULL_PAGE
#ifdef USE_FULL_PAGE
U8G2_SSD1327_EA_W128128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); /* Uno: A4=SDA, A5=SCL, add "u8g2.setBusClock(400000);" into setup() for speedup if possible */
#else
U8G2_SSD1327_EA_W128128_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);
#endif

const int COLOR_WHITE = 1;
const int COLOR_BLACK = 0;
class OLEDWrapper {
  public:
    void u8g2_prepare(void) {
      u8g2.setFont(u8g2_font_fur49_tn);
      u8g2.setFontRefHeightExtendedText();
      u8g2.setDrawColor(COLOR_WHITE);
      u8g2.setFontDirection(0);
    }
    void startup() {
      pinMode(10, OUTPUT);
      pinMode(9, OUTPUT);
      digitalWrite(10, 0);
      digitalWrite(9, 0);
      u8g2.begin();
      u8g2.setBusClock(400000);
    }
    void startDisplay(const uint8_t *font) {
      u8g2_prepare();
      u8g2.clearBuffer();
      u8g2.setFont(font);
    }
    void endDisplay() {
      u8g2.sendBuffer();
    }
    void test() {
      for (u8g2_uint_t h = 97; h >= 95; h--) {
        u8g2.clear();
        delay(1000);
#ifdef USE_FULL_PAGE
        useFullPage(h);
#else
        usePageBuffer(h);
#endif
        delay(2000);
      }
    }
    void useFullPage(int h) {
        startDisplay(u8g2_font_fur11_tf);
        u8g2.drawFrame(0, 0, getWidth(), h);
        String s(h);
        s.concat(" (full)");
        u8g2.drawUTF8(8, 32, s.c_str());
        endDisplay();
    }
    void usePageBuffer(int h) {
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_fur11_tf);
        u8g2.drawFrame(0, 0, getWidth(), h);
        String s(h);
        s.concat(" (page)");
        u8g2.drawUTF8(8, 32, s.c_str());
      } while ( u8g2.nextPage() );
    }
  int getHeight() {
      return 96; // ??? why does u8g2.getHeight() return 128 ???
    }
    int getWidth() {
      return u8g2.getWidth();
    }
};
OLEDWrapper* oledWrapper = new OLEDWrapper();

void setup() {
  oledWrapper->startup();
}

void loop() {
  oledWrapper->test();
}
