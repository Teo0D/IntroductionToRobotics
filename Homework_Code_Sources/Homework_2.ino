/*
Use a separate potentiometer for controlling each color of the RGB LED: Red,
Green, and Blue. This control must leverage digital electronics. Specifically,
you need to read the potentiometer’s value with Arduino and then write a
mapped value to the LED pins.
*/
const int sliderBlue = A0;
const int sliderGreen = A1;
const int sliderRed = A2;
const int outBlue = 9;
const int outGreen = 10;
const int outRed = 11;

int rawValueBlue = 0;
int rawValueGreen = 0;
int rawValueRed = 0;
int mappedValueBlue = 0;
int mappedValueRed = 0;
int mappedValueGreen = 0;

const int minValueSlider = 0;
const int maxValueSlider = 1023;
const int minValueChannel = 0;
const int maxValueChannel = 255;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // set pinmode for the RGB as outputs
  pinMode(outBlue, OUTPUT);
  pinMode(outGreen, OUTPUT);
  pinMode(outRed, OUTPUT);
}
// the loop routine runs over and over again forever:
void loop() {
  //// get raw values from sliders:
  rawValueBlue = analogRead(sliderBlue); 
  rawValueGreen = analogRead(sliderGreen); 
  rawValueRed = analogRead(sliderRed); 

  //// get mapped values from slider range to channel range:
  mappedValueBlue = map(rawValueBlue, 
                      minValueSlider, maxValueSlider, 
                      minValueChannel, maxValueChannel);
  mappedValueGreen = map(rawValueGreen, 
                      minValueSlider, maxValueSlider, 
                      minValueChannel, maxValueChannel);
  mappedValueRed = map(rawValueRed, 
                      minValueSlider, maxValueSlider, 
                      minValueChannel, maxValueChannel);

  // print out the values you output:
  Serial.println(String(mappedValueBlue) + ' ' + String(mappedValueGreen) + ' ' + String(mappedValueRed));
  
  //// output mapped values on channels:
  analogWrite(outBlue, mappedValueBlue);
  analogWrite(outGreen, mappedValueGreen);
  analogWrite(outRed, mappedValueRed);

  delay(1);    	// delay in between reads for stability
}
