#include "BiTechx.h"
AudioPlayer player;

void setup() {
   Serial.begin(115200);
  Initialize();
// esp_task_wdt_delete(NULL);
 }

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    readMedicines();
    int sector=WhichSector();
    print_time();
    if (is_time(sector)) {
      if (!touch_flag) {
        while (!(digitalRead(TOUCH_SENSOR_PIN)) && is_time(sector)) {
          print_time();
          player.PlayTrack(9);
        } /** Testing might change it to while (digitalRead(TOUCH_SENSOR_PIN)); **/
       player.Audio_Stop();
        touch_flag = 1;
      }
         /*******************************************************/
     lcd.setCursor(0, 1);
      lcd.print(medicines[sector].name);
      lcd.print(":");
      lcd.print(medicines[sector].num);
      lcd.print(+"bills");
      lcd.write(0);
      player.PlayTrack(sector);
      activateMotor(sector);
      /***********************************************************/
      openServo();
      closeServo();
    Serial.println("done");
    
    }
  }
}
/**
 * SoftwareSerial is less efficient and may not work well
 * at higher baud rates. For your project, 
 * HardwareSerial is the better choice.
**/