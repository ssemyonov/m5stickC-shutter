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

uint8_t brightness = 100;

BleKeyboard bleKeyboard;

void setup() {
  
  M5.begin();

  // Init LCD
  M5.Lcd.setBrightness(brightness);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(1);
  pinMode(frontButton, INPUT);
  pinMode(sideButton, INPUT);

  drawBatteryPercentage();

  delay(1000);
  
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
  drawBatteryPercentage();
}

void drawStatusIndicator(){
  M5.Lcd.fillCircle(15, 15, 5, millis()%1000 > 500 ? GREEN : BLACK);
}

void drawBatteryPercentage(){
  M5.Lcd.setRotation(0);
  M5.Lcd.setCursor(8, 8);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("%d%%",getBatteryPercentage());
  M5.Lcd.setRotation(1);
}

int getBatteryPercentage(){
  uint16_t vbatData = M5.Axp.GetVbatData();
  double vbat = vbatData * 1.1 / 1000;
  return ((int)(100.0 * ((vbat - 3.0) / (4.07 - 3.0)))/10)*10;
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
