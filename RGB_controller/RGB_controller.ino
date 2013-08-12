/*-------------------------------------------------------------------
  Code for the controller in the RGB LED strip project - 
  http://doayee.co.uk/rgb-led-strip-build-part-2/
  Mode 1 - allows control of colour via 3 potentiometers -
    + Red pot connected to pin A1
    + Green pot connected to pin A2
    + Blue pot connected to pin A3
  Mode 2 - Fades through the colour spectrum - 
    + Blue pot controls brightness (needs improving)
    + Green pot controls speeed
    + Red pot is acting as a switch into "Rave mode"
  Mode 3 - Reacts to music (still being developed)
    + Pots do nothing
  Written and tested by Tom Cousins
  http://www.doayee.co.uk
  -------------------------------------------------------------------*/

#define red 9     //Define inputs and pins
#define green 10
#define blue 11
#define b1 4
#define b2 3
#define b3 2
#define rPot A1
#define gPot A2
#define bPot A3
#define audio A0

int Vol = 0;     //Variables used in the music reaction mode
int Vmax = 0;
int tic = 0;

int mode = 1;    // Starts of in Mode 1

int prevVals[3] = {0, 0, 0};       //Used for keeping a track of the current colours
int names[3] = {red, green, blue}; //Used as a reference

void setup()
{
  pinMode(b1, INPUT);         //Set all the digital input pins and enables internal pull-up resistors
  digitalWrite(b1, HIGH);
  pinMode(b2, INPUT);
  digitalWrite(b2, HIGH);
  pinMode(b3, INPUT);
  digitalWrite(b3, HIGH);
}

void loop()
{
  if(digitalRead(b1) == LOW)        //If button 1 is pressed
  {
    while(digitalRead(b1) == LOW);  //Mode is set to 1
    mode = 1;
  }
  if(digitalRead(b2) == LOW)        //etc...
  {
    while(digitalRead(b2) == LOW);
    mode = 2;
  }
  if(digitalRead(b3) == LOW)
  {
    while(digitalRead(b3) == LOW);
    mode = 3;
  }
  if(mode == 2)                     //If the mode is equal to 2
  {
    allOff();                       //Directs program to a command, this fades all the colours to 0 
    up(red);                        //Directs program to a command, this fades the desired colour to the desired maximum 
    while(digitalRead(b3) == HIGH && digitalRead(b1) == HIGH && analogRead(rPot) <= 1000)  //Repeat until the mode is changed
    {
      up(green);                    //Fades through the colour spectrum
      down(red);
      up(blue);
      down(green);
      up(red);
      down(blue);
    }  
    if(digitalRead(b3) == HIGH && analogRead(rPot) >= 1000 && digitalRead(b1) == HIGH)    //If the "rave mode" is on
    {
      allOff();
      while(digitalRead(b3) == HIGH && analogRead(rPot) >= 1000 && digitalRead(b1) == HIGH)  //While the "rave mode" is on
      {
        int i = random(9, 12);      //Have a rave       yay
        analogWrite(i, 255);
        prevVals[i - 9] = 255;
        delay(random(50, 100));
        analogWrite(i, 0);
        prevVals[i - 9] = 0;
        delay(random(50, 100));
      }
    }
  }
  if(mode == 1) //If the mode is 1
  {
    analogWrite(red, map(analogRead(rPot), 0, 1024, 0, 255));   //Map the values from the pots to the colours
    prevVals[0] = map(analogRead(rPot), 0, 1024, 0, 255);       //and save the colours in the prevVals variable
    analogWrite(green, map(analogRead(gPot), 0, 1024, 0, 255));
    prevVals[1] = map(analogRead(gPot), 0, 1024, 0, 255);
    analogWrite(blue, map(analogRead(bPot), 0, 1024, 0, 255));
    prevVals[2] = map(analogRead(bPot), 0, 1024, 0, 255);
  }
  if(mode == 3) //If the mode is 3
  {
    allOff();   //Fade the colours down
    while(digitalRead(b1) == HIGH && digitalRead(b2) == HIGH) //Repeat until mode is changed
    {
      for(int d = 0; d <= 2; d++) //contantly fade the colours down 
      {
        if(prevVals[d] > 0)
        {
          analogWrite(names[d], prevVals[d] - 1);
          prevVals[d]--;
          delay(1);
        }
      }
      if (Vmax > 0 && tic == 4) //Every 4 cycles
      {
        Vmax--;  //Reduce the maximum volume by 1
        tic = 0;
      }
      else tic++;
      Vol = analogRead(audio);  //Read from the analog input
      Vmax = max(Vmax, Vol);    //Set the max volume to be which ever is biggest out of the previous saves and now
      if (Vol > (Vmax / 1.1))   //if the vol is greater than Vmax / 1.1
      {
        analogWrite(red, 255);  //Make the red colour flash up
        prevVals[0] = 255;
        analogWrite(green, prevVals[1] / 2);
        prevVals[1] /= 2;
        analogWrite(blue, prevVals[2] / 2);
        prevVals[2] /= 2;
      }
      else if(Vol > (Vmax / 1.25))  //If the vol is greater than Vmax / 1.25
      {
        analogWrite(green, 255);  //Make the green colour flash
        prevVals[1] = 255;
        analogWrite(blue, prevVals[2] / 2);
        prevVals[2] /= 2;
      }
      else if(Vol > (Vmax / 1.5)) //If th evol is greater than Vmax / 1.25
      {
        analogWrite(blue, 255); //Make the blue flash up
        prevVals[3] = 255;
      }
    }
  }
}
  
void up(int pin) //Fade a colour up to the desired level at the desired speed
{
  for(int i = 0; i <= 255; i++)
  {
    analogWrite(pin, i / (((1024 - analogRead(bPot)) / 100) +1));
    prevVals[pin - 9] = i / (((1024 - analogRead(bPot)) / 100) +1);
    delay(30 - map(analogRead(gPot), 0, 1024, 0, 30));
    if(digitalRead(b3) == LOW || digitalRead(b1) == LOW || analogRead(rPot) >= 1000) break;  //If a different mode is activated break
  }
}

void down(int pin) //Fade a colour down from the desired level at the desired speed
{
  for(int i = 255; i >=0 ; i--)
  {
    analogWrite(pin, i / (((1024 - analogRead(bPot)) / 100) +1));
    prevVals[pin - 9] = i / (((1024 - analogRead(bPot)) / 100) +1);
    delay(30 - map(analogRead(gPot), 0, 1024, 0, 30));
    if(digitalRead(b3) == LOW || digitalRead(b1) == LOW || analogRead(rPot) >= 1000) break;  //If a different mode is activated break
  }
}
  
void allOff() //Fades colours to 0
{
  for(int i = prevVals[0]; i >= 0; i--)
  {
    analogWrite(red, i);
    prevVals[0] = i;
    delay(1);
  }
  for(int i = prevVals[1]; i >= 0; i--)
  {
    analogWrite(green, i);
    prevVals[1] = i;
    delay(1);
  }
  for(int i = prevVals[2]; i >= 0; i--)
  {
    analogWrite(blue, i);
    prevVals[2] = i;
    delay(1);
  }
}

