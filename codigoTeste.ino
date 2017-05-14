// BIBLIOTECAS NECESSÁRIAS
#include <EmonLib.h>    // Emon Library : cálculos para os sensores.
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// VARIÁVEIS GLOBAIS
EnergyMonitor emon1;    // Uma instância de um monitor de energial da Emon Library.
const int CT_PINcor = A1;   // Pino onde está conectado o sinal do sensor de corrente.
const int CT_PINten = 6;   // Pino onde está conectado o sinal de sensor de tensao.


//  Blocos do programa:
//  definir pinos de aquisicao de dados
//  calibrar a bilioteca EmonLib de acordo com o medidor de corrente e tensao usado.
//  display LCD
//  
//  Duvidas: 
//  Como funciona a biblioteca EmonLib?
//  Qual a taxa de aquisicao de dados? Por que foi usado um dela de 10s no loop?


void setup()
{ 
  delay(10000);
  // Prepara a porta serial para ser utilizada para depuração. 
  Serial.begin(9600);
  Serial.println("\n\nInicio do programa.");

  // Inicializa o monitor de energia.
  emon1.current(CT_PINcor, 111.1);    // Pino de leitura de corrente e Calibração do sensor (deveria ser 111.1 para o SCT-013-100).
  emon1.voltage(CT_PINten, 234.26, 1.7);       // Pino de leitura da tensao e Calibracao do sensor de tensao  e defasagem de fase

 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Medicao de Irms:");
}

void loop()
{
  // Mede a corrente usando a biblioteca EmonLib e calcula potência.
  // Imprime dados na serial para depuração.
  double Irms = emon1.calcIrms(1480);   // Mede a corrente RMS. // o que eh esse 1480?
//  double Vrms = emon1.calcVrms(1480);   // Mede a tensao RMS (???) 
//  double Potencia = Irms * Vrms;   // Calcula a potência aparente

  // Display resultados no LCD
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(Irms/10); // conferir valor do LCD com multimetro

  delay(100); // aguarde 0.1s antes de atualizar o LCD

}
