#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define LED_PIN 9
#define CURRENT_SENSOR_PIN A0
#define VOLTAGE_SENSOR_PIN A1

#define DayPushButton 4         //D4
#define MonthPushButton 5       //D5
#define reset 6                 //D6


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(DayPushButton, INPUT);
  pinMode(MonthPushButton, INPUT);
  pinMode(reset, INPUT);
  
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
}

void loop() {
  // Read the analog value from the current sensor
  int currentSensorValue = analogRead(CURRENT_SENSOR_PIN);

  int prevDayPower;
  int prevDayCost;
  int prevMonthPower;
  int prevMonthAmount;
  
  // Convert the analog value to current (in milliamps)
  // ACS712 sensitivity is 66mV/A for ACS712-05/ACS712-10, and 185mV/A for ACS712-30
  // Depending on your specific module, adjust the sensitivity value accordingly
  float current = (-currentSensorValue+512) / 66.0; // for ACS712-05/ACS712-10
  
  // Read the analog value from the voltage sensor
  int voltageSensorValue = analogRead(VOLTAGE_SENSOR_PIN);
  
  // Convert the analog value to voltage (0-5V)
  float voltage = (voltageSensorValue / 1023.0) * 5.0;

   int costperwatt=5;
  int timeInhours=24;
  int power;
  power=voltage*current;
  Serial.println(power);

  if(DayPushButton==HIGH){

                                            //day wise power calculation
  int totalPowerPerday=power*timeInhours;
  int dayWiseAmount=totalPowerPerday*costperwatt;                 // Display voltage and current on OLED

  display.clearDisplay();
  display.print("Power Per Day : ");
  display.print(totalPowerPerday, 2); // Display voltage with 2 decimal points
  display.println("Watt");
  display.print("Bill Per Day : ");
  display.print(dayWiseAmount, 2); // Display current with 2 decimal points
  display.println(" rupees");
  display.display();

} else if(MonthPushButton==HIGH){

  int monthWisePower=power*720;       //30 days
  int monthWiseAmount;
  if(monthWisePower<=30){

  monthWiseAmount=monthWisePower*1.9;

  display.clearDisplay();
  display.print("Power Per Month : ");
  display.print(monthWisePower, 2); // Display voltage with 2 decimal points
  display.println("watt");
  display.print("Bill Per Month : ");
  display.print(monthWiseAmount, 2); // Display current with 2 decimal points
  display.println(" rupees");
  display.display();
  
  }
  else if(monthWisePower>=30 && monthWisePower<=45){
 
  monthWiseAmount=monthWisePower*3;
  display.clearDisplay();
  display.print("Power Per Month : ");
  display.print(monthWisePower, 2); // Display voltage with 2 decimal points
  display.println("Watt");
  display.print("Bill Per Month : ");
  display.print(monthWiseAmount, 2); // Display current with 2 decimal points
  display.println(" rupees");
  display.display();

  }
  

} else if (reset==HIGH){

  display.clearDisplay();
  display.print("Hello Avengers");

  display.println("ELECTRIC BILL GENERATOR");
  display.display();

} 
  
 
  
  delay(1000); // Wait for 1 second before the next reading
}
