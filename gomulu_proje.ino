
#include <ESP8266WiFi.h>

//ortak wifi ağına bağlanmak için bilgiler
const char* ssid     = "hakanesp";
const char* password = "24321008";

// port numarası
WiFiServer server(80);

String header;


String output5State = "off";
String output4State = "off";
String output6State = "off";

// pin ataması
const int output5 = 5;
const int output4 = 4;


unsigned long currentTime = millis();
 
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // çıkışlar ayarlanması
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);

  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);


  // tanımlanan şifre ve wifi adından bağlanmayı başlatıyor
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // serial mönütüre ipadresini yazıyor
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // istemcileri dinliyor

  if (client) {                             // yeni bir istemci gelince çalışan blok
    Serial.println("New Client.");         
    String currentLine = "";                // istemcinin bilgilerini basıyor
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // istemci bağlı olduğu sürece çalışan döngü
      currentTime = millis();         
      if (client.available()) {              
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                     
         
          if (currentLine.length() == 0) {
           
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // butonların etkileşimini ayarlıyor
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }else if(header.indexOf("GET /6/on") >= 0){
              Serial.println("GPIO 6 on");
              output6State = "on";
              digitalWrite(output5, HIGH);
              digitalWrite(output4, HIGH);
              
                
              
            }else if(header.indexOf("GET /6/off") >= 0){
              Serial.println("GPIO 6 off");
              output6State = "off";
              digitalWrite(output5, LOW);
              digitalWrite(output4, LOW);
            }
            
            // burda html ile sayfanın basit tasarımı yapılıyor
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS ile tasarımı ayarlanıyor
             
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // sayfanın başlığı
            client.println("<body><h1>Gomulu Sistemler </h1>");
            
             
            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            
            client.println("<p>GPIO 4 - State " + output4State + "</p>");
                
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>GPIO 6 - State " + output6State + "</p>");
            if (output6State=="off") {
              client.println("<p><a href=\"/6/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/6/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("</body></html>");
            
            
            client.println();
            // while döngüsü kırılıyor
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;     
        }
      }
    }
  
    header = "";
    // bağlantıyı koparıyor
    client.stop();
    Serial.println("istemci çıktı.");
    Serial.println("");
  }
}
