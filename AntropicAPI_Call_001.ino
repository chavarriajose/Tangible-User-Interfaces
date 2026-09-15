/*
  Copenhagen Institute of Interaction Design
  Tangible User Interfaces
  Last modified: Sept. 15th. 2026
  Instructors: David Sjunnesson & Jose Chavarria

  Claude API - Basic Example
  Sends a prompt to Claude (Haiku model) over WiFi and prints the response.
*/

#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include "secrets.h"  // WiFi and API credentials (separate file, not shared)

const char* server = "api.anthropic.com";
const int   port   = 443;

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, server, port);

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  delay(2000);  // give the SSL context time to settle before the first request
}

void askClaude(String prompt) {

  // Build the request body as JSON
  JsonDocument requestDoc;
  requestDoc["model"]       = "claude-haiku-4-5";
  requestDoc["max_tokens"]  = 100;    // keep responses short to save memory and tokens
  requestDoc["temperature"] = 1.0;   // 0.0 = predictable, 1.0 = more varied

  JsonArray messages   = requestDoc["messages"].to<JsonArray>();
  JsonObject message   = messages.add<JsonObject>();
  message["role"]      = "user";
  message["content"]   = prompt;

  String body;
  serializeJson(requestDoc, body);

  Serial.println("Sending: " + prompt);

  // Send the HTTPS POST request
  client.beginRequest();
  client.post("/v1/messages");
  client.sendHeader("Content-Type",      "application/json");
  client.sendHeader("x-api-key",         SECRET_API_KEY);
  client.sendHeader("anthropic-version", "2023-06-01");
  client.sendHeader("Content-Length",    body.length());
  client.beginBody();
  client.print(body);
  client.endRequest();

  int    statusCode = client.responseStatusCode();
  String response   = client.responseBody();

  if (statusCode == 200) {
    // Pull the text out of the response JSON
    JsonDocument responseDoc;
    DeserializationError error = deserializeJson(responseDoc, response);
    if (error) {
      Serial.println("JSON parse error: " + String(error.c_str()));
      return;
    }
    Serial.println("Response: " + responseDoc["content"][0]["text"].as<String>());

  } else {
    // The API returned an error — print it to understand what went wrong
    Serial.println("Error " + String(statusCode) + ": " + response);
  }
}

void loop() {
  delay(15000);

  String prompt = "Tell me an interesting fact about the number "
                  + String(millis())
                  + ". Keep it under 50 words.";

  askClaude(prompt);
}