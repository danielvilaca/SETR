// Pinos
const int TRIG = 3;       // Pino Trigger do HC-SR04
const int ECHO = 4;       // Pino Echo do HC-SR04
const int LEDVERM = 12;   // LED vermelho
const int BUZZ = 13;      // Buzzer
const int BTN = 8;        // Botão para desarmar

// Variáveis de controle
bool alarmArmed = true;        // Estado do alarme (armado por padrão)
bool lastButtonState = HIGH;   // Estado anterior do botão
unsigned long lastDebounceTime = 0; // Para debounce
unsigned long debounceDelay = 50;   // Tempo para debounce
unsigned long lastAlarmTime = 0;    // Para o controle do alarme
unsigned long alarmStartTime = 0;   // Marca o início do alarme

bool isAlarmActive = false;    // Indica se o alarme está ativo ou no intervalo
const int DISTANCE_THRESHOLD = 50; // Distância mínima (em cm) para ativar o alarme

void setup() 
{
  pinMode(TRIG, OUTPUT);       // Configura o pino Trigger como saída
  pinMode(ECHO, INPUT);        // Configura o pino Echo como entrada
  pinMode(LEDVERM, OUTPUT);    // Configura o LED como saída
  pinMode(BUZZ, OUTPUT);       // Configura o buzzer como saída
  pinMode(BTN, INPUT_PULLUP);  // Configura o botão como entrada com pull-up interno
  Serial.begin(9600);          // Inicia a comunicação serial
}

void loop() 
{
  // Leitura do estado do botão com debounce
  int buttonState = digitalRead(BTN);
  if (buttonState == LOW && lastButtonState == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
    alarmArmed = !alarmArmed; // Alterna o estado do alarme
    lastDebounceTime = millis(); // Atualiza o tempo do debounce

    // Debug: Estado atual do alarme
    if (alarmArmed) 
    {
      Serial.println("Alarme ARMADO.");
    } else {
      Serial.println("Alarme DESARMADO.");
      isAlarmActive = false; // Desativa qualquer alarme ativo
      digitalWrite(LEDVERM, LOW); // Garante que o LED esteja apagado
      noTone(BUZZ);              // Garante que o buzzer esteja desligado
    }
  }
  lastButtonState = buttonState; // Atualiza o estado anterior do botão

  // Verifica o estado do sensor ultrassônico se o alarme estiver armado
  if (alarmArmed) 
  {
    int distance = measureDistance(); // Mede a distância

    if (distance > 0 && distance <= DISTANCE_THRESHOLD && !isAlarmActive) 
    {
      // Inicia o alarme
      isAlarmActive = true;
      Serial.println("Movimento detectado! Alarme ativado.");
    }

    // Ciclo contínuo de 10 segundos de som seguido de 5 segundos de pausa
    if (isAlarmActive) 
    {
      unsigned long currentTime = millis();

      // Controle do ciclo de tocar/pausa
      static bool isAlarmPlaying = true; // Indica se está a tocar ou em pausa
      static unsigned long cycleStartTime = currentTime; // Tempo de início do ciclo atual

      if (isAlarmPlaying && currentTime - cycleStartTime <= 10000) 
      {
        // Tocar o alarme (LED pisca e buzzer toca)
        digitalWrite(LEDVERM, millis() % 500 < 250 ? HIGH : LOW); // LED pisca a cada 250ms
        tone(BUZZ, 1000); // Sinal sonoro com frequência de 1000Hz
      } 
      else if (isAlarmPlaying && currentTime - cycleStartTime > 10000) 
      {
        // Termina o ciclo de tocar e inicia a pausa
        isAlarmPlaying = false;
        cycleStartTime = currentTime; // Reinicia o tempo do ciclo
        digitalWrite(LEDVERM, LOW); // Garante que o LED esteja apagado
        noTone(BUZZ);              // Silencia o buzzer
        Serial.println("Pausa do alarme.");
      } 
      else if (!isAlarmPlaying && currentTime - cycleStartTime >= 5000) 
      {
        // Termina a pausa e volta a tocar
        isAlarmPlaying = true;
        cycleStartTime = currentTime; // Reinicia o tempo do ciclo
        Serial.println("Alarme retomado.");
      }
    }
  }

  delay(10); // Pequeno atraso para estabilidade
}

// Função para medir a distância com o sensor HC-SR04
int measureDistance() 
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH); // Mede o tempo de ida e volta do sinal ultrassônico
  int distance = duration * 0.034 / 2; // Calcula a distância em cm

  Serial.print("Distância medida: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}
