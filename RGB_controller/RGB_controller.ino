#define red 9
#define green 10
#define blue 11
#define b1 4
#define b2 3
#define b3 2
#define rPot A1
#define gPot A2
#define bPot A3
#define audio A0

int Vol = 0;
int Vmax = 0;
int tic = 0;

int mode = 1;

int prevVals[3] = {0, 0, 0};
int names[3] = {red, green, blue};

void setup()
{
  pinMode(b1, INPUT);
  digitalWrite(b1, HIGH);
  pinMode(b2, INPUT);
  digitalWrite(b2, HIGH);
  pinMode(b3, INPUT);
  digitalWrite(b3, HIGH);
}

void loop()
{
  if(digitalRead(b1) == LOW)
  {
    while(digitalRead(b1) == LOW);
    mode = 1;
  }
  if(digitalRead(b2) == LOW)
  {
    while(digitalRead(b2) == LOW);
    mode = 0;
  }
  if(digitalRead(b3) == LOW)
  {
    while(digitalRead(b3) == LOW);
    mode = 2;
  }
  if(mode == 0)
  {
    allOff();
    up(red);
    while(digitalRead(b3) == HIGH && digitalRead(b1) == HIGH && analogRead(rPot) <= 1000)
    {
      up(green);
      down(red);
      up(blue);
      down(green);
      up(red);
      down(blue);
    }  
    if(digitalRead(b3) == HIGH && analogRead(rPot) >= 1000 && digitalRead(b1) == HIGH)
    {
      allOff();
      while(digitalRead(b3) == HIGH && analogRead(rPot) >= 1000 && digitalRead(b1) == HIGH)
      {
        int i = random(9, 12);
        analogWrite(i, 255);
        prevVals[i - 9] = 255;
        delay(random(50, 100));
        analogWrite(i, 0);
        prevVals[i - 9] = 0;
        delay(random(50, 100));
      }
    }
  }
  if(mode == 1)
  {
    analogWrite(red, map(analogRead(rPot), 0, 1024, 0, 255));
    prevVals[0] = map(analogRead(rPot), 0, 1024, 0, 255);
    analogWrite(green, map(analogRead(gPot), 0, 1024, 0, 255));
    prevVals[1] = map(analogRead(gPot), 0, 1024, 0, 255);
    analogWrite(blue, map(analogRead(bPot), 0, 1024, 0, 255));
    prevVals[2] = map(analogRead(bPot), 0, 1024, 0, 255);
  }
  if(mode == 2)
  {
    allOff();
    while(digitalRead(b1) == HIGH && digitalRead(b2) == HIGH)
    {
      for(int d = 0; d <= 2; d++)
      {
        if(prevVals[d] > 0)
        {
          analogWrite(names[d], prevVals[d] - 1);
          prevVals[d]--;
        }
      }
      if (Vmax > 0 && tic == 4) 
      {
        Vmax--;
        tic = 0;
      }
      else tic++;
      Vol = analogRead(audio);
      Vmax = max(Vmax, Vol);
      if (Vol > (Vmax / 1.1))
      {
        analogWrite(red, 255);
        prevVals[0] = 255;
        analogWrite(green, prevVals[1] / 2);
        prevVals[1] /= 2;
        analogWrite(blue, prevVals[2] / 2);
        prevVals[2] /= 2;
      }
      else if(Vol > (Vmax / 1.25))
      {
        analogWrite(green, 255);
        prevVals[1] = 255;
        analogWrite(blue, prevVals[2] / 2);
        prevVals[2] /= 2;
      }
      else if(Vol > (Vmax / 1.5))
      {
        analogWrite(blue, 255);
        prevVals[3] = 255;
      }
    }
  }
}
  
void up(int pin) //Fade a colour up
{
  for(int i = 0; i <= 255; i++)
  {
    analogWrite(pin, i / (((1024 - analogRead(bPot)) / 100) +1));
    prevVals[pin - 9] = i / (((1024 - analogRead(bPot)) / 100) +1);
    delay(30 - map(analogRead(gPot), 0, 1024, 0, 30));
    if(digitalRead(b3) == LOW || digitalRead(b1) == LOW || analogRead(rPot) >= 1000) break;
  }
}

void down(int pin) //Fade a colour down
{
  for(int i = 255; i >=0 ; i--)
  {
    analogWrite(pin, i / (((1024 - analogRead(bPot)) / 100) +1));
    prevVals[pin - 9] = i / (((1024 - analogRead(bPot)) / 100) +1);
    delay(30 - map(analogRead(gPot), 0, 1024, 0, 30));
    if(digitalRead(b3) == LOW || digitalRead(b1) == LOW || analogRead(rPot) >= 1000) break;
  }
}
  
void allOff()
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

