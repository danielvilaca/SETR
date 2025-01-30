#include <IRremote.hpp>
#include <Servo.h>

// Definir pinos
const int IR_RECEIVE_PIN = 2;
const int SERVO_PIN = 9;
const int LED_PIN = 13;

// Objeto servo
Servo gateServo;

// Códigos IR para abrir e fechar o portão
const uint32_t OPEN_GATE_CODE = 0xF609FF00;
const uint32_t CLOSE_GATE_CODE = 0xF807FF00;

// Variáveis para controle
bool gateOpened = false;
bool interruptAction = false; // Flag de interrupção
unsigned long previousMillis = 0;
//unsigned long previousMillisBlink = 0;
//unsigned long previousMillisServo = 0;
unsigned long gateTimer = 0;
const unsigned long blinkInterval = 250;
const unsigned long autoCloseDelay = 5000; // Tempo antes de fechar automaticamente

void setup() {
  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN); // Inicializar receptor IR
  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Portão fechado
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Sistema pronto para receber sinais IR.");
}

void loop() {

  // Receber código IR
  if (IrReceiver.decode()) {
    uint32_t receivedCode = IrReceiver.decodedIRData.decodedRawData;

    if (receivedCode != 0xFFFFFFFF) { // Ignorar repetições
      Serial.print("Código IR recebido: ");
      Serial.println(receivedCode, HEX);

      if (receivedCode == OPEN_GATE_CODE && !gateOpened) {
        interruptAction = true; // Interromper ação atual
        abrirPortao();
      } else if (receivedCode == CLOSE_GATE_CODE && gateOpened) {
        interruptAction = true; // Interromper ação atual
        fecharPortao();
      }
    }

    IrReceiver.resume(); // Preparar para o próximo sinal
  }

  // Fechar automaticamente
  if (gateOpened && millis() - gateTimer >= autoCloseDelay) {
    fecharPortao();
  }

  // Controlo de piscar do LED
  if (gateOpened) { 
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  } else {
    pinMode(LED_PIN, INPUT); // Desativa o LED eletricamente
  }
}

void abrirPortao() {
  Serial.println("Abrindo portão...");
  pinMode(LED_PIN, OUTPUT); // Ativa o LED novamente ao abrir o portão
  gateOpened = true;
  gateTimer = millis(); // Iniciar temporizador de fechamento automático
  interruptAction = false; // Reiniciar flag
  moverServo(0, 90);    // Abrir portão (0 a 90 graus)
  Serial.println("Portão aberto.");
}

void fecharPortao() {
  Serial.println("Fechando portão...");
  gateOpened = false;
  interruptAction = false; // Reiniciar flag
  moverServo(90, 0);    // Fechar portão (90 a 0 graus)
  digitalWrite(LED_PIN, LOW);
  Serial.println("Portão fechado.");
}

void moverServo(int inicio, int fim) {
  int step = (fim > inicio) ? 1 : -1;
  for (int pos = inicio; pos != fim + step; pos += step) {
    // Verificar interrupção
    if (interruptAction) {
      Serial.println("Ação interrompida!");
      return; // Sair imediatamente da função
    }

    gateServo.write(pos);

    // Controlo de piscar do LED durante o movimento
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
    delay(15); // Reduz velocidade do movimento
  }
}