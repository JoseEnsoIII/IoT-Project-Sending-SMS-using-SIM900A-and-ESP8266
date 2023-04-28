#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

// replace with your network credentials
const char* ssid = "B315_220D4";
const char* password = "A559Q732M6L";

SoftwareSerial sim900a(2, 3); // RX, TX pins of SIM900A module
const char* phone_number = "+639675027614"; // replace with the phone number you want to send SMS to

WiFiServer server(80); // create a web server on port 80

void setup() {
  Serial.begin(115200);
  sim900a.begin(9600);
  
  // connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  server.begin();
  Serial.println("Web server started.");
  
  // print the IP address of the ESP8266 module
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client.");
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n') {
          // end of HTTP request header
          break;
        }
      }
    }
    Serial.println(request);
    // send HTTP response header
   client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
// send HTML page with button and message input field
client.println("<!DOCTYPE html>");
client.println("<html>");
client.println("<head><title>ESP8266 SIM900A SMS</title>");
client.println("<style>");
client.println("body {text-align: center;}");
client.println("input[type=\"submit\"] {background-color: blue; color: white; font-size: 16px; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer;}");
client.println("input[type=\"text\"] {font-size: 16px; padding: 10px; border: 1px solid #ccc; border-radius: 4px;}");
client.println("h1 {background-color: blue; color: white; padding: 10px; border-radius: 4px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<h1>ESP8266+SIM900A SMS</h1>");
client.println("<form method=\"POST\">");
client.println("<label for=\"message\">Message:</label>");
client.println("<br>");
client.println("<input type=\"text\" id=\"message\" name=\"message\">");
client.println("<br><br>");
client.println("<input style=\"background-color: green\" type=\"submit\" name=\"send_sms\" value=\"Send SMS\">");
client.println("</form>");
client.println("</body>");
client.println("</html>");

if (request.indexOf("send_sms") != -1) {
  // get the message from the input field
  String message = "";
  if (request.indexOf("message=") != -1) {
    message = request.substring(request.indexOf("message=") + 8);
    message.replace("+", " ");
  }
  
  Serial.println("Sending SMS...");
  Serial.print("Message: ");
  Serial.println(message);
  // AT commands to send SMS via SIM900A module
  sim900a.println("AT+CMGF=1");
  delay(500);
  sim900a.println("AT+CMGS=\"" + String(phone_number) + "\"");
  delay(500);
  sim900a.println(message);
  delay(500);
  sim900a.write((byte) 26);
  delay(500);
  Serial.println("SMS sent.");
  
}if (request.indexOf("send_sms") != -1) {
  // get the message from the input field
  String message = "";
if (request.indexOf("message=") != -1) {
    message = request.substring(request.indexOf("message=") + 8);
    message.replace("+", " ");
    Serial.print("Received message: ");
    Serial.println(message); // print the message to serial monitor
  }
  
  Serial.println("Sending SMS...");
  Serial.print("Message: ");
  Serial.println(message);
  // AT commands to send SMS via SIM900A module
  sim900a.println("AT+CMGF=1");
  delay(1000);
  sim900a.println("AT+CMGS=\"" + String(phone_number) + "\"");
  delay(1000);
  sim900a.println(message);
  delay(1000);
  sim900a.write((byte) 26);
  delay(3000);

}

// close the connection
client.stop();
Serial.println("Client disconnected.");

  }
}
