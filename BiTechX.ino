#include <BiTechx.h>

void setup() 
{
 Initialize();
}

void loop() 
{
  timeClient.update();
  esp_Hour= timeClient.getHours();
  esp_minute= timeClient.getMinutes();
    lcd.setCursor(5, 0);
    lcd.print(string(esp_Hour)+":"+string(esp_minute));
    lcd.write(1);
lcd.print(battery);
lcd.print("%");   
  if ((Medicine_Hour == esp_Hour ) &&  !(esp_minute<Medicine_Minute) && (Medicine_Minute+2 >= esp_minute) && !touch_flag)         
  {
    lcd.print(medecine_name);
lcd.print(":");
lcd.print(doses);
lcd.print(+"bills");
lcd.write(0);
    if(!touch_flag)
    {
 PlayTrack();
    while (!(digitalRead(TOUCH_SENSOR_PIN))); /** Testing might change it to while (digitalRead(TOUCH_SENSOR_PIN)); **/
     Audio_Stop();
     touch_flag=1;
    }
    PlayTrack(Medecine_Sector);  // firebase
    activateMotor(Medecine_Sector);
    openServo();
    closeServo();
  }
}
/**
 * SoftwareSerial is less efficient and may not work well
 * at higher baud rates. For your project, 
 * HardwareSerial is the better choice.
**/