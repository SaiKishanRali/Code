
void init_neopixel() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  setPixelOff();
}

void setPixel(int r, int g, int b) {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void setPixelOff() {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();   // Send the updated pixel colors to the hardware.
}
