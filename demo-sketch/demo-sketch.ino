
// test sketch producing simple sine & cosine wave
// LED_BUILTIN is toggled to show that something is actually happening 

long count = 0;
bool on = true;

const char DELIMITER = ',';

void setup() {

  Serial.begin(9600);
  Serial.print("sin");
  Serial.print(DELIMITER);
  Serial.print("cos");
  Serial.print(DELIMITER);
  Serial.print("random");
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
}

// print values to serial port, all values in one line
// values separated by DELIMITER
void printToSerialPort(float sine, float cosine, float rnd) {
  
  Serial.print(sine); 
  Serial.print(DELIMITER);
  Serial.print(cosine); 
  Serial.print(DELIMITER);
  Serial.print(rnd);
  Serial.println();  
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
  
  float sine = sin(rad);
  float cosine = 2 * cos(rad);
  float rnd = (float)random(500)/1000;

  printToSerialPort(sine, cosine, rnd);
  delay(1);
}
