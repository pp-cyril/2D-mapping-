int curr_x = 0, curr_y = 0; // variables that store current x and y ticks
int cush = 50;

void actualRun()
{
  x_count = 0, y_count = 0;
  for (int distt = 1; distt < k; distt++)
  {
    int next_x = P[distt][0] * val, next_y = P[distt][1] * val;

    if (abs(next_x - curr_x) <= cush && abs(next_y - curr_y) > cush) {  // if x coordinates are same and y coordinates are different of the current and target variables
      move_y(next_y > curr_y);                                              //the bot moves along the y axis //

    }
    if (abs(next_x - curr_x) > cush && abs(next_y - curr_y) <= cush)  {  // if y coordinates are same and x coordinates are different of the current and target variables
      move_x(next_x > curr_x);                                                  //the bot moves along the x axis //

    }
    if (abs(next_x - curr_x) <= cush && abs(next_y - curr_y) <= cush) {   // if y coordinates are same and x coordinates are same of the current and target variables
      stopp();                                                                   //the bot stops //
                                                                            
    }
    curr_x = next_x, curr_y = next_y;                                 // updates the current x and y variables
    motion();                                                         // invoke motion function
  }
  analogWrite(m1, 0); 
  analogWrite(m2, 0);
  analogWrite(m3, 0);
  analogWrite(m4, 0);
}

void stopp()
{
  x = 0, y = 0, w = 0;
}
int spd = 150;

void move_x(bool dir)   
{
  if (dir) x = spd;    // if target is greater than current ticks, move in positive direction else in negative direction
  else x = -spd;
  y = 0;
  w = 0;
}

void move_y(bool dir)
{
  if (dir) y = spd;
  else y = -spd;
  x = 0;
  w = 0;
}
void motion () {
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
  
//  Serial.print(" x ");
//  Serial.print(x);
//  Serial.print(" y ");
//  Serial.print(y);
//  Serial.print(" w ");
//  Serial.print(w);
//  Serial.println();
//  Serial.print("S1= ");
//  Serial.print(s1);
//  Serial.print("  S2= ");
//  Serial.print(s2);
//  Serial.print("  S3= ");
//  Serial.print(s3);
//  Serial.print("  S4= ");
//  Serial.print(s4);

//  Serial.print("\t\t");
//  Serial.print("D1= ");
//  Serial.print(dir1);
//  Serial.print("  D2= ");
//  Serial.print(dir2);
//  Serial.print("  D3= ");
//  Serial.println(dir3);
//  Serial.print("  D4= ");
//  Serial.println(dir4);
  delay(3000);
}
