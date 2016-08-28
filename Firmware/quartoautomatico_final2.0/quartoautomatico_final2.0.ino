//-------------------------------------------------------------------------------------------------------------------------
//EM BUSCA DO QUARTO AUTOMATICO 2.9.5
//-------------------------------------------------------------------------------------------------------------------------
//----------IPFIXO=quartoautomatico.no-ip.biz-----------
/*ConfigMode
 "*" -> CONFIG
 "#" -> SUBCONFIGS++

 "A"->acesso
 "B"->automatic
 "C"->servidor
 "D"->debug

 CONFIGMODE   0-desativado
              1-get root pass
              2-espera de comado
              3-servidor
              4-debug
              5-acesso1 - adriana
              6-acesso2 - rodrigo
              7-vars1 - autoLock
              8-vars2 - autoLamp
              9-vars3 - autoPC
             10-acesso sueli
             11-vars4 - tv
             12-vars5 - filtro
             13 --- sleep
             14-visualizar 1
	     15-visualizar 2
	     16-visualizar 3
	     17-date

 ***PRE CONFIGS
 *"* =SaiDasConfigs"|
 *"ROOT IS REQUIRED"|
 *"# =NavegaSubConf"|
 *"ROOT IS REQUIRED"|

 ***CONFIGS
 *"A=acesso|B=vars "|
 *"C=server|D=debug"|
 ------------------------TELA nova
 *"A=acesso|B=vars "|
 *"C=Control|D=Dbug"|
 -----------------------

 ***acesso
 *" Adriana = true "| // o acesso + estado
 *"1=true | 2=false"|

 ***automatic
 *"AutoLock = true "| // o q esta mudando + estado
 *"1=true | 2=false"|

 ***servidor
 *"192.168.127.453 "| // servidor
 *"Porta: 8080     "|

 ***debug
 *"DebugMode= true "| // o q esta mudando + estado
 *"1=true | 2=false"|
  ConfigMode*/

/*Telas
 *"1234567890123456"|

 ***TELA INICIAL/descanso
 *"  Em busca do   "|
 *"QuartoAutomatico"|

 ***TELA 01 //padrao
 *"D 07/01/15 23:33"|
 *" Digite a Senha "|  //senha
 *"D 07/01/15 T 35°"|
 *" Digite a Senha "|  //senha

 ***TELA SenhaOK
 *"ACESSO GARANTIDO"|
 *" Seja Bem-Vindo "|

 ***TELA SenhaErrada
 *" ACESSO NEGADO  "|
 *"TenteNovamente 0"|//numero de erros se tiver mais que 5 so libera o root

 ***TELA SENHA 5erros
 *"Senha Incorreta "|
 *"5 Vezes Seguidas"|

 ***TELA PROTEÇÃO
 *" MODO SEGURANÇA "|
 *"ROOT IS REQUIRED"|

  ***TELA MAXIMO DE ENTRADAS
 *" VOCE ATINGIU O "|
 *" LIMITE DIARIO  "|

  ***ACESSO REVOGADO
 *"  VOCE NAO TEM  "|
 *"    PERMISSAO   "|

   ***exibir variaveis
 *"Adriana Count:##"|
 *"Rodrigo Count:##"|

 *"Senhas erradas:#"|
 *"NºEmergencias:##"|

 *"Luminosidad:###%"|
 *"----------------"|
  Telas*/

/*PROTOCOLO
 ***Entrada no arduino
 *senha
 *"!"
 *"campo a ser alterado"
 * "!"
 *"valor a ser definido"
 *"??"||"\n\n"  (fim da transmissao)


 ***Resposta do arduino
 **for filds
   *"fild"
   *"!"
   *"value"
   *"\n"
   *"fild"
   *"!"
   *"value"
 *"\n\n"  (fim da transmissao)


 ******************************
 IDS DE PACOTE (android/arduino)
 ******************************
 *00-Trancar porta/Estado da tranca
 *01-Desligar Lampada/Estado da lampada
 *02-Pulsar Computador/Estado do computador
 *03-Desligar TV/Estado TV
 *04-Acesso Adriana/Estado do acesso
 *05-Acesso rodrigo/estado acesso
 *06-enviar luminosidade para ligar/luminosidade
 *07-atualizar/estado da porta
 *08-atualizar/estado da janela
 *09-atualizar/estado de emergencia
 *10-atualizar/entradas adriana
 *11-atualizar/entradas rodrigo
 *12-atualizar/entradas thiago
 *13-ligar luz automaticament/estado da luz automatica
 *14-horario de ligar a luz/horario de ligar a luz
 *15-ligar pc ao entrar/estado do autopc
 *16-horarios de ligar o pc/horarios de ligar o pc
 *17-auto trancar porta/auto trancar porta
 *18-ligar filtro estado/filtro
 *19-atualizar/temperatura
 *20-Acesso sueli/estado acesso
 *21-horario de acordar luz/horario de acordar luz
 *22-entrada de adm/null
  PROTOCOLO*/

/*KEYPAD
 * idle=1023
 * 1=8,7,6
 * 2=251,252
 * 3=401,402
 * A=503,504
 * 4=84,83
 * 5=297,296
 * 6=431,432
 * b=525,524
 * 7=149,150
 * 8=337
 * 9=458
 * c=543,544
 * *=208
 * 0=374
 * #=483
 * d=561
  KEYPAD*/
//LIBS---------------------------------------------------------------------------------------------------------------------
//ethernet
#include <SPI.h>
#include <UIPEthernet.h>
//rtc
#include <RTClib.h>
#include <Wire.h>
//sensor de temperatura
#include <DallasTemperature.h>
#include <OneWire.h>
//thread
#include <Thread.h>
//display
#include <LiquidCrystal.h>
//whatdogtimer
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
//-------------------------------------------------------------------------------------------------------------------------
//VARIAVEIS DE SISTEMA-----------------------------------------------------------------------------------------------------
//timer
unsigned long timer;
const int timerSize = 8;
unsigned short seconds[timerSize];
boolean trigger[timerSize];

//rtc
unsigned short ano;
unsigned short mes;
unsigned short dia;
unsigned short hora;
unsigned short minuto;
unsigned short segundo;
unsigned short dow;
float temperature;
boolean resetedRTC = false;

//senha
unsigned short adrianaCount = 0;
unsigned short rodrigoCount = 0;
unsigned short rootCount = 0;
String currentPassword = "";
unsigned short wrongPass = 0;
//debouce
const int debouceSize = 8;
unsigned short microSeconds[debouceSize];
boolean debouceTrigger[debouceSize];

//musica
short int musica = 2;
char music_names[] = {
  'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'
};
int music_tones[] = {
  1915, 1700, 1519, 1432, 1275, 1136, 1014, 956
};

const int maiormusica = 32; //tamanho da maior musica
int music_length = 18;
char music_notes[maiormusica + 1];
int music_beats[maiormusica + 1];
int music_tempo;
//musica 1
const int music1_length = 18; // número de notas
char music1_notes[music1_length + 1] = {"gggebgdddebgebgggg"}; // espaços representam pausas
int music1_beats[music1_length + 1] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,};
int music1_tempo = 300;
//musica 2
const int music2_length = 32; // número de notas
char music2_notes[music2_length + 1] = {"ccdcgfccCagaafgfccdcgfccCagaafgf"}; // espaços representam pausas
int music2_beats[music2_length + 1] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,};
int music2_tempo = 5000;
//---------------------
boolean music_inBigestLoop = false;
short int music_bigestValue = -1;
boolean music_inLoop = false;
long music_loopValue = -1;
int music_tone;
int music_duration;
boolean music_inDelay = false;
boolean music_delaySucess = false;
boolean music_needSucess = false;
unsigned long music_timer;
float music_microdelay;

//luminosidade
unsigned short lightTries = 0;
unsigned short lightPercentage;
const unsigned short lightMax = 45;

//permissao
boolean acessAdriana = true;
boolean acessRodrigo = true;
boolean acessSueli = false;

//auxiliar
boolean btnTV;
boolean locking = false;
boolean unlocking = false;
boolean lampSleeping = false;
unsigned short durationLamp;
boolean refreshDigitalOutputs = true;
unsigned short debouceKeyId = 65535;
unsigned short configMode = 0;
unsigned short screen = 0;
boolean beginLCD = false;
boolean reprintLCD = false;
boolean alarmBuzState = false;
boolean toInvertLamp = false;

//emergencia
boolean emergencyUnlock = false;
boolean emergencyMode = false;
boolean emergencyTV;
boolean emergencyFiltro;
boolean emergencyLamp;
unsigned int emergencyCount = 0;

//estados
boolean locked = false;
boolean statePC = false;
boolean emergency = false;
boolean stateWindow = false;
boolean stateDoor = false;
boolean stateLamp = false;
boolean blockTV = false;
boolean stateFiltro = true;
boolean sleeping = false;

//changeDateTime
short int changeState = 0; //0=dia1 1=dia2 2=mes1 3=mes2 4=ano1 5=ano2 6=hora1 7=hora2 8=minuto1 9=minuto2
boolean changeDay = false;
boolean changeMonth = false;
boolean changeYear = false;
boolean changeHour = false;
boolean changeMinute = false;
short int newDay1 = 4;
short int newDay2 = 10;
short int newMonth1 = 2;
short int newMonth2 = 10;
short int newYear1 = 10;
short int newYear2 = 10;
short int newHour1 = 3;
short int newHour2 = 10;
short int newMinute1 = 7;
short int newMinute2 = 10;
//-------------------------------------------------------------------------------------------------------------------------
//VARIAVEIS DE USUARIO/CONFIG----------------------------------------------------------------------------------------------
//config
boolean debug = true;
boolean TCPserver = true;
boolean SerialServer = true;
boolean passMask = true;
//automatic
boolean autoLock = true;
boolean autoLamp = false;
boolean autoPC = false;
//lampada
unsigned short percentageMinToLamp = 30;
unsigned short hourLamp = 23;
unsigned short minuteLamp = 59;
unsigned short hourToAwakeLamp = 7;
//PC
unsigned short hourPC1 = 11;
unsigned short minutePC1 = 40;
unsigned short hourPC2 = 13;
unsigned short minutePC2 = 20;
//senhas
const String passwordRoot = "129854";
const String sPasswordRoot = "781227";
const String passwordAdriana = "007";
const String passwordRodrigo = "72";
const String passwordSueli = "159";
const unsigned short maxWrongPass = 5;
const unsigned short rodrigoMaxCount = 10;
const unsigned short adrianaMaxCount = 10;
//timer
unsigned short timeTo[timerSize] = { //SEGUNDOS
  1,//lock
  2,//erase pass
  10,//sleep
  3,//backScreen
  10,//outconfigs
  1200,//testar luz
  2,//changescreen
  15//debug
  //2//ETHERNET
};
const unsigned short debouceDelay[debouceSize] = { //MILISEGUNDOS * 100
  5,//unlock
  1,//btnlock
  5,//sensor da porta
  10,//pulse PC
  3,//beep
  3,//keypad
  10,//ligar pc, relay delay
  3//lampada relay delay
};
//-------------------------------------------------------------------------------------------------------------------------

//Ethernet vars------------------------------------------------------------------------------------------------------------
int ip[] = {192, 168, 25, 6};
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
IPAddress myIP(ip[0], ip[1], ip[2], ip[3]);
int port = 1000;
EthernetServer server = EthernetServer(port);
//-------------------------------------------------------------------------------------------------------------------------

//Temperature vars---------------------------------------------------------------------------------------------------------
unsigned char DS18B20_PIN = 23;
OneWire ds(DS18B20_PIN);
DallasTemperature rtcTemperature(&ds);
//-------------------------------------------------------------------------------------------------------------------------

//RTC vars-----------------------------------------------------------------------------------------------------------------
RTC_DS1307 RTC;
//-------------------------------------------------------------------------------------------------------------------------

//LCD vars-----------------------------------------------------------------------------------------------------------------
LiquidCrystal lcd(37, 39, 41, 43, 45, 47);
byte graus[8] = { B00000,
                  B01110,
                  B01010,
                  B01110,
                  B00000,
                  B00000,
                  B00000,
                  B00000
                };
//-------------------------------------------------------------------------------------------------------------------------

//Threads------------------------------------------------------------------------------------------------------------------
Thread PlayMusic;
//-------------------------------------------------------------------------------------------------------------------------

//Keypad macros------------------------------------------------------------------------------------------------------------
#define BTN_IDLE(a) (a>=1010 && a<1024)//idle

#define BTN_A(a) (a>4 && a<10)//1
#define BTN_B(a) (a>246 && a<256)//2
#define BTN_C(a) (a>396 && a<406)//3
#define BTN_D(a) (a>498 && a<508)//A

#define BTN_E(a) (a>78 && a<88)//4
#define BTN_F(a) (a>294 && a<304)//5
#define BTN_G(a) (a>426 && a<436)//6
#define BTN_H(a) (a>520 && a<530)//B

#define BTN_I(a) (a>145 && a<155)//7
#define BTN_J(a) (a>332 && a<342)//8
#define BTN_K(a) (a>455 && a<465)//9
#define BTN_L(a) (a>540 && a<550)//C

#define BTN_M(a) (a>204 && a<214)//*
#define BTN_N(a) (a>368 && a<378)//0
#define BTN_O(a) (a>478 && a<488)//#
#define BTN_P(a) (a>556 && a<566) //D
//-------------------------------------------------------------------------------------------------------------------------

//Pinout-------------------------------------------------------------------------------------------------------------------
unsigned char TECLADO = A0;
unsigned char LUMINOSIDADE = A1;
unsigned char ABRIR = 24;
unsigned char FECHAR = 22;
unsigned char EMERGENCIA = 26;
unsigned char ESTADOPORTA = 28;//28
unsigned char ESTADOJANELA = 30;//30
unsigned char LEDLOCK = 8;
unsigned char BTNLOCK = 34;
unsigned char BTNTV = 36;
unsigned char LIGARTV = 38;
unsigned char LIGARLAMP = 40;
unsigned char LIGARFILTRO = 42;
unsigned char LIGARPC = 44;
unsigned char ESTADOPC = 46;
unsigned char ESTADOLAMP = 48;
unsigned char BACKLIGTH = 49;
unsigned char BUZZER = 35;
/*NAO LISTADOS
  Sensor De Temperatura = 23
  LCD RS = 37
  LCD E = 39
  LCD D4 = 41
  LCD D5 = 43
  LCD D6 = 45
  LCD D7 = 47
  NAO LISTADOS*/
//-------------------------------------------------------------------------------------------------------------------------

//FUNCAO SETUP-------------------------------------------------------------------------------------------------------------
void setup() {
  cli();//disativa as interupcoes
  wdt_reset();//reseta o temporizador do WDT
  /*settings
   * WDIE = 1 interupt enable
   * WDE = 1 reset enable
   * WDP3=1 prescaler bits to 4 sec timeout
   * WDP2=0 prescaler bits to 4 sec timeout
   * WDP1=0 prescaler bits to 4 sec timeout
   * WDP0=0 prescaler bits to 4 sec timeout
   */
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDE) | (1 << WDP3) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);
  sei();//ativa as interupcoes
  //Cria caractere º
  lcd.createChar(0, graus);
  //configura o thread
  PlayMusic.onRun(TurnOnMusic);
  //inicia SERIAL/I2C/RTC/LCD/ETHERNET/SERVIDOR/TEMPERATURE
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  lcd.begin(16, 2);
  Ethernet.begin(mac, myIP);
  server.begin();
  rtcTemperature.begin();
  //ajusta a data do modulo rtc caso nao tenha sido iniciada
  if (!RTC.isrunning())
    RTC.adjust(DateTime(__DATE__, __TIME__));
  if (debug) {
    Serial.print("[Compiled at]:");
    Serial.print(__DATE__);
    Serial.print("  ");
    Serial.println(__TIME__);
  }
  //configura os pinos do arduino
  pinMode(13, OUTPUT);
  pinMode(ABRIR, OUTPUT);
  digitalWrite(ABRIR, LOW);
  pinMode(FECHAR, OUTPUT);
  digitalWrite(FECHAR, LOW);
  pinMode(EMERGENCIA, INPUT);
  pinMode(ESTADOPORTA, INPUT);
  pinMode(ESTADOJANELA, INPUT);
  pinMode(BTNTV, INPUT);
  digitalWrite(BTNTV, HIGH); //PULL-UP RESISTOR
  pinMode(BTNLOCK, INPUT);
  digitalWrite(BTNLOCK, HIGH); //PULL-UP RESISTOR
  pinMode(LIGARLAMP, OUTPUT);
  digitalWrite(LIGARLAMP, LOW);
  pinMode(LIGARFILTRO, OUTPUT);
  digitalWrite(LIGARFILTRO, LOW);
  pinMode(LIGARPC, OUTPUT);
  digitalWrite(LIGARPC, LOW);
  pinMode(ESTADOPC, INPUT);
  pinMode(LIGARTV, OUTPUT);
  digitalWrite(LIGARTV, LOW);
  pinMode(ESTADOLAMP, INPUT);
  pinMode(BACKLIGTH, OUTPUT);
  digitalWrite(BACKLIGTH, LOW);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  //ajusta a chave de controle manual da tv
  if (digitalRead(BTNTV) == HIGH)
    btnTV = true;
  else
    btnTV = false;
  //ajusta o sensor da porta
  if (digitalRead(ESTADOPORTA) == HIGH)
    stateDoor = true;
  else
    stateDoor = false;
  //zera timers
  for (int i = 0; i < timerSize; i++) {
    seconds[i] = 0;
    trigger[i] = false;
  }
  for (int i = 0; i < debouceSize; i++) {
    microSeconds[i] = 0;
    debouceTrigger[i] = false;
  }
  trigger[6] = true;
  if (debug) trigger[7] = true;

  for (int i = 0; i < timerSize; i++) //ajusta os segundos para obter mais precisao
    timeTo[i] = timeTo[i] * 10;
  //destranca a porta
  unlock();

  timer = millis();

  printLCD();
  rtcTemperature.requestTemperatures();
  if (debug) Serial.println("[Debug]: Funtion setup runned");
}
//FUNCAO LOOP--------------------------------------------------------------------------------------------------------------
void loop() {
  wdt_reset();//reseta o temporizador do WDT
  if (emergency && !emergencyMode) redCode(true); //trata emergencia
  else if (emergency && emergencyMode) PlayMusic.run(); //toca o alarme
  if (TCPserver) checkTCPServer();
  if (SerialServer) checkSerialServer();
  if (refreshDigitalOutputs && !emergency) writePorts();
  if (reprintLCD) printLCD();
  if (sleeping && !statePC) {
    stateFiltro = false;
    refreshDigitalOutputs = true;
  }
  if (music_inBigestLoop) bigestForAlarm();
  if (music_inDelay) music_count();
  if (toInvertLamp) invertLamp();
  checkKeypad();
  checkPass();
  checkDigitalInputs();
  refreshRTC();
  LDR();

  count();
}
//-------------------------------------------------------------------------------------------------------------------------

//TIMER--------------------------------------------------------------------------------------------------------------------
void count() {
  //conta
  if (timer <= millis() - 100) {
    timer = millis();
    if (digitalRead(13) == LOW)
      digitalWrite(13, HIGH);
    else
      digitalWrite(13, LOW);
    for (int i = 0; i < debouceSize; i++)
      if (debouceTrigger[i])
        microSeconds[i]++;
      else
        microSeconds[i] = 0;

    for (int i = 0; i < timerSize; i++) ////EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERRRO SEGUNDOS DIFERENTE DE MILISEGUNDOS * 100
      if (trigger[i])
        seconds[i]++;
      else
        seconds[i] = 0;
    //checa
    for (int i = 0; i < debouceSize; i++)
      if (microSeconds[i] >= debouceDelay[i]) {
        switch (i) {
          default: return;

          case 0:
            digitalWrite(ABRIR, LOW);
            unlock();
            break;

          case 1:
            if (digitalRead(BTNLOCK) == LOW) {
              if (debug) Serial.println("[Debug]: Btn_lock pressed...");
              invertLock();
            }
            break;

          case 2:
            if (digitalRead(ESTADOPORTA) == LOW && locked) {
              emergency = true;
              emergencyUnlock = false;
              if (debug) Serial.println("[Debug]: Breaking-in");
            }
            break;

          case 3:
            digitalWrite(LIGARPC, LOW);
            break;

          case 4:
            digitalWrite(BUZZER, LOW);
            break;

          case 5:
            debouceKeyId = 65535;
            break;

          case 6:
            pulsePC();
            break;

          case 7:
            if (lightPercentage <= percentageMinToLamp) {
              if (!stateLamp && !lampSleeping) {
                if (lightTries >= 15) break;
                lightTries++;
                toInvertLamp = true;
              }
            }
            break;
        }
        debouceTrigger[i] = false;
        microSeconds[i] = 0;
      }

    for (int i = 0; i < timerSize; i++)
      if (seconds[i] >= timeTo[i]) {
        switch (i) {
          default: return;
          case 0:
            if (locking) {
              digitalWrite(FECHAR, LOW);
              locking = false;
            }
            else if (unlocking) {
              digitalWrite(ABRIR, LOW);
              unlocking = false;
            }
            break;

          case 1:
            if (currentPassword.length() >= 6) {
              wrongPass++;
              changeScreen(3);
            }
            changeScreen(0);
            currentPassword = "";
            break;

          case 2:
            changeScreen(0);
            if (locked)
              analogWrite(LEDLOCK, 15);
            digitalWrite(BACKLIGTH, LOW);
            break;

          case 3:
            changeScreen(0);
            break;

          case 4:
            changeScreen(0);
            configMode = 0;
            break;

          case 5:
            if (stateLamp)
              toInvertLamp = true;
            break;

          case 6:
            if (!trigger[1] && (screen == 0 || screen == 1 || screen == 2)) {
              unsigned short temp = screen;
              temp++;
              if (temp > 2)
                temp = 0;
              changeScreen(temp);
            } else if (screen == 9 || screen == 10) {
              if (screen == 9)
                changeScreen(10);
              else
                changeScreen(9);
            }
            break;

          case 7:
            showDebug();
            break;
        }
        seconds[i] = 0;
        if (i != 6 && i != 7)
          trigger[i] = false;
      }
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//Tranca-------------------------------------------------------------------------------------------------------------------
void unlock() {
  if (!locking) {
    if (debug) Serial.println("[Debug]: Unlocking...");
    digitalWrite(ABRIR, HIGH);
    locked = false;
    analogWrite(LEDLOCK, 0);
    if (unlocking)
      trigger[0] = true;
    else
      debouceTrigger[0] = true;
    unlocking = true;
  }
}
void lock() {
  if (!unlocking && stateDoor) {
    if (debug) Serial.println("[Debug]: Locking...");
    digitalWrite(FECHAR, HIGH);
    locked = true;
    analogWrite(LEDLOCK, 15);
    locking = true;
    trigger[0] = true;
  }
}
void invertLock() {
  if (locked)
    unlock();
  else
    lock();
}
void invertLamp() {
  toInvertLamp = false;
  if (digitalRead(LIGARLAMP) == LOW)
    digitalWrite(LIGARLAMP, HIGH);
  else
    digitalWrite(LIGARLAMP, LOW);
}
//-------------------------------------------------------------------------------------------------------------------------

//DigitalInputs------------------------------------------------------------------------------------------------------------
void checkDigitalInputs() {
  //checa se há alimentacao
  if (digitalRead(EMERGENCIA) == HIGH)
    emergency = false;
  else {
    emergency = true;
    emergencyUnlock = true;
  }
  //ve se o computador esta ligado
  if (digitalRead(ESTADOPC) == HIGH)
    statePC = true;
  else
    statePC = false;
  //ve se a janela esta fechada
  if (digitalRead(ESTADOJANELA) == HIGH)
    stateWindow = false;
  else
    stateWindow = true;
  //ve se a chave da TV teve mudanca de estado
  if (digitalRead(BTNTV) == HIGH) {
    if (!btnTV) {
      if (blockTV)
        blockTV = false;
      else
        blockTV = true;
      refreshDigitalOutputs = true;
    }
    btnTV = true;
  } else {
    if (btnTV) {
      if (blockTV)
        blockTV = false;
      else
        blockTV = true;
      refreshDigitalOutputs = true;
    }
    btnTV = false;
  }
  //ve se o botao do painel foi pressionado
  if (digitalRead(BTNLOCK) == LOW)
    debouceTrigger[1] = true;
  //verifica o estado da porta
  if (digitalRead(ESTADOPORTA) == HIGH) {
    if (stateDoor == false) {
      stateDoor = true;
      if (!locked && autoLock && !emergency)
        lock();
    } else
      stateDoor = true;
  } else {
    stateDoor = false;
    if (locked) {
      debouceTrigger[2] = true;
    }
  }
  //verifica se a lampada esta acessa
  durationLamp = pulseIn(ESTADOLAMP, LOW, 21000);
  if (durationLamp > 8000 && durationLamp < 15000) {
    stateLamp = true;
  }
  else {
    stateLamp = false;
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//Computador--------------------------------------------------------------------------------------------------------------
void pulsePC() {
  if (!stateFiltro) {
    digitalWrite(LIGARFILTRO, LOW);
    stateFiltro = true;
    refreshDigitalOutputs = true;
    if (sleeping) {
      lampSleeping = false;
    }
    sleeping = false;
  }
  // if(!statePC){
  digitalWrite(LIGARPC, HIGH);
  debouceTrigger[3] = true;
  if (debug) Serial.println("[Debug]: Pulse PC...");
  // }
}

void autoTurnOnPC() {
  if ((hora > hourPC1 || (hora == hourPC1 && minuto >= minutePC1)) && (hora < hourPC2 || (hora == hourPC2 && minuto <= minutePC2) && !statePC))
    debouceTrigger[6] = true;
}
//-------------------------------------------------------------------------------------------------------------------------

//BEEP buzzer--------------------------------------------------------------------------------------------------------------
void beep() {
  digitalWrite(BUZZER, HIGH);
  debouceTrigger[4] = true;
  if (debug) Serial.println("[Debug]: Beep...");
}
//-------------------------------------------------------------------------------------------------------------------------

//Refresh RTC--------------------------------------------------------------------------------------------------------------
void refreshRTC() {
  DateTime now = RTC.now();
  ano = now.year();
  mes = now.month();
  dia = now.day();
  hora = now.hour();
  minuto = now.minute();
  segundo = now.second();
  dow = now.dayOfWeek();

  if ((dow == 2||dow == 4) && hora > 6 && hora < 18 && !acessSueli)
    acessSueli = true;
  else if ((dow == 2||dow == 4) && hora >= 18 && acessSueli)
    acessSueli = false;

  temperature = rtcTemperature.getTempCByIndex(0);
  if (hora == 0 && minuto == 0 && !resetedRTC) {
    rodrigoCount = 0;
    adrianaCount = 0;
    rootCount = 0;
    acessSueli = false;
    acessAdriana = true;
    acessRodrigo = true;
    resetedRTC = true;
  }
  else if (hora == 0 && minuto > 0)
    resetedRTC = false;
}
//-------------------------------------------------------------------------------------------------------------------------

//LDR----------------------------------------------------------------------------------------------------------------------
void LDR() {
  lightPercentage = map(analogRead(LUMINOSIDADE), 0, lightMax, 0, 100);
  if (!autoLamp) {
    lampSleeping = false;
    return;
  }
  if (lampSleeping) {
    if (hora >= hourToAwakeLamp && rootCount > 0) {
      lampSleeping = false;
    }
  } else {
    if (stateLamp)
      trigger[5] = true;
    if ((hora > hourLamp || (hora == hourLamp && minuto >= minuteLamp)) || hora <= hourToAwakeLamp) {
      if (stateLamp)
        toInvertLamp = true;
      lampSleeping = true;
    } else if (lightPercentage <= percentageMinToLamp) {
      debouceTrigger[7] = true;
      /*if(!stateLamp)
        if(digitalRead(LIGARLAMP)==LOW)
          digitalWrite(LIGARLAMP, HIGH);
        else
          digitalWrite(LIGARLAMP, LOW);*/
    }
  }
  if (stateLamp) lightTries = 0;
}
//-------------------------------------------------------------------------------------------------------------------------

//Digital OUT--------------------------------------------------------------------------------------------------------------
void writePorts() {
  refreshDigitalOutputs = false;

  if (blockTV)
    digitalWrite(LIGARTV, HIGH);
  else
    digitalWrite(LIGARTV, LOW);

  if (stateFiltro)
    digitalWrite(LIGARFILTRO, LOW);
  else
    digitalWrite(LIGARFILTRO, HIGH);
}
//-------------------------------------------------------------------------------------------------------------------------

//KEYPAD-------------------------------------------------------------------------------------------------------------------
void checkKeypad() {
  unsigned short previous = debouceKeyId;
  if (BTN_A(analogRead(TECLADO))) { //BOTAO 1
    debouceKeyId = 0;
    debouceTrigger[5] = true;
  }
  else if (BTN_B(analogRead(TECLADO))) { //BOTAO 2
    debouceKeyId = 1;
    debouceTrigger[5] = true;
  }
  else if (BTN_C(analogRead(TECLADO))) { //BOTAO 3
    debouceKeyId = 2;
    debouceTrigger[5] = true;
  }
  else if (BTN_D(analogRead(TECLADO))) { //BOTAO A
    debouceKeyId = 3;
    debouceTrigger[5] = true;
  }
  else if (BTN_E(analogRead(TECLADO))) { //BOTAO 4
    debouceKeyId = 4;
    debouceTrigger[5] = true;
  }
  else if (BTN_F(analogRead(TECLADO))) { //BOTAO 5
    debouceKeyId = 5;
    debouceTrigger[5] = true;
  }
  else if (BTN_G(analogRead(TECLADO))) { //BOTAO 6
    debouceKeyId = 6;
    debouceTrigger[5] = true;
  }
  else if (BTN_H(analogRead(TECLADO))) { //BOTAO B
    debouceKeyId = 7;
    debouceTrigger[5] = true;
  }
  else if (BTN_I(analogRead(TECLADO))) { //BOTAO 7
    debouceKeyId = 8;
    debouceTrigger[5] = true;
  }
  else if (BTN_J(analogRead(TECLADO))) { //BOTAO 8
    debouceKeyId = 9;
    debouceTrigger[5] = true;
  }
  else if (BTN_K(analogRead(TECLADO))) { //BOTAO 9
    debouceKeyId = 10;
    debouceTrigger[5] = true;
  }
  else if (BTN_L(analogRead(TECLADO))) { //BOTAO C
    debouceKeyId = 11;
    debouceTrigger[5] = true;
  }
  else if (BTN_M(analogRead(TECLADO)) && !emergency) { //BOTAO *
    debouceKeyId = 12;
    debouceTrigger[5] = true;
  }
  else if (BTN_N(analogRead(TECLADO))) { //BOTAO 0
    debouceKeyId = 13;
    debouceTrigger[5] = true;
  }
  else if (BTN_O(analogRead(TECLADO))) { //BOTAO #
    debouceKeyId = 14;
    debouceTrigger[5] = true;
  }
  else if (BTN_P(analogRead(TECLADO))) { //BOTAO D
    debouceKeyId = 15;
    debouceTrigger[5] = true;
  }

  if (debouceTrigger[5]) {
    trigger[1] = true;
    trigger[2] = false;
    if (locked)
      analogWrite(LEDLOCK, 255);
    if (beginLCD) {
      if (debug) Serial.println("[Debug]: Begining lsd");
      lcd.begin(16, 2);
      reprintLCD = true;
      beginLCD = false;
    }
  } else {
    trigger[2] = true;
    trigger[1] = false;
  }
  if (previous == debouceKeyId || debouceKeyId == 65535)
    return;


  seconds[1] = 0;
  switch (debouceKeyId) {
    default: return;

    case 0:
      if (debug) Serial.println("[Debug]: Keypad=1");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '1';      
      } else if (configMode == 4) {
        debug = true;
        changeScreen(0);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 5) {
        acessAdriana = true;
        changeScreen(14);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 6) {
        acessRodrigo = true;
        changeScreen(14);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 10) {
        acessSueli = true;
        changeScreen(0);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 7) {
        autoLock = true;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 8) {
        autoLamp = true;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 9) {
        autoPC = true;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 11) {
        blockTV = true;
        refreshDigitalOutputs = true;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 12) {
        stateFiltro = true;
        refreshDigitalOutputs = true;
        changeScreen(0);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:
            newDay1 = 1;
            break;

          case 1:
            newDay2 = 1;
            break;

          case 2:
            newMonth1 = 1;
            break;

          case 3:
            newMonth2 = 1;
            break;

          case 4:
            newYear1 = 1;
            break;

          case 5:
            newYear2 = 1;
            break;

          case 6:
            newHour1 = 1;
            break;

          case 7:
            newHour2 = 1;
            break;

          case 8:
            newMinute1 = 1;
            break;

          case 9:
            newMinute2 = 1;
            break;
        }
        changeState++;
      }

      break;

    case 1:
      if (debug) Serial.println("[Debug]: Keypad=2");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '2';
      } else if (configMode == 4) {
        debug = false;
        changeScreen(0);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 5) {
        acessAdriana = false;
        changeScreen(14);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 6) {
        acessRodrigo = false;
        changeScreen(14);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 10) {
        acessSueli = false;
        changeScreen(0);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 7) {
        autoLock = false;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 8) {
        autoLamp = false;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 9) {
        autoPC = false;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 11) {
        blockTV = false;
        changeScreen(15);
        refreshDigitalOutputs = true;
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 12) {
        stateFiltro = false;
        changeScreen(0);
        refreshDigitalOutputs = true;
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:
            newDay1 = 2;
            break;

          case 1:
            newDay2 = 2;
            break;

          case 2:
            newMonth1 = 2;
            break;

          case 3:
            newMonth2 = 2;
            break;

          case 4:
            newYear1 = 2;
            break;

          case 5:
            newYear2 = 2;
            break;

          case 6:
            newHour1 = 2;
            break;

          case 7:
            newHour2 = 2;
            break;

          case 8:
            newMinute1 = 2;
            break;

          case 9:
            newMinute2 = 2;
            break;
        }
        changeState++;
      }
      break;

    case 2:
      if (debug) Serial.println("[Debug]: Keypad=3");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '3';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:
            newDay1 = 3;
            break;

          case 1:
            newDay2 = 3;
            break;

          case 2:
            newMonth1 = 3;
            break;

          case 3:
            newMonth2 = 3;
            break;

          case 4:
            newYear1 = 3;
            break;

          case 5:
            newYear2 = 3;
            break;

          case 6:

            break;

          case 7:
            newHour2 = 3;
            break;

          case 8:
            newMinute1 = 3;
            break;

          case 9:
            newMinute2 = 3;
            break;
        }
        changeState++;
      }
      break;

    case 3:
      if (debug) Serial.println("[Debug]: Keypad=A");
      if (configMode == 2) {
        changeScreen(14);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      }
      break;

    case 4:
      if (debug) Serial.println("[Debug]: Keypad=4");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '4';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:

            break;

          case 1:
            newDay2 = 4;
            break;

          case 2:
            newMonth1 = 4;
            break;

          case 3:
            newMonth2 = 4;
            break;

          case 4:
            newYear1 = 4;
            break;

          case 5:
            newYear2 = 4;
            break;

          case 6:

            break;

          case 7:
            newHour2 = 4;
            break;

          case 8:
            newMinute1 = 4;
            break;

          case 9:
            newMinute2 = 4;
            break;
        }
        changeState++;
      }
      break;

    case 5:
      if (debug) Serial.println("[Debug]: Keypad=5");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '5';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:

            break;

          case 1:
            newDay2 = 5;
            break;

          case 2:
            newMonth1 = 5;
            break;

          case 3:
            newMonth2 = 5;
            break;

          case 4:
            newYear1 = 5;
            break;

          case 5:
            newYear2 = 5;
            break;

          case 6:

            break;

          case 7:
            newHour2 = 5;
            break;

          case 8:
            newMinute1 = 5;
            break;

          case 9:
            newMinute2 = 5;
            break;
        }
        changeState++;
      }
      break;

    case 6:
      if (debug) Serial.println("[Debug]: Keypad=6");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '6';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:

            break;

          case 1:
            newDay2 = 6;
            break;

          case 2:
            newMonth1 = 6;
            break;

          case 3:
            newMonth2 = 6;
            break;

          case 4:
            newYear1 = 6;
            break;

          case 5:
            newYear2 = 6;
            break;

          case 6:

            break;

          case 7:
            newHour2 = 6;
            break;

          case 8:
            newMinute1 = 6;
            break;

          case 9:
            newMinute2 = 6;
            break;
        }
        changeState++;
      }
      break;

    case 7:
      if (debug) Serial.println("[Debug]: Keypad=B");
      if (configMode == 2) {
        changeScreen(12);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      }
      break;

    case 8:
      if (debug) Serial.println("[Debug]: Keypad=7");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '7';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:

            break;

          case 1:
            newDay2 = 7;
            break;

          case 2:
            newMonth1 = 7;
            break;

          case 3:
            newMonth2 = 7;
            break;

          case 4:
            newYear1 = 7;
            break;

          case 5:
            newYear2 = 7;
            break;

          case 6:

            break;

          case 7:
            newHour2 = 7;
            break;

          case 8:

            break;

          case 9:
            newMinute2 = 7;
            break;
        }
        changeState++;
      }
      break;

    case 9:
      if (debug) Serial.println("[Debug]: Keypad=8");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '8';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:

            break;

          case 1:
            newDay2 = 8;
            break;

          case 2:
            newMonth1 = 8;
            break;

          case 3:
            newMonth2 = 8;
            break;

          case 4:
            newYear1 = 8;
            break;

          case 5:
            newYear2 = 8;
            break;

          case 6:

            break;

          case 7:
            newHour2 = 8;
            break;

          case 8:

            break;

          case 9:
            newMinute2 = 8;
            break;
        }
        changeState++;
      }
      break;

    case 10:
      if (debug) Serial.println("[Debug]: Keypad=9");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '9';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:

            break;

          case 1:
            newDay2 = 9;
            break;

          case 2:
            newMonth1 = 9;
            break;

          case 3:
            newMonth2 = 9;
            break;

          case 4:
            newYear1 = 9;
            break;

          case 5:
            newYear2 = 9;
            break;

          case 6:

            break;

          case 7:
            newHour2 = 9;
            break;

          case 8:

            break;

          case 9:
            newMinute2 = 9;
            break;
        }
        changeState++;
      }
      break;

    case 11:
      if (debug) Serial.println("[Debug]: Keypad=C");
      if (configMode == 2) {
        //configMode=7;
        changeScreen(15);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      }
      break;

    case 12:
      if (debug) Serial.println("[Debug]: Keypad=*");
      if (configMode != 0) {
        currentPassword = "";
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        changeScreen(0);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      } else {
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        currentPassword = "";
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        changeScreen(9);
      }
      break;

    case 13:
      if (debug) Serial.println("[Debug]: Keypad=0");
      if (configMode == 0 || configMode == 1 || configMode == 13) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        currentPassword += '0';
      } else if (configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        reprintLCD = true;
        switch (changeState) {
          default:
            newDay1 = 0;
            break;

          case 1:
            newDay2 = 0;
            break;

          case 2:
            newMonth1 = 0;
            break;

          case 3:
            newMonth2 = 0;
            break;

          case 4:
            newYear1 = 0;
            break;

          case 5:
            newYear2 = 0;
            break;

          case 6:
            newHour1 = 0;
            break;

          case 7:
            newHour2 = 0;
            break;

          case 8:
            newMinute1 = 0;
            break;

          case 9:
            newMinute2 = 0;
            break;
        }
        changeState++;
      }
      break;

    case 14:
      if (debug) Serial.println("[Debug]: Keypad=#");
      if (configMode == 0 || configMode == 1) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        currentPassword = "";
        changeScreen(16);
      } else if (configMode == 13) {
        if (digitalRead(BACKLIGTH) == LOW)
          beginLCD = true;
        changeScreen(0);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        currentPassword = "";
      } else if (configMode == 5 || configMode == 6 || configMode == 10) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        changeScreen(14);
      } else if (configMode == 7 || configMode == 8 || configMode == 9 || configMode == 11 || configMode == 12 || configMode == 17) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        changeScreen(15);
      } else if (configMode == 14 || configMode == 15 || configMode == 16 || configMode == 3) {
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
        changeScreen(12);
      }
      break;

    case 15:
      if (debug) Serial.println("[Debug]: Keypad=D");
      if (configMode == 2) {
        changeScreen(13);
        if ((hora < hourLamp || (hora == hourLamp && minuto <= minuteLamp)) && hora >= hourToAwakeLamp)
          beep();
      }
      break;
  }
  digitalWrite(BACKLIGTH, HIGH);
}
//-------------------------------------------------------------------------------------------------------------------------

//VERIFICAR SENHA----------------------------------------------------------------------------------------------------------
void checkPass() {
  if (configMode == 1) {
    if (currentPassword == passwordRoot || currentPassword == sPasswordRoot) {
      if (debug) Serial.println("Config MODE: " + currentPassword);
      changeScreen(11);
    }
    else if (currentPassword.length() >= 8) {
      if (debug)
        Serial.println("ACESSO NEGADO " + currentPassword);
      changeScreen(0);
    }
  } else if (configMode == 13) {
    if (currentPassword == passwordRoot || currentPassword == sPasswordRoot) {
      if (debug) Serial.println("Config MODE: " + currentPassword);
      if (statePC) {
        pulsePC();
      }
      if (stateLamp)
        toInvertLamp = true;
      sleeping = true;
      lampSleeping = true;
      rootCount = 0;
      changeScreen(17);
    } else if (currentPassword.length() >= 8) {
      if (debug)
        Serial.println("ACESSO NEGADO " + currentPassword);
      changeScreen(0);
    }
  } else {
    if (currentPassword == passwordRoot) {
      if (debug)
        Serial.println("ROOT LOGIN " + currentPassword);
      rootCount++;
      redCode(false);
      if (sleeping) {
        stateFiltro = true;
        refreshDigitalOutputs = true;
        lampSleeping = false;
      }
      sleeping = false;
      if (autoPC && !emergency) autoTurnOnPC();
      if (locked) {
        changeScreen(3);
      } else {
        if (sleeping)
          changeScreen(8);
        else
          changeScreen(17);
      }
      invertLock();
      if (dia == 10 && mes == 3)TurnOnMusic();
    } else if (currentPassword == sPasswordRoot) {
      if (debug)
        Serial.println("ROOT2 LOGIN " + currentPassword);
      if (rootCount > 0)rootCount++;
      redCode(false);
      if (locked) {
        changeScreen(3);
      } else {
        if (sleeping)
          changeScreen(8);
        else
          changeScreen(17);
      }
      invertLock();
      if (dia == 10 && mes == 3)TurnOnMusic();
    } else if (currentPassword == passwordAdriana) {
      if (!acessAdriana || emergency) {
        Serial.println("SEM PERMISSAO" + currentPassword);
        changeScreen(7);
        return;
      }
      if (wrongPass < 5) {
        if (debug)
          Serial.println("ADRIANA LOGIN " + currentPassword);
        adrianaCount++;
        changeScreen(3);
        unlock();
        if (adrianaCount >= adrianaMaxCount) {
          acessAdriana = false;
        }
      } else {
        Serial.println("Passou do limite diario" + currentPassword);
        changeScreen(6);
      }
    }
    else if (currentPassword == passwordRodrigo) {
      if (!acessRodrigo || emergency) {
        Serial.println("SEM PERMISSAO" + currentPassword);
        changeScreen(7);
        return;
      }
      if (wrongPass < 5) {
        if (debug)
          Serial.println("RODRIGO LOGIN " + currentPassword);
        rodrigoCount++;
        changeScreen(3);
        unlock();
        if (rodrigoCount >= rodrigoMaxCount) {
          acessRodrigo = false;
        }
      } else {
        Serial.println("Passou do limite diario" + currentPassword);
        changeScreen(6);
        unlock();
      }
    } else if (currentPassword == passwordSueli) {
      if (!acessSueli || emergency) {
        Serial.println("SEM PERMISSAO" + currentPassword);
        changeScreen(7);
        return;
      }
      if (wrongPass < 5) {
        if (debug)
          Serial.println("SUELI LOGIN " + currentPassword);
        changeScreen(3);
        unlock();
      }
    } else if (currentPassword.length() >= 8) {
      if (debug)
        Serial.println("ACESSO NEGADO " + currentPassword);
      changeScreen(4);
    }
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//Troca tela---------------------------------------------------------------------------------------------------------------
void changeScreen(int newScreen) {
  if (wrongPass >= maxWrongPass)
    newScreen = 5;
  switch (newScreen) {
    default: return;

    case 0:
      configMode = 0;
      screen = 0;
      debouceTrigger[5] = false;
      currentPassword = "";
      break;

    case 1:
      configMode = 0;
      screen = 1;
      break;

    case 2:
      configMode = 0;
      screen = 2;
      break;

    case 3: //garantido
      screen = 3;
      currentPassword = "";
      wrongPass = 0;
      configMode = 0;
      debouceTrigger[5] = false;
      trigger[3] = true;
      break;

    case 4: //negado
      screen = 4;
      trigger[3] = true;
      currentPassword = "";
      debouceTrigger[5] = false;
      wrongPass++;
      configMode = 0;
      PlayMusic.run();
      break;

    case 5:
      screen = 5;
      configMode = 0;
      break;

    case 6://limite
      screen = 6;
      debouceTrigger[5] = false;
      configMode = 0;
      PlayMusic.run();
      currentPassword = "";
      trigger[3] = true;
      break;

    case 7://sem permiso
      screen = 7;
      debouceTrigger[5] = false;
      configMode = 0;
      PlayMusic.run();
      currentPassword = "";
      trigger[3] = true;
      break;

    case 8: //lock
      screen = 8;
      currentPassword = "";
      wrongPass = 0;
      configMode = 0;
      debouceTrigger[5] = false;
      trigger[3] = true;
      break;

    case 9:
      screen = 9;
      configMode = 1;
      trigger[4] = true;
      seconds[4] = 0;
      break;

    case 10:
      screen = 10;
      configMode = 1;
      trigger[4] = true;
      seconds[4] = 0;
      break;

    case 11:
      currentPassword = "";
      screen = 11;
      configMode = 2;
      trigger[4] = true;
      seconds[4] = 0;
      break;

    case 12:
      screen = 12;
      trigger[4] = true;
      seconds[4] = 0;
      if (configMode == 14) {
        configMode = 15;
      } else if (configMode == 15) {
        configMode = 3;
      } else if (configMode == 3) {
        configMode = 16;
      } else if (configMode == 16) {
        configMode = 14;
      } else {
        configMode = 14;
      }
      break;

    case 13:
      screen = 13;
      configMode = 4;
      trigger[4] = true;
      seconds[4] = 0;
      break;

    case 14:
      screen = 14;
      trigger[4] = true;
      seconds[4] = 0;
      if (configMode == 5) {
        configMode = 6;
      } else if (configMode == 6) {
        configMode = 10;
      } else if (configMode == 10) {
        configMode = 5;
      } else {
        configMode = 5;
      }
      break;

    case 15:
      screen = 15;
      trigger[4] = true;
      seconds[4] = 0;
      changeDay = false;
      changeMonth = false;
      changeYear = false;
      changeHour = false;
      changeMinute = false;
      newDay1 = 4;
      newDay2 = 10;
      newMonth1 = 2;
      newMonth2 = 10;
      newYear1 = 10;
      newYear2 = 10;
      newHour1 = 3;
      newHour2 = 10;
      newMinute1 = 7;
      newMinute2 = 10;
      if (configMode == 7) {
        configMode = 8;
      } else if (configMode == 8) {
        configMode = 9;
      } else if (configMode == 9) {
        configMode = 11;;
      } else if (configMode == 11) {
        configMode = 12;
      } else if (configMode == 12) {
        configMode = 17;
      } else if (configMode == 17) {
        configMode = 7;
      } else {
        configMode = 7;
      }
      break;

    case 16:
      configMode = 13;
      trigger[4] = true;
      seconds[4] = 0;
      screen = 16;
      break;

    case 17:
      screen = 17;
      currentPassword = "";
      configMode = 0;
      debouceTrigger[5] = false;
      trigger[3] = true;
      break;
  }
  printLCD();
}
//-------------------------------------------------------------------------------------------------------------------------

//printLCD------------------------------------------------------------------------------------------------------------
void printLCD() {
  reprintLCD = false;
  lcd.noCursor();
  lcd.clear();
  String temp = "";
  if (currentPassword != "" && screen == 0) {
    if (configMode == 0)
      screen = 1;
  }
  if (dia == 10 && mes == 3) {
    screen = 18;
  }
  switch (screen) {
    default: return;
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("  Em busca do   ");
      lcd.setCursor(0, 1);
      lcd.print("QuartoAutomatico");
      break;

    case 1:
      temp = "";
      switch (dow) {
        case 0:
          temp += "D ";
          break;
        case 1:
          temp += "S ";
          break;
        case 2:
          temp += "T ";
          break;
        case 3:
          temp += "QA";
          break;
        case 4:
          temp += "QI";
          break;
        case 5:
          temp += "SE";
          break;
        case 6:
          temp += "SA";
          break;
      }
      temp += doisCharInt(dia);
      temp += '/';
      temp += doisCharInt(mes);
      temp += '/';
      temp += doisCharInt(AnoDoisChar(ano));
      temp += ' ';
      temp += doisCharInt(hora);
      temp += ':';
      temp += doisCharInt(minuto);
      lcd.setCursor(0, 0);
      lcd.print(temp);
      temp = "";
      if (currentPassword == "")
        temp = " Digite a Senha ";
      else {
        if (passMask) {
          temp = "";
          for (int i = 0; i < currentPassword.length(); i++)
            temp += "*";
        } else {
          temp = currentPassword;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print(temp);
      break;

    case 2:
      temp = "";
      switch (dow) {
        case 0:
          temp += "D ";
          break;
        case 1:
          temp += "S ";
          break;
        case 2:
          temp += "T ";
          break;
        case 3:
          temp += "QA";
          break;
        case 4:
          temp += "QI";
          break;
        case 5:
          temp += "SE";
          break;
        case 6:
          temp += "SA";
          break;
      }
      temp += doisCharInt(dia);
      temp += '/';
      temp += doisCharInt(mes);
      temp += '/';
      temp += doisCharInt(AnoDoisChar(ano));
      temp += ' ';
      temp += "T ";
      temp += String((int)temperature, DEC);
      lcd.setCursor(0, 0);
      lcd.print(temp);
      lcd.write((byte)0);//escreve  º
      temp = "";
      if (currentPassword == "")
        temp = " Digite a Senha ";
      else {
        if (passMask) {
          temp = "";
          for (int i = 0; i < currentPassword.length(); i++)
            temp += "*";
        } else {
          temp = currentPassword;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print(temp);
      break;

    case 3:
      lcd.setCursor(0, 0);
      lcd.print("ACESSO GARANTIDO");
      lcd.setCursor(0, 1);
      lcd.print(" Seja Bem-Vindo ");
      break;

    case 4:
      temp = " ACESSO NEGADO  ";
      lcd.setCursor(0, 0);
      lcd.print(temp);
      temp = "Tente Novamente";
      temp += String(wrongPass, DEC);
      if (wrongPass == 4)
        temp = "5 Vezes Seguidas";
      if (wrongPass == 3)
        temp = "Ultima Tentativa";
      lcd.setCursor(0, 1);
      lcd.print(temp);
      break;

    case 5:
      lcd.setCursor(0, 0);
      lcd.print(" MODO SEGURANCA ");
      lcd.setCursor(0, 1);
      lcd.print("ROOT IS REQUIRED");
      break;

    case 6:
      lcd.setCursor(0, 0);
      lcd.print(" VOCE ATINGIU O ");
      lcd.setCursor(0, 1);
      lcd.print(" LIMITE DIARIO  ");
      break;

    case 7:
      lcd.setCursor(0, 0);
      lcd.print("  VOCE NAO TEM  ");
      lcd.setCursor(0, 1);
      lcd.print("    PERMISSAO   ");
      break;

    case 8:
      if (stateDoor) {
        lcd.setCursor(0, 0);
        lcd.print("A PORTA TRANCADA");
        lcd.setCursor(0, 1);
        lcd.print("    GOOD FUN    ");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("NADA ACONTECEU..");
        lcd.setCursor(0, 1);
        lcd.print("RETARDADO...    ");
      }
      break;

    case 9:
      lcd.setCursor(0, 0);
      lcd.print("* =SaiDasConfigs");
      if (currentPassword == "")
        temp = "ROOT IS REQUIRED";
      else {
        if (passMask) {
          temp = "";
          for (int i = 0; i < currentPassword.length(); i++)
            temp += "*";
        } else {
          temp = currentPassword;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print(temp);
      break;

    case 10:
      lcd.setCursor(0, 0);
      lcd.print("# =NavegaSubConf");
      if (currentPassword == "")
        temp = "ROOT IS REQUIRED";
      else {
        if (passMask) {
          temp = "";
          for (int i = 0; i < currentPassword.length(); i++)
            temp += "*";
        } else {
          temp = currentPassword;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print(temp);
      break;

    case 11:
      lcd.setCursor(0, 0);
      lcd.print("A=Acesso|B=Vars ");
      lcd.setCursor(0, 1);
      lcd.print("C=Control|D=Dbug");
      break;

    case 12:
      switch (configMode) {
        case 3:
          for (int i = 0; i < 4; i++) {
            temp += ip[i];
            if (i < 3)
              temp += '.';
          }
          lcd.setCursor(0, 0);
          lcd.print(temp);
          temp = "PORTA: " + String(port, DEC);
          lcd.setCursor(0, 1);
          lcd.print(temp);
          break;

        case 14:
          temp = "Adriana Count:";
          temp += doisCharInt(adrianaCount);
          lcd.setCursor(0, 0);
          lcd.print(temp);
          temp = "Rodrigo Count:";
          temp += doisCharInt(rodrigoCount);
          lcd.setCursor(0, 1);
          lcd.print(temp);
          break;

        case 15:
          temp = "Senhas erradas:";
          temp += String(wrongPass, DEC);
          lcd.setCursor(0, 0);
          lcd.print(temp);
          temp = "N Emergencias:";
          temp += doisCharInt(emergencyCount);
          lcd.setCursor(0, 1);
          lcd.print(temp);
          lcd.setCursor(1, 1);
          lcd.write((byte)0);//escreve  º
          emergencyCount = 0;
          break;

        case 16:
          temp = "Luminosidad:";
          if (lightPercentage > 99)
            temp += String(lightPercentage, DEC);
          else
            temp += doisCharInt(lightPercentage);
          temp += '%';
          lcd.setCursor(0, 0);
          lcd.print(temp);
          temp = "----------------";
          lcd.setCursor(0, 1);
          lcd.print(temp);
          break;
      }
      break;

    case 13:
      temp += "DebugMode= ";
      if (debug)
        temp += "true ";
      else
        temp += "false";
      lcd.setCursor(0, 0);
      lcd.print(temp);
      lcd.setCursor(0, 1);
      lcd.print("1=true | 2=false");
      break;

    case 14:
      switch (configMode) {
        case 5:
          temp += " Adriana = ";
          if (acessAdriana)
            temp += "true ";
          else
            temp += "false";
          break;

        case 6:
          temp += " Rodrigo = ";
          if (acessRodrigo)
            temp += "true ";
          else
            temp += "false";
          break;

        case 10:
          temp += " Sueli  = ";
          if (acessSueli)
            temp += "true ";
          else
            temp += "false";
          break;
      }
      lcd.setCursor(0, 0);
      lcd.print(temp);
      lcd.setCursor(0, 1);
      lcd.print("1=true | 2=false");
      break;

    case 15:
      switch (configMode) {
        case 7:
          temp += "AutoLock = ";
          if (autoLock)
            temp += "true ";
          else
            temp += "false";
          break;

        case 8:
          temp += "AutoLamp = ";
          if (autoLamp)
            temp += "true ";
          else
            temp += "false";
          break;

        case 9:
          temp += "AutoPC   = ";
          if (autoPC)
            temp += "true ";
          else
            temp += "false";
          break;

        case 11:
          temp += "Block TV = ";
          if (blockTV)
            temp += "true ";
          else
            temp += "false";
          break;

        case 12:
          temp += "Filtro   = ";
          if (stateFiltro)
            temp += "ON ";
          else
            temp += "OFF";
          break;
        case 17:
          temp = "";
          switch (dow) {
            case 0:
              temp += "D ";
              break;
            case 1:
              temp += "S ";
              break;
            case 2:
              temp += "T ";
              break;
            case 3:
              temp += "QA";
              break;
            case 4:
              temp += "QI";
              break;
            case 5:
              temp += "SE";
              break;
            case 6:
              temp += "SA";
              break;
          }
          temp += doisCharInt(dia);
          temp += '/';
          temp += doisCharInt(mes);
          temp += '/';
          temp += doisCharInt(AnoDoisChar(ano));
          temp += ' ';
          temp += doisCharInt(hora);
          temp += ':';
          temp += doisCharInt(minuto);
          lcd.setCursor(0, 0);
          lcd.print(temp);

          temp = "";
          lcd.setCursor(0, 1);

          if (newDay1 > 3 || newDay2 > 9) {
            if (changeState == 0)
              temp += "  ";
            else if (changeState == 1)
              temp += String(newDay1, DEC) + " ";
            else
              temp += doisCharInt(dia);
          } else {
            changeDay = true;
            temp += String(newDay1, DEC);
            temp += String(newDay2, DEC);
          }
          temp += "/";
          if (newMonth1 > 1 || newMonth2 > 9) {
            if (changeState == 2)
              temp += "  ";
            else if (changeState == 3)
              temp += String(newMonth1, DEC) + " ";
            else if (changeState < 2)
              temp += "  ";
            else
              temp += doisCharInt(mes);
          } else {
            changeMonth = true;
            temp += String(newMonth1, DEC);
            temp += String(newMonth2, DEC);
          }
          temp += "/";
          if (newYear1 > 9 || newYear2 > 9) {
            if (changeState == 4)
              temp += "  ";
            else if (changeState == 5)
              temp += String(newYear1, DEC) + " ";
            else if (changeState < 4)
              temp += "  ";
            else
              temp += doisCharInt(AnoDoisChar(ano));
          } else {
            changeYear = true;
            temp += String(newYear1, DEC);
            temp += String(newYear2, DEC);
          }
          temp += " ";
          if (newHour1 > 2 || newHour2 > 9) {
            if (changeState == 6)
              temp += "  ";
            else if (changeState == 7)
              temp += String(newHour1, DEC) + " ";
            else if (changeState < 6)
              temp += "  ";
            else
              temp += doisCharInt(hora);
          } else {
            changeHour = true;
            temp += String(newHour1, DEC);
            temp += String(newHour2, DEC);
          }
          temp += ":";
          if (newMinute1 > 6 || newMinute2 > 9) {
            if (changeState == 8)
              temp += "  ";
            else if (changeState == 9)
              temp += String(newMinute1, DEC) + " ";
            else if (changeState < 8)
              temp += "  ";
            else
              temp += doisCharInt(minuto);
          } else {
            changeMonth = true;
            temp += String(newMinute1, DEC);
            temp += String(newMinute2, DEC);
          }

          switch (dow) {
            case 0:
              temp += "D ";
              break;
            case 1:
              temp += "S ";
              break;
            case 2:
              temp += "T ";
              break;
            case 3:
              temp += "QA";
              break;
            case 4:
              temp += "QI";
              break;
            case 5:
              temp += "SE";
              break;
            case 6:
              temp += "SA";
              break;
          }
          lcd.print(temp);
          switch (changeState) {
            default:
              lcd.setCursor(0, 1);
              break;
            case 1:
              lcd.setCursor(1, 1);
              break;
            // /
            case 2:
              lcd.setCursor(3, 1);
              break;
            case 3:
              lcd.setCursor(4, 1);
              break;
            // /
            case 4:
              lcd.setCursor(6, 1);
              break;
            case 5:
              lcd.setCursor(7, 1);
              break;
            // " "
            case 6:
              lcd.setCursor(9, 1);
              break;
            case 7:
              lcd.setCursor(10, 1);
              break;
            // :
            case 8:
              lcd.setCursor(12, 1);
              break;

            case 9:
              lcd.setCursor(13, 1);
              break;
          }

          if (changeDay) {
            dia = newDay1 * 10 + newDay2;
            if (dia > 31) {
              configMode = 12;
              changeScreen(15);
              return;
            }
            RTCadjust();
            newDay1 = 4;
            newDay2 = 10;
            changeDay = false;
            refreshRTC();
          }
          if (changeMonth) {
            mes = newMonth1 * 10 + newMonth2;
            if (mes > 12) {
              configMode = 12;
              changeScreen(15);
              return;
            }
            if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) {
              if (dia > 31) {
                configMode = 12;
                changeScreen(15);
                return;
              }
            } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
              if (dia > 30) {
                configMode = 12;
                changeScreen(15);
                return;
              }
            } else {
              if (dia > 28) {
                configMode = 12;
                changeScreen(15);
                return;
              }
            }
            RTCadjust();
            newMonth1 = 2;
            newMonth2 = 10;
            changeMonth = false;
            refreshRTC();
          }
          if (changeYear) {
            ano = newYear1 * 10 + newYear2;
            if (ano > 99) {
              configMode = 12;
              changeScreen(15);
              return;
            }
            ano += 2000;
            RTCadjust();
            newYear1 = 10;
            newYear2 = 10;
            changeYear = false;
            refreshRTC();
          }
          if (changeHour) {
            hora = newHour1 * 10 + newHour2;
            if (hora > 24) {
              configMode = 12;
              changeScreen(15);
              return;
            }
            RTCadjust();
            newHour1 = 4;
            newHour2 = 10;
            changeHour = false;
            refreshRTC();
          }
          if (changeMinute) {
            minuto = newMinute1 * 10 + newMinute2;
            if (minuto > 60) {
              configMode = 12;
              changeScreen(15);
              return;
            }
            RTCadjust();
            newMinute1 = 4;
            newMinute2 = 10;
            changeMinute = false;
            refreshRTC();
          }
          return;
      }
      lcd.setCursor(0, 0);
      lcd.print(temp);
      lcd.setCursor(0, 1);
      lcd.print("1=true | 2=false");
      break;
    case 16:
      lcd.setCursor(0, 0);
      lcd.print("SleepTime ZZzzz");
      if (currentPassword == "")
        temp = "ROOT IS REQUIRED";
      else {
        if (passMask) {
          temp = "";
          for (int i = 0; i < currentPassword.length(); i++)
            temp += "*";
        } else {
          temp = currentPassword;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print(temp);
      break;

    case 17:
      if (sleeping) {
        lcd.setCursor(0, 0);
        lcd.print("MODO SONECA ON..");
        lcd.setCursor(0, 1);
        lcd.print("ZZZzzzzzzzzzzzzz");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("MODO SONECA OFF");
        lcd.setCursor(0, 1);
        lcd.print("Wake-Up B1tch...");
      }
      break;

    case 18:
      lcd.setCursor(0, 0);
      lcd.print("FelizAniversario");
      temp = "  ";
      temp += String(ano - 1998, DEC);
      temp += " aninhos   ";
      lcd.setCursor(0, 1);
      lcd.print(temp);
      break;
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//DEBUG VARS---------------------------------------------------------------------------------------------------------------
void showDebug() {
  if (TCPserver)
    Serial.println("[Debug at " + doisCharInt(hora) + ":" + doisCharInt(minuto) + ":" + doisCharInt(segundo) + "]: Running at " + String(ip[0], DEC) + "." + String(ip[1], DEC) + "." + String(ip[2], DEC) + "." + String(ip[3], DEC) + ":" + String(port, DEC));
  else
    Serial.println("[Debug at " + doisCharInt(hora) + ":" + doisCharInt(minuto) + ":" + doisCharInt(segundo) + "]: Server isn't running");
  Serial.println("    [Temperature]: " + String(temperature, DEC) + (char)177 + "C");
  Serial.println("    [MS Count]: " + String(millis(), DEC));
  Serial.println("    [Entrances]: Adriana=" + String(adrianaCount, DEC) + " Rodrigo=" + String(rodrigoCount, DEC) + " Root=" + String(rootCount, DEC));
  Serial.println("    [Lamp]: PulseIN=" + String(durationLamp, DEC) + " Percentage=" + String(lightPercentage, DEC) + " SleepState=" + String(lampSleeping, BIN));
  Serial.println("    [Acess]: Adriana=" + String(acessAdriana, BIN) + " Rodrigo=" + String(acessRodrigo, BIN));
  Serial.println("    [Config]: Serial Server=" + String(SerialServer, BIN) + " PassMask=" + String(passMask, BIN));
  Serial.println("    [Screen]: Config Mode=" + String(configMode, DEC) + " Screen=" + String(screen, DEC));
  Serial.println("    [State]: Locked=" + String(locked, BIN) + " State PC=" + String(statePC, BIN) + " Emergency=" + String(emergency, BIN) + " State Window=" + String(stateWindow, BIN));
  Serial.println("    [State]: State Door=" + String(stateDoor, BIN) + " State Lamp=" + String(stateLamp, BIN) + " Block TV=" + String(blockTV, BIN) + " State Filtro=" + String(stateFiltro, BIN));
  Serial.println("    [Auto]: Auto Lock=" + String(autoLock, BIN) + " Auto Lamp=" + String(autoLamp, BIN) + " Auto PC=" + String(autoPC, BIN));
}
//-------------------------------------------------------------------------------------------------------------------------

//EMERGENCY MODE-----------------------------------------------------------------------------------------------------------
void redCode(boolean active) {
  if (active) {
    if (emergencyMode == false) {
      if (!emergency)
        PlayMusic.run();
      if (emergencyUnlock) {
        unlock();
      }
      emergencyCount++;
      emergencyTV = blockTV;
      emergencyFiltro = stateFiltro;
      emergencyLamp = stateLamp;
      digitalWrite(LIGARTV, LOW);
      digitalWrite(LIGARLAMP, LOW);
      digitalWrite(LIGARFILTRO, LOW);
      emergencyMode = true;
    }
  } else {
    if (emergencyMode) {
      if (emergencyTV)
        digitalWrite(LIGARTV, HIGH);
      if (!emergencyFiltro)
        digitalWrite(LIGARFILTRO, HIGH);
      if (emergencyLamp)
        digitalWrite(LIGARLAMP, HIGH);
      if (emergencyUnlock) {
        lock();
      }
    }
    emergencyUnlock = false;
    emergencyMode = false;
    emergency = false;
  }

}
//-------------------------------------------------------------------------------------------------------------------------

//Serial Server------------------------------------------------------------------------------------------------------------
void checkSerialServer() {
  String ClientData = "\n";
  while (Serial.available()) {
    ClientData += (char)Serial.read();
  }

  if (ClientData != "\n") {
    if (debug)
      Serial.println("DATA FROM SERIAL:" + ClientData);
    //processa
    ProcessData(ClientData);
    //enviar dados
    for (int i = 0; i <= 21; i++) {
      Serial.print(String(i, DEC) + "!");
      switch (i) {
        case 0:
          if (locked)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 1:
          if (stateLamp)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 2:
          if (statePC)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 3:
          if (blockTV)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 4:
          if (acessAdriana)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 5:
          if (acessRodrigo)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 6:
          Serial.println(String(lightPercentage, DEC));
          break;

        case 7:
          if (stateDoor)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 8:
          if (stateWindow)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 9:
          if (emergency)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 10:
          Serial.println(String(adrianaCount, DEC));
          break;

        case 11:
          Serial.println(String(rodrigoCount, DEC));
          break;

        case 12:
          Serial.println(String(rootCount, DEC));
          break;

        case 13:
          if (autoLamp)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 14:
          Serial.print(doisCharInt(hourLamp));
          Serial.print(":");
          Serial.println(doisCharInt(minuteLamp));
          break;

        case 15:
          if (autoPC)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 16:
          Serial.print(doisCharInt(hourPC1));
          Serial.print(":");
          Serial.print(doisCharInt(minutePC1));
          Serial.print(doisCharInt(hourPC2));
          Serial.print(":");
          Serial.println(doisCharInt(minutePC2));
          break;

        case 17:
          if (autoLock)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 18:
          if (stateFiltro)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 19:
          Serial.println((int)temperature);
          break;

        case 20:
          if (acessSueli)
            Serial.println("1");
          else
            Serial.println("0");
          break;

        case 21:
          Serial.println((int)hourToAwakeLamp);
          break;
      }
    }
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//Ethernet Server----------------------------------------------------------------------------------------------------------
void checkTCPServer() {
  String ClientData = "";
  boolean readPass = true;
  boolean passOK = false;
  String pass = "";
  if (EthernetClient client = server.available()) {
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (passOK)
          ClientData += c;

        if (readPass) {
          if (c == '!' || c == '\n' || pass.length() > 8)
            readPass = false;
          else
            pass += c;
        }
        if (!readPass) {
          Serial.println("PASS FROM CLIENT: " + pass);
          if (pass == passwordRoot || pass == sPasswordRoot)
            passOK = true;
          if (!passOK) {
            client.print("HTTP/1.0 200 OK\r\n");
            client.print("Content-Type: text/html\r\n");
            // client.print("Refresh: 5\r\n");
            client.print("\r\n");
            client.print("<html>");
            client.print("   <head>");
            client.print("      <title>Wrong Protocol B1tch</title>");
            client.print("   </head>");
            client.print("   <body>");
            client.print("      <h3>Try Again :p.</h3>");
            client.print("      <p><em>I caught you<br />=D</em></p>");
            client.print("   </body>");
            client.print("</html>");
            client.stop();

          } else {
            if ((c == '\n' || c == '?') && current_line_is_blank) {
              if (debug)
                Serial.println("DATA FROM NET: " + ClientData);
              //processa
              ProcessData(ClientData);
              //enviar dados
              for (int i = 0; i <= 21; i++) {
                client.print(String(i, DEC) + "!");
                switch (i) {
                  case 0:
                    if (locked)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 1:
                    if (stateLamp)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 2:
                    if (statePC)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 3:
                    if (blockTV)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 4:
                    if (acessAdriana)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 5:
                    if (acessRodrigo)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 6:
                    client.print(String(lightPercentage, DEC));
                    break;

                  case 7:
                    if (stateDoor)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 8:
                    if (stateWindow)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 9:
                    if (emergency)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 10:
                    client.print(String(adrianaCount, DEC));
                    break;

                  case 11:
                    client.print(String(rodrigoCount, DEC));
                    break;

                  case 12:
                    client.print(String(rootCount, DEC));
                    break;

                  case 13:
                    if (autoLamp)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 14:
                    client.print(doisCharInt(hourLamp));
                    client.print(":");
                    client.print(doisCharInt(minuteLamp));
                    break;

                  case 15:
                    if (autoPC)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 16:
                    client.print(doisCharInt(hourPC1));
                    client.print(":");
                    client.print(doisCharInt(minutePC1));
                    client.print(doisCharInt(hourPC2));
                    client.print(":");
                    client.print(doisCharInt(minutePC2));
                    break;

                  case 17:
                    if (autoLock)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 18:
                    if (stateFiltro)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 19:
                    client.print((int)temperature);
                    break;

                  case 20:
                    if (acessSueli)
                      client.print("1");
                    else
                      client.print("0");
                    break;

                  case 21:
                    client.print((int)hourToAwakeLamp);
                    break;
                }
                client.print("\n");
              }
              //client.stop();
              client.print("\n");
            }
            if (c == '\n' || c == '?') {
              current_line_is_blank = true;
            }
            else if (c != '\r') {
              current_line_is_blank = false;
            }
          }
        }
      }
    }
    client.stop();
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//Packet Decoder-----------------------------------------------------------------------------------------------------------
void ProcessData(String dataToProcess) {
  if (dataToProcess == "" || dataToProcess == "/n/n")
    return;
  String field = "-1";
  String value = "-1";
  for (int i = 0; i < dataToProcess.length(); i++) {
    if (dataToProcess[i] == '!') {
      field = "0";
      value = "0";
      for (int j = 0; j < i; j++) {
        if (dataToProcess[j] != '\n')
          field += dataToProcess[j];
      }
      for (int j = (i + 1); j <= dataToProcess.length(); j++) {
        if (dataToProcess[j] != '\n')
          value += dataToProcess[j];
      }
    }
  }

  switch (field.toInt()) {
    default:
      return;
    case 0:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      invertLock();
      redCode(false);
      break;

    case 1:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      toInvertLamp = true;
      break;

    case 2:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      pulsePC();
      break;

    case 3:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      if (blockTV) {
        digitalWrite(LIGARTV, LOW);
        blockTV = false;
      } else {
        digitalWrite(LIGARTV, HIGH);
        blockTV = true;
      }
      break;

    case 4:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      if (acessAdriana)
        acessAdriana = false;
      else {
        adrianaCount = 0;
        acessAdriana = true;
      }
      break;

    case 5:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      if (acessRodrigo)
        acessRodrigo = false;
      else {
        rodrigoCount = 0;
        acessRodrigo = true;
      }
      break;

    case 6:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      percentageMinToLamp = value.toInt();
      break;

    case 13:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      if (autoLamp)
        autoLamp = false;
      else
        autoLamp = true;
      break;

    case 14:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      traslateTimeLamp(value);
      break;

    case 15:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      if (autoPC)
        autoPC = false;
      else
        autoPC = true;
      break;

    case 16:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      traslateTimePC(value);
      break;

    case 17:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      if (autoLock)
        autoLock = false;
      else
        autoLock = true;
      break;

    case 18:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      configMode = 0;


      if (statePC) {
        pulsePC();
      }

      if (stateFiltro) {
        if (statePC) {
          pulsePC();
        }
        sleeping = true;
        lampSleeping = true;
      } else {
        stateFiltro = true;
        sleeping = false;
        lampSleeping = false;
      }

      refreshDigitalOutputs = true;

      break;
    case 20:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      if (acessSueli)
        acessSueli = false;
      else
        acessSueli = true;
      break;
    case 21:
      if (debug) Serial.println("Packet ID: " + field + " Packet VALUE: " + value);
      hourToAwakeLamp = value.toInt();
      break;

    case 22:
      redCode(false);
      if (sleeping) {
        stateFiltro = true;
        refreshDigitalOutputs = true;
        lampSleeping = false;
      }
      sleeping = false;
      break;
  }
  refreshDigitalOutputs = true;
}
//-------------------------------------------------------------------------------------------------------------------------

//Alarme-------------------------------------------------------------------------------------------------------------------
void TurnOnMusic() {
  if (dia == 10 && mes == 3) {
    music_length = music2_length;
    for (int m = 0; m < music_length; m++) {
      music_notes[m] = music2_notes[m];
      music_beats[m] = music2_beats[m];
    }
    music_tempo = music2_tempo;
  } else
    switch (musica) {
      default:
        music_length = music1_length;
        for (int m = 0; m < music_length; m++) {
          music_notes[m] = music1_notes[m];
          music_beats[m] = music1_beats[m];
        }
        music_tempo = music1_tempo;
        break;

      case 2:
        music_length = music2_length;
        for (int m = 0; m < music_length; m++) {
          music_notes[m] = music2_notes[m];
          music_beats[m] = music2_beats[m];
        }
        music_tempo = music2_tempo;
        break;

      case -1:
        if (!alarmBuzState) {
          beep();
          alarmBuzState = false;
        } else {
          digitalWrite(BUZZER, HIGH);
          void desligarBuzzer();
          alarmBuzState = true;
        }
        break;
    }
  bigestForAlarm();
}

//função do loop principal
void bigestForAlarm() {
  if (music_inBigestLoop == false && music_bigestValue != -1)
    return;
  if (music_bigestValue >= music_length) {
    digitalWrite(BUZZER, LOW);
    music_inBigestLoop = false;
    music_bigestValue = -1;
    return;
  }
  if (music_inDelay == false)
    music_bigestValue++;
  music_inBigestLoop = true;
  if (music_delaySucess && music_inLoop == false) {
    music_needSucess = false;
    music_uDelay(music_tempo / 2 * 1000);
    music_delaySucess = false;
    return;
  }

  if (music_notes[music_bigestValue] == ' ') {
    if (music_inDelay == false && music_inLoop == false) {
      music_needSucess = true;
      music_uDelay(music_beats[music_bigestValue] * music_tempo * 1000); // rest
      return;
    }
  } else {
    //toca a nota correspondente
    if (music_inLoop == false)
      for (int i = 0; i < 8; i++) {
        if (music_names[i] == music_notes[music_bigestValue]) {
          //faz o tom da nota
          music_tone = music_tones[i];
          music_duration = music_beats[music_bigestValue] * music_tempo;
          forAlarm();
          break;
        }
      }
    else
      forAlarm();
  }
}

//função do loop secundario
void forAlarm() {
  if (music_inLoop == false && music_loopValue != -1)
    return;
  if (music_loopValue >= music_duration * 1000L) {
    digitalWrite(BUZZER, LOW);
    music_inLoop = false;
    music_loopValue = -1;
    return;
  }
  if (music_inDelay == false)
    music_loopValue++;
  music_inLoop = true;
  if (music_loopValue == 0 && music_inDelay == false) {
    digitalWrite(BUZZER, HIGH);
    music_needSucess = false;
    music_uDelay(music_tone);
  }
  if (music_loopValue > 0)
    if (music_delaySucess) {
      digitalWrite(BUZZER, HIGH);
      music_needSucess = false;
      music_uDelay(music_tone);
      music_delaySucess = false;
      return;
    } else if (music_inDelay == false) {
      digitalWrite(BUZZER, LOW);
      music_needSucess = true;
      music_uDelay(music_tone);
    }

}

//função que ativa o delay
void music_uDelay(float alarmdelay) {
  music_inDelay = true;
  music_timer = micros();
  music_microdelay = alarmdelay;
}

//função que conta o delay
void music_count() {
  if (music_timer <= micros() - music_microdelay) {
    music_timer = micros();
    if (music_needSucess == true)
      music_delaySucess = true;
    music_needSucess = false;
    music_inDelay = false;
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//UTIL---------------------------------------------------------------------------------------------------------------------
int AnoDoisChar(int anoquatro) {
  int anodois;
  anodois = anoquatro % 10;
  anoquatro = anoquatro / 10;
  anodois += (anoquatro % 10) * 10;
  return anodois;
}

String doisCharInt(int umChar) {
  String temp = "";
  if (umChar < 10) {
    temp += "0";
    temp += String (umChar, DEC);
  }
  else {
    temp += String (umChar, DEC);
  }
  return temp;
}

void traslateTimeLamp(String time) {
  String temp;
  for (int i = 0; i < time.length(); i++) {
    if (time[i] == ':') {
      temp = "";
      temp += time[i - 2];
      temp += time[i - 1];
      hourLamp = temp.toInt();
      temp = "";
      temp += time[i + 1];
      temp += time[i + 2];
      minuteLamp = temp.toInt();
    }
  }
}

void traslateTimePC(String time) {
  String temp;
  boolean firtSeparator = false;
  for (int i = 0; i < time.length(); i++) {
    if (time[i] == ':') {
      temp = "";
      temp += time[i - 2];
      temp += time[i - 1];
      if (!firtSeparator)
        hourPC1 = temp.toInt();
      else
        hourPC2 = temp.toInt();
      temp = "";
      temp += time[i + 1];
      temp += time[i + 2];
      if (!firtSeparator)
        minutePC1 = temp.toInt();
      else
        minutePC2 = temp.toInt();
      firtSeparator = true;
    }
  }
}
void RTCadjust() {
  Wire.beginTransmission(0x68);
  Wire.write((byte) 0);
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(minuto));
  Wire.write(bin2bcd(hora));
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(dia));
  Wire.write(bin2bcd(mes));
  Wire.write(bin2bcd(ano - 2000));
  Wire.write((byte) 0);
  Wire.endTransmission();
}
static uint8_t bin2bcd (uint8_t val) {
  return val + 6 * (val / 10);
}
//DateTime Format
//Feb 24 2011     DATE
//19:20:28        TIME
String RTCdate() {
  String date = "";
  switch (mes) {
    default:
      date += "Jan";
      break;
    case 2:
      date += "Feb";
      break;
    case 3:
      date += "Mar";
      break;
    case 4:
      date += "Apr";
      break;
    case 5:
      date += "May";
      break;
    case 6:
      date += "Jun";
      break;
    case 7:
      date += "Jul";
      break;
    case 8:
      date += "Aug";
      break;
    case 9:
      date += "Sep";
      break;
    case 10:
      date += "Oct";
      break;
    case 11:
      date += "Nov";
      break;
    case 12:
      date += "Dec";
      break;
  }
  date += " ";
  date += String(dia, DEC);
  date += " ";
  date += String(ano, DEC);
  return date;
}

String RTCtime() {
  String time = "";
  time += String(hora, DEC);
  time += ":";
  time += String(minuto, DEC);
  time += ":";
  time += String(segundo, DEC);
  return time;
}
//-------------------------------------------------------------------------------------------------------------------------
//WDT interrupt------------------------------------------------------------------------------------------------------------
ISR(WDT_vect) {
  wdt_reset();
  cli();
  wdt_disable();
  sei();
  //code
  unlock();
  loop();
}
//-------------------------------------------------------------------------------------------------------------------------
