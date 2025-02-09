#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define Password_Length 7
#define Max_Attempts 3
#define Lock_Duration 30000 // 30 seconds in milliseconds
#define Full_Lock_Attempts 2

int pos = 0;
char Data[Password_Length];
char Master[Password_Length] = "123456";
byte data_count = 0;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true;
bool locked = false;
bool fullyLocked = false;
unsigned long lockTime = 0;
int attempts = 0;
int fullLockAttempts = 0;

byte rowPins[ROWS] = {1, 2, 3, 4};
byte colPins[COLS] = {5, 6, 7, 8};

Keypad customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int buzzerPin = 11; // Connect the piezo buzzer to digital pin 11

void setup()
{
  pinMode(10, OUTPUT);
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output
  myservo.attach(9);
  ServoClose();
  lcd.begin(16, 2);
  lcd.print(" Arduino Door");
  lcd.setCursor(0, 1);
  lcd.print("--Lock project--");
  delay(3000);
  lcd.clear();
}

void loop()
{
  if (fullyLocked) {
    // Display fully locked message
    lcd.clear();
    lcd.print("Fully Locked");
    customBeep(800, 200); // Beep to indicate fully locked
    delay(2000);
  } else if (locked) {
    // Check if the lock period is over
    if (millis() - lockTime >= Lock_Duration) {
      locked = false;
      lcd.clear();
      lcd.print("Enter Password");
      delay(1000);
    }
  } else if (door == 0) {
    customKey = customKeypad.getKey();

    if (customKey == '#') {
      lcd.clear();
      ServoClose();
      lcd.print("  Door is closed");
      customBeep(1000, 500); // Beep to indicate the door is closed
      digitalWrite(10, LOW);
      delay(3000);
      door = 1;
    }
  } else {
    Open();
  }
}

void clearData()
{
  while (data_count != 0)
  {
    Data[data_count--] = 0;
  }
  return;
}

void ServoOpen()
{
  for (pos = 180; pos >= 0; pos -= 5)
  {
    myservo.write(pos);
    delay(15);
  }
  customBeep(1500, 500); // Beep to indicate the door is opened
}

void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5)
  {
    myservo.write(pos);
    delay(15);
  }
  customBeep(1000, 500); // Beep to indicate the door is closed
}

void customBeep(int frequency, int duration) {
  if (frequency > 0) {
    tone(buzzerPin, frequency, duration);
  } else {
    noTone(buzzerPin);
  }
  delay(duration);
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print("  Enter Password");

  customKey = customKeypad.getKey();
  if (customKey)
  {
    if (customKey == '*')
    {
      if (data_count > 0)
      {
        data_count--;
        lcd.setCursor(data_count, 1);
        lcd.print(" "); // Clear the character from the display
      }
    }
    else
    {
      Data[data_count] = customKey;
      lcd.setCursor(data_count, 1);
      lcd.print('*');
      data_count++;
    }
  }

  if (data_count == Password_Length - 1)
  {
    if (!strcmp(Data, Master))
    {
      lcd.clear();
      digitalWrite(10, HIGH);
      ServoOpen();
      lcd.print("  Door is Open");

      // Activate the piezo buzzer for success
      digitalWrite(buzzerPin, HIGH);
      customBeep(1500, 70); // Beep to indicate success
      digitalWrite(buzzerPin, LOW);

      door = 0;
    }
    else
    {
      lcd.clear();
      lcd.print("Wrong Password");
      customBeep(500, 500); // Beep to indicate wrong password
      delay(1000);
      door = 1;

      // Lock the system after a certain number of attempts
      attempts++;
      if (attempts >= Max_Attempts && !fullyLocked) {
        locked = true;
        lockTime = millis(); // Record lock start time
        lcd.clear();
        lcd.print("System Locked");
        customBeep(800, 200); // Beep to indicate system locked
        delay(2000);
        lcd.clear();
        lcd.print("Try again in 30sec");
      }

      // Fully lock the system after additional attempts
      if (fullyLocked || (attempts >= (Max_Attempts + Full_Lock_Attempts))) {
        fullyLocked = true;
        lcd.clear();
        lcd.print("Door is ");
        delay(2000);
      }
    }
    
    clearData();
  }
}
