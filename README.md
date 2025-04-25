# BiTechX: Smart Medication Dispenser 💊

## 🚀 Overview  
Millions of people suffer from chronic diseases, requiring continuous monitoring and timely medication. Many elderly individuals and patients struggle with remembering medication schedules, keeping up with doctor visits, or dealing with unexpected health issues. Our project aims to **bridge the gap between patients, caregivers, and doctors** by providing an **intelligent and connected health management system**.  

## 🔧 Key Features  
- **💊 Automated Medication Dispenser** – Uses NFC to ensure users receive their medicine at the right time, reducing missed doses.  
- **📱 Companion Mobile App** – Connects patients, caregivers, and doctors for real-time health tracking, medication updates, and appointment reminders.  

## 🎯 Objectives  
✔️ Ensure patients never miss a dose again.    
✔️ Reduce unnecessary hospital visits.  
✔️ Improve the quality of life for both patients and their families.  

By integrating **wearable technology, IoT, and smart automation**, our project offers a seamless and efficient solution to healthcare challenges. 💡  

---

## System Components
- **Automated Dispenser & Multiplexer Control**  
  Utilizes a multiplexer system to manage multiple medication sectors. The dispenser is activated via sensor inputs and ensures proper dosing 
- **Sensor Integration**  
  - **IR Sensor:** Detects the presence of objects for safe dispenser operation.  
  - **Laser Sensor:** Monitors the dispenser’s status (e.g., checking if the pill has dispensed from sector).  
  - **Touch Sensor:** Allows users to interact with the system to acknowledge alerts.

- **Audio Alerts**  
  The DFPlayer Mini is integrated to play audio alerts and notifications. An AudioPlayer class handles track selection, volume control, and playback, ensuring that patients are alerted audibly when it’s time to take their medication.

- **WiFi & Time Synchronization**  
  The system connects to WiFi to synchronize time using an NTP client. This guarantees accurate scheduling for medication dispensing and alerts.

- **LCD Display**  
  A LiquidCrystal_I2C display provides real-time status updates, including the current time and battery status, with custom characters for a visually appealing interface.


*BiTechX is dedicated to merging cutting-edge technology with healthcare solutions for a safer, more reliable medication management system.*
