//FUNÇÃO PARA CONECTAR WIFI
void conexaoWifi(String rede, String senha)
{
  Serial.println("Conectando em ");
  Serial.println(rede);
  WiFi.begin(rede, senha);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
}

//FUNÇÃO PARA LER TEMPERATURA DO SENSOR DS18B20
float lerTemperatura()
{
  sensors.requestTemperatures();
  float temperaturaC = sensors.getTempCByIndex(0);
  Serial.print("Temperatura: ");
  Serial.print(temperaturaC);
  Serial.print("ºC");
  //  float temperatureF = sensors.getTempFByIndex(0);
  //  Serial.print(temperatureF);
  //  Serial.println("ºF");

  return temperaturaC;
}

//FUNÇÃO DE ENVIAR EMAIL
//valor1 = endereço de email, valor2 = titulo do email, valor3 = corpo do email
void email(String valor1, String valor2, String valor3)
{
  MakerIFTTT_Key = "oOMSFKDYWxPcOPiUvbmx7kbi-zhfAMOd7MWZYbiIPJ0";
  MakerIFTTT_Event = "email";
  p = post_rqst;
  p = append_str(p, "POST /trigger/");
  p = append_str(p, MakerIFTTT_Event);
  p = append_str(p, "/with/key/");
  p = append_str(p, MakerIFTTT_Key);
  p = append_str(p, " HTTP/1.1\r\n");
  p = append_str(p, "Host: maker.ifttt.com\r\n");
  p = append_str(p, "Content-Type: application/json\r\n");
  p = append_str(p, "Content-Length: ");
  content_length_here = p;
  p = append_str(p, "NN\r\n");
  p = append_str(p, "\r\n");
  json_start = p;
  p = append_str(p, "{\"value1\":\"");
  p = append_str(p, valor1);
  p = append_str(p, "\",\"value2\":\"");
  p = append_str(p, valor2);
  p = append_str(p, "\",\"value3\":\"");
  p = append_str(p, valor3);
  p = append_str(p, "\"}");

  compi = strlen(json_start);
  content_length_here[0] = '0' + (compi / 10);
  content_length_here[1] = '0' + (compi % 10);
  client.print(post_rqst);
  Serial.println("Email enviado");
}

//FUNÇÃO DE ENVIAR SMS
//valor1, valor2 e valor3 = mensagem SMS
void sms(String valor1,String valor2,String valor3)
{
  MakerIFTTT_Key = "oOMSFKDYWxPcOPiUvbmx7kbi-zhfAMOd7MWZYbiIPJ0";
  MakerIFTTT_Event = "sms";
  p = post_rqst;
  p = append_str(p, "POST /trigger/");
  p = append_str(p, MakerIFTTT_Event);
  p = append_str(p, "/with/key/");
  p = append_str(p, MakerIFTTT_Key);
  p = append_str(p, " HTTP/1.1\r\n");
  p = append_str(p, "Host: maker.ifttt.com\r\n");
  p = append_str(p, "Content-Type: application/json\r\n");
  p = append_str(p, "Content-Length: ");
  content_length_here = p;
  p = append_str(p, "NN\r\n");
  p = append_str(p, "\r\n");
  json_start = p;
  p = append_str(p, "{\"value1\":\"");
  p = append_str(p, valor1);
  p = append_str(p, "\",\"value2\":\"");
  p = append_str(p, valor2);
  p = append_str(p, "\",\"value3\":\"");
  p = append_str(p, valor3);
  p = append_str(p, "\"}");

  compi = strlen(json_start);
  content_length_here[0] = '0' + (compi / 10);
  content_length_here[1] = '0' + (compi % 10);
  client.print(post_rqst);
  Serial.println("SMS enviado");
}

//FUNÇÃO PARA ENVIAR O EMAIL CORRETO PARA A TEMPERATURA RECEBIDA
void lidoEnviado(float temperatura)
{
  String valor = (String)temperatura + "°C";
  
  if (temperatura < 35) //Temperatura - de 35
  {
    //enviar aviso de hipotermia
    Serial.println(" --> HIPOTERMIA");

    if (controle != 1)
    {
      //enviando email
      email("pidevestrusis@gmail.com", "HIPOTERMIA", valor + " TEMPERATURA BAIXA!");

      //enviando SMS
      sms(valor,"TEMPERATURA BAIXA!","");
    }

    controle = 1;
  }
  else if (temperatura > 37) //Temperatura + de 37
  {
    //enviar aviso de febre
    Serial.println(" --> FEBRE");

    if (controle != 2)
    {
      //enviando email
      email("pidevestrusis@gmail.com", "FEBRE", valor + " TEMPERATURA ALTA!");

      //enviando SMS
      sms(valor,"TEMPERATURA ALTA!","");
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

//FUNÇÃO PARA ENVIAR DADOS PARA O THINGSPEAK
void thingspeak(String key, float temperatura)
{
  String postStr = key;
  postStr += "&field1=";
  postStr += temperatura;
  postStr += "r\n";
  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
  Serial.println("Dados enviados ao Thingspeak");
}
