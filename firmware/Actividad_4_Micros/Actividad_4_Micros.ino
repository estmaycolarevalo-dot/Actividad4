const int LED_AMARILLO = 16;
const int LED_AZUL = 17;
const int LED_ROJO = 18;
const int FREC = 5000;
const int RESOLUCION = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_AMARILLO, FREC, RESOLUCION);
    pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
}
void apagarTodo() {
  ledcWrite(LED_AMARILLO, 0);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_ROJO, LOW);
}
void loop() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    if (comando == "MODE_30") {
      apagarTodo();
      ledcWrite(LED_AMARILLO, 76);
    } 
    else if (comando == "MODE_70") {
      apagarTodo();
      digitalWrite(LED_AZUL, HIGH);
    } 
    else if (comando == "MODE_100") {
      apagarTodo();
      digitalWrite(LED_ROJO, HIGH);
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
    for (int i = 0; i < 3; i++) {
    ledcWrite(LED_AMARILLO, 255); 
    delay(150);
    digitalWrite(LED_AZUL, HIGH); 
    delay(150);
    digitalWrite(LED_ROJO, HIGH); 
    delay(150);
    apagarTodo();
    delay(150);
  }
}
