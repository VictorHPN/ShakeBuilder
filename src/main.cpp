#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include "ShakeBuilder_lib.h"

/*===================== Máquina de estados ====================*/
typedef enum
{
  ESTADO_INICIO = 0,
  ESTADO_SUPLEMENTO = 1,
  ESTADO_PAGAMENTO = 2,
  ESTADO_PREPARO = 3
} Estados;

static Estados estado = ESTADO_INICIO;

/*=================== Variáveis de controle ===================*/
bool continue_pressed = false; // controle do botão de continue
Shake shake; // Objeto do shake a ser preparado
int t_max_pagamento = 30;
int temp = t_max_pagamento;

/*======================= Configs LCD =========================*/
LiquidCrystal_I2C lcd(0x3F, 20, 4); // Inicialização Display LCD
// Funções LCD:
void Tela_incio();
void Tela_suplemento();
void Tela_suplemento_atualizada(float preco, int id_sup, int doses, bool leite);
void Tela_pagamento_aguardando(int tempo);
void Tela_pagamento_confirmado();
void Tela_preparo_esperando_copo();
void Tela_preparo_servindo();
void Tela_fim();
void Tela_Pedido_Cancelado();

/*======================= Configs RFID =========================*/
#define ID "8C 07 02 04"
#define SS_PIN 5
#define RST_PIN 15
//Objeto módulo RFID:
MFRC522 mfrc522(SS_PIN, RST_PIN);
// Funções RFID
bool PagamentoRFID();

/*=============================================================*/

void setup()
{
  /*===================== Inputs/Outputs ======================*/
  pinMode(SENSOR_COPO, INPUT);
  pinMode(BUTTON_CONTINUE, INPUT);
  pinMode(BUTTON_RETIRA, INPUT);
  for (int i = 0; i<4; i++)
  {
    pinMode(button_state[i], INPUT);
    pinMode(led_state[i], OUTPUT);
    digitalWrite(led_state[i], LOW);
    pinMode(motor[i], OUTPUT);
    digitalWrite(motor[i], HIGH);
  }
  pinMode(BOMBA_AGUA, OUTPUT);
  digitalWrite(BOMBA_AGUA, HIGH);

  /*================ Inicialização Display LCD ================*/
  lcd.begin();
  lcd.backlight();

  /*=================== Inicialização RFID ====================*/
  // inicia a comunicacao SPI que sera usada para comunicacao com o mudulo RFID
  SPI.begin();
  mfrc522.PCD_Init();  //inicia o modulo RFID
}

void loop()
{
  /*=============== Máquina de Estados =================*/
  switch (estado)
  {
  case ESTADO_INICIO:
    while (1){ // Fica em repouso enquanto o botão não é apertado e solto
      Tela_incio();
      if(digitalRead(BUTTON_CONTINUE) == HIGH)
        break;
    }
    lcd.clear(); // Limpa display
    estado = ESTADO_SUPLEMENTO; // Avança para próximo estado
    break;

  case ESTADO_SUPLEMENTO:
    Tela_suplemento();
    while (continue_pressed == false || shake.get_id_suplemento() == -1) // Enquanto
    {
      for (int i = 0; i < 4; i++) // loop que monitora os 4 botões de suplemento e o botão do Leite
      {
        //Verifica se botão de retirar dose foi pressionado
        if(digitalRead(BUTTON_RETIRA) == HIGH && shake.get_id_suplemento() != -1 )
        {
          shake.Remove_do_Pedido(shake.get_id_suplemento());
          delay(400);
        }
        else {
          if(digitalRead(button_state[i]) == HIGH) // Ao identificar o pressionamento de um botão...
          {
            shake.Adiciona_ao_Pedido(i);
            delay(400);
          }
        }
        Tela_suplemento_atualizada(shake.get_preco_total(), shake.get_id_suplemento(),
                                  shake.get_doses(), shake.get_leite_st());
      }
      if (shake.get_id_suplemento() != -1)
      { // Verifica se Continue foi pressionado e armazena na variável de controle
        // apenas se algum suplemento foi selecionado: id =-1 -> pedido vazio 
        continue_pressed = digitalRead(BUTTON_CONTINUE);
      }
    }
    lcd.clear(); // Limpa display
    continue_pressed = false; // reset var de controle continue
    estado = ESTADO_PAGAMENTO; // Avança para próximo estado
    break;

  case ESTADO_PAGAMENTO:
    while(1) // Avança para o prepario somente se identificar o pagamaento
    {
        Tela_pagamento_aguardando(temp);
        if(PagamentoRFID() == true || temp == 0)
          break;
        else{
          delay(1000);
          temp = temp - 1;
        }
    }
    if (temp == 0) // se o pagamento não foi efetuado no tempo máximo
    {
      lcd.clear(); // Limpa display
      Tela_Pedido_Cancelado(); // Exibe tela de cancelamento devido a falta de pagamento
      delay(5000);
      shake.Limpa_Objeto(); // Reseta pedido
      lcd.clear();
      estado = ESTADO_INICIO; // Retorna ao inicio
    }
    else{
      lcd.clear(); // Limpa display
      Tela_pagamento_confirmado();
      delay(3000);
      estado = ESTADO_PREPARO;  // Avança para próximo estado
    }
    temp = t_max_pagamento; // reset temporizador para proxima compra
    break;

  case ESTADO_PREPARO:
    while (1) // Prepara o Shake somente se identificar o copo
    {
      Tela_preparo_esperando_copo();
      if(LOW == digitalRead(SENSOR_COPO))
        break;
    }
    lcd.clear(); // Limpa display
    Tela_preparo_servindo();
    shake.Prepara_Shake();
    Tela_fim();
    delay(5000);
    shake.Limpa_Objeto();
    lcd.clear();
    estado = ESTADO_INICIO;
    break;

  default:
    estado = ESTADO_INICIO;
    break;
  }
}

void Tela_incio()
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(5, 1);
  lcd.print("Bem-Vindo!");
  lcd.setCursor(0, 2);
  lcd.print("Clique em Continuar.");
  lcd.setCursor(0, 3);
  lcd.print("#==================#");
}

void Tela_suplemento()
{
  lcd.setCursor(4, 0);
  lcd.print("Preco:R$");
  lcd.setCursor(0, 1);
  lcd.print("Suplemento:");
  lcd.setCursor(13, 1);
  lcd.print("Doses:");
  lcd.setCursor(0, 2);
  lcd.print("Agua:   ml");
  lcd.setCursor(11, 2);
  lcd.print("Leite:");
  lcd.setCursor(4, 3);
  lcd.print("Continuar...?");
}

void Tela_suplemento_atualizada(float preco, int id_sup, int doses, bool leite)
{
  //Atualiza preço
  lcd.setCursor(12, 0);
  lcd.print(preco);
  //Atualiza id do suplemento selecionado
  lcd.setCursor(11, 1);
  if (id_sup != -1)
    lcd.print(id_sup+1);
  else
    lcd.print(" ");
  //Atualiza o número de doses
  lcd.setCursor(19, 1);
  lcd.print(doses);
  //Atualiza se o quantidade de água
  lcd.setCursor(5, 2);
  lcd.print(200*doses);
  //Atualiza se o leite foi selecionado
  lcd.setCursor(17, 2);
  if (leite == false)
  {
    lcd.print("Nao");
  }
  else
  {
    lcd.print("Sim");
  }
}

void Tela_pagamento_aguardando(int tempo)
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(0, 1);
  lcd.print("Aguardando Pagamento");
  lcd.setCursor(0, 2);
  lcd.print("Tempo Limite:");
  lcd.print(tempo);
  lcd.print("...");
  lcd.setCursor(0, 3);
  lcd.print("#==================#");
}

void Tela_pagamento_confirmado()
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(6, 1);
  lcd.print("Pagamento");
  lcd.setCursor(5, 2);
  lcd.print("Aprovado!:)");
  lcd.setCursor(0, 3);
  lcd.print("#==================#");
}

void Tela_preparo_esperando_copo()
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(0, 1);
  lcd.print("Insira seu copo para");
  lcd.setCursor(0, 2);
  lcd.print("receber seu Shake...");
  lcd.setCursor(0, 3);
  lcd.print("#==================#");
}

void Tela_preparo_servindo()
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(6, 1);
  lcd.print("Servindo");
  lcd.setCursor(6, 2);
  lcd.print("Shake...");
  lcd.setCursor(0, 3);
  lcd.print("#==================#");
}

void Tela_fim()
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(2, 1);
  lcd.print("Agradecemos pela");
  lcd.setCursor(5, 2);
  lcd.print("Compra! :)");
  lcd.setCursor(0, 3);
  lcd.print("#==================#");
}

void Tela_Pedido_Cancelado()
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(1, 1);
  lcd.print("Pedido Cancelado...");
  lcd.setCursor(0, 3);
  lcd.print("#==================#");
}

bool PagamentoRFID()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
     return false;                 // se nao tiver um cartao para ser lido recomeça a função
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;                  //se nao conseguir ler o cartao recomeça a função tambem
  }

  String conteudo = "";      // cria uma string

  for (byte i = 0; i < mfrc522.uid.size; i++){       // faz uma verificacao dos bits da memoria do cartao
    //ambos comandos abaixo vão concatenar as informacoes do cartao...
    //porem os 2 primeiros irao mostrar na serial e os 2 ultimos guardarao os valores na string de conteudo para fazer as verificacoes
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();                      // deixa as letras da string todas maiusculas

  if (conteudo.substring(1) == ID)// verifica se o ID do cartao lido tem o mesmo ID do cartao que queremos liberar o acesso
  {
    return true;
  }
  else // caso o cartao lido não foi registrado
  {
    return false;
  }
}
