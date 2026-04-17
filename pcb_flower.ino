int buttonstate = 0;                      //button starts at 0
unsigned long previousMillis = 0;           // starts time last checked at 0
const long mode2interval = 750;                                  // blink interval for buttonstate 2   (bigger# = slower, smaller# = faster)
const long mode3interval = 750;
long pwminterval = 5;
long prevpwm = 0;
bool ledState = false;     // initial conditions
int brightness =0;         //pwm start at zero
int fader = 5;
int blinkbrightness= 255;

void setup() {
  pinMode(23, OUTPUT);  // Leaf #1
  pinMode(32, OUTPUT); //leaf 2
  pinMode(25, OUTPUT); // leaf 3
  pinMode(17, OUTPUT); // leaf 4
  pinMode(4, OUTPUT);   // LEaf #5
  pinMode(22, OUTPUT); // leaf 6
  pinMode(27, OUTPUT); // inner circle
  
  pinMode(19, INPUT_PULLUP); // button with internal pull-up READS FROM PIN 18
  pinMode(18, OUTPUT);       // CONSTANT HIGH FOR BUTTON TO USE
  Serial.begin(9600);
  //ledcSetup(0, 5000, 12);  
  //ledcAttachPin(23,0);          //led pin at 0
}

void loop() {
  static int lastButton = HIGH;
  int reading = digitalRead(19);      //gives out sensing pin a value of eiher on or off
  digitalWrite(18,HIGH);
  // Debounce: detect press transition
  if (reading == LOW && lastButton == HIGH) {     //2 conditions to make reading sensor read just that specific readign and not multiple, keeps it from reading multiple if left pushed
    buttonstate++;                                 // increments
    if (buttonstate > 3) buttonstate = 0;         // keeps button state between wanted values of 0,1,2, 3
    delay(50); // debounce delay
  }
  lastButton = reading;   // what allows the second contion to update   

  Serial.println(buttonstate);

  if (buttonstate == 0) {           // just a solid high signal
    analogWrite(23, 50);           // brihtness depends on this value (max = 250) for all
      analogWrite(25, 50);
      analogWrite(4, 50);
      analogWrite(27, 50);
      analogWrite(32,50);
      analogWrite(17, 50);
      analogWrite(22, 50);
  }
  else if (buttonstate == 1) {           // (pwm/fade on evrything)
    digitalWrite(4, LOW);
    int currentMillis = millis();
      if (currentMillis - prevpwm >= pwminterval){
        if (brightness <= 0){
          brightness = 0;
          fader = -fader;     // //fader only switches when it hits the boundaries like a wall
        }
        else if (brightness >= 248){                                       //adjust for brightness (248 max)
          brightness= 248;
          fader = -fader;
        }
      int gammaCorrected = pow(brightness / 255.0, 2.2) * 255;
      analogWrite(23, gammaCorrected);
      analogWrite(25, gammaCorrected);
      analogWrite(4, gammaCorrected);
      analogWrite(27, gammaCorrected);
      analogWrite(32, gammaCorrected);
      analogWrite(17, gammaCorrected);
      analogWrite(22, gammaCorrected);
      brightness += fader;                          // gets brighter
      prevpwm = currentMillis;
      }
      
     
  }  
  else if (buttonstate == 2) {              // flashing triangle
    //analogWrite(23, LOW); 
    unsigned long currentMillis =   millis();                 //check how long program has been running 
    if (currentMillis - previousMillis >= mode2interval) {         // prevMills starts at 0, if time checked now - time checked last time is greater than mode2interval millisecond
      previousMillis = currentMillis;                         // update time checked
      blinkbrightness = -blinkbrightness + 255;               // led state toggles
      analogWrite(23, blinkbrightness);
      analogWrite(25, blinkbrightness);
      analogWrite(4, blinkbrightness);
      analogWrite(32, -blinkbrightness + 255);
      analogWrite(17, -blinkbrightness + 255);
      analogWrite(22, -blinkbrightness + 255);
    }
  }
  else if (buttonstate ==3 ){          // circle
    unsigned long currentMillis =   millis();                 //check how long program has been running 
    if (currentMillis - previousMillis >= mode3interval) {         // prevMills starts at 0, if time checked now - time checked last time is greater than 1 second
      previousMillis = currentMillis;  
    } 
  }
}
