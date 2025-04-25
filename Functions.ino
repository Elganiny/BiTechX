void Initialize()
{
  /****************Lcd Battery and time***************/
    lcd.init();
    lcd.createChar(0, pill);
    lcd.createChar(1, bat);
    lcd.setCursor(0, 0);
     lcd.print("Time:");
   /****************Wifi & Firebase***************/
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println("\nConnected with IP: " + WiFi.localIP().toString());
    /*Firebase Setting */
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    Firebase.reconnectNetwork(true);
    fbdo.setBSSLBufferSize(4096, 1024);
    fbdo.setResponseSize(2048);
    Firebase.begin(&config, &auth);
    Firebase.setDoubleDigits(5);
    config.timeout.serverResponse = 10 * 1000;
   /********************************************/
    // Start NTP client
  //  Audio_Init();
      timeClient.begin();  
    /*********************************************************/
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(LASER_SENSOR_PIN, INPUT);
    pinMode(TOUCH_SENSOR_PIN, INPUT);
    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
    pinMode(LED_ENABLE_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_MUX_S0,OUTPUT);
    pinMode(LED_MUX_S1,OUTPUT);
    pinMode(LED_MUX_S2,OUTPUT);
    pinMode(MOTOR_MUX_S0,OUTPUT);
    pinMode(MOTOR_MUX_S1,OUTPUT);
    pinMode(MOTOR_MUX_S2,OUTPUT);
    dispenserServo.attach(SERVO_PIN);
    digitalWrite(MOTOR_ENABLE_PIN, LOW);
    digitalWrite(LED_ENABLE_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

void selectMultiplexerChannel(int channel, int s0, int s1, int s2) {
    digitalWrite(s0, channel & 0x01 ? LOW : HIGH); 
    digitalWrite(s1, (channel >> 1) & 0x01 ? LOW : HIGH);
    digitalWrite(s2, (channel >> 2) & 0x01 ? LOW : HIGH);
}

bool detectLaser() {
    return digitalRead(LASER_SENSOR_PIN) == LOW; // Laser receiver is now digital
}

bool detectIR() {
    return digitalRead(IR_SENSOR_PIN) == HIGH;
}

bool detectTouch() {
    return digitalRead(TOUCH_SENSOR_PIN) == HIGH;
}

void activateMotor(int sector) {
    selectMultiplexerChannel(sector, MOTOR_MUX_S0, MOTOR_MUX_S1, MOTOR_MUX_S2);
    digitalWrite(MOTOR_ENABLE_PIN, HIGH); // LOW if active low
    selectMultiplexerChannel(sector, LED_MUX_S0, LED_MUX_S1, LED_MUX_S2);
    digitalWrite(LED_ENABLE_PIN, HIGH); // Turn on LED
    unsigned long startTime = millis();
    while (!detectLaser() && ((millis() - startTime ) < sectorTime)) {
        // Keep motor on
    }
    digitalWrite(MOTOR_ENABLE_PIN, LOW);
  
}

void openServo() {
    while (detectIR()) {
        dispenserServo.write(90);
    }

}
void closeServo() {
   
        dispenserServo.write(0);
        
    selectMultiplexerChannel(Medecine_Sector, LED_MUX_S0, LED_MUX_S1, LED_MUX_S2);
    digitalWrite(LED_ENABLE_PIN, LOW); // Ensure LED turns off
            lcd.setCursor(0, 1);
            lcd.print("                          ");
}

void alertMedicineTime() {
        selectMultiplexerChannel(Medecine_Sector, LED_MUX_S0, LED_MUX_S1, LED_MUX_S2);
        digitalWrite(LED_ENABLE_PIN, HIGH); // Turn on LED
        player.PlayTrack(9); // Turn on buzzer
        unsigned long alertStart = millis();
        while (millis() - alertStart < 600000) { // 10 minutes alert duration
            if (detectTouch()) {
                digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer if touch detected
                return;
            }
        }
        digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer after 10 minutes
}

bool is_time(int sector)
{
  get_time();
  sector--;
  if((medicines[sector].hour == esp_Hour ) &&  !(esp_minute < medicines[sector].minute) && ((medicines[sector].minute)+10 >= esp_minute))
  {
    return true;
  }
 touch_flag=0;
  return false;
}

void get_time()
{
   timeClient.update();
  esp_Hour= timeClient.getHours();
  esp_minute= timeClient.getMinutes();
}

void print_time()
{
  lcd.setCursor(5, 0);
  lcd.print(String(esp_Hour) + ":" + String(esp_minute));
  lcd.write(1);
  lcd.print(battery_percent);
  lcd.print("%");   
}

void readMedicines() {
   String path = "/patient1";  

    for (int i = 0; i < 8; i++) {
        String medicinePath = path + "/medicine" + String(i+1);

        Firebase.RTDB.getInt(&fbdo, medicinePath + "/medicine_hour", &medicines[i].hour);
        Firebase.RTDB.getInt(&fbdo, medicinePath + "/medicine_minute", &medicines[i].minute);
        Firebase.RTDB.getInt(&fbdo, medicinePath + "/medicine_state", &medicines[i].state);
        Firebase.RTDB.getInt(&fbdo, medicinePath + "/medicine_num", &medicines[i].num);
        Firebase.RTDB.getString(&fbdo, medicinePath + "/medicine_name");
        medicines[i].name = fbdo.stringData();
    }
}

 int WhichSector()
 {
  for (int i = 0; i < 8; i++) {
      if((medicines[i].state)==1)
      {
        return i+1;
      }  
    }
 }
/*******************************************************************************************/
/**
 * Constructor: Initializes the playerSerial pointer to use UART2 (Serial2).
 */
AudioPlayer::AudioPlayer() : playerSerial(&Serial2) {
    /** 
     * playerSerial is a pointer to the HardwareSerial object (UART2 on ESP32).
     * This allows us to use UART2 for communication with the DFPlayer Mini.
     */
}

/**
 * Initializes the DFPlayer Mini module.
 * @return True if initialization is successful, false otherwise.
 */
bool AudioPlayer::Audio_Init() 
{
    /** 
     * Begin serial communication with the DFPlayer Mini.
     * Uses the baud rate and pins defined in AudioConfig.
     */
    playerSerial->begin(AudioConfig::BAUD_RATE, SERIAL_8N1, AudioConfig::RX_PIN, AudioConfig::TX_PIN);
    
    /** 
     * Attempt to initialize the DFPlayer Mini.
     * If initialization fails, print an error message and return false.
     */
    if (!dfPlayer.begin(*playerSerial)) 
	{
        Serial.println("DFPlayer initialization failed!");
        isInitialized = false; /** Mark initialization as failed */
        return false; /** Return false to indicate failure */
    }
    
    /** 
     * If initialization is successful:
     * - Print a success message.
     * - Set the initial volume.
     * - Mark initialization as successful.
     * - Return true to indicate success.
     */
    Serial.println("DFPlayer initialized successfully");
    dfPlayer.volume(AudioConfig::INIT_VOLUME); /** Set initial volume */
    isInitialized = true; /** Mark initialization as successful */
    return true; /** Return true to indicate success */
}

/**
 * Plays a specific track by track number.
 * @param trackNumber The track number to play (1 to MAX_FILES).
 */
void AudioPlayer::PlayTrack(uint8_t trackNumber) 
{
    /** 
     * Check if the player is initialized and the track number is valid.
     * If not, print an error message and return.
     */
    if (!isInitialized || trackNumber > AudioConfig::MAX_FILES) 
	{
        Serial.println("Invalid track number or player not initialized!");
        return;
    }
    /** Play the specified track */
    dfPlayer.play(trackNumber);
}

/**
 * Plays the next track in the sequence.
 */
void AudioPlayer::PlayNext() 
{
    /** 
     * Check if the player is initialized.
     * If so, play the next track.
     */
    if (isInitialized) 
	{
        dfPlayer.next(); /** Play the next track */
    }
}

/**
 * Plays the previous track in the sequence.
 */
void AudioPlayer::PlayPrevious() 
{
    /** 
     * Check if the player is initialized.
     * If so, play the previous track.
     */
    if (isInitialized) 
	{
        dfPlayer.previous(); /** Play the previous track */
    }
}

/**
 * Sets the volume level (0-30).
 * @param volume The volume level to set (0-30).
 */
void AudioPlayer::SetVolume(uint8_t volume) 
{
    /** 
     * Check if the player is initialized and the volume is within the valid range.
     * If so, set the volume.
     */
    if (isInitialized && volume <= 30) 
	{
        dfPlayer.volume(volume); /** Set the volume */
    }
}

/**
 * Pauses the currently playing track.
 */
void AudioPlayer::Pause() 
{
    /** 
     * Check if the player is initialized.
     * If so, pause playback.
     */
    if (isInitialized) 
	{
        dfPlayer.pause(); /** Pause playback */
    }
}

/**
 * Resumes playback of the paused track.
 */
void AudioPlayer::Resume() 
{
    /** 
     * Check if the player is initialized.
     * If so, resume playback.
     */
    if (isInitialized) 
	{
        dfPlayer.start(); /** Resume playback */
    }
}

/**
 * Stops playback.
 */
void AudioPlayer::Audio_Stop() 
{
    /** 
     * Check if the player is initialized.
     * If so, stop playback.
     */
    if (isInitialized) 
	{
        dfPlayer.stop(); /** Stop playback */
    }
}

/**
 * Plays a specific track from a specific folder.
 * @param folderNumber The folder number (1-99).
 * @param trackNumber The track number within the folder (1-255).
 */
void AudioPlayer::PlayFolder(uint8_t folderNumber, uint8_t trackNumber) 
{
    /** 
     * Check if the player is initialized.
     * If so, play the specified track from the folder.
     */
    if (isInitialized) 
	{
        dfPlayer.playFolder(folderNumber, trackNumber); /** Play the track from the folder */
    }
}

/**
 * Checks if the DFPlayer Mini is busy (playing or paused).
 * @return True if the player is initialized and available, false otherwise.
 */
bool AudioPlayer::CheckStatus() 
{
    /** 
     * Return true if the player is initialized and available.
     */
    return isInitialized && dfPlayer.available();
}