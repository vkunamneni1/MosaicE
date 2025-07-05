#include <RotaryEncoder.h>
const uint8_t BUTTON1 = 26;
const uint8_t BUTTON2 = 27;
const uint8_t BUTTON3 = 28;
const uint8_t LED1 = 29;
const uint8_t LED2 = 6;
const uint8_t LED3 = 7;
const uint8_t LED4 = 0;
const uint8_t ENC_A = 2;
const uint8_t ENC_B = 1;

RotaryEncoder encoder(ENC_A, ENC_B, RotaryEncoder::LatchMode::TWO03);
volatile long lastEncoderPos = 0;

enum Mode : uint8_t {
  IDLE,
  CLOCK_INIT, CLOCK_RUN,
  BIN_INIT,   BIN_RUN,
  LIGHT_INIT, LIGHT_RUN
};
Mode mode = IDLE;

unsigned long lastSecond  = 0;
unsigned long lastMinute  = 0;
unsigned long lastHour    = 0;
unsigned long lastBinTick = 0;

uint8_t binValue = 0;

const long ENC_MIN = 0;
const long ENC_MAX = 200;
long encPosition   = 0;

const uint16_t PWM_MAX = 255;

inline void blinkLed(uint8_t pin, uint8_t times, uint16_t period_ms = 1000) {
  for (uint8_t i = 0; i < times; ++i) {
    digitalWrite(pin, HIGH);  delay(period_ms / 2);
    digitalWrite(pin, LOW);   delay(period_ms / 2);
  }
}

inline void flashAll(uint8_t times, uint16_t period_ms = 1000) {
  for (uint8_t i = 0; i < times; ++i) {
    digitalWrite(LED1, HIGH); digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH); digitalWrite(LED4, HIGH);
    delay(period_ms / 2);
    digitalWrite(LED1, LOW);  digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);  digitalWrite(LED4, LOW);
    delay(period_ms / 2);
  }
}

void updateLightBar(long pos) {
  const long step = ENC_MAX / 4;
  int levels[4] = {0, 0, 0, 0};

  if (pos < step) {
    levels[0] = map(pos, 0, step, 0, PWM_MAX);
  } else {


    levels[0] = PWM_MAX;
    if (pos < 2 * step) {
      levels[1] = map(pos, step, 2 * step, 0, PWM_MAX);
    } else {
      levels[1] = PWM_MAX;
      if (pos < 3 * step) {
        levels[2] = map(pos, 2 * step, 3 * step, 0, PWM_MAX);
      } else {
        levels[2] = PWM_MAX;
        levels[3] = map(pos, 3 * step, ENC_MAX, 0, PWM_MAX);
      }
    }
  }

  analogWrite(LED1, levels[0]);
  analogWrite(LED2, levels[1]);
  analogWrite(LED3, levels[2]);
  analogWrite(LED4, levels[3]);
}

void setup() {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

  pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT); pinMode(LED4, OUTPUT);

  encoder.setPosition(0);
  
}

void loop() {
  const bool b1 = !digitalRead(BUTTON1);
  const bool b2 = !digitalRead(BUTTON2);
  const bool b3 = !digitalRead(BUTTON3);
  const unsigned long now = millis();

  if (b3 && mode != IDLE) {
    mode = IDLE;
    flashAll(5);
    updateLightBar(0);
  }

  if (mode == IDLE) {
    if (b1) {
      mode = CLOCK_INIT;
    } else if (b2) {
      mode = BIN_INIT;
    } else {
      encoder.tick();
      long newPos = encoder.getPosition();
      if (newPos != lastEncoderPos) {
        lastEncoderPos = newPos;
        mode = LIGHT_INIT;
      }
    }
  }

  switch (mode) {
    case CLOCK_INIT:
      blinkLed(LED1, 3);
      lastSecond = lastMinute = lastHour = now;
      mode = CLOCK_RUN;
      break;

    case CLOCK_RUN:
      if (now - lastSecond >= 1000UL) {
        digitalWrite(LED4, HIGH); delay(50); digitalWrite(LED4, LOW);
        lastSecond += 1000UL;

      }
      if (now - lastMinute >= 60000UL) {
        digitalWrite(LED3, HIGH); delay(50); digitalWrite(LED3, LOW);
        lastMinute += 60000UL;
      }
      if (now - lastHour >= 3600000UL) {
        digitalWrite(LED2, HIGH); delay(100); digitalWrite(LED2, LOW);
        lastHour += 3600000UL;
      }
      break;

    case BIN_INIT:
      blinkLed(LED1, 4);
      binValue = 0;
      lastBinTick = now;
      mode = BIN_RUN;
      break;

    case BIN_RUN:
      if (now - lastBinTick >= 1000UL) {
        digitalWrite(LED4, (binValue >> 0) & 1);
        digitalWrite(LED3, (binValue >> 1) & 1);
        digitalWrite(LED2, (binValue >> 2) & 1);

        digitalWrite(LED1, (binValue >> 3) & 1);

        binValue = (binValue + 1) & 0x0F;
        lastBinTick += 1000UL;
      }
      break;

    case LIGHT_INIT:
      blinkLed(LED1, 2);
      encPosition = constrain(encoder.getPosition(), ENC_MIN, ENC_MAX);
      updateLightBar(encPosition);
      mode = LIGHT_RUN;
      break;




    case LIGHT_RUN:
      encoder.tick();
      {
        long newPos = encoder.getPosition();
        if (newPos != lastEncoderPos) {
          lastEncoderPos = newPos;

          encPosition = constrain(newPos, ENC_MIN, ENC_MAX);
          updateLightBar(encPosition);
        }
      }
      break;

    default: break;
  }
}
