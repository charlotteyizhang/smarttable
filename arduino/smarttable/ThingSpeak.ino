/*
 * Send some data to ThingSpeak
 */
void sendDataToThingSpeak(String data)
{
  HttpClient client;
  String clientStr;

  Serial.println("connecting to ThingSpeak ...");
  
  // Sometimes the DNS lookup doesn't work so it's best to use ThingSpeak's
  // static IP address.
  clientStr = "http://184.106.153.149/update?api_key=" + APIkey + "&field1=";
  clientStr += data;
  clientStr += "&field2=";
  clientStr += data;
  
  Serial.println(clientStr);

  // The next line sends the data to ThingSpeak
  client.get(clientStr);
  Serial.println("...done");

  // Read the entry number returned by ThingSpeak
  String result;
  while (client.available()) 
  {
    char c = client.read();
    result += String(c);
  }
  
  if (result.toInt() == 0)
  {
    Serial.println("Failed to send to ThingSpeak");
  }
  else
  {
    // Success - display the entry number returned by ThingSpeak
    Serial.println("Created entry " + result);
  }
}


