
// test sketch producing simple sine & cosine wave
// LED_BUILTIN is toggled to show that something is actually happening 

long count = 0;
bool on = true;

const char DELIMITER = ',';

void setup() {

  Serial.begin(9600);
  Serial.println("#\tsin\tcos");

  pinMode(LED_BUILTIN, OUTPUT);
}

// print values to serial port, all values in one line
// values separated by DELIMITER
void printToSerialPort(float sine, float cosine) {
  
  Serial.print(sine); Serial.print(DELIMITER);
  Serial.print(cosine); Serial.println();  
}


void loop() {

  if (on) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  on = !on;

  ++count;

  // sine and cosine expect angle in radians
  float rad = 0.01745329252 * (count % 360);
  
  double sine = sin(rad);
  float cosine = cos(rad);

  printToSerialPort(sine, cosine);
  delay(10);
}
