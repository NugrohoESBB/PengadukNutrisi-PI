# Pengaduk-Nutrisi
Run Website -> not available

# Schematic (not final yet)
![Wiring](https://github.com/multimedia-dan-robotika/Pengaduk-nutrisi/blob/main/Schematic_Pengaduk_Nutrisi_Pupuk_Cair.png)

# Documentation (not final yet)

## LCD 20x4 with I2C Pin Use to Mega

| PIN LCD | Type     | Pin Mega| 
| :-------- | :------- |  :------- |
| `SDA` | `Communication Serial` |`SDA` |
| `SCL` | `Communication Serial` |`SCL`|
| `GND` | `Ground` |`GND`|
| `5V` | `VCC` | `5V`|

## Keypad 4x4 with I2C Pin Use to Mega

| PIN LCD | Type     | Pin Mega| 
| :-------- | :------- |  :------- |
| `SDA` | `Communication Serial` |`SDA` |
| `SCL` | `Communication Serial` |`SCL`|
| `GND` | `Ground` |`GND`|
| `5V` | `VCC` | `5V`|



## Rumus Water Flow Sensor
```c++
void speedrpm() {
  TURBINE++;
}

void getValueFS() {
  TURBINE = 00;
  sei();
  delay(1000);
  cli();
  Calc = (TURBINE * 60 /7.5);
}
```
