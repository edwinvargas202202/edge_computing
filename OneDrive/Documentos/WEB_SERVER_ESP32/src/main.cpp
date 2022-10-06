#include <WiFi.h>

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

const char *ssid = "motog";
const char *password = "edwinv123";

//---------------------VARIABLES GLOBALES-------------------------

String header; // Variable para guardar el HTTP request
String auxnum;
int aux;
String auxnum2;
int j = 0;
int cont = 0;
boolean flag = false;
boolean entry_method = false;
String bits[] = {"0000001", "1001111", "0010010", "0000110", "1001100", "0100100", "0100000", "0001101", "0000000", "0001100", "0001000", "0000000", "0110001", "0000001", "0110000", "0111000"};
int pines[] = {22, 23, 4, 18, 5, 21, 19};


String pagina ="<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>Servidor Web ESP32</title>"
"</head>"
"<body style='background-color: ghostwhite;'>"
"<center>"
"<h1>Servidor Web ESP32</h1>"
"<p><a href='/start'><button style='background-color: #33af37;border: none;border-radius: 20px;color: white;padding: 15px 32px; text-align: center;text-decoration: none;display: inline-block;font-size: 50px;'>EMPEZAR</button></a></p>"
"<p><a href='/finish'><button style='background-color: #b0bb1c;border: none;border-radius: 20px;color: white;padding: 15px 32px; text-align: center;text-decoration: none;display: inline-block;font-size: 50px;'>PAUSAR</button></a></p>"
"<p><a href='/reset'><button style='background-color: #b11212;border: none;border-radius: 20px;color: white;padding: 15px 32px; text-align: center;text-decoration: none;display: inline-block;font-size: 50px;'>REINICIAR</button></a></p>";


//---------------------------SETUP--------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.println("");
  pinMode(22, OUTPUT); // a
  pinMode(23, OUTPUT); // b
  pinMode(4, OUTPUT);  // c
  pinMode(18, OUTPUT); // d
  pinMode(5, OUTPUT);  // e
  pinMode(21, OUTPUT); // f
  pinMode(19, OUTPUT); // g
  pinMode(2, OUTPUT);

  // Conexión WIFI
  WiFi.mode(WIFI_STA);

  Serial.print("NUMERO DE REDES ENCONTRADAS: ");
  Serial.println(WiFi.scanNetworks());
  Serial.println("LISTA DE REDES CERCANAS");
  Serial.println("_________________________");
  for (int k = WiFi.scanNetworks()-1;k>=0;k--){
    Serial.print("* ");
    Serial.print(WiFi.SSID(k));
    Serial.print(" ");
    Serial.print(WiFi.RSSI(k));
    Serial.println();
  }
  WiFi.begin(ssid, password);
  Serial.print("CONECTANDO....");
  int i=0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.println();
  Serial.println ("***** CONEXIÓN EXITOSA A LA RED *****");
  Serial.println("___________________________________________");
  Serial.print("> ID SERVER : ");
  Serial.println(WiFi.getHostname());
  Serial.print("> DIRECCIÓN IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("> IP-PUERTA ENLACE:");
  Serial.println(WiFi.gatewayIP());
  Serial.print("> MAXIMA POTENCIA DE LA SEÑAL WIFI: ");
  Serial.print(WiFi.getTxPower());
  Serial.println(" db");
  byte bssid[6];
  Serial.print("> MAC-ROUTER ");
  for(int k=0;k<6;k++){
    Serial.print(":");
    Serial.print(bssid[k],HEX);
  }
  Serial.println();
  server.begin(); // iniciamos el servidor
  digitalWrite(2,HIGH);
}




//----------------------------LOOP----------------------------------

void loop()
{
  if (flag == true)
  {
    for (int x = 0; x < bits[cont].length(); x++)
    {
      if (bits[cont][x] == '0')
      {
        digitalWrite(pines[x], LOW);
      }else{
         digitalWrite(pines[x], HIGH);
      }
      delay(0);
    }
    delay(300);
    if(cont<15){
      cont++;
    }else{
      cont=0;
    }

  }
  WiFiClient client = server.available();
  if (client)
  {                          // Escucha a los clientes entrantes                           // Si se conecta un nuevo cliente        //
    String currentLine = ""; //
    while (client.connected())
    { // loop mientras el cliente está conectado
      // renueve las posiciones pero si no entro a algun emtodo
      if (client.available())
      {               
        Serial.println();     // si hay bytes para leer desde el cliente
        char c = client.read(); // lee un byte                  // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          { // si el byte es un caracter de salto de linea
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /start") >= 0)
            {
              flag = true;
              entry_method = true;
            }
            else if (header.indexOf("GET /finish") >= 0)
            {
              flag = false;
            }
            else if (header.indexOf("GET /reset") >= 0)
            {
              cont = 0;
              flag = true;
            }
            String num=String(cont);
            client.print(pagina+"<h1>"+num+"</h1></center></body></html>");
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Cliente desconectado");
    Serial.println("");
  }
}