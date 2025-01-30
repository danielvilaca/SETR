// Definição dos pinos
#define LDR A0
#define LED1 3
#define LED2 5
#define LED3 6
#define LED4 9

void setup() {
  // Configuração dos pinos como saída
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  Serial.begin(9600); // Inicialização da comunicação serial
}

void loop() {
  int LDRStatus = analogRead(LDR); // Leitura do valor do sensor LDR
  Serial.print("Nivel atual da luminosidade: ");
  Serial.println(LDRStatus);

  // Brilho
  if (LDRStatus < 200) {
    analogWrite(LED1, 0); // Brilho nulo (0%)
    analogWrite(LED2, 0);
    analogWrite(LED3, 0);
    analogWrite(LED4, 0);
    } 
  else if (LDRStatus >= 200 && LDRStatus < 400) {
    analogWrite(LED1, 64); // Brilho baixo (25%)
    analogWrite(LED2, 64);
    analogWrite(LED3, 64);
    analogWrite(LED4, 64);
    } 
  else if (LDRStatus >= 400 && LDRStatus < 600) {
    analogWrite(LED1, 128); // Brilho médio (50%)
    analogWrite(LED2, 128);
    analogWrite(LED3, 128);
    analogWrite(LED4, 128);
  } 
  else if (LDRStatus >= 600 && LDRStatus < 800) {
    analogWrite(LED1, 192); // Brilho alto (75%)
    analogWrite(LED2, 192);
    analogWrite(LED3, 192);
    analogWrite(LED4, 192);
  } 
  else if (LDRStatus >= 800) {
    analogWrite(LED1, 255); // Brilho máximo (100%)
    analogWrite(LED2, 255);
    analogWrite(LED3, 255);
    analogWrite(LED4, 255);
  }

  delay(500); // Pequeno atraso para estabilidade
}
