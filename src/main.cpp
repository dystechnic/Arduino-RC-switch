#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600

#define RC_NUM_CHANNELS  2
#define RC_CH1  0
#define RC_CH2  1

// pins receiver is connected to
#define RC_CH1_INPUT  14  //  == pin A0
#define RC_CH2_INPUT  15  //  == pin A1

// definition of signal thresholds
#define INPUT_LOW_THRESHOLD 1000 // Low to Mid input signal threshold. 
#define INPUT_MID_THRESHOLD 1500 // Mid to High input signal threshold.

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() { 
  calc_input(RC_CH1, RC_CH1_INPUT); 
}

void calc_ch2() {
  calc_input(RC_CH2, RC_CH2_INPUT); 
}

void debug() {
  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
  Serial.print("CH2:"); Serial.println(rc_values[RC_CH2]);
}

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
 
//  Enable interrupts on input channels
  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
//  enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);
//  enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);
}

void loop() {
  rc_read_values();
  if (rc_values[RC_CH1] < INPUT_LOW_THRESHOLD) {
    Serial.print("CH1: 3POS switch 0");  // 3POS switch position 0
    }
  if (rc_values[RC_CH1] > INPUT_LOW_THRESHOLD && rc_values[RC_CH1] < INPUT_MID_THRESHOLD) {
    Serial.print("CH1: 3POS switch 1");  // 3POS switch position 1
    }
  if (rc_values[RC_CH1] > INPUT_MID_THRESHOLD) {
    Serial.print("CH1: 3POS switch 2");  // 3POS switch position 2
    }
      //  debug(); // enable for serial debugging of receiver inputs
  }

