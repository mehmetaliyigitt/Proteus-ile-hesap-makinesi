#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'7','8','9','D'},
  {'4','5','6','C'},
  {'1','2','3','B'},
  {'*','0','#','A'}
};

byte rowPins[ROWS] = { 0, 1, 2, 3 };
byte colPins[COLS] = { 4, 5, 6, 7 };

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int lcdRs = 8, lcdEn = 9, lcdD4 = 10, lcdD5 = 11, lcdD6 = 12, lcdD7 = 13;
LiquidCrystal lcd(lcdRs, lcdEn, lcdD4, lcdD5, lcdD6, lcdD7);

long ilksayi, ikincisayi, sonuc;
char currentKey, islem;
boolean calculationComplete = false;

void setup() {
  lcd.begin(16, 2);
  lcd.print("HESAP MAKINESI");
  lcd.setCursor(0, 1);
  lcd.print("CALISIYOR");

  delay(2000);
  lcd.clear();
}

void loop() {
  currentKey = customKeypad.getKey();

  if (currentKey != NO_KEY)
    processKey();

  if (calculationComplete)
    performCalculation();

  displayResult();
}

void processKey() {
  lcd.clear();
  if (currentKey == '*') {
    resetCalculator();
  } else if (isDigit(currentKey)) {
    addDigitTosonuc(currentKey - '0');
  } else if (currentKey == '#') {
    finalizeikincisayi();
  } else {
    setislem();
  }
}

void resetCalculator() {
  Serial.println("RESETLENIYOR");
  sonuc = ilksayi = ikincisayi = 0;
  calculationComplete = false;
}

bool isDigit(char key) {
  return key >= '0' && key <= '9';
}

void addDigitTosonuc(int digit) {
  Serial.print("Rakam ekleme: ");
  Serial.println(digit);
  sonuc = (sonuc * 10) + digit;
}

void finalizeikincisayi() {
  Serial.println("Ikinci Sayi Tamamlama");
  ikincisayi = sonuc;
  calculationComplete = true;
}

void setislem() {
  ilksayi = sonuc;
  sonuc = 0;
  if (currentKey == 'A') {
    Serial.println("TOPLAMA");
    islem = '+';
  } else if (currentKey == 'B') {
    Serial.println("CIKARMA");
    islem = '-';
  } else if (currentKey == 'C') {
    Serial.println("CARPMA");
    islem = '*';
  } else if (currentKey == 'D') {
    Serial.println("BOLME");
    islem = '/';
  }
}

void performCalculation() {
  if (islem == '+')
    sonuc = ilksayi + ikincisayi;
  else if (islem == '-')
    sonuc = ilksayi - ikincisayi;
  else if (islem == '*')
    sonuc = ilksayi * ikincisayi;
  else if (islem == '/')
    sonuc = ilksayi / ikincisayi;
}

void displayResult() {
  lcd.setCursor(0, 0);
  lcd.print(ilksayi);
  lcd.print(islem);
  lcd.print(ikincisayi);

  if (calculationComplete) {
    lcd.print(" =");
    lcd.print(sonuc);
  }

  lcd.setCursor(0, 1);
  lcd.print(sonuc);
}
