#include <WiFi.h>
#include <SPI.h>
#include <SD.h>

const char* ssid = "PicoW-TabishAccess";  
const char* password = "Azmee@123";   

// SD card chip select pin
const int chipSelect = 13;

// Built-in LED pin (On the Pico W, it's usually GP25)
const int ledPin = 25;

void setup() {
    Serial.begin(115200);
    
    // Set the built-in LED pin as OUTPUT
    pinMode(ledPin, OUTPUT);
    
    // Set up the Wi-Fi access point
    WiFi.softAP(ssid, password);
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    // Initialize SD card
    if (!SD.begin(chipSelect)) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    Serial.println("SD Card initialized.");
    
    // Create a simple web server
    WiFiServer server(80);
    server.begin();
    Serial.println("Server started.");

    bool clientConnected = false; // Track connection status

    while (true) {
        // Check for client connections
        WiFiClient client = server.available();
        if (client) {
            // Client has connected
            Serial.println("New Client Connected");
            clientConnected = true; // Update connection status
            digitalWrite(ledPin, HIGH); // Turn on LED
            String currentLine = "";
            while (client.connected()) {
                if (client.available()) {
                    char c = client.read();
                    Serial.write(c);
                    if (c == '\n') {
                        // If the line is blank, you can send the response
                        if (currentLine.length() == 0) {
                            // Send HTTP headers
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();
                            
                            // HTML content
                            client.println("<!DOCTYPE HTML>");
                            client.println("<html>");
                            client.println("<h1>SD Card Contents</h1>");
                            client.println("<ul>");

                            // List the files on the SD card
                            listFiles(client);
                            
                            client.println("</ul>");
                            client.println("</html>");
                            break;
                        } else {
                            currentLine = "";
                        }
                    } else if (c != '\r') {
                        currentLine += c;
                    }
                }
            }
            // Close the connection
            client.stop();
            Serial.println("Client Disconnected");
            digitalWrite(ledPin, LOW); // Turn off LED when client disconnects
            clientConnected = false; // Update connection status
        }
        
        // If no client is connected, ensure the LED is off
        if (!clientConnected) {
            digitalWrite(ledPin, LOW);
        }
    }
}

void listFiles(WiFiClient &client) {
    File root = SD.open("/");
    File file = root.openNextFile();
    while (file) {
        client.print("<li>");
        client.print(file.name());
        client.print(" - ");
        client.print(file.size());
        client.println(" bytes</li>");
        file = root.openNextFile();
    }
    root.close();
}

void loop() {
    // Nothing here
}
