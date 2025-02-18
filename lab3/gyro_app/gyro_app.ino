#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// WiFi Credentials (Change these)
const char* ssid = "8====D";
const char* password = "coffeebread123";

// MPU6050 and Web Server setup
Adafruit_MPU6050 mpu;
AsyncWebServer server(80);

// Define LED pins
const int ledPins[5] = {13, 12, 27, 33, 15};

// LED states
bool ledStates[5] = {false, false, false, false, false};


// HTML Page for Web Server
const char* index_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 MPU6050</title>
    <style>
        body { 
        font-family: Arial, sans-serif; 
        text-align: center; 
        padding: 20px;
        background: linear-gradient(to right, pink, purple);
        color: white;
        margin: 0;
    }
    .banner {
        background-color: black;
        padding: 20px;
        font-size: 36px;
        font-weight: bold;
        width: 100%;
        position: fixed;
        top: 0;
        left: 0;
        text-align: center;
    }
    .container { 
        max-width: 400px; 
        margin: auto; 
        padding: 20px; 
        border-radius: 10px; 
        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); 
        background: rgba(0, 0, 0, 0.3);
        margin-top: 80px; /* Adjusted to avoid overlap with banner */
    }
    .data { font-size: 20px; font-weight: bold; }
    .led { 
        display: inline-block; 
        width: 40px; 
        height: 40px; 
        margin: 5px; 
        border-radius: 50%; 
        background-color: gray;
    }
    .on { background-color: green; }
    </style>
    <script>
        function updateData() {
            fetch('/data')
            .then(response => response.json())
            .then(data => {
                document.getElementById("x").innerText = data.x;
                document.getElementById("y").innerText = data.y;
                document.getElementById("z").innerText = data.z;
                
                let leds = data.leds;
                for (let i = 0; i < 5; i++) {
                    document.getElementById("led" + i).className = leds[i] ? "led on" : "led";
                }
            });
        }
        setInterval(updateData, 500);
    </script>
</head>
<body>
    <div class="banner">Anirudhhan Raghuraman Lab 3: Accelerometer</div>
    <div class="container">
        <p>X-Acceleration: <span class="data" id="x">0.00</span> m/s²</p>
        <p>Y-Acceleration: <span class="data" id="y">0.00</span> m/s²</p>
        <p>Z-Acceleration: <span class="data" id="z">0.00</span> m/s²</p>
        <div>
            <div id="led0" class="led"></div>
            <div id="led1" class="led"></div>
            <div id="led2" class="led"></div>
            <div id="led3" class="led"></div>
            <div id="led4" class="led"></div>
        </div>
    </div>
</body>
</html>
)rawliteral";

// Initialize MPU6050 and LEDs
void setup() {
    Serial.begin(115200);
    WiFi.setSleep(false);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi: " + WiFi.localIP().toString());

    // Setup MPU6050
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050");
        while (1);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    // Setup LED pins as output
    for (int i = 0; i < 5; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    // Serve the HTML page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", index_html);
    });

    // API to get sensor data & LED states
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        sensors_event_t accel, gyro, temp;
        mpu.getEvent(&accel, &gyro, &temp);

        // Read acceleration on all axes
        float xAccel = accel.acceleration.x;
        float yAccel = accel.acceleration.y;
        float zAccel = accel.acceleration.z;
        zAccel = accel.acceleration.z - 9.8;
        // Compute net acceleration
        float netAccel = sqrt(xAccel*xAccel + yAccel*yAccel + zAccel*zAccel);

        // Update LEDs based on net acceleration
        updateLEDs(netAccel);

        // Create JSON response
        String json = "{";
        json += "\"x\":" + String(xAccel, 2) + ",";
        json += "\"y\":" + String(yAccel, 2) + ",";
        json += "\"z\":" + String(zAccel, 2) + ",";
        json += "\"leds\":[" + 
            String(ledStates[0]) + "," +
            String(ledStates[1]) + "," +
            String(ledStates[2]) + "," +
            String(ledStates[3]) + "," +
            String(ledStates[4]) + "]";
        json += "}";

        request->send(200, "application/json", json);
    });

    server.begin();
}

// Function to control LEDs based on net acceleration
void updateLEDs(float netAccel) {
    // Define acceleration thresholds for LED activation
    float thresholds[5] = {1.5, 3.0, 4.5, 6.0, 7.5}; // Adjust as needed

    for (int i = 0; i < 5; i++) {
        if (netAccel >= thresholds[i]) {
            digitalWrite(ledPins[i], HIGH);
            ledStates[i] = true;
        } else {
            digitalWrite(ledPins[i], LOW);
            ledStates[i] = false;
        }
    }
}

void loop() {
    // Nothing needed, handled by server
}