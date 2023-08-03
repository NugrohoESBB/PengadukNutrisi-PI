# ☢️Pengaduk-Nutrisi☢️
Run Website -> not available

# Schematic🗺️
![Wiring](https://github.com/NugrohoESBB/PengadukNutrisi-PI/blob/main/SchematicSystem.jpg)

# Documentation📒

## LCD 20x4 with I2C Pin Use to WeMos D1 R2

| PIN LCD | Type     | Pin WeMos D1 R2| 
| :-------- | :------- |  :------- |
| `SDA` | `Communication Serial` |`SDA` |
| `SCL` | `Communication Serial` |`SCL`|
| `GND` | `Ground` |`GND`|
| `5V` | `VCC` | `5V`|

## pH Sensor pH-4502C Pin Use to WeMos D1 R2

| PIN pH-4502C | Type     | Pin WeMos D1 R2| 
| :-------- | :------- |  :------- |
| `DATA` | `Communication` |`A0`|
| `GND` | `Ground` |`GND`|
| `5V` | `VCC` | `5V`|

## Water Level Sensor XKC-Y25-V Pin Use to WeMos D1 R2

| PIN XKC-Y25-V | Type     | Pin WeMos D1 R2| 
| :-------- | :------- |  :------- |
| `DATA` | `Communication` |`D7`|
| `GND` | `Ground` |`GND`|
| `5V` | `VCC` | `5V`|

## LED Pin Use to WeMos D1 R2

| PIN LED | Type     | Pin WeMos D1 R2| 
| :-------- | :------- |  :------- |
| `DATA` | `Communication` |`D8`|
| `GND` | `Ground` |`GND`|
| `5V` | `VCC` | `5V`|

## Relay Pin Use to WeMos D1 R2

| PIN RELAY| Type     | Pin WeMos D1 R2| 
| :-------- | :------- |  :------- |
| `IN1` | `Communication` |`D6`|
| `IN2` | `Communication` |`D5`|
| `IN3` | `Communication` |`D4`|
| `IN4` | `Communication` |`D3`|
| `GND` | `Ground` |`GND`|
| `5V` | `VCC` | `5V`|



## Rumus pH Sensor pH-4502C
```c++
// pH sensor
float calibration = 30.00; //21.42 - change this value to calibrate
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
float phValueFinal;

void pHsensor() {
  for(int i=0;i<10;i++) 
 { 
 buf[i]=analogRead(analogInPin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
   }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + calibration;
 phValueFinal = phValue;
 Serial.print("pH: ");
 Serial.println(phValue);
}
```
