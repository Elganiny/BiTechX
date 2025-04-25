/********************** Icludes **********************/

#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DFRobotDFPlayerMini.h"
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>

/********************** Wifi & Firebase **********************/

const char* ssid = "MIDO";
const char* password = "a0102005588e";

/********************** Pins **********************/

#define SERVO_PIN 6
#define LED_MUX_S0 7
#define LED_MUX_S1 8
#define LED_MUX_S2 9
#define BUZZER_PIN 13
#define IR_SENSOR_PIN 2
#define MOTOR_MUX_S0 10
#define MOTOR_MUX_S1 11
#define MOTOR_MUX_S2 12
#define LED_ENABLE_PIN 14 // Added LED control pin
#define LASER_SENSOR_PIN 3
#define TOUCH_SENSOR_PIN 4
#define MOTOR_ENABLE_PIN 5

/********************** Function Declaration **********************/

bool detectIR();
void openServo();
void closeServo();
bool detectLaser();
bool detectTouch();
void alertMedicineTime();
void activateMotor(int sector);
void selectMultiplexerChannel(int channel, int s0, int s1, int s2);

/********************** Lcd Figures **********************/

byte pill[] = {
  B00000,
  B01110,
  B11111,
  B11011,
  B11011,
  B11111,
  B01110,
  B00000
};
byte bat[] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B00000
};

/********************** variables **********************/

#define NUM_SECTORS 8
WiFiUDP ntpUDP;
bool touch_flag=0;
AudioPlayer player;

char battery_percent;
Servo dispenserServo;
char Medicine_name[50];
unsigned long sectorTime = 5000;
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool activeSectors[NUM_SECTORS] = {false};
int esp_Hour,esp_minute,Medicine_Hour,Medicine_Minute,Medicine_Dosesl,Medecine_Sector;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200, 60000); // 0 is UTC offset, 60000ms update interval

/********************** Audio **********************/

/** Configuration namespace **/
namespace AudioConfig {
    const uint8_t RX_PIN = 16;     /** ESP32 RX2 pin **/
    const uint8_t TX_PIN = 17;     /** ESP32 TX2 pin **/
    const uint32_t BAUD_RATE = 9600;
    const uint8_t INIT_VOLUME = 15;  /** From 0 to 30 **/
    const uint8_t MAX_FILES = 255;   /** Maximum track numbers **/
};

class AudioPlayer {
  public:
    AudioPlayer();
    bool Audio_Init();  /** Initialize player **/
    void PlayTrack(uint8_t trackNumber);
    void PlayNext();
    void PlayPrevious();
    void SetVolume(uint8_t volume);
    void Pause();
    void Resume();
    void Audio_Stop();
    void PlayFolder(uint8_t folderNumber, uint8_t trackNumber);
    void Initialize();
    void getTime();

    bool CheckStatus();  /** Check if player is busy **/

  private:
    DFRobotDFPlayerMini dfPlayer;
    HardwareSerial *playerSerial;
    bool isInitialized = false;
};

