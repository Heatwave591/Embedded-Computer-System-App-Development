#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// Pin definitions
#define DHTPIN 23
#define led1 22
#define led2 21
#define led3 19
#define led4 18
#define led5 5
#define led6 17
#define DHTTYPE DHT11

// Network credentials
const char* ssid = "8====D";
const char* password = "coffeebread123";

WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Initialize LED pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Define web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  
  server.begin();
}

void loop() {
  server.handleClient();
  updateLEDs();
}

void updateLEDs() {
  float t = dht.readTemperature();
  
  if (isnan(t)) {
    return;
  }

  // LED control logic remains the same as your original code
  if(t <= 21.5) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }
  else if(t > 21.5 && t <= 21.7) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }
    else if(t > 21.7 && t <= 22.2){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }

  else if(t > 22.2 && t <= 22.7){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }

  else if(t > 22.7 && t <= 23.2){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }

    else if(t > 23.2 && t <= 23.7){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, LOW);
  }

    else if(t > 23.7){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
}

void handleData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  String json = "{";
  json += "\"temperature\":" + String(t) + ",";
  json += "\"humidity\":" + String(h) + ",";
  json += "\"led1\":" + String(digitalRead(led1)) + ",";
  json += "\"led2\":" + String(digitalRead(led2)) + ",";
  json += "\"led3\":" + String(digitalRead(led3)) + ",";
  json += "\"led4\":" + String(digitalRead(led4)) + ",";
  json += "\"led5\":" + String(digitalRead(led5)) + ",";
  json += "\"led6\":" + String(digitalRead(led6));
  json += "}";
  
  server.send(200, "application/json", json);
}
void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Anirudhhan Raghuraman\nLab3</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css" rel="stylesheet">
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #1e2a78, #ff6b6b);
            min-height: 100vh;
            color: white;
            padding: 20px;
        }
        
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.1);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
        }
        
        h1 {
            text-align: center;
            margin-bottom: 30px;
            font-size: 2.5em;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.2);
        }
        
        .readings {
            display: flex;
            justify-content: space-around;
            margin-bottom: 40px;
        }
        
        .reading-card {
            background: rgba(255, 255, 255, 0.2);
            padding: 20px;
            border-radius: 15px;
            text-align: center;
            width: 45%;
            transition: transform 0.3s;
        }
        
        .reading-card:hover {
            transform: translateY(-5px);
        }
        
        .reading-card i {
            font-size: 2em;
            margin-bottom: 10px;
        }
        
        .reading-value {
            font-size: 2em;
            font-weight: bold;
            margin: 10px 0;
        }
        
        .reading-label {
            font-size: 1.2em;
            opacity: 0.9;
        }
        
        .led-container {
            display: flex;
            justify-content: center;
            flex-wrap: wrap;
            gap: 20px;
            margin-top: 30px;
        }
        
        .led {
            width: 60px;
            height: 60px;
            border-radius: 50%;
            background: #2c3e50;
            position: relative;
            transition: all 0.3s ease;
        }
        
        .led::after {
            content: '';
            position: absolute;
            top: 5px;
            left: 5px;
            right: 5px;
            bottom: 5px;
            border-radius: 50%;
            background: radial-gradient(circle at 30% 30%, rgba(255, 255, 255, 0.8), transparent);
        }
        
        .led.on {
            background: #ffd700;
            box-shadow: 0 0 30px #ffd700;
        }
        
        .led-label {
            text-align: center;
            margin-top: 5px;
            font-size: 0.8em;
            color: rgba(255, 255, 255, 0.8);
        }
        
        .led-wrapper {
            display: flex;
            flex-direction: column;
            align-items: center;
        }
        
        @media (max-width: 600px) {
            .readings {
                flex-direction: column;
                gap: 20px;
            }
            
            .reading-card {
                width: 100%;
            }
            
            .led {
                width: 50px;
                height: 50px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Anirudhhan Raghuraman: Lab 3</h1>
        <div class="readings">
            <div class="reading-card">
                <i class="fas fa-thermometer-half"></i>
                <div class="reading-value">
                    <span id="temperature">--</span>
                </div>
                <div class="reading-label">Temperature</div>
            </div>
            <div class="reading-card">
                <i class="fas fa-tint"></i>
                <div class="reading-value">
                    <span id="humidity">--</span>%
                </div>
                <div class="reading-label">Humidity</div>
            </div>
        </div>
        <div class="led-container">
            <div class="led-wrapper">
                <div class="led" id="led1"></div>
                <div class="led-label">LED 1</div>
            </div>
            <div class="led-wrapper">
                <div class="led" id="led2"></div>
                <div class="led-label">LED 2</div>
            </div>
            <div class="led-wrapper">
                <div class="led" id="led3"></div>
                <div class="led-label">LED 3</div>
            </div>
            <div class="led-wrapper">
                <div class="led" id="led4"></div>
                <div class="led-label">LED 4</div>
            </div>
            <div class="led-wrapper">
                <div class="led" id="led5"></div>
                <div class="led-label">LED 5</div>
            </div>
            <div class="led-wrapper">
                <div class="led" id="led6"></div>
                <div class="led-label">LED 6</div>
            </div>
        </div>
    </div>
    <script>
        function updateData() {
            fetch('/data')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temperature').innerHTML = parseFloat(data.temperature).toFixed(1) + " C";
                    document.getElementById('humidity').textContent = data.humidity.toFixed(1);
                    
                    for(let i = 1; i <= 6; i++) {
                        const led = document.getElementById('led' + i);
                        if(data['led' + i]) {
                            led.classList.add('on');
                        } else {
                            led.classList.remove('on');
                        }
                    }
                });
        }
        
        setInterval(updateData, 1000);
        updateData();
    </script>
</body>
</html>
)";
  server.send(200, "text/html", html);
}

