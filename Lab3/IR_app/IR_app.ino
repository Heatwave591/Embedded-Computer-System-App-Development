#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

const char* ssid = "8====D";
const char* password = "coffeebread123";

// Pin connected to Arduino's LED status output
const int LED_INPUT_PIN = 32;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// HTML content as a string
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>LED Status Monitor</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            min-height: 100vh;
            background: linear-gradient(to right, #6a11cb, #ff2d95);
            font-family: Arial, sans-serif;
        }

        .title-block {
            background-color: rgba(0, 0, 0, 0.8);
            color: white;
            padding: 20px;
            text-align: center;
            margin-bottom: 40px;
        }

        .title-block h1 {
            margin: 0;
            font-size: 2em;
        }

        .led-box {
            background-color: rgba(255, 255, 255, 0.2);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            padding: 40px;
            max-width: 800px;
            margin: 0 auto;
            box-shadow: 0 8px 32px 0 rgba(31, 38, 135, 0.37);
        }

        .led-container {
            display: flex;
            justify-content: center;
            flex-wrap: wrap;
            gap: 30px;
            margin: 20px auto;
        }

        .led {
            width: 100px;
            height: 100px;
            border-radius: 50%;
            transition: all 0.3s ease;
        }

        .led-off {
            background-color: #666;
            box-shadow: 0 0 10px #444;
        }

        .led-on {
            background-color: #ff0;
            box-shadow: 0 0 30px #ff0;
        }

        .status-text {
            color: white;
            text-align: center;
            font-size: 1.2em;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <div class="title-block">
        <h1>ANIRUDHHAN RAGHURAMAN: LAB 3</h1>
    </div>
    
    <div class="led-box">
        <div class="led-container">
            <div class="led led-off" id="led1"></div>
            <div class="led led-off" id="led2"></div>
            <div class="led led-off" id="led3"></div>
            <div class="led led-off" id="led4"></div>
        </div>
        <div class="status-text">
            LED Status: <span id="statusText">OFF</span>
        </div>
    </div>

    <script>
        function updateLEDs(isOn) {
            const leds = ['led1', 'led2', 'led3', 'led4'].map(id => 
                document.getElementById(id)
            );
            const statusText = document.getElementById('statusText');
            
            leds.forEach(led => {
                if (isOn) {
                    led.classList.remove('led-off');
                    led.classList.add('led-on');
                } else {
                    led.classList.remove('led-on');
                    led.classList.add('led-off');
                }
            });
            
            statusText.textContent = isOn ? 'ON' : 'OFF';
        }

        const ws = new WebSocket('ws://' + window.location.hostname + '/ws');
        
        ws.onmessage = function(event) {
            const state = event.data === '1';
            updateLEDs(state);
        };
    </script>
</body>
</html>
)rawliteral";

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());

    // Configure LED input pin
    pinMode(LED_INPUT_PIN, INPUT);

    // Set up WebSocket event handler
    ws.onEvent([](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
        if(type == WS_EVT_CONNECT) {
            Serial.println("WebSocket client connected");
        }
    });
    server.addHandler(&ws);

    // Serve the web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", index_html);
        Serial.println("Serving index page");
    });

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    static unsigned long lastUpdate = 0;
    static int lastState = -1;
    
    // Read LED state
    int ledState = digitalRead(LED_INPUT_PIN);
    
    // Only send updates when the state changes or every 100ms
    if (ledState != lastState || (millis() - lastUpdate) > 100) {
        ws.textAll(String(ledState));
        lastState = ledState;
        lastUpdate = millis();
    }
    
    delay(10);
}


