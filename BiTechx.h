/********************** Icludes **********************/
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DFRobotDFPlayerMini.h"
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>

/********************** Wifi & Firebase **********************/

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

/*  WiFi */
#define WIFI_SSID "MIDO"
#define WIFI_PASSWORD "a0102005588e"

/*  Firebase API */
#define API_KEY "AIzaSyCrST7GnkApOy_VI-XkLk27MN9ETvSbeZ0"

/* Firebase URL */
#define DATABASE_URL "https://test-16784-default-rtdb.firebaseio.com/" 

/* Login */
#define USER_EMAIL "ae1652003@gmail.com"
#define USER_PASSWORD "123456"

/* Firebase Objects */
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

/* changing time */
unsigned long sendDataPrevMillis = 0;

/* Medicines Struct */
struct Medicine {
    String name;
    int hour;
    int minute;
    int state;
    int num;
};

/* Medicines Array */
Medicine medicines[8];


/********************** Pins **********************/

#define SERVO_PIN 2  
#define LED_MUX_S0 4
#define LED_MUX_S1 5
#define LED_MUX_S2 33
#define BUZZER_PIN 22
#define MOTOR_MUX_S0 10
#define MOTOR_MUX_S1 18
#define MOTOR_MUX_S2 19
#define IR_SENSOR_PIN 21
#define LED_ENABLE_PIN 14 // Added LED control pin
#define LASER_SENSOR_PIN 15
#define TOUCH_SENSOR_PIN 12
#define MOTOR_ENABLE_PIN 13

/********************** Function Declaration **********************/
void readMedicines();
void Initialize();
int WhichSector();
bool is_time(int sector);
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
char battery_percent;
Servo dispenserServo;
char Medicine_name[50];
unsigned long sectorTime = 5000;
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool activeSectors[NUM_SECTORS] = {false};
int esp_Hour,esp_minute,Medicine_Hour,Medicine_Minute,Medicine_Doses,Medecine_Sector,medicineTime;
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

    bool CheckStatus();  /** Check if player is busy **/

  private:
    DFRobotDFPlayerMini dfPlayer;
    HardwareSerial *playerSerial;
    bool isInitialized = false;
};