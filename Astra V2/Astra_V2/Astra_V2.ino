#include <SoftwareSerial.h>

// Motor Driver Pins (L293N)
#define ENA 10  // Left motors
#define ENB 9   // Right motors
#define IN1 7  
#define IN2 6  
#define IN3 5  
#define IN4 4  

// IR Sensor Pins
#define CENTER_SENSOR 8
#define INSIDE_RIGHT 12
#define INSIDE_LEFT 13

SoftwareSerial Bluetooth(3, 2);  // RX, TX for HC-05
char receivedValue = '0'; // Store received Bluetooth value
int motorSpeed = 130;  // Default speed
int j = 0;  // Junction counter

void setup() {
    Serial.begin(9600);
    Bluetooth.begin(9600);  // Start Bluetooth communication

    // Motor pins as output
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // IR sensor pins as input
    pinMode(CENTER_SENSOR, INPUT);
    pinMode(INSIDE_RIGHT, INPUT);
    pinMode(INSIDE_LEFT, INPUT);

    stopMotor();  // Ensure motors are stopped initially
}

void loop() {
    int center = digitalRead(CENTER_SENSOR);
    int inRight = digitalRead(INSIDE_RIGHT);
    int inLeft = digitalRead(INSIDE_LEFT);

    if (Bluetooth.available()) {  
        receivedValue = Bluetooth.read();  
        Serial.print("Received: ");
        Serial.println(receivedValue);
    }

    if (receivedValue != '0') 
    {
        if (center == 0 && inLeft == 0 && inRight == 0) 
        {  
            j++;  // Increment junction counter

            stopMotor();
            delay(2000);  // Short stop before decision

            int value = receivedValue - '0'; // Convert char to int

            if (j == 1) { (value <= 3) ? turnLeft() : turnRight(); }

            else if (j == 2) 
            {  
                if (value == 1 || value == 5) 
                {
                    moveForward();
                } else if (value == 2 || value == 6) 
                {
                    turnLeft();
                } else if (value == 3 || value == 7) 
                {
                    turnRight();
                }
            } 
            else if (j == 3) {  // 🚨 Stop at 3rd junction
                stopMotor();
                Serial.println("Final Junction Reached - Stopping Robot");
                return;
            }

            delay(1000);  // Time to execute movement
        } 
        else if (inLeft == 1 && center == 0 && inRight == 1) {
            moveForward();
        } 
        else if (inRight == 0 && inLeft == 1) {
            turnRight();
        } 
        else if (inLeft == 0 && inRight == 1) {
            turnLeft();
        } 
    }
}

// Motor control functions
void moveForward() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnRight() {
    analogWrite(ENA, 240);
    analogWrite(ENB, 0);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnLeft() {
    analogWrite(ENA, 0); 
    analogWrite(ENB, 240);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void stopMotor() {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
