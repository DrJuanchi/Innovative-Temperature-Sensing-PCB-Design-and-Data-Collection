#include <Wire.h>

const byte ADC_0 = 0x48;        // These use the Wire commands (BUS 0)
const byte ADC_1 = 0x4A;
const byte ADC_2 = 0x49;

const byte ADC_3 = 0x4B;        // These use the Wire1 commands (BUS 1)
const byte ADC_4 = 0x48;

//int ADC_ADDR = {ADC_0, ADC_1, ADC_2, ADC_3, ADC_4};
int ADC_ARRAY[10][4] =  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

//Set pins for I2C_0
#define I2C0_SDA 23
#define I2C0_SCL 22
//Set pins for I2C_1
#define I2C1_SDA 21
#define I2C1_SCL 19

TwoWire I2Cone = TwoWire(0); //I2C0 bus
TwoWire I2Ctwo = TwoWire(1); //I2C1 bus

const int Channel_Array[8] = {0b10001100, 0b11001100, 0b10011100, 0b11011100, 0b10101100, 0b11101100, 0b10111100, 0b11111100};

void setup() {
  Serial.begin(115200);
  //Initiate buses
  I2Cone.begin(I2C0_SDA, I2C0_SCL, 115200); 
  I2Ctwo.begin(I2C1_SDA, I2C1_SCL, 115200);
}

void ADC0(){
  I2Cone.beginTransmission(ADC_0);
  for (int i = 0; i < 2; i++){
    for(int j = 0; j < 4; j++){
       I2Cone.write(Channel_Array[i+j]);
       I2Cone.endTransmission();
       I2Cone.requestFrom(ADC_0,8);
      if(I2Cone.available() == 1){
      ADC_ARRAY[8+i][3-j] = I2Cone.read();
    }
  }
  }
}

void ADC1(){
  I2Cone.beginTransmission(ADC_1);
  for (int i = 0; i < 2; i++){
    for(int j = 0; j < 4; j++){
       I2Cone.write(Channel_Array[i+j]);
       I2Cone.endTransmission();
       I2Cone.requestFrom(ADC_1,8);
      if(I2Cone.available() == 1){
      ADC_ARRAY[6+i][3-j] = I2Cone.read();
    }
  }
  }
}


void ADC2(){
  I2Cone.beginTransmission(ADC_2);
  for (int i = 0; i < 2; i++){
    for(int j = 0; j < 4; j++){
       I2Cone.write(Channel_Array[i+j]);
       I2Cone.endTransmission();
       I2Cone.requestFrom(ADC_2,8);
      if(I2Cone.available() == 1){
      ADC_ARRAY[4+i][3-j] = I2Cone.read();
    }
  }
  }
}


void ADC3(){
  I2Ctwo.beginTransmission(ADC_3);
  for (int i = 0; i < 2; i++){
    for(int j = 0; j < 4; j++){
       I2Ctwo.write(Channel_Array[i+j]);
       I2Ctwo.endTransmission();
       I2Ctwo.requestFrom(ADC_3,8);
      if(I2Ctwo.available() == 1){
      ADC_ARRAY[2+i][3-j] = I2Ctwo.read();
    }
  }
  }
}


void ADC4(){
  I2Ctwo.beginTransmission(ADC_4);
  for (int i = 0; i < 2; i++){
    for(int j = 0; j < 4; j++){
       I2Ctwo.write(Channel_Array[i+j]);
       I2Ctwo.endTransmission();
       I2Ctwo.requestFrom(ADC_4,8);
      if(I2Ctwo.available() == 1){
      ADC_ARRAY[i][3-j] = I2Ctwo.read();
    }
  }
  }
}


void loop() {
   ADC0();
   ADC1();
   ADC2();
   ADC3();
   ADC4();
}
