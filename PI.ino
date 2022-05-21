///////////////CÓDIGO QUE FUNCIONOU COM THINGSPEAK and EMAILLLLLLL////////////////

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
  // Iniciando Serial Monitor
  Serial.begin(115200);
  // Iniciando sensor DS18B20
  sensors.begin();

  //conectando no WiFi
  Serial.println("Conectando em ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
}

void loop()
{
  //pegando temperatura do sensor DS18B20
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print("Temperatura: ");
  Serial.print(temperatureC);
  Serial.print("ºC");
  //  float temperatureF = sensors.getTempFByIndex(0);
  //  Serial.print(temperatureF);
  //  Serial.println("ºF");

  //conectando ao ifttt (para enviar o email)
  bool iftttConnect = client.connect("maker.ifttt.com", 80);

  if (iftttConnect)
  {
    if (temperatureC < 35) //Temperatura - de 35
    {
      //enviar aviso de hipotermia
      Serial.println(" --> HIPOTERMIA");

      if (controle != 1)
      {
        //enviando email
        email("pidevestrusis@gmail.com", "HIPOTERMIA", "Temperatura Baixa!");
      }

      controle = 1;
    }
    else if (temperatureC > 37) //Temperatura + de 37
    {
      //enviar aviso de febre
      Serial.println(" --> FEBRE");

      if (controle != 2)
      {
        //enviando email
        email("pidevestrusis@gmail.com", "FEBRE", "Temperatura Alta!");
      }

      controle = 2;
    }
    else //temperatura entre 35 e 37
    {
      //não enviar aviso
      Serial.println(" --> TEMPERATURA NORMAL");
      controle = 0;
    }
  }
  delay(500);
  client.stop();

  delay(500);
  //conectando ao thingspeak
  bool tsConnect = client.connect(server, 80); // "184.106.153.149" or api.thingspeak.com

  if (tsConnect)
  {
    //enviando dados ao thingspeak
    thingspeak(apiKey, temperatureC);
  }
  delay(500);
  client.stop(); //desconectando
  Serial.println("Esperando...");
  Serial.println();

  delay(5000);
}
