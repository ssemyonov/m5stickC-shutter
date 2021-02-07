#include <BleKeyboard.h>
#include <M5StickC.h>


#define HEIGHT 80
#define WIDTH 160
#define RADAR_WIDTH 5

const int frontButton = G37;
const int sideButton = G39;

int last_front_value = 0;
int cur_front_value = 0;

int last_side_value = 0;
int cur_side_value = 0;

int radarLine = 0;
int radarColour = ORANGE;

BleKeyboard bleKeyboard;

void setup() {
  
  M5.begin();

  // Init LCD
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(1);
  pinMode(frontButton, INPUT);
  pinMode(sideButton, INPUT);
  
  bleKeyboard.begin();
}

void loop() {

  if (!bleKeyboard.isConnected()) {
    drawRadar();
    return;
  }

  cur_front_value = digitalRead(frontButton);
  cur_side_value = digitalRead(sideButton);

  if(cur_front_value != last_front_value){

    if(cur_front_value==0){
      bleKeyboard.write(KEY_RETURN);
      M5.Lcd.fillScreen(GREEN);
    }
    else{
      M5.Lcd.fillScreen(BLACK);
    }
    last_front_value = cur_front_value;
  }

  if(cur_side_value != last_side_value){

    if(cur_side_value==0){
      bleKeyboard.write(KEY_RIGHT_ARROW);
      M5.Lcd.fillScreen(RED);
    }
    else{
      M5.Lcd.fillScreen(BLACK);
    }
    last_side_value = cur_side_value;
  }


  drawStatusIndicator();

//  if(bleKeyboard.isConnected()) {
//    M5.Lcd.fillScreen(GREEN);
////    Serial.println("Sending 'Hello world'...");
////    bleKeyboard.print("Hello world");
////
////    delay(1000);
////
////    Serial.println("Sending Enter key...");
////    bleKeyboard.write(KEY_RETURN);
//
//    delay(1000);
//
//    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
//
//    delay(1000);
//
//    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
//
////    Serial.println("Sending Ctrl+Alt+Delete...");
////    bleKeyboard.press(KEY_LEFT_CTRL);
////    bleKeyboard.press(KEY_LEFT_ALT);
////    bleKeyboard.press(KEY_DELETE);
////    delay(100);
////    bleKeyboard.releaseAll();
//
//  }
//  
//  M5.Lcd.setCursor(20, 10);
//  M5.Lcd.printf("Waiting for connection...");
//  
//  delay(5000);
}

void drawStatusIndicator(){
  M5.Lcd.fillCircle(15, 15, 5, millis()%1000 > 500 ? GREEN : BLACK);
}

void drawRadar(){
  M5.Lcd.fillRect(radarLine, 0, RADAR_WIDTH, HEIGHT-1, radarColour);
  radarLine += RADAR_WIDTH;
  if (radarLine > WIDTH) {
    radarLine = 0;
    radarColour = radarColour == ORANGE ? BLACK : ORANGE;
  }
  delay(50);
}
