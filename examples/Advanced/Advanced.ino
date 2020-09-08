#include <xSG35.h>

xSG35 SG35;
xSG35::DATA data;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  SG35.begin();
  SG35.passiveMode();    // Switch to passive mode
}

void loop()
{
  Serial.println("Waking up, wait 30 seconds for stable readings...");
  SG35.wakeUp();
  delay(30000);

  Serial.println("Send read request...");
  SG35.requestRead();

  Serial.println("Wait max. 1 second for read...");
  if (SG35.readUntil(data))
  {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);
  }
  else
  {
    Serial.println("No data.");
  }

  Serial.println("Going to sleep for 60 seconds.");
  SG35.sleep();
  delay(60000);
}