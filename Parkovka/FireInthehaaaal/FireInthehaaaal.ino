#include <Wire.h>
#include <MGS_FR403.h>
MGS_FR403 Fire;
float visib = 0;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  Fire.begin();
}

void loop() {
  Fire.get_ir_and_vis();
  visib = Fire.vis_data;
  if (visib >= 8000){
    Serial.println("Пожар");
  }
  Serial.print("Видимый: ");
 // Serial.println(String(Fire.vis_data, 1));
}