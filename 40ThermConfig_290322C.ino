// Requires Wire, TimeLib libraries (possibly math)

#include <Wire.h>
//#include <math.h> // if needed for log function
//#include <TimeLib.h>
//#include <WiFi.h>

// Define the SDA and SCL pins for the I2C buses
#define I2C_SDA_0 23  //change later if incorrect pins
#define I2C_SCL_0 22

#define I2C_SDA_1 21  //change later if incorrect pins
#define I2C_SCL_1 19

// These ADCs use the I2C0 BUS  (I2Cone)
#define ADC_0 0x48
#define ADC_1 0x4A
#define ADC_2 0x49
// These ADCs use the I2C1 BUS  (I2Ctwo)
#define ADC_3 0x4B
#define ADC_4 0x48

// Create arrays to store the Addresses and Command bytes to write to the NCD9830
int ADC_ADDR[] = {ADC_0, ADC_1, ADC_2, ADC_3, ADC_4};
int ADC_ADDR_R[] = {0b1001001, 0x1001101, 0x1001011, 0x1001111, 0x1001001};
int ADC_COMM[] = {0x8C, 0xCC, 0x9C, 0xDC, 0xAC, 0xEC, 0xBC, 0xFC};

// Arrays to store ADC Readings and Temperature values
int ADC_ARRAY[10][4] = {0};
float TMP_ARRAY[10][4] = {0};
float TMP2_ARRAY[10][4] = {0};

// Reference Voltage, Source Voltage, Max Value of ADC Reading (2^8 - 1), Resistor value in circuit
const float VREF = 2.5, VCC = 3.3, ADCMAX = 255, RST0 = 1000.0;
const float inv_TMP0 = 1/298.15, inv_BETA = 1/4500.0;

// Wifi, replace w/ ssid pw at site
//char ssid[] = "Batch.space";
//char password[] = "BatchspaceU5!";
//char server[] = "pool.ntp.org";
//const int gmtOffset = 0;
//const int dstOffset = 0; //3600 for DST;

TwoWire I2Cone = TwoWire(0); //I2C0 bus
TwoWire I2Ctwo = TwoWire(1); //I2C1 bus

void setup() {
 I2Cone.begin(I2C_SDA_0, I2C_SCL_0);
 I2Ctwo.begin(I2C_SDA_1, I2C_SCL_1);
 Serial.begin(9600);
// WiFi.begin(ssid,password);
// while (WiFi.status() != WL_CONNECTED) {
//  delay(500);
//  Serial.print(".");
// }
//  Serial.println("");
//  Serial.println("WiFi connected.");
//  configTime(gmtOffset, dstOffset, server);
}

void Read_ADC_Values() {
  // iterate over ADC Addresses 0 1 2
  for (int i = 0; i < 3; i++){
    // iterate over ADC Channels 0 1 2 3 4 5 6 7             
    for (int j = 0; j < 8; j++){
      // Read from each ADC Channel and store value in the array
      I2Cone.beginTransmission(ADC_ADDR[i]);
      I2Cone.write(ADC_COMM[j]);
      I2Cone.endTransmission();
      //I2Cone.beginTransmission(ADC_ADDR[i]);
      I2Cone.requestFrom(ADC_ADDR[i], 1);
      if (I2Cone.available() <= 1){
        if (j < 4){
          ADC_ARRAY[(8 - (2 * i))][3 - j] = I2Cone.read();
        }
        else{
          ADC_ARRAY[(9 - (2 * i))][7 - j] = I2Cone.read();
        }
      }
    }
  } 
  // iterate over ADC Addresses 3 4
  for (int i = 3; i < 5; i++){
    for (int j = 0; j < 8; j++){
      I2Ctwo.beginTransmission(ADC_ADDR[i]);
      I2Ctwo.write(ADC_COMM[j]);
      I2Ctwo.endTransmission();
      //I2Ctwo.beginTransmission(ADC_ADDR[i]);
      I2Ctwo.requestFrom(ADC_ADDR[i], 1);
      if (I2Ctwo.available() <= 1){
        if (j < 4){
          ADC_ARRAY[(8 - (2 * i))][3 - j] = I2Ctwo.read();
        }
        else{
          ADC_ARRAY[(9 - (2 * i))][7 - j] = I2Ctwo.read();
        }
      }
    }
  }
}

void Convert_ADC_Values() {
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 4; j++){
      
      // Rearrange ADCMAX/ADCVAL = VREF / VO and
      // voltage divider equation VO = VCC * R0/(Rt+R0) to find Rt
      float Rt = RST0 * ( (VCC * ADCMAX) / (VREF * (float)ADC_ARRAY[i][j]) - 1.0);

      // Use equation 1/T = 1/T0 +(1/B)*ln(R/R0) , convert Temperature value from Kelvin to Celsius
      float logR_R0 = log(Rt / RST0);
      TMP_ARRAY[i][j] = 1.0 / ( inv_TMP0 + (inv_BETA * logR_R0) );
      TMP_ARRAY[i][j] -= 273.15;
    }
  }
}

void Display_TMP_Values() {
  // First displays "Battery Temperature Recording (°C)  -  yy-mm-dd  hh:mm:ss"  // FIX THIS SOMEHOW

//  struct tm timeinfo;
//  getLocalTime(&timeinfo);
  Serial.print("Battery Temperature Recording (°C)  -  ");

//  Serial.print(&timeinfo, "%F");
//  Serial.print("  ");
//  Serial.print(&timeinfo, "%T");
//  Serial.print(" UTC");
  Serial.print("\n\n");

  // Then displays the temperature values
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 4; j++){
      Serial.print(ADC_ARRAY[i][j], 2);
      //Serial.print(TMP2_ARRAY[i][j], 2);
      Serial.print("\t");
      TMP2_ARRAY[i][j] += 1;
    }
    Serial.print("\n");
  }
  Serial.print("\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Read_ADC_Values();
  //Convert_ADC_Values();
  Display_TMP_Values();
  delay(2000);
}
