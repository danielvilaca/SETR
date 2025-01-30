#include <LiquidCrystal.h>

#define LEDVERM 6   // LED Vermelho (arrefecimento)
#define LEDVERD 7    // LED Verde (estabilizado)
#define IN1 8        // Entrada 1 do L293D (controle de direção) -> Entrada 2 do L293D
#define IN2 9        // Entrada 2 do L293D (controle de direção) -> Entrada 7 do L293D
#define ENA 10       // Pino de ativação (ENA) do L293D (controle de velocidade)
#define TMP A0       // Sensor TMP36

LiquidCrystal LCD(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

void setup() {
  
  pinMode(LEDVERM, OUTPUT);
  pinMode(LEDVERD, OUTPUT);
  
  // Configuração das entradas do L293D
  pinMode(IN1, OUTPUT); // Pino 2 do L293D
  pinMode(IN2, OUTPUT); // Pino 7 do L293D
  pinMode(ENA, OUTPUT); // Pino 1 do L293D (controle de velocidade)

  // Iniciar comunicação com LCD
  LCD.begin(16, 2); // Configurar o LCD com 16 colunas e 2 linhas

  // Mensagem inicial no LCD
  LCD.print("Sistema B"); 
  delay(2000);
  LCD.clear();

  Serial.begin(9600);
}

void loop() {
  
  int leituraTMP = analogRead(TMP);

  //Serial.print("Valor da leitura TMP: ");
  //Serial.println(leituraTMP);

  // Converter a leitura para a temperatura em Celsius
  float voltagem = leituraTMP * (5.0 / 1023.0);
  //float temperatura = (voltagem - 0.5) * 100.0;
  float temperatura = map(leituraTMP, 60, 90, 20, 30);


  Serial.print("Valor da leitura TMP: ");
  Serial.println(leituraTMP);
  
  // Mostrar a temperatura no LCD
  LCD.setCursor(0, 0); // Primeira linha
  
  if (temperatura > 24) {
    LCD.print("Fan ON      ");
    
    // Ativar o motor e ajustar a velocidade
    analogWrite(ENA, 255);  // Ativar o motor na velocidade máxima
    digitalWrite(IN1, HIGH); // Definir a direção do motor (sentido horário)
    digitalWrite(IN2, LOW);  // Definir a direção do motor
    
    digitalWrite(LEDVERM, HIGH); // Liga LED Vermelho
    digitalWrite(LEDVERD, LOW);  // Desliga LED Verde
  } 
  else if (temperatura < 20) {
    LCD.print("Fan OFF     ");
    
    // Desliga o motor
    analogWrite(ENA, 0);    // Desativa o motor
    digitalWrite(IN1, LOW); // Garante que o motor está desligado
    digitalWrite(IN2, LOW); // Garante que o motor está desligado
    
    digitalWrite(LEDVERM, LOW);  // Desliga LED Vermelho
    digitalWrite(LEDVERD, HIGH); // Liga LED Verde
  } 
  else {
    LCD.print("Fan Stable  ");
    
    digitalWrite(LEDVERM, LOW);  // Desliga LED Vermelho
    digitalWrite(LEDVERD, HIGH); // Liga LED Verde
  }

  // Segunda linha para temperatura
  LCD.setCursor(0, 1); // Segunda linha
  LCD.print("Temp: ");
  LCD.print(temperatura);
  LCD.print((char)223); // Símbolo de grau
  LCD.print("C  ");

  delay(1000); // Atualização a cada 1 segundo
}
