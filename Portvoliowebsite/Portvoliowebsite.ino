#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Arduino.h>

// Network credentials
const char* ssid = "Tabish realme 8 5G";
const char* password = "Zaintab@123";
const byte DNS_PORT = 53;
const char* hostName = "tabish"; // This will be used for the local DNS

IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
WebServer server(80);

// HTML for the portfolio website
const char* html = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Tabish Ahmed Khan - Portfolio</title>
    <style>
        :root {
            --primary-color: #2d2d2d;
            --secondary-color: #4CAF50;
            --text-color: #ffffff;
        }
        
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: #1a1a1a;
            color: var(--text-color);
            line-height: 1.6;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
        }
        
        header {
            background: var(--primary-color);
            padding: 2rem 0;
            text-align: center;
        }
        
        .profile-section {
            text-align: center;
            margin: 2rem 0;
        }
        
        .section {
            background: var(--primary-color);
            border-radius: 10px;
            padding: 20px;
            margin: 20px 0;
        }
        
        .skills {
            display: flex;
            flex-wrap: wrap;
            gap: 10px;
            margin: 10px 0;
        }
        
        .skill-tag {
            background: var(--secondary-color);
            padding: 5px 15px;
            border-radius: 20px;
            font-size: 0.9em;
        }
        
        .languages {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
            margin: 20px 0;
        }
        
        .language-card {
            background: #3d3d3d;
            padding: 15px;
            border-radius: 8px;
            text-align: center;
        }
        
        .education {
            margin: 20px 0;
        }
        
        .education-item {
            border-left: 3px solid var(--secondary-color);
            padding-left: 20px;
            margin: 20px 0;
        }
        
        @media (max-width: 768px) {
            .container {
                padding: 10px;
            }
        }
    </style>
</head>
<body>
    <header>
        <div class="container">
            <h1>Tabish Ahmed Khan</h1>
            <p>Electrical and Electronics Engineering Student | TH Rosenheim</p>
        </div>
    </header>

    <div class="container">
        <div class="profile-section">
            <p>Passionate engineering student with a focus on electrical and information technology. 
            Actively involved in university projects and committed to continuous learning.</p>
        </div>

        <div class="section">
            <h2>Skills</h2>
            <div class="skills">
                <span class="skill-tag">Digital Tools</span>
                <span class="skill-tag">Communication</span>
                <span class="skill-tag">Research</span>
                <span class="skill-tag">C++</span>
                <span class="skill-tag">Arduino</span>
                <span class="skill-tag">Robotics</span>
                <span class="skill-tag">Web Development</span>
            </div>
        </div>

        <div class="section">
            <h2>Languages</h2>
            <div class="languages">
                <div class="language-card">
                    <h3>German</h3>
                    <p>Full Professional</p>
                </div>
                <div class="language-card">
                    <h3>English</h3>
                    <p>Native/Bilingual</p>
                </div>
                <div class="language-card">
                    <h3>Hindi</h3>
                    <p>Native/Bilingual</p>
                </div>
                <div class="language-card">
                    <h3>Urdu</h3>
                    <p>Limited Working</p>
                </div>
                <div class="language-card">
                    <h3>French</h3>
                    <p>Elementary</p>
                </div>
            </div>
        </div>

        <div class="section">
            <h2>Education</h2>
            <div class="education">
                <div class="education-item">
                    <h3>Rosenheim Technical University of Applied Sciences</h3>
                    <p>Bachelor of Engineering, Electrical and Electronics Engineering</p>
                    <p>March 2023 - Present</p>
                </div>
                <div class="education-item">
                    <h3>Ajyal Al Marefaah International School</h3>
                    <p>High School Diploma</p>
                    <p>2019 - 2022</p>
                </div>
                 <div class="education-item">
                    <h3>Riyadh Jazan International School</h3>
                    <p>Primary School Education</p>
                    <p>August 2011 - January 2018</p>
                </div>
            </div>
        </div>

        <div class="section">
            <h2>Experience</h2>
            <div class="education-item">
                <h3>ROCkET / HSRV_Infrastruktur Entrepreneurship</h3>
                <p>Event Planning, Research, Documentation</p>
                <p>October 2024 - Present</p>
            </div>
            <div class="education-item">
                <h3>Social Media Marketing Manager</h3>
                <p>International Wood Technology Program</p>
                <p>September 2024 - Present</p>
            </div>
            <div class="education-item">
                <h3>Study Captain</h3>
                <p>Guided new students and organized orientation activities</p>
                <p>March 2024 - August 2024</p>
            </div>
        </div>

        <div class="section">
            <h2>Contact</h2>
            <p>Email: ahmedkhantabish@gmail.com</p>
            <p>Phone: 017673922957</p>
            <p>LinkedIn: <a href="https://www.linkedin.com/in/tabish-ahmed-khan" target="_blank">www.linkedin.com/in/tabish-ahmed-khan</a></p>
        </div>
    </div>
</body>
</html>
)html";

void handleRoot() {
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure DNS Server
  dnsServer.start(DNS_PORT, hostName, WiFi.localIP());
  Serial.print("You can access your website at: http://");
  Serial.println(WiFi.localIP());

  // Setup web server routes
  server.on("/", HTTP_GET, handleRoot);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}