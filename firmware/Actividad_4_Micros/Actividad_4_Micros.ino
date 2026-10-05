// Asignación de pines para los LEDs
const int LED_AMARILLO = 16;
const int LED_AZUL = 17;
const int LED_ROJO = 18;

// Configuración PWM (Sintaxis ESP32 Arduino Core v3.x)
const int FREC = 5000;
const int RESOLUCION = 8; // Rango de 0 a 255

void setup() {
  Serial.begin(115200);
  
  // Configurar canal PWM en el LED Amarillo
  ledcAttach(LED_AMARILLO, FREC, RESOLUCION);
  
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
}

void apagarTodo() {
  ledcWrite(LED_AMARILLO, 0); // Apagar PWM amarillo
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_ROJO, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "MODE_30") {
      apagarTodo();
      ledcWrite(LED_AMARILLO, 76); // 30% de intensidad (~76/255)
    } 
    else if (comando == "MODE_70") {
      apagarTodo();
      digitalWrite(LED_AZUL, HIGH); // 70% Intensidad
    } 
    else if (comando == "MODE_100") {
      apagarTodo();
      digitalWrite(LED_ROJO, HIGH); // 100% Intensidad
    } 
    else if (comando == "SEQ_1") {
      apagarTodo();
      secuenciaModo1();
    } 
    else if (comando == "SEQ_2") {
      apagarTodo();
      secuenciaModo2();
    }
  }
}

void secuenciaModo1() {
  // Secuencia 1: Parpadeo alternado Azul y Rojo
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_AZUL, HIGH); 
    delay(200);
    digitalWrite(LED_AZUL, LOW);
    
    digitalWrite(LED_ROJO, HIGH); 
    delay(200);
    digitalWrite(LED_ROJO, LOW);
  }
}

void secuenciaModo2() {
  // Secuencia 2: Barrido en cascada Amarillo -> Azul -> Rojo
  for (int i = 0; i < 3; i++) {
    // Encendido de LED Amarillo mediante PWM
    ledcWrite(LED_AMARILLO, 255); 
    delay(150);
    
    digitalWrite(LED_AZUL, HIGH); 
    delay(150);
    
    digitalWrite(LED_ROJO, HIGH); 
    delay(150);
    
    // Apagar todos los LEDs al finalizar la pasada
    apagarTodo();
    delay(150);
  }
}