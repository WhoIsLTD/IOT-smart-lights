#include <ESP8266WiFi.h>

const char *ssid = "Redmi";
const char *password = "12346789";

const int ledPin = 15;

const int ldrPin = 5;
int check = 0;
int ldrStatus;
WiFiServer server(80);

  
void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  //  digitalWrite(ledPin,LOW);

  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("NodeMCU Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}


void loop()
{

  ldrStatus = digitalRead(ldrPin);
  if (check == 0) {
    if (ldrStatus == 1)
    {
      digitalWrite(ledPin, LOW);
    }
    else
    {
      digitalWrite(ledPin, HIGH);
    }
  }
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Wait until the client sends some data
  Serial.println("Hello New client");
  while (!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value;

  if (request.indexOf("/CONTROL/LED=ON") != -1)
  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
    check = 1;
  }
  if (request.indexOf("/CONTROL/LED=OFF") != -1)
  {
    digitalWrite(ledPin, LOW);
    value = LOW;
    check = 1;
  }
  if (request.indexOf("/AUTO") != -1)
  {
    check = 0;
  }

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #77878A;}</style></head>");
  client.println("<h1>ESP8266 Web Server Light Sensor</h1>");
  client.println("<h3>Control</h3><br>");
  if (value == LOW)
  {
    client.println("<p><a href=\"/CONTROL/LED=ON\"\"><button class=\"button\" id = \"btn0\">ON</button></a></p>");
  }
  else
  {
    client.println("<p><a href=\"/CONTROL/LED=OFF\"\"><button class=\"button button2\"  id = \"btn1\">OFF</button></a></p>");
  }
  client.println("<h3>Auto</h3><br>");
  client.println("<p><a href=\"/AUTO\"\"><button class=\"button\" id = \"btn2\">AUTO</button></a></p>");

  client.println("<script>");
  client.println("var b = document.getElementById(\"btn2\");");
  if ( check == 1) {
    client.println("b.style.background = '#77878A';");
  }
  else {
    client.println("b.style.background = '#195B6A';");
  }
  client.println("</script>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  Serial.print("bright, turn off: ");
  Serial.println(ldrStatus);
}
