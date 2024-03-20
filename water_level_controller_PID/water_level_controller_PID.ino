#include <NewPing.h>
#include <PID_v1.h>
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 200
#define RELAY_PIN 12
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
// PID variables
double setpoint = 8; // Tinggi air yang diinginkan dalam centimeter
double input, output;
double Kp = 1, Ki = 0.5, Kd = 2; // Nilai Kp, Ki, Kd yang dapat disesuaikan
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
bool pumpActive = false; // Untuk memantau status pompa air
int tinggiWadah = 13; // Tinggi dasar wadah kosong dalam centimeter
unsigned long startTime = 0;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);
  startTime = millis();
}
void loop() {
  delay(1000);
  input = sonar.ping_cm(); // Baca tinggi air dari sensor ultrasonik
  int tinggiAir = tinggiWadah - input; // Menghitung tinggi aktual air dalam wadah
  myPID.Compute(); // Hitung output PID berdasarkan input
  unsigned long currentTime = millis(); // Dapatkan waktu sekarang
  unsigned long elapsedTime = (currentTime - startTime)/1000; //Hitung waktu yang telah berlalu
  if (output >= 0) {
    digitalWrite(RELAY_PIN, HIGH); // Aktifkan relay jika output dari PID positif dan pompa tidak aktif sebelumnya
    pumpActive = true;
    Serial.println("Water pump ON");
    Serial.print("Setpoint: ");
    Serial.println(setpoint);
    Serial.print("TinggiAir: ");
    Serial.println(tinggiAir);
    Serial.print("Output: ");
    Serial.println(output);
    Serial.print("Time: ");
    Serial.println(elapsedTime);
    Serial.print("Input: ");
    Serial.println(input);
    Serial.println("-------------------------------");
  }
  if (tinggiAir >= setpoint) {
    pumpActive = false;
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Water pump OFF");
    Serial.print("Setpoint: ");
    Serial.println(setpoint);
    Serial.print("TinggiAir: ");
    Serial.println(tinggiAir);
    Serial.print("Output: ");
    Serial.println(output);
    Serial.print("Time: ");
    Serial.println(elapsedTime);
    Serial.print("Input: ");
    Serial.println(input);
    Serial.println("-------------------------------");
  }
}