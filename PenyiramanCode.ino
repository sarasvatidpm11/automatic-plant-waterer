#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C dan ukuran LCD 

#define Float_Switch_Bawah 12
#define Soil_Sensor A0
#define Relay_Pompa 4

// Fungsi Perhitungan Kelembaban Tanah
float bacaKelembabanTanah(int pinSensor) {
  int hasilPembacaan = analogRead(pinSensor);
  return (1023.0 - hasilPembacaan) / 1023.0 * 100;
}

void setup() {
  pinMode(Float_Switch_Atas, INPUT_PULLUP);
  pinMode(Float_Switch_Bawah, INPUT_PULLUP);
  pinMode(Relay_Pompa, OUTPUT);

  lcd.init(); // Inisialisasi LCD
  lcd.backlight(); // Nyalakan backlight LCD  

  Serial.begin(9600);
}

void loop() {
  int tinggiAirBawah = digitalRead(Float_Switch_Bawah);
  
   struct Tumbuhan {
    const char *nama;
    int kelembabanMin;
    int kelembabanMax;
  };

  //Kelembaban tanah sesuai jenis tanaman
  Tumbuhan kelembaban[] = {
    {"Cabe", 40, 60},
    {"Tomat", 40, 60},
    {"Terong", 40, 60},
    {"Jagung", 30, 50},
    {"Melon", 30, 70},
    {"Semangka", 50, 70},
  };

  if(tinggiAirBawah == HIGH) {
    float kelembabanTanah = bacaKelembabanTanah(Soil_Sensor);
    Serial.println("Persentase Kelembaban Tanah = " + String(kelembabanTanah) + "%");

    int perbedaanKelembaban = kelembaban[0].kelembabanMin - kelembabanTanah;
    Serial.println("Perbedaan Kelembaban terhadap kelembaban yang diinginkan = " + String(perbedaanKelembaban) + "%");

    //Tampilkan di LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil: " + String(kelembabanTanah) + "%");

    
    if (kelembabanTanah < kelembaban[0].kelembabanMin) {
      // Hidupkan pompa jika kelembaban tanah di bawah ambang batas
      Serial.println("Relay pompa hidup");
      digitalWrite(Relay_Pompa, LOW);
      //Tampilkan di LCD
      lcd.setCursor(0, 1);
      lcd.print("Pompa hidup");
    } else {
      // Matikan pompa jika kelembaban tanah sudah mencukupi
      Serial.println("Relay pompa mati");
      digitalWrite(Relay_Pompa, HIGH);
      //Tampilkan di LCD
      lcd.setCursor(0, 1);
      lcd.print("Pompa mati");
    }
  } else {
    // Matikan pompa karena air tandon habis
    Serial.println("Relay pompa mati, silahkan isi air tandon terlebih dahulu!");
    digitalWrite(Relay_Pompa, HIGH);
    //Tampilkan di LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Air sudah habis");
    lcd.setCursor(0, 1);
    lcd.print("Silahkan diisi");
  }

  delay(1000);
}
