#include <SPI.h>
#include <LoRa.h>

// --- Pinos LoRa ---
#define SS_PIN   15 // D8
#define RST_PIN  16 // D0
#define DIO0_PIN 5  // D1

// --- LED Externo ---
#define LED_PIN  4  // D2 (LED Externo)

// --- Frequência ---
#define FREQUENCY 433E6 

// --- Variáveis ---
unsigned long lastPacketTime = 0;
const long timeoutInterval = 2000; // 2 segundos sem sinal desliga tudo
bool isConnected = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Começa desligado

  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("Erro no LoRa Receptor!");
    while (1);
  }
  Serial.println("Receptor pronto.");
}

void loop() {
  // Verifica se chegou pacote
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    String message = "";
    
    // Lê a mensagem inteira
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    lastPacketTime = millis();
    isConnected = true;

    // --- LÓGICA DO BOTÃO ---
    if (message == "1") {
      // Recebeu "1" (Botão Solto no TX) -> LIGA O LED
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Comando: LIGAR");
    } 
    else if (message == "0") {
      // Recebeu "0" (Botão Apertado no TX) -> APAGA O LED
      digitalWrite(LED_PIN, LOW);
      Serial.println("Comando: APAGAR");
    }
  }

  // --- SEGURANÇA (TIMEOUT) ---
  // Se o transmissor for desligado ou sair de alcance, apaga o LED
  if (isConnected && (millis() - lastPacketTime > timeoutInterval)) {
    isConnected = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("SINAL PERDIDO -> DESLIGANDO TUDO");
  }
}