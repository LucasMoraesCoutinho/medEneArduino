// BIBLIOTECAS NECESSÁRIAS
#include <EmonLib.h>    // Emon Library : cálculos para os sensores.
#include <SPI.h>        // SPI Library : necessária para o Shield Ethernet.
#include <Ethernet.h>   // Ethernet Library : para o comunicação com a nuvem.

// VARIÁVEIS GLOBAIS
EnergyMonitor emon1;    // Uma instância de um monitor de energial da Emon Library.
const int CT_PIN = 1;   // Pino onde está conectado o sinal do sensor de corrente.

//byte ip[]   = {192,168,  0,101};                      // ip address (if dhcp not used)
//byte gw[]   = {192,168,  0,  1};                      // gateway address
//byte ns[]   = {  8,  8,  8,  8};                      // name server address
//byte sub[]  = {255,255,255,  0};                      // network mask
//byte mac[]  = { 0x90, 0xA2, 0xDA, 0x0E, 0x04, 0x21 }; // Endereço MAC do Shield Ethernet (olhe atrás dele).
//EthernetClient client;    // Uma instância de um cliente TCP (usaremos eles para enviar os dados).


void setup()
{ 
  delay(10000);
  // Prepara a porta serial para ser utilizada para depuração. 
  Serial.begin(9600);
  Serial.println("\n\nInicio do programa.");

  // Inicializa o monitor de energia.
  emon1.current(CT_PIN, 64);    // Calibração do sensor (deveria ser 111.1 para o SCT-013-100).

  // Inicializa o Shield Ethernet utilizando DHCP.
  Serial.print("Inicializando Ethernet com DHCP...");
  if (Ethernet.begin(mac) == 0) { 
    Serial.println("falhou.");
    Serial.print("Inicializando com IP fixo...");
    Ethernet.begin(mac,ip,ns,gw,sub);
    Serial.println("feito!");
    //Serial.println("Aplicação interrompida.");
    //while(1);   // Fica em loop indefinidamente (aplicação interrompida).
  } else {
    Serial.println("feito!");
  }
  delay(1000);
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP()); // Imprime o endereço IP conseguido.

  Serial.println("Iniciando medições: \n");
}

// Envia dados para servidor.
void sendData(double Irms, double Potencia) {
  
  client.connect("192.168.0.100",4000); // Conecta no servidor na porta 80 (vamos usar http).
  delay(500);
  
  if (client.connected()) {
    client.print("GET /medida?");
    client.print("irms=");
    client.print(Irms);
    client.print("&potencia=");
    client.print(Potencia);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.0.100");
    client.println();
    delay(500);


    while (client.available()) {
      char c = client.read();
    }

    delay(100);
    client.stop();
    Serial.println("Dados enviados!");
  } else {
    Serial.println("Falha na conexão.");
  }
}


void loop()
{
  // Mede a corrente usando a biblioteca EmonLib e calcula potência.
  // Imprime dados na serial para depuração.
  double Irms = emon1.calcIrms(1480);  // Mede a corrente RMS.
  double Potencia = Irms * 127.0;   // Calcula a potência aparente (supondo que a rede elétrica esteja em 127 V).
  Serial.print("Irms: ");
  Serial.print(Irms);   // Imprime a corrente na serial.
  Serial.print(" A \t");
  Serial.print("Potencia: ");
  Serial.print(Potencia); // Imprime a potência na serial.
  Serial.print(" W");
  Serial.println();

  // Envia dados para o servidor.
  sendData(Irms, Potencia);

  // Aguarde 10 segundos e siga em frente.
  delay(2000);

}
