#include <WiFi.h>

WiFiServer server(80);

const char* ssid = "ACNET2";
const char* password = "";

String header;  // Variable para guardar el HTTP request
const int PIN_RELAY = 26 ;   // 2

//------------------------CODIGO HTML------------------------------
String pagina = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='utf-8' />"
                "<title>Servidor Web ESP32</title>"
                "</head>"
                "<body>"
                "<center>"
                "<h1>Servidor Web ESP32</h1>"
                "<p><a href='/on'><button style='height:50px;width:150px;color:green'>ON</button></a></p>"
                "<p><a href='/off'><button style='height:50px;width:150px;color:red'>OFF</button></a></p>"
                "</center>"
                "</body>"
                "</html>";

String paginaOn = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='utf-8' />"
                "<title>Servidor Web ESP32</title>"
                "</head>"
                "<body>"
                "<center>"
                "<h1>Servidor Web ESP32</h1>"
                "<p><a href='/on'><button style='height:50px;width:150px;color:green'>Activado</button></a></p>"
                "<p><a href='/off'><button style='height:50px;width:150px;color:red'>OFF</button></a></p>"
                "</center>"
                "</body>"
                "</html>";

String paginaOff = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='utf-8' />"
                "<title>Servidor Web ESP32</title>"
                "</head>"
                "<body>"
                "<center>"
                "<h1>Servidor Web ESP32</h1>"
                "<p><a href='/on'><button style='height:50px;width:150px;color:green'>ON</button></a></p>"
                "<p><a href='/off'><button style='height:50px;width:150px;color:red'>Desactivado</button></a></p>"
                "</center>"
                "</body>"
                "</html>";
void setup() {
  Serial.begin(115200);
  Serial.println("");

  pinMode(PIN_RELAY, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 500 milisegundos para preguntar nuevemante si se puede conectar
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("SSID:  ");
  Serial.println(WiFi.SSID());
  Serial.print("ID:   ");
  Serial.println(WiFi.localIP());
  server.begin();  // iniciamos el servidor
}

void loop() {
//  WiFiClient client = server.available();  // Escucha a los clientes entrantes. Verifica si hay alguna solicitud de conexión pendiente
  // Se declara un objeto client que representa a un navegador web (o dispositivo) que se conecta al ESP32.
  WiFiClient client = server.accept();       

  if (client) {   // Si se conecta un nuevo cliente
    Serial.println("NUEVO CLIENTE conectado");
    //Se inicializa una cadena vacía para leer línea por línea lo que ese cliente está pidiendo.
    String currentLine = "";        
    while (client.connected()) {    // loop mientras el cliente está conectado
      if (client.available()) {     // si hay bytes para leer desde el cliente

        char c = client.read();     // lee un byte y lo elimina del buffer
        header += c;
        // si el byte es un caracter de salto de linea
        if (c == '\n') {                        
          // y la línea actual está vacía, significa que terminó el encabezado del mensaje HTTP, y ahora se puede responder
          if (currentLine.length() == 0) {      
            client.println("HTTP/1.1 200 OK");        // la solicitud HTTP se ha procesado correctamente
            // establece el tipo de contenido que se enviará al cliente en la respuesta. En este caso se trata de una página HTML
            client.println("Content-type:text/html"); 
            client.println("Connection: close");      // la conexión entre el servidor y el cliente se cerrará después de enviar la respuesta
            client.println();

            //busca la primera aparición de "GET /on" y devuelve la posición donde se encuentra. Si no se encuentra devuelve -1
            if (header.indexOf("GET /on") >= 0) {   
              digitalWrite(PIN_RELAY, HIGH);
              digitalWrite(LED_BUILTIN, HIGH);
              client.println(paginaOn);  // Muestra página web de encendido del pin

            } else if (header.indexOf("GET /off") >= 0) {  // busca la primera aparición de "GET /off"
              digitalWrite(PIN_RELAY, LOW);
              digitalWrite(LED_BUILTIN, LOW);
        
              client.println(paginaOff);  // Muestra página web de apagado del pin
            } else {
        
               client.println(pagina);  // Muestra página web inicial
            }

            // La respuesta HTTP debe terminar con una linea en blanco, indicando que el contenido finalizó
            client.println();  

            break;    // Finaliza el procesamiento del cliente actual. No sigue leyendo más líneas
          } else {  // si tenemos una nueva linea limpiamos currentLine
          
          // si tenemos una nueva linea limpiamos currentLine para preparar la lectura de la próxima línea del encabezado HTTP
            currentLine = ""; 
          }
        } else if (c != '\r') {  // si "c" es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
  }
}
