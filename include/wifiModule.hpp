
void wifiLoop()
{
    unsigned long currentMillis = millis();

    if (WiFi.status() != WL_CONNECTED && (currentMillis - previousMillisWIFI >= intervalWIFI))
    {
        // to blink a led when connected to non-blocking Wi-Fi
        // Single Led blink every 100ms
        blinkSingle(100, WIFILED);
        WiFi.disconnect();
        WiFi.reconnect();
        previousMillisWIFI = currentMillis;
    }
    else
    {
        // LED flashing Time On and Time Off
        blinkSingleAsy(10, 500, WIFILED);
    }
}

void connectToWiFi()
{
    Serial.print("Info: Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PWD);

    byte b = 0;
    while (WiFi.status() != WL_CONNECTED && b < 30)
    {
        b++;
        log("Warning: Attempting Wi-Fi connection...");
        delay(500);
        blinkSingle(100, WIFILED);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        // WiFi Station conectado
        Serial.print("Info: Connected. IP: ");
        Serial.println(WiFi.localIP());
        blinkRandomSingle(10, 100, WIFILED);
        delay(100);
    }
    else
    {
        log(F("Error: WiFi no conectado"));
        blinkRandomSingle(10, 100, WIFILED);
        delay(100);
    }
}