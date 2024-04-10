 #include "HX711.h"

#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

HX711 scale;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  // Serial.println("Readings:");
}

void loop() {
  // Serial.print("Reading: ");
  long reading = scale.get_units(10);
  //Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  float final = reading*0.453592;
  //final = final - 6.3;
  if(final >0 && final <=6.3)
  {
    final = 0.5;
  Serial.println(final);
  }
  else if (final >= 6.3)
  {
    final = final - 5.8;
    Serial.println(final);
  }
  else
  {
    Serial.println(0);
  }
  // Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  //Serial.println();
  final = 0;
  delay(5000);
}
