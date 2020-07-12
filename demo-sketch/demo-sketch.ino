
// test sketch producing simple sine & cosine wave
// LED_BUILTIN is toggled to show that something is actually happening 

long count = 0;
bool on = true;

const char DELIMITER = ';';

void setup() {

  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("sig1\tsig2\tsig3\tsig4\tsig5\tsig6\tsig7\tsig8\tsig9\t\n");
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
  float rad = 0.01745329252 * (count * 10 % 360);

  // sig1
  Serial.print(sin(rad));
  Serial.print(DELIMITER);

  // sig2
  Serial.print(cos(rad));
  Serial.print(DELIMITER);

  // sig3
  Serial.print(sin(2 * rad));
  Serial.print(DELIMITER);

  // sig4
  Serial.print(cos(2 * rad));
  Serial.print(DELIMITER);

  // sig5
  Serial.print(sin(3 * rad));
  Serial.print(DELIMITER);

  // sig6
  Serial.print(cos(3 * rad));
  Serial.print(DELIMITER);

  // sig7
  Serial.print(sin(4 * rad));
  Serial.print(DELIMITER);

  // sig8
  Serial.print(cos(4 * rad));
  Serial.print(DELIMITER);

  // sig9
  Serial.print(sin(0.01745329252 * count * 1u) * 10);
  Serial.println();

  delay(1);
}
