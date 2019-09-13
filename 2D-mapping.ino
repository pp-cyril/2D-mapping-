#include <PS4USB.h>
#include <usbhub.h>


#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS4USB PS4(&Usb);

bool run_flag = true;  // boolean flag that calls the actualRun function when set at false which invokes the retracing of path after dry run //
bool matrix = true;  // boolean flag which when set as false disables the updation of path array //
double x = 0, y = 0, w = 0;
double s1, s2, s3, s4;  // variables to store speed

//direction and motor pins//
int d1 = 23, d2 = 25, d3 = 27, d4 = 29;
int m1 = 2, m2 = 3, m3 = 4, m4 = 5;

float sf = 1;     // scale factor for speeds along x and y
float sf_yaw = 1; // scale factor for yaw speed

bool dir1 = LOW, dir2, dir3 = LOW, dir4;
//encoder variables//
#define n 10      // number of rows of path array
int val = 1690;   // number of encounder ticks that forms one coordibate
int flag;
int check_x = -1, check_y = -1;
// encoder pins //
const int enc2pinA = 21;
const int enc2pinB = A14;
const int enc1pinA = 20;
const int enc1pinB = A11;
volatile long x_count = 0, y_count = 0;   // counters for x and y encoder ticks //
long lastx_count = 0, lasty_count = 0;
int x_cord = 0, y_cord = 0, last_x = 0, last_y = 0, k = 1;    //coordinate variables
int P[n][2];  // path array ( a 2-D array that is used to store x and y coordinates //
int t = 0;
bool RunOnce = true; // a boolean flag which when set as false does not allows x,y,w values from the PS4 controller //
int Map[7][9] = {
  {1, 7, 7, 7, 7, 7, 7, 7, 7},
  {7, 7, 7, 7, 7, 7, 7, 7, 7},
  {7, 7, 7, 7, 7, 7, 7, 7, 7},     // Path Map (presently a 7x9 array //
  {7, 7, 7, 7, 7, 7, 7, 7, 7},
  {7, 7, 7, 7, 7, 7, 7, 7, 7},
  {7, 7, 7, 7, 7, 7, 7, 7, 7},
  {7, 7, 7, 7, 7, 7, 7, 7, 7},
};
// prototype declarations //
void show_matrix();
void Count_X();
void Count_Y();
void actualRun();
void Path_process();
void Fill_matrix();
void show_array();

void setup()
{
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 USB Library Started"));
  motorSetup();
  encoderSetup();
}

void motorSetup()
{
  pinMode(m1, OUTPUT); pinMode(d1, OUTPUT);
  pinMode(m2, OUTPUT); pinMode(d2, OUTPUT);
  pinMode(m3, OUTPUT); pinMode(d3, OUTPUT);
  pinMode(m4, OUTPUT); pinMode(d4, OUTPUT);
}
void encoderSetup () {
  pinMode(enc1pinA, INPUT);
  digitalWrite(enc1pinA, HIGH);
  pinMode(enc1pinB, INPUT);
  digitalWrite(enc1pinB, HIGH);
  pinMode(enc2pinA, INPUT);
  digitalWrite(enc2pinA, HIGH);
  pinMode(enc2pinB, INPUT);
  digitalWrite(enc2pinB, HIGH);
  attachInterrupt(digitalPinToInterrupt(enc1pinA), Count_X, RISING);
  attachInterrupt(digitalPinToInterrupt(enc2pinA), Count_Y, RISING);
  Serial.println("Setup executed");
  P[0][0] = 0;
  P[0][1] = 0;
}
void loop()
{

  Usb.Task();
  PS4.setLed(Red);

  if (PS4.connected())
  {
    if (run_flag == true)
    {
      if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117 || PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2))
      {
        x = -2 * (PS4.getAnalogHat(RightHatX) - 128);
        y = 2 * (PS4.getAnalogHat(LeftHatY) - 128);
        w = PS4.getAnalogButton(R2) ? PS4.getAnalogButton(R2) / sf_yaw : 0;
        w = PS4.getAnalogButton(L2) ? -PS4.getAnalogButton(L2) / sf_yaw : w;

        if (w >= -10 && w <= 10)  w = 0;
        if (x < 20 && x > -20)  x = 0;
        if (y < 20 && y > -20)  y = 0;
      }
      else
      {
        x = 0;
        y = 0;
        w = 0;
      }
      // holonomic matrix for speed calculation //
      s1 =  sf * (-0.35 * x + 0.35 * y + 0.25 * w);
      s2 =  sf * (-0.35 * x + -0.35 * y + 0.25 * w);
      s3 =  sf * (0.35 * x + -0.35 * y + 0.25 * w);
      s4 =  sf * (0.35 * x + 0.35 * y + 0.25 * w);

      dir1 = s1 >= 0 ? 0 : 1;
      dir2 = s2 >= 0 ? 0 : 1;
      dir3 = s3 >= 0 ? 1 : 0;
      dir4 =  s4 >= 0 ? 0 : 1;

      digitalWrite(d1, dir1);
      digitalWrite(d2, dir2);
      digitalWrite(d3, dir3);
      digitalWrite(d4, dir4);

      analogWrite(m1, abs(s1));
      analogWrite(m2, abs(s2));
      analogWrite(m3, abs(s3));
      analogWrite(m4, abs(s4));



    }

  }
  else
  {
    analogWrite(m1, 0);
    analogWrite(m2, 0);
    analogWrite(m3, 0);
    analogWrite(m4, 0);

  }

  if (PS4.getButtonClick(UP))
  {
    sf += 0.5;
  }

  if (PS4.getButtonClick(DOWN))
  {
    sf -= 0.5;
  }
  if (PS4.getButtonClick(RIGHT))
  {
    sf_yaw += 0.5;
  }
  if (PS4.getButtonClick(LEFT))
  {
    sf_yaw -= 0.5;
  }
  sf = (sf > 1.5) ? 2 : sf;
  sf = (sf < 0) ? 0 : sf;

  //  Serial.print("x= ");
  //  Serial.print(x);
  //  Serial.print("  y= ");
  //  Serial.print(y);
  //  Serial.print("  w= ");
  //  Serial.print(w);

  //    Serial.print("\t\t");
  //    Serial.print("S1= ");
  //    Serial.print(s1);
  //    Serial.print("  S2= ");
  //    Serial.print(s2);
  //    Serial.print("  S3= ");
  //    Serial.print(s3);
  //    Serial.print("  S4= ");
  //    Serial.print(s4);

  //  Serial.print("\t\t");
  //  Serial.print("D1= ");
  //  Serial.print(dir1);
  //  Serial.print("  D2= ");
  //  Serial.print(dir2);
  //  Serial.print("  D3= ");
  //  Serial.println(dir3);
  //  Serial.print("  D4= ");
  //  Serial.println(dir4);
  //  Serial.print("x_count=");
  //  Serial.print(x_count);
  //  Serial.print('\t');
  //  Serial.print("y_count=");
  //  Serial.print(y_count);
  //  Serial.print('\t');
  //  Serial.println();
  Path_process();
  Fill_matrix();
  if (PS4.getButtonClick(SQUARE)) {
    matrix = false;
    show_array ();
    Serial.print(k);
  }
  if (PS4.getButtonClick(CIRCLE)) {
    run_flag = false;
  }
  if (run_flag == false) {
    if (RunOnce == true)
      actualRun();
    RunOnce = false;
  }
  if (matrix == true) {
    Serial.print("x_count=");
    Serial.print(x_count);
    Serial.print('\t');
    Serial.print("y_count=");
    Serial.print(y_count);
    Serial.print('\t');
    Serial.print(" x_cord ");
    Serial.print(x_cord);
    Serial.print(" y_cord ");
    Serial.print(y_cord);
    Serial.print(" arrayx_cord ");
    Serial.print(P[t][0]);
    Serial.print(" arrayy_cord ");
    Serial.print(P[t][1]);
    Serial.print(" x ");
    Serial.print(x);
    Serial.println();
  }
}


