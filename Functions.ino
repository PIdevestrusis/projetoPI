//FUNÇÃO PARA ENVIAR EMAIL
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
