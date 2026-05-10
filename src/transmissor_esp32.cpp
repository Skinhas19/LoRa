#include <SPI.h>
#include <LoRa.h>

// --- Configuração dos Pinos (Padrão ESP32) ---
#define SS_PIN   5   // CS
#define RST_PIN  14  // Reset
#define DIO0_PIN 2   // IRQ
#define BUTTON_PIN 4 // GPIO 4 (Botão ligado entre este pino e o GND)

// --- Frequência ---
// Mantenha igual ao receptor (915E6 ou 433E6)
#define FREQUENCY 433E6 

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // No ESP32 também usamos INPUT_PULLUP
  // Botão Solto = HIGH (1), Botão Apertado = LOW (0)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Iniciando LoRa no ESP32...");

  // Configura os pinos antes de iniciar
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
  
  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("ERRO: Falha ao iniciar LoRa!");
    while (1); // Trava o código se falhar
  }
  Serial.println("LoRa Transmissor (ESP32) Iniciado!");
}

void loop() {
  // Lê o estado do botão
  int buttonState = digitalRead(BUTTON_PIN);

  LoRa.beginPacket();
  
  if (buttonState == LOW) {
    // Botão APERTADO (conectado ao GND) -> Envia "0" (Para apagar)
    LoRa.print("0");
    Serial.println("Enviando: 0 (Apagar LED)");
  } else {
    // Botão SOLTO (Pull-up interno) -> Envia "1" (Para ligar/manter ligado)
    LoRa.print("1");
    // Comentei o print abaixo para não poluir demais o serial, pois ele envia muito rápido
    // Serial.println("Enviando: 1 (Ligar LED)");
  }
  
  LoRa.endPacket();

  // Delay importante para estabilidade
  delay(100); 
}