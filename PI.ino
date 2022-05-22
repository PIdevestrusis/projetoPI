#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

//THINGSPEAK
String apiKey = "4PQ7TFWH4YKFU5DN"; //chave do thingspeak
const char *ssid = "Galaxy M320539"; //rede WiFI
const char *pass = "ceae7950"; //senha WiFi
const char* server = "api.thingspeak.com"; //servidor do thingspeak
WiFiClient client;

//EMAIL
int controle = 0; //variável de controle para enviar apenas 1 email
String MakerIFTTT_Key ;
; String MakerIFTTT_Event;
char *append_str(char *here, String s)
{
  int i = 0;
  while (*here++ = s[i])
  {
    i++;
  };
  return here - 1;
}
char *append_ul(char *here, unsigned long u)
{
  char buf[20];
  return append_str(here, ultoa(u, buf, 10));
}
char post_rqst[256]; char *p; char *content_length_here; char *json_start; int compi;

//SENSOR DE TEMPERATURA DS1820
// GPIO onde o DS18B20 está conectado
const int oneWireBus = 2;
// Configurando uma instância oneWire para se comunicar com qualquer dispositivo OneWire
OneWire oneWire(oneWireBus);
// Passando a referência oneWire para o sensor de temperatura Dallas
DallasTemperature sensors(&oneWire);

void setup()
{
  //iniciando Serial Monitor
  Serial.begin(115200);

  //iniciando sensor DS18B20
  sensors.begin();

  //iniciando led da ESP8266
  pinMode(LED_BUILTIN, OUTPUT);

  //conectando no WiFi
  conexaoWifi(ssid, pass);
}

void loop()
{
  //pegando temperatura do sensor DS18B20
  float celsius = lerTemperatura();

  //conectando ao ifttt (para enviar o email)
  bool iftttConnect = client.connect("maker.ifttt.com", 80);

  if (iftttConnect)
  {
    //enviando email
    lidoEnviado(celsius);
  }
  
  delay(500);
  client.stop();//desconectando

  delay(500);
  //conectando ao thingspeak
  bool tsConnect = client.connect(server, 80); // "184.106.153.149" or api.thingspeak.com

  if (tsConnect)
  {
    //enviando dados ao thingspeak
    thingspeak(apiKey, celsius);
  }
  
  delay(500);
  client.stop(); //desconectando
  Serial.println("Esperando...");
  Serial.println();

  delay(5000);
}
