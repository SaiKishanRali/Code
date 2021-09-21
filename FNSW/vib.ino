const int PWM_CHANNEL = 0; // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 500;  // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8; 

// The max duty cycle value based on PWM resolution (will be 255 if resolution is 8 bits)
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);

// The pin numbering on the Huzzah32 is a bit strange so always helps to consult the pin diagram
// See pin diagram here: https://makeabilitylab.github.io/physcomp/esp32/
const int VIB_OUTPUT_PIN = 12;

const int DELAY_MS = 4;    // delay between fade increments
int _ledFadeStep = 5; // amount to fade per loop

void init_vib() {
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(VIB_OUTPUT_PIN, PWM_CHANNEL);
}


// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  //calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void vibrate(int intensity){
    ledcWrite(PWM_CHANNEL, intensity);
  
}
