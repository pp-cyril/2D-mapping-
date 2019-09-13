int j=0;

// ISR for encoder along x axis //
void Count_X ()
{
  int A1state = digitalRead(enc1pinA);
  int B1state = digitalRead(enc1pinB);
  if (B1state == A1state)
    x_count--;
  else
    x_count++;
}
// ISR for encoder along y axis //
void Count_Y () {
  int A2state = digitalRead(enc2pinA);
  int B2state = digitalRead(enc2pinB);
  if (B2state == A2state)
    y_count--;
  else
    y_count++;
}
// function that increments and decrements x and y coordinates according to encoder ticks //
void Path_process () {
  if ((x_count - lastx_count)> 1650 && (x_count - lastx_count )<1750) {
    last_x = x_cord;
    x_cord++;
    lastx_count = x_count;
  }
  else if ((x_count - lastx_count)< -1650 && (x_count - lastx_count )>-1750) {
    last_x = x_cord;
    x_cord--;
    lastx_count = x_count;
  }
  if ((y_count - lasty_count)> 1650 && (y_count - lasty_count )<1750) {
    last_y = y_cord;
    y_cord++;
    lasty_count = y_count;
  }

  else if ((y_count - lasty_count)< -1650 && (y_count - lasty_count )>-1750) {
    last_y = y_cord;
    y_cord--;
    lasty_count = y_count;
  }
}

void Fill_matrix() {
  if (matrix == true) {
  if ((x_cord != last_x && y_cord == last_y) || (x_cord == last_x && y_cord != last_y) || (x_cord != last_x && y_cord != last_y)) {
    P[k][0] = x_cord;
    P[k][1] = y_cord;
    // if only coordinates are different then print the new coordinates //
    if ( ((check_x != P[k][0]) && (check_y != P[k][1])) || ((check_x != P[k][0]) && (check_y == P[k][1])) || ((check_x == P[k][0]) && (check_y != P[k][1]))) {
      Serial.print("x-");
      Serial.print(P[k][0]);
      Serial.print('\t');
      Serial.print("y-");
      Serial.print(P[k][1]);
      Serial.println();
      flag = k;
      k++;
    }
    if(PS4.getButtonClick(TRIANGLE))
    Map[abs(y_cord)][abs(x_cord)] = 1;
    else {
    if (Map[abs(y_cord)][abs(x_cord)] == 7) {
    Map[abs(y_cord)][abs(x_cord)]=0;
    }
    check_x = P[flag][0]; 
    check_y = P[flag][1];
  }
}
}
}

void show_matrix () {
  Serial.println();
  Serial.print("path-map:");
  Serial.println();
  for ( int i = 0; i < 7; i++) {
    for ( int j = 0; j < 9; j++) {
      Serial.print(Map[i][j]);
      Serial.print('\t');
    }
    Serial.println();
  }
}
void show_array () {
  Serial.println();
  Serial.print("array");
  Serial.println();
 do {
    Serial.print(P[j][0]);
    Serial.print(",");
    Serial.print(P[j][1]);
    Serial.println();
    j++;
  }while(j<k);
  }
