/**
 *  Traffic Lights is a simulated traffic lights system.
 *  Two traffic lights, one for vehicles and one for
 *  pedestrians, are illustrated with LEDs. There are two
 *  buttons used in this program. When either is pressed,
 *  the "change lights" sequence begins, stopping vehicles
 *  and allowing pedestrians to cross the road.
 *  
 *  When the changing lights sequence is nearing its end,
 *  the green light of the pedestrian traffic light blinks
 *  as well a short beep sound is played to warn pedestrians
 *  the time is nearly over for crossing.
 *  
 *  When it is dark, the traffic lights system is inactive.
 *  This is illustrated by the yellow light on the vehicle
 *  traffic light blinking. Darkness is measured with a
 *  photoresistor that has to be calibrated upon start.
 *  
 *  Author: Børre A. Opedal Lunde
 *  Date created (yyyy/mm/dd): 2019/05/09
 *  Date updated (yyyy/mm/dd): 2019/05/09
 */

// Led.h and Button.h libraries are included in the project.
#include <Led.h>
#include <Button.h>

// HIGH and LOW is redefined ON and OFF to create
// an easier understanding of the code when read.
// This is used particularly when turning a light
// (LED) on or off.
#define ON HIGH
#define OFF LOW

// The light emitting diodes of the vehicle and pedestrian traffic lights.
Led vehicleTrafficLightGreenLed(5), vehicleTrafficLightYellowLed(6), vehicleTrafficLightRedLed(7),
    pedestrianTrafficLightGreenLed(2), pedestrianTrafficLightRedLed(3);

// On-board Arduino Uno light emitting diode used to signal calibration of photoresistor.
Led internalLed(13);

// The traffic light button is in reality two buttons that
// work as one in the program. This is because the button is
// in practice on either side of the road, controlling the
// same traffic light system.
Button button(8);

const char photoresistorPin = A0; // the analog pin the photoresistor element is connected to.
int photoresistorValue;           // the analog value of the photoresistor.
int photoresistorHigh = 0;        // high value, used for calibration of the photoresistor.
int photoresistorLow = 1023;      // low value, used for calibration of the photoresistor.

const int blinkInterval = 500;  // interval in milliseconds used when blinking lights.

const char piezoPin = 12;  // the digital pin the piezo element is connected to.

// Functions memberfield.
void calibratePhotoresistor();
void playTone(long duration);
boolean isDark();
void changeLights();

/**
 * Calibrates the photoresistor, setting highest and lowest values.
 * To calibrate, move your hand up and down over the photoresistor.
 * This changes the amount of light that reaches it. Its high
 * and low values are calibrated accordingly.
 */
void calibratePhotoresistor() {
  internalLed.setState(ON);  // the internal LED is turned on to signal the calibration is active.
  // Calibration lasts 5 seconds.
  while (millis() < 5000) {
    photoresistorValue = analogRead(photoresistorPin);  // the photoresistor value is updated in real time.
    // High and low photoresistor values are stored in variables, only updated if they are higher or lower than before.
    photoresistorHigh = photoresistorValue > photoresistorHigh ? photoresistorValue : photoresistorHigh;
    photoresistorLow  = photoresistorValue < photoresistorLow  ? photoresistorValue : photoresistorLow;
  }
  internalLed.setState(OFF);  // the internal LED is turned off to signal the calibration is inactive.
}

/**
 * Plays a tone on the piezo element.
 * @param  duration
 *         duration the piezo element plays
 *         the tone in milliseconds.
 */
void playTone(long duration) {
  const int pitch = 3000;
  tone(piezoPin, pitch, duration);
}

/**
 * Returns true if the photoresistor is held over.
 */
boolean isDark() {
  // The calibrated high and low values of the photoresistor
  // is used to measure what is considered dark.
  return analogRead(photoresistorPin) < (photoresistorHigh + photoresistorLow) / 2;
}

/**
 * "Change lights" sequence.
 * Allows a pedestrian to cross the road. The vehicle
 * traffic light turns red and the pedestrian traffic
 * light turns green. Afterwards it returns to the
 * default state.
 */
void changeLights() {
  // Vehicle traffic light:
  // green off, yellow on
  vehicleTrafficLightGreenLed.setState(OFF);
  vehicleTrafficLightYellowLed.setState(ON);

  delay(3000); // Wait 3 seconds.

  // Vehicle traffic light:
  // yellow off, red on
  vehicleTrafficLightYellowLed.setState(OFF);
  vehicleTrafficLightRedLed.setState(ON);

  // Pedestrian traffic light:
  // red off, green on
  pedestrianTrafficLightRedLed.setState(OFF);
  pedestrianTrafficLightGreenLed.setState(ON);

  delay(3000); // Wait 3 seconds.

  // Pedestrian traffic light:
  // Blink green on/off 5 times.
  for (int i = 0; i < 5; i++) {
    pedestrianTrafficLightGreenLed.setState(OFF);
    delay(blinkInterval);  // Wait 0.5 seconds.
    pedestrianTrafficLightGreenLed.setState(ON);
    playTone(100);         // Play short (100 ms) beep sound.
    delay(blinkInterval);  // Wait 0.5 seconds.
  }

  // Pedestrian traffic light:
  // green off, red on
  pedestrianTrafficLightGreenLed.setState(OFF);
  pedestrianTrafficLightRedLed.setState(ON);

  // Vehicle traffic light:
  // yellow on
  vehicleTrafficLightYellowLed.setState(ON);

  delay(3000); // Wait 3 seconds.

  // Vehicle traffic light:
  // red off, yellow off, green on
  vehicleTrafficLightRedLed.setState(OFF);
  vehicleTrafficLightYellowLed.setState(OFF);
  vehicleTrafficLightGreenLed.setState(ON);
}

/**
 * Setup is run once the program starts.
 */
void setup() {
  // Calibrates the photoresistor so that it is
  // possible to measure when it is light and dark.
  // Calibration takes 5 seconds.
  calibratePhotoresistor();
}

/**
 * Loop runs over and over once setup is complete.
 */
void loop() {
  // If it is dark, the yellow light on the vehicle
  // traffic light blinks on and off. All other
  // lights are turned off.
  if (isDark()) {
    // Turns off other lights of both traffic lights.
    vehicleTrafficLightRedLed.setState(OFF);
    vehicleTrafficLightGreenLed.setState(OFF);
    pedestrianTrafficLightRedLed.setState(OFF);
    pedestrianTrafficLightGreenLed.setState(OFF);

    // Blinking the yellow lights.
    vehicleTrafficLightYellowLed.setState(ON);
    delay(blinkInterval); // Wait 0.5 seconds.
    vehicleTrafficLightYellowLed.setState(OFF);
    delay(blinkInterval); // Wait 0.5 seconds.
    return;
  }

  // The default state is active because it is not dark.
  // Vehicle traffic light: green on
  // Pedestrian traffic light: red on
  vehicleTrafficLightGreenLed.setState(ON);
  pedestrianTrafficLightRedLed.setState(ON);

  // Updates the button so it is possible to test
  // if it's pressed or has switched state.
  button.update();

  // Starts the "change lights" sequence if the button is pressed.
  // No matter how long the button is pressed, only one press is
  // registered.
  if (button.isPressed() && button.hasSwitchedState()) {
    changeLights();
  }
}
