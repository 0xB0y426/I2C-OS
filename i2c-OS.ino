#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROW_NUM    = 4;
const byte COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'A','B','C','D'},
  {'E','F','G','H'},
  {'I','J','K','L'},
  {'M','N','O','#'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

LiquidCrystal_I2C lcd(0x27, 16, 2);

char input[17];
int inputIndex = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I2C-OS");
  lcd.setCursor(0, 1);
  lcd.print("Loading...");
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    if (key == '#') {
      input[inputIndex] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Exec: ");
      lcd.setCursor(0, 1);
      lcd.print(input);
      Serial.print("Command Executed: ");
      Serial.println(input);
      
      executeCommand(input);
      
      inputIndex = 0;
      memset(input, 0, sizeof(input));
    } else if (inputIndex < 16) {
      input[inputIndex] = key;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("I2C-OS");
      lcd.setCursor(0, 1);
      lcd.print(input);
      Serial.println(key);
      inputIndex++;
    }
  }
}

void executeCommand(char* command) {
  if (strcmp(command, "GININFO") == 0) {
    displayPinState();
  } else if (strcmp(command, "CNGINFO") == 0) {
    displayCpuInfo();
  } else if (strcmp(command, "CEGI") == 0) { 
    displayRegisters(); 
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid Command");
    Serial.println("Invalid Command!");
  }
}

void displayRegisters() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Registers:");
  
  lcd.setCursor(0, 1);
  lcd.print("PORTB...");
  Serial.println("Register values:");
  
  displayRegisterInfo("PORTB:", PINB);
  displayRegisterInfo("PORTC:", PINC);
  displayRegisterInfo("PORTD:", PIND);
  
  lcd.setCursor(0, 1);
  lcd.print("Finished: 0");
}

void displayRegisterInfo(String title, byte regValue) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(title);
  lcd.setCursor(0, 1);
  lcd.print(regValue, BIN);
  Serial.print(title);
  Serial.print(" ");
  Serial.println(regValue, BIN);
  
  delay(2000);
}

void displayPinState() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pin States:");

  for (int i = 0; i < 14; i++) {
    lcd.setCursor(0, 1);
    lcd.print("Pin ");
    lcd.print(i);
    lcd.print(": ");
    lcd.print(digitalRead(i));
    Serial.print("Pin ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(digitalRead(i));
    
    delay(2000); 
  }
  
  lcd.setCursor(0, 1);
  lcd.print("14 Pins.");
}

void displayCpuInfo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CPU Info:");
  
  displayInfo("Clock Speed:", String(F_CPU / 1000000) + " MHz");
  delay(2000);

  displayInfo("Flash Memory:", String(FLASHEND / 1024) + " KB");
  delay(2000);

  displayInfo("SRAM:", "2 KB");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CPU Info Done.");
}

void displayInfo(String title, String info) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(title);
  lcd.setCursor(0, 1);
  lcd.print(info);
  Serial.print(title);
  Serial.println(info);
}
