/*
 * Code for ESP 32 microcontroller for Fall 2023 ECE 445 senior design project.
 *
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *   https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
 *   https://randomnerdtutorials.com/esp32-client-server-wi-fi/#:~:text=Set%20your%20ESP32%20as%20an,be%20listening%20for%20incoming%20requests.
 * 
*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <time.h>
#include <string.h>

// Constants (pins)
const int SENSOR_PIN_1 = 1;  // Sensor input pin 1
const int SENSOR_PIN_2 = 1;  // Sensor input pin 2
const int SENSOR_PIN_3 = 1;  // Sensor input pin 3
const int CAMERA_PIN_1 = 1;  // Camera output pin 1
const int CAMERA_PIN_2 = 1;  // Cmaera output pin 2
const int MOTOR_FWD = 1;  // Motor output pin 1
const int MOTOR_BCK = 1;  // Motor output pin 2
const int BUTTON_PIN = 1; // For testing

// Wifi Connection Information
const char* ssid = "Bring Out The Whole Ocean";
const char* password = "AnotherOne";
AsyncWebServer server(80);

// Global Variables
int SENSOR_EN = 0;
int SENS_TIMER = 0;
int DOOR_EN = 0;
int DOOR_TIMER = 0;
int DOOR_ST = 0;
int SIG_APP = 0;
int prev_time;

/* 
    void setup()

    Set up Wifi, connect the pins properly to each other, setup the timers, pull down/pull up resistors

*/
void setup() {
    Serial.begin(115200);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/button", HTTP_GET, [](AsyncWebServerRequest *request){
        SIG_APP = 1;
        request->send(200, "text/plain", "pressed"); 
    });

    server.begin();

    pinMode(SENSOR_PIN_1, INPUT);
    pinMode(SENSOR_PIN_2, INPUT);
    pinMode(SENSOR_PIN_3, INPUT);
    pinMode(CAMERA_PIN_1, OUTPUT);
    pinMode(CAMERA_PIN_2, OUTPUT);
    pinMode(MOTOR_FWD, OUTPUT);
    pinMode(MOTOR_BCK, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

}

/* 
    void loop()

    For pins, 
        - Be able to recieve sensor signal with timer
        - Feed out signal to camera to turn it on
        - Recieve signal from application
        - Turn motors with said signal from appliation
        - Be able to signal if animal is still within door to stop door from closing
        - Signal for latch

*/
void loop() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime( &rawtime );
    int curr_sec = timeinfo->tm_sec;

    // Check for edge cases (both cameras on)
    if (digitalRead(SENSOR_PIN_1) == HIGH) {
        digitalWrite(CAMERA_PIN_1, HIGH);
        SENS_TIMER = 0;
        SENSOR_EN = 1;
    }
    
    if (digitalRead(SENSOR_PIN_2) == HIGH) {
        digitalWrite(CAMERA_PIN_2, HIGH);
        SENS_TIMER = 0;
        SENSOR_EN = 1;
    }

    // Check if enough time has passed for camera to be turned off
    if (SENS_TIMER==120) {
        digitalWrite(CAMERA_PIN_1, LOW);
        digitalWrite(CAMERA_PIN_2, LOW);
        SENSOR_EN = 0;
    }

    // Get signal from app

    // if (digitalRead(BUTTON_PIN)==HIGH) {
    //     if (DOOR_EN==0) {
    //         DOOR_TIMER = 0;
    //         DOOR_EN = 1;
    //         if (DOOR_ST) {
    //             DOOR_ST = 0;
    //             digitalWrite(MOTOR_FWD, LOW);
    //             digitalWrite(MOTOR_BCK, HIGH);
    //         } else {
    //             DOOR_ST = 1;
    //             digitalWrite(MOTOR_BCK, LOW);
    //             digitalWrite(MOTOR_FWD, HIGH);
    //         }
    //     }
    // }

    if (SIG_APP) {
        if (DOOR_EN==0) {
            DOOR_TIMER = 0;
            DOOR_EN = 1;
            if (DOOR_ST) {
                DOOR_ST = 0;
                digitalWrite(MOTOR_FWD, LOW);
                digitalWrite(MOTOR_BCK, HIGH);
            } else {
                DOOR_ST = 1;
                digitalWrite(MOTOR_BCK, LOW);
                digitalWrite(MOTOR_FWD, HIGH);
            }
        }
        SIG_APP = 0;
    }

    if (DOOR_ST==0) {
        if (DOOR_TIMER==3) {
            DOOR_EN = 0;
            DOOR_TIMER = 0;
            digitalWrite(MOTOR_BCK, LOW);
        }
    } else if (DOOR_ST==1) {
        if (DOOR_TIMER==2) {
            DOOR_EN = 0;
            DOOR_TIMER = 0;
            digitalWrite(MOTOR_FWD, LOW);
        }
    }


    /*
    if (digitalRead(SENSOR_PIN_3) == HIGH) {
        // turn door back, hold door, and reset door timer
    }
    */

    // Increment timers
    if (prev_time==NULL) {
        prev_time = curr_sec;
    } else if (curr_sec!=prev_time) {
        prev_time = curr_sec;
        if (SENSOR_EN) SENS_TIMER++;
        if (DOOR_EN) DOOR_TIMER++;
    }

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
}
