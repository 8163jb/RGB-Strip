/* -------------------------------------------------------------------
   Test code written for the RGB LED strip project -
   http://doayee.co.uk/rgb-led-strip-room-lighting-build-part-1/
   Fades between the colour spectrum 
   Written and tested by Tom Cousins
   http://www.doayee.co.uk
   -------------------------------------------------------------------*/

#define red 9      //Define the colour pins
#define green 10
#define blue 11

void setup()
{
  digitalWrite(red, OUTPUT);    //Set colours as outputs
  digitalWrite(green, OUTPUT);
  digitalWrite(blue, OUTPUT);
  analogWrite(red, 0);          //Set them off
  analogWrite(green, 0);
  analogWrite(blue, 0);
  delay(100);                   //Makes it look smoother when turning on
  up(red);                      //Have to do this outside main loop
}

void loop()
{
  up(green);   //Fade through spectrum
  down(red);
  up(blue);
  down(green);
  up(red);
  down(blue);
}

void up(int pin) //Fade a colour up
{
  for(int i = 0; i <= 255; i++)
  {
    analogWrite(pin, i);
    delay(10);
  }
}

void down(int pin) //Fade a colour down
{
  for(int i = 255; i >= 0; i--)
  {
    analogWrite(pin, i);
    delay(10);
  }
}
