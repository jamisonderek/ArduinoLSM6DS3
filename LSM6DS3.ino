#include <Wire.h>

#define ACCEL_ADDRESS 0x6A

byte response[2] = {0x0, 0x0};
unsigned int len = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  Wire.begin(ACCEL_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void receiveEvent(int howMany)
{
  Serial.print("receiveEvent ");
  Serial.println(howMany);

  int data = Wire.read();
  Serial.print("receiveEvent data ");
  Serial.println(data);

  if (data == 0x0F && howMany == 1)
  {
    response[0] = 0x69;
    len = 1;
    Serial.print("Set response to ");
    Serial.println(response[0]);
  }
  else if (data == 0x12 && howMany == 2)
  {
    data = Wire.read();
    Serial.print("Request to reset device. Data byte 2 is ");
    Serial.println(data);
  }
  else if (data == 0x12 && howMany == 1)
  {
    Serial.println("Returning that we are out of reset.");
    response[0] = 0x4;
    len = 1;
    Serial.print("Set response to ");
    Serial.println(response[0]);
  }
  else if (data == 0x10 && howMany == 2)
  {
    data = Wire.read();
    Serial.print("CTRL1_XL - Set range and frequency.  data:");
    Serial.println(data);
    int theSpeed = (data >> 2) & 3;
    Serial.print("theSpeed is ");
    Serial.println(theSpeed);
    if (theSpeed == 2) {
      Serial.println("Speed is +-4g");
    } else {
      Serial.println("Unsupported speed.");
    }

    if ((data>>4) == 1) {
      Serial.println("12.5 Hz");
    } else {
      Serial.println("Unsupported freq.");
    }
  }
  else if (data == 0x2C && howMany == 1)
  {
    Serial.println("Request for Z-axis");
    int acc = random(8000,9000);
    Serial.print("Returning value ");
    Serial.println(acc);
    response[0] = acc & 255;
    response[1] = acc >> 8;
    len = 2;
  }
  else if (data == 0x1E && howMany == 1)
  {
    Serial.println("STATUS_REG");
    Serial.println("Setting XLDA to 1, we have accelerometer data.");
    response[0] = 0x1;
    len = 1;
  }
  else
  {
    while (Wire.available())
    {
      data = Wire.read();
      Serial.print("data ");
      Serial.println(data);
    }
  }
}

void requestEvent()
{
  Serial.println("requestEvent");
  int t = 0;
  for (int i=1;i<3000;i++)
   for (int j=1;j<1000;j++)
      if (i<<2==j) t++; else t--;
  Serial.print(t);
  Serial.println("Sending response");
  Wire.write(response, len);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}
