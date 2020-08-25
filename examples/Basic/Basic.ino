#include "xSG35.h"
#include <Wire.h>

xSG35 SG35;
xSG35::DATA data;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  SG35.begin();
}

void loop()
{
  if (SG35.read(data))
  {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }
}