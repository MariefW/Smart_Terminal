#include "AntaresESP8266HTTP.h"
#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>

// Set pin Tx ke pin D6 dan Rx ke pin D7
PZEM004Tv30 pzem(D6, D7);

// Inisialisasi Variabel Konektivitas
#define ACCESSKEY "ce1971d35a88d305:5e15f019ac0590bc"
#define WIFISSID "Coles"
#define PASSWORD "123456"

// Inisialisasi Variabel Project Antares
#define projectName "Smart_Terminal"
#define deviceName "Terminal_1"
#define deviceControlName "control_terminal_1"

#define led D4
#define relay D5

AntaresESP8266HTTP antares(ACCESSKEY);

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(relay, OUTPUT);
  
  antares.setDebug(true);
//  antares.wifiConnection(WIFISSID, PASSWORD);
  antares.wifiConnectionNonSecure(WIFISSID,PASSWORD);
}

void loop() {
float detik = (millis() / 1000);
  float menit = detik / 60.0;
  float jam = menit / 60.0;
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  float energy = (power / 1000) * jam;
  float rupiah = energy * 1467.28;

  // Mengambil nilai tegangan pada sensor PZEM
  if (voltage != NAN) {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  } else {
    Serial.println("Error reading voltage");
  }

  // Mengambil nilai arus pada sensor PZEM
  if (current != NAN) {
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  } else {
    Serial.println("Error reading current");
  }

  // Mengambil nilai Daya pada sensor PZEM
  if (current != NAN) {
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  } else {
    Serial.println("Error reading power");
  }

  // Mengambil nilai Energi pada sensor PZEM
  // float energy = pzem.energy();
  if (current != NAN) {
    Serial.print("Energy: "); Serial.print(energy, 3); Serial.println("kWh");
  } else {
    Serial.println("Error reading energy");
  }

  // Mengambil nilai Frekuensi pada sensor PZEM
  if (current != NAN) {
    Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
  } else {
    Serial.println("Error reading frequency");
  }

  // Mengambil nilai Power Faktor pada sensor PZEM
  if (current != NAN) {
    Serial.print("PF: "); Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }
  Serial.println();

  Serial.print("Waktu (detik ) : ");Serial.println(detik );

  
  // Store latest value in buffer
//  antares.get(projectName, deviceName); 
  antares.getNonSecure(projectName, deviceControlName);
  
    //  Check if we're actually getting data
  if (antares.getSuccess()) {
    // Get each values
    String RelayState = antares.getString("state");

    if (RelayState == "ON") {
      Serial.println("LAMPU HIDUP");
      digitalWrite(led, LOW);
      digitalWrite(relay, HIGH);


    } else if (RelayState == "OFF") {
      Serial.println("LAMPU MATI");
      digitalWrite(led, HIGH);
      digitalWrite(relay, LOW);
      
      
    }

    // Print each values
    Serial.println("Relay State: " + String(RelayState));
  }
  
  // Add variable data to storage buffer
  
  antares.add("tegangan", voltage);
  antares.add("arus", current);
  antares.add("daya", power);
  antares.add("energi", energy);
  antares.add("rupiah", rupiah);


  // Send from buffer to Antares
//  antares.send(projectName, deviceName);
 antares.sendNonSecure(projectName, deviceName); 
  delay(2000);

}
