#include <xSG35.h>

// To use Deep Sleep  uncomment below line.
// #define DEEP_SLEEP

// PMS_READ_INTERVAL (4:30 min) and PMS_READ_DELAY (30 sec) CAN'T BE EQUAL! Values are also used to detect sensor state.
static const uint32_t PMS_READ_INTERVAL = 270000;
static const uint32_t PMS_READ_DELAY = 30000;

// Default sensor state.
uint32_t timerInterval = PMS_READ_DELAY;

xSG35 SG35;
void setup()
{
  // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial.begin(115200);
  Wire.begin();
  SG35.begin();

  // Switch to passive mode.
  SG35.passiveMode();

  // Default state after sensor power, but undefined after ESP restart e.g. by OTA flash, so we have to manually wake up the sensor for sure.
  // Some logs from bootloader is sent via Serial port to the sensor after power up. This can cause invalid first read or wake up so be patient and wait for next read cycle.
  SG35.wakeUp();

#ifdef DEEP_SLEEP
  delay(PMS_READ_DELAY);
  readData();
  SG35.sleep();
#endif // DEEP_SLEEP
}

void loop()
{
#ifndef DEEP_SLEEP
  static uint32_t timerLast = 0;

  uint32_t timerNow = millis();
  if (timerNow - timerLast >= timerInterval) {
    timerLast = timerNow;
    timerCallback();
    timerInterval = timerInterval == PMS_READ_DELAY ? PMS_READ_INTERVAL : PMS_READ_DELAY;
  }

  // Do other stuff...
#endif // DEEP_SLEEP
}

#ifndef DEEP_SLEEP
void timerCallback() {
  if (timerInterval == PMS_READ_DELAY)
  {
    readData();
    Serial.println("Going to sleep.");
    SG35.sleep();
  }
  else
  {
    Serial.println("Waking up.");
    SG35.wakeUp();
  }
}
#endif // DEEP_SLEEP

void readData()
{
  xSG35::DATA data;

  Serial.println("Send read request...");
  SG35.requestRead();

  Serial.println("Reading data...");
  if (SG35.readUntil(data))
  {
    Serial.println();

    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }
  else
  {
    Serial.println("No data.");
  }
}