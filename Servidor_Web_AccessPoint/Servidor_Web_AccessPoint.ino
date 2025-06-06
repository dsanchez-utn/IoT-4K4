#include <WiFi.h>

WiFiServer server(80);

const char* ssid = "ESP32-Access-Point";
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
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop() {
  WiFiClient client = server.accept();  

  if (client) {

    String currentLine = "";        //
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /on") >= 0) {
              digitalWrite(PIN_RELAY, HIGH);
              digitalWrite(LED_BUILTIN, HIGH);
              client.println(paginaOn);
            } else if (header.indexOf("GET /off") >= 0) {
              digitalWrite(PIN_RELAY, LOW);
              digitalWrite(LED_BUILTIN, LOW);
              client.println(paginaOff);
            } else {
               client.println(pagina);
            }

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
}
