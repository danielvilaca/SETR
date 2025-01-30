#include <Arduino_FreeRTOS.h>

// Definição dos pinos
#define LDR A2
#define LED1 3
#define LED2 5
#define LED3 6
#define LED4 9
#define FIXED_RESISTOR 5000 // Resistor fixo de 5kΩ
#define SUPPLY_VOLTAGE 5.0   // Tensão de alimentação

// Variáveis globais
float voltage = 0;
float resistance = 0;
float lux = 0;
int LDRStatus = 0;

// Protótipos das tarefas
void TaskReadSensor(void *pvParameters);
void TaskUpdateLEDs(void *pvParameters);

void setup() {
  // Configuração dos pinos como saída
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  Serial.begin(9600); // Inicialização da comunicação série

  // Criação das tarefas FreeRTOS
  xTaskCreate(TaskReadSensor, "ReadSensor", 128, NULL, 2, NULL);
  xTaskCreate(TaskUpdateLEDs, "UpdateLEDs", 128, NULL, 1, NULL);

  // Inicia o agendador do FreeRTOS
  vTaskStartScheduler();
}

void loop() {
  // O loop principal é gerido pelo FreeRTOS
}

// Tarefa para ler o sensor LDR
void TaskReadSensor(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    // Leitura do valor do LDR
    LDRStatus = analogRead(LDR);

    // Converter o valor do ADC para tensão
    voltage = LDRStatus * (SUPPLY_VOLTAGE / 1023.0);

    // Calcular a resistência do LDR
    resistance = (SUPPLY_VOLTAGE - voltage) * FIXED_RESISTOR / voltage;

    // Estimar a luminosidade em lux (constantes A e B são específicas do LDR)
    const float A = 1000; // Constante de exemplo (substituir por valores do datasheet)
    const float B = 2.8; // Constante de exemplo (substituir por valores do datasheet)
    lux = A * pow((resistance / 1000.0), B);

    // Imprimir os resultados
    Serial.print("Tensão: ");
    Serial.print(voltage);
    Serial.print(" V, Resistência: ");
    Serial.print(resistance);
    Serial.print(" ohms, Lux: ");
    Serial.println(lux);

    vTaskDelay(pdMS_TO_TICKS(500)); // Aguarda 500ms
  }
}

// Tarefa para atualizar os LEDs com base no valor do LDR
void TaskUpdateLEDs(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    // Controlar o brilho dos LEDs
    if (LDRStatus < 200) {
      analogWrite(LED1, 0); // Brilho nulo (0%)
      analogWrite(LED2, 0);
      analogWrite(LED3, 0);
      analogWrite(LED4, 0);
    } else if (LDRStatus >= 200 && LDRStatus < 400) {
      analogWrite(LED1, 64); // Brilho baixo (25%)
      analogWrite(LED2, 64);
      analogWrite(LED3, 64);
      analogWrite(LED4, 64);
    } else if (LDRStatus >= 400 && LDRStatus < 600) {
      analogWrite(LED1, 128); // Brilho médio (50%)
      analogWrite(LED2, 128);
      analogWrite(LED3, 128);
      analogWrite(LED4, 128);
    } else if (LDRStatus >= 600 && LDRStatus < 800) {
      analogWrite(LED1, 192); // Brilho alto (75%)
      analogWrite(LED2, 192);
      analogWrite(LED3, 192);
      analogWrite(LED4, 192);
    } else if (LDRStatus >= 800) {
      analogWrite(LED1, 255); // Brilho máximo (100%)
      analogWrite(LED2, 255);
      analogWrite(LED3, 255);
      analogWrite(LED4, 255);
    }

    vTaskDelay(pdMS_TO_TICKS(100)); // Aguarda 100ms
  }
}