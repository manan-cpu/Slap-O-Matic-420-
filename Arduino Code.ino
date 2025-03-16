#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define SERVO_PIN 9
#define LED_PIN 11
#define BUZZER_PIN 10
#define HIT_ANGLE 90   // Fast slap down
#define RETURN_ANGLE 0  // Slow return up

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo slapServo;

void setup() {
    Serial.begin(9600);
    slapServo.attach(SERVO_PIN);
    slapServo.write(RETURN_ANGLE);  // Start at resting position

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // LCD setup
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slap-O-Matic 420");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time Left:");
}

void loop() {
    int slapTime = random(1000, 5000); // Random slap between 1-5 sec
    unsigned long startTime = millis();
    int fakeCountdown = 10;  // Fake countdown

    while (millis() - startTime < slapTime) {  // Show decreasing time
        int timePassed = (millis() - startTime) / 1000;
        int displayTime = fakeCountdown - timePassed; 
        
        if (displayTime < 0) displayTime = 0;

        lcd.setCursor(0, 1);
        lcd.print("   ");  // Clear old number
        lcd.setCursor(0, 1);
        lcd.print(displayTime);
        lcd.print(" sec  ");

        delay(1000);
    }

    activateSlap();  // Surprise slap!
    delay(3000);  // Short pause before restarting
}

void activateSlap() {
    Serial.println("SLAP!");
    lcd.setCursor(0, 1);
    lcd.print(randomMessage());

    digitalWrite(LED_PIN, HIGH);   // LED ON
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer ON
    delay(random(100, 800));  // Random timing for extra chaos

    // FAST SLAP DOWN
    for (int pos = 0; pos <= HIT_ANGLE; pos += 10) {  
        slapServo.write(pos);  
        delay(5);  // Small delay = faster movement
    }

    delay(200);  // Hold position for 0.2 sec

    // SLOW RETURN UP
    for (int pos = HIT_ANGLE; pos >= RETURN_ANGLE; pos -= 2) {  
        slapServo.write(pos);
        delay(50);  // Longer delay = slower movement
    }

    digitalWrite(LED_PIN, LOW);   // LED OFF
    digitalWrite(BUZZER_PIN, LOW); // Buzzer OFF
}

String randomMessage() {
    String messages[] = {
        "Slap-O-Matic 420 STRIKES!",
        "Oops, did that hurt? Oh well.",
        "Too slow! Haha!",
        "Your reflexes are trash!",
        "BOOM! Roasted!",
        "Useless Machine Wins Again!",
        "Better luck next time... LOL",
        "You vs. Slap-O-Matic? 0-100",
        "Hope that left a mark!"
    };
    return messages[random(0, 9)];
}
