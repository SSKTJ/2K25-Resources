/**
Morse Code Encoder Decoder
--------------------------
Encodes and Decodes Morse Code
Sends the data to the ESP module through Serial port
Also displays output of decoding in LCD Display
To be uploaded in the Arduino
**/


#define OUT_PIN 12
#define DOT_LEN 200
#define DASH_LEN 600
#define PUSH_INPUT 11
#define MODE_LED 9
#define MODE_SWITCH 10
#define DOT_THRESHOLD 200
#define DASH_THRESHOLD 600
#define CHAR_THRESHOLD 600
#define WORD_THRESHOLD 2250
#include <string.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define LCD_RS 13
#define LCD_E 8
LiquidCrystal lcd(LCD_RS, LCD_E, 7, 6, 5 ,4);
SoftwareSerial esp(2,3);

static char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R 
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // S-Z
};

static char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....",
  "-....", "--...", "---..", "----."
};

void setup() {
  pinMode(OUT_PIN, OUTPUT);
  pinMode(PUSH_INPUT, INPUT);
  pinMode(MODE_LED, OUTPUT);
  pinMode(MODE_SWITCH, INPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  esp.begin(9600);

}

void loop() {

  //Serial.println(digitalRead(MODE_SWITCH));
  if (digitalRead(MODE_SWITCH) == HIGH){
  //  Serial.println("Decoding Mode");
    digitalWrite(MODE_LED, HIGH);
    decode();
  } else {
  //  Serial.println("Encoding Mode");
    digitalWrite(MODE_LED, LOW);
    encode();
  }
  delay(500);

  //decode();
}

void encode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Encoding mode");
  esp.write("1_Enter the word/sentence");
  String message = esp.readString();
  int len = message.length();
  Serial.println(message);
  esp.println(message);
  //esp.write("\n");
  message = message.substring(2);
  for (int i = 0; i < len; i++){
    char lett = message.charAt(i);
    int ascode = int(lett);
    if (ascode >= 48 && ascode <= 57){ //is a number
      numFlash(ascode);      
    } else if (ascode >= 65 && ascode <= 90){
      ascode += 32;
      wordFlash(ascode);
    } else if (ascode >= 97 && ascode <= 122){
      wordFlash(ascode);
    } if (ascode == 32){
      delay(DASH_LEN*2);
      Serial.print("/ ");
      esp.write("/ ");
    }
  }
  if (message != ""){
    Serial.println("");
    esp.write("\n");
  }
  delay(500);
}

void wordFlash(int ascode){
  String morse = letters[ascode - 97];
  int mlen = morse.length();
  for (int i = 0; i < mlen; i++){
    char mlett = morse.charAt(i);
    Flash(mlett);
  }
  Serial.print(" ");
  esp.write(" ");
  delay(DASH_LEN);
}

void numFlash(int ascode){
  String morse = numbers[ascode-48];
  int mlen = morse.length();
  for (int i = 0; i < mlen; i++){
    char mlett = morse.charAt(i);
    Flash(mlett);
  }
  Serial.print(" ");
  esp.write(" ");
  delay(DASH_LEN);
}

void Flash(char letter){
  if (letter == '.'){
    Serial.print(".");
    esp.write(".");
    digitalWrite(OUT_PIN, HIGH);
    delay(DOT_LEN);
    digitalWrite(OUT_PIN, LOW);
    delay(DOT_LEN/2);
  } else if (letter == '-'){
    Serial.print("-");
    esp.write("-");
    digitalWrite(OUT_PIN, HIGH);
    delay(DASH_LEN);
    digitalWrite(OUT_PIN, LOW);
    delay(DOT_LEN/2);
  }
}

void decode() {
  esp.write("0_Enter the Morse Code");
  int len = 0;
  int mlen = 0;
  int scroll_cnt = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  static String curr_val = "0_";

  while(1){
  int dur_press = 0;
  static String str = ""; 

  // Detect button press
  if (digitalRead(PUSH_INPUT) == HIGH) {
    dur_press = 0;

    // Measure press duration
    while (digitalRead(PUSH_INPUT) == HIGH) {
      delay(10);
      dur_press += 10;
    }

    // Determine if it's a dot or dash based on press duration
    lcd.setCursor(mlen, 1);
    if (dur_press < DOT_THRESHOLD) {   
      lcd.print(".");
    //  Serial.print(".");
      str += "."; // Append a dot
      mlen++;
    } else if (dur_press >= DOT_THRESHOLD) {
      str += "-"; // Append a dash
    //  Serial.print("-");
      lcd.print("_");
      mlen++;
    }
  }

  int low_count = 0;

  // Detect button release and measure the gap duration
  if (digitalRead(PUSH_INPUT) == LOW) {
    low_count = 0;
    while (digitalRead(PUSH_INPUT) == LOW) {
      delay(10);
      low_count += 10;
        if (low_count > 7000){
        Serial.println("<EOT>");
        esp.write("0_<EOT>\n");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("<EOT>");
        str = "";
        curr_val = "0_";
        delay(400);
        return;
      }
    }
    
    lcd.setCursor(len, 0);
    if (low_count > CHAR_THRESHOLD ) {
      int found =0;
      for(int i = 0; i< 26 && found ==0; i++){
          if(str.compareTo(letters[i]) == 0){
            if (len >= 16){
              lcd.scrollDisplayLeft();
      //        Serial.print("scrolled");
              scroll_cnt++;
            }
            lcd.print((char)(i+'A'));
      //      Serial.print((char)(i+'A'));
            curr_val += (char)(i+'A');
            found =1;
            len++;
            break;
          }
      }
      for(int i =0; i<10 && found == 0; i++){
        if(str.compareTo(numbers[i]) == 0){
          if (len >= 16){
            lcd.scrollDisplayLeft();
            //Serial.print("scrolled");
            scroll_cnt++;
          }
          lcd.print((char)(i+'0'));
        //  Serial.print((char)(i+'0'));
          curr_val += (char)(i+'0');
          found =1;
          len++;
          break;
        }
        
      }
      Serial.println(curr_val);
      esp.println(curr_val);
      //esp.write("\n");
      if(low_count> WORD_THRESHOLD){
        if (len >= 16){
            lcd.scrollDisplayLeft();
            //Serial.print("scrolled");
            scroll_cnt++;
          }
          lcd.print(" ");
        //  Serial.print(" ");
          curr_val += " ";
          found =1;
          len++;
        }

      if (found == 0){
        lcd.print("!");
  //      Serial.print("!");
      }
        str = "";
    //  Serial.println("");
      lcd.setCursor(scroll_cnt, 1);
      lcd.print("                "); 
      mlen = scroll_cnt;         // Reset the string for the next character
    }

  }
  }
}