String mensajeRecibido = "";  // Guardará el mensaje completo
void setup() {
  
  Serial.begin(115200);   // Inicia el puerto serial
  
  // Imprime mensaje inicial en el Monitor Serie
  Serial.println("Placa ESP32 lista. Ingrese un mensaje:");
}

void loop() {
  // Chequea si hay datos disponibles en el buffer serial
  while (Serial.available()) {
    char caracterNuevo = Serial.read();  // Lee cada caracter del buffer
    
    if (caracterNuevo == '\n') {  // Chequea si el usuario presionó Enter (new line)
      // Imprime mensaje
      Serial.print("Ud. envió: ");
      Serial.println(mensajeRecibido);
      
      // Libera el buffer
      mensajeRecibido = "";
    } else {
      // Agrega el caracter a la cadena de mensaje
      mensajeRecibido += caracterNuevo;
    }
  }
}