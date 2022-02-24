#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 9
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

RTC_DS1307 rtc; //OBJETO DO TIPO RTC_DS1307(Relogio)

float tempMin = 999;
float tempMax = 0;

DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
 
// Inicializa o LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  Serial.println();
  Serial.println();
  
  lcd.begin(16, 2);
   if (! rtc.begin()) { 
    Serial.println("DS1307 não encontrado");
    while(1);
  }
  if (! rtc.isrunning()) { 
    Serial.println("DS1307 rodando!"); 
    //Para atualizar o relogio, descomentar a linha abaixo
    //rtc.adjust(DateTime(2018, 7, 5, 15, 33, 15)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
   
}
  
void loop()
{
  DateTime now = rtc.now(); //relogio
  // Le a informacao do sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);
  // Atualiza temperaturas minima e maxima
  if (tempC < tempMin)
  {
    tempMin = tempC;
  }
  if (tempC > tempMax)
  {
    tempMax = tempC;
  }
  // Mostra dados no serial monitor
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Min : ");
  Serial.print(tempMin);
  Serial.print(" Max : ");
  Serial.println(tempMax);
   
  // Mostra dados no LCD  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp.:       ");
  //Simbolo grau
  lcd.write(223);
  lcd.print("C");
  lcd.setCursor(7,0);
  lcd.print(tempC);
  lcd.setCursor(0,1);
 
  lcd.setCursor(0, 1);
  lcd.print("Hora.:");
  lcd.setCursor(7, 1);
  lcd.print(now.hour());
  lcd.setCursor(9, 1);
  lcd.print(":");
  lcd.setCursor(10, 1);
  lcd.print(now.minute());
  
  delay(1000);
}

void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
