#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <String.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pumpa1 = 3;
const int pumpa2 = 5;
const int pumpa3 = 6;
const int pumpa4 = 9;

unsigned long trenutniMillis;
unsigned long prethodniMillis = 0;
unsigned long comp1;
unsigned long comp2;
unsigned long comp3;
unsigned long comp4;
int i = 0;
int postotak;

bool flag1 = true;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;
bool vrijeme = true;
bool start = false;
bool faktor = false;

char c;
String readStr = "";
float mililitar;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(pumpa1, OUTPUT);
  pinMode(pumpa2, OUTPUT);
  pinMode(pumpa3, OUTPUT);
  pinMode(pumpa4, OUTPUT);
  EEPROM.get(0, mililitar);
}

void loop() {
  if (i < 4) {
    lcd.setCursor(2, 0);
    lcd.print("Drink Mixer");
    lcd.setCursor(1, 1);
    lcd.print("Odabir recepta");
  }
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'a') {
      break;
    }
    if (c == 'f') {
      faktor = true;
      break;
    }
    readStr += c;
  }
  if (readStr.length() > 0 && faktor == true) {
    mililitar = readStr.toFloat();
    EEPROM.put(0, mililitar);
    EEPROM.get(0, mililitar);
    delay(5);
    faktor = false;
    readStr = "";
  }

  if (readStr.length() > 0) {
    i++;
    switch (i) {
      case 1:
        comp1 = readStr.toInt() * mililitar * 1000;
        break;
      case 2:
        comp2 = readStr.toInt() * mililitar * 1000;
        break;
      case 3:
        comp3 = readStr.toInt() * mililitar * 1000;
        break;
      case 4:
        comp4 = readStr.toInt() * mililitar * 1000;
        break;
    }
    readStr = "";
  }

  if (i == 4) {

    if (vrijeme == true) {
      prethodniMillis = millis();
      vrijeme = false;
      lcd.clear();
    }
    trenutniMillis = millis();

    postotak = (((trenutniMillis - prethodniMillis) / (float)(comp1 + comp2 + comp3 + comp4)) * 100);
    postotak = (postotak / 10 + (postotak % 10 > 8)) * 10;
    lcd.setCursor(0, 0);
    lcd.print("Priprema: ");
    lcd.print(postotak, 1);
    lcd.print('%');

    LCD_napredak(1, trenutniMillis - prethodniMillis, 0, comp1 + comp2 + comp3 + comp4);

    if (comp1 > 0) {
      if (flag1 == true) {
        Serial.print("started");
        analogWrite(pumpa1, 255);
        analogWrite(pumpa2, 0);
        analogWrite(pumpa3, 0);
        analogWrite(pumpa4, 0);
        flag1 = false;
        flag2 = true;
      }
    }
    else if (flag1 == true && comp1 == 0) {
      Serial.print("started");
      flag2 = true;
      flag1 = false;
    }
    if (comp2 > 0) {
      if (trenutniMillis - prethodniMillis >= comp1 && flag2 == true) {
        analogWrite(pumpa1, 0);
        analogWrite(pumpa2, 255);
        analogWrite(pumpa3, 0);
        analogWrite(pumpa4, 0);
        flag2 = false;
        flag3 = true;
      }
    }
    else if (flag2 == true && comp2 == 0) {
      flag3 = true;
      flag2 = false;
    }
    if (comp3 > 0) {
      if (trenutniMillis - prethodniMillis >= comp1 + comp2 && flag3 == true) {
        analogWrite(pumpa1, 0);
        analogWrite(pumpa2, 0);
        analogWrite(pumpa3, 255);
        analogWrite(pumpa4, 0);
        flag3 = false;
        flag4 = true;
      }
    }
    else if (flag3 == true && comp3 == 0) {
      flag4 = true;
      flag3 = false;
    }
    if (comp4 > 0) {
      if (trenutniMillis - prethodniMillis >= comp1 + comp2 + comp3 && flag4 == true) {
        analogWrite(pumpa1, 0);
        analogWrite(pumpa2, 0);
        analogWrite(pumpa3, 0);
        analogWrite(pumpa4, 255);
        flag4 = false;
      }
    }

    if (trenutniMillis - prethodniMillis >= comp1 + comp2 + comp3 + comp4) {
      prethodniMillis = trenutniMillis;
      Serial.print("finish");
      analogWrite(pumpa1, 0);
      analogWrite(pumpa2, 0);
      analogWrite(pumpa3, 0);
      analogWrite(pumpa4, 0);
      flag1 = true;
      lcd.clear();
      i = 0;
      vrijeme = true;
      lcd.setCursor(2, 0);
      lcd.print("Posluzite se!");
      delay(2000);
      lcd.clear();
    }
  }
}

void LCD_napredak (int red, unsigned long int var, unsigned long int minVrijednost, unsigned long int maxVrijednost)
{
  int blok = map(var, minVrijednost, maxVrijednost, 0, 16);
  int linija = map(var, minVrijednost, maxVrijednost, 0, 80);
  int bar = (linija - (blok * 5));
  byte linija1[8] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}; // |
  byte linija2[8] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}; // ||
  byte linija3[8] = { 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C}; // |||
  byte linija4[8] = { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E}; // ||||
  byte linija5[8] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}; // |||||
  lcd.createChar(1, linija1);
  lcd.createChar(2, linija2);
  lcd.createChar(3, linija3);
  lcd.createChar(4, linija4);
  lcd.createChar(5, linija5);

  for (int x = 0; x < blok; x++)
  {
    lcd.setCursor (x, red);
    lcd.write (5);
  }
  lcd.setCursor (blok, red);
  if (bar != 0) lcd.write (bar);
  if (blok == 0 && linija == 0) lcd.write (1022);

  for (int x = 16; x > blok; x--)
  {
    lcd.setCursor (x, red);
    lcd.write (1022);
  }
}
