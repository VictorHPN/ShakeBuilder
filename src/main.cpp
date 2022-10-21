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
  ESTADO_SUPLEMENTO,
  ESTADO_PAGAMENTO,
  ESTADO_PREPARO
} Estados;

static Estados estado = ESTADO_INICIO;

/*=================== Variáveis de controle ===================*/
bool continue_pressed = false; // controle do botão de continue

Shake shake; // Objeto do shake a ser preparado

/*======================= Configs LCD =========================*/
LiquidCrystal_I2C lcd(0x27, 20, 4); // Inicialização Display LCD
// Funções:
void Tela_incio();
void Tela_suplemento();
void Tela_suplemento_atualizada(float preco, int id_sup, int doses, bool leite);
void Tela_pagamento_aguardando();
void Tela_pagamento_confirmado();
void Tela_preparo_esperando_copo();
void Tela_preparo_servindo();
void Tela_fim();


void setup()
{
  /*===================== Inputs/Outputs ======================*/
  pinMode(SENSOR_COPO, INPUT);
  pinMode(BUTTON_CONTINUE, INPUT_PULLDOWN);
  for (int i = 0; i<5; i++)
  {
    pinMode(button_state[i], INPUT_PULLDOWN);
    pinMode(led_state[i], OUTPUT);
    pinMode(motor[i], OUTPUT);
  }
  pinMode(BOMBA_AGUA, OUTPUT);
  pinMode(LED_LEITE, OUTPUT);

  /*================ Inicialização Display LCD ================*/
  lcd.begin();
  lcd.backlight();
}

void loop()
{
  /*=============== estado Machine =================*/
  switch (estado)
  {
  case ESTADO_INICIO:
    while (digitalRead(BUTTON_CONTINUE) == LOW)
    {
      Tela_incio(); // Estado de repouso enquanto a compra não é iniciada
    }
    lcd.clear(); // Limpa display
    estado = ESTADO_SUPLEMENTO; // Avança para próximo estado
    break;

  case ESTADO_SUPLEMENTO:
    Tela_suplemento();
    while (continue_pressed == false || shake.get_preco_total() <= 0) // Enquanto
    {
      for (int i = 0; i < 5; i++) // loop que monitora os 4 botões de suplemento
      {
        if(digitalRead(button_state[i]) == HIGH) // Ao identificar o pressionamento de um botão...
        {
          shake.Adiciona_ao_Pedido(i);
          Tela_suplemento_atualizada(shake.get_preco_total(), shake.get_id_suplemento(),
                                    shake.get_doses(), shake.get_leite_st());
          delay(400);
        }
      }
      if (continue_pressed == false)
      { // Verifica se Continue foi pressionado e armazena na variável de controle
        continue_pressed = digitalRead(BUTTON_CONTINUE);
      }
    }
    lcd.clear(); // Limpa display
    continue_pressed = false; // reset var de controle continue
    estado = ESTADO_PAGAMENTO; // Avança para próximo estado
    break;

  case ESTADO_PAGAMENTO:
    if (1) // Avança para o prepario somente se identificar o pagamaento
    {
      
    }
    lcd.clear(); // Limpa display
    estado = ESTADO_PREPARO;  // Avança para próximo estado
    break;

  case ESTADO_PREPARO:
    while (digitalRead(SENSOR_COPO) == LOW) // Prepara o Shake somente se identificar o copo
    {
      Tela_preparo_esperando_copo();
    }
    lcd.clear(); // Limpa display
    Tela_preparo_servindo();
    shake.Prepara_Shake();
    Tela_fim();
    delay(5000);
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
  lcd.print("Continuar...");
}

void Tela_suplemento_atualizada(float preco, int id_sup, int doses, bool leite)
{
  //Atualiza preço
  lcd.setCursor(12, 0);
  lcd.print(preco);
  //Atualiza id do suplemento selecionado
  lcd.setCursor(11, 1);
  lcd.print(id_sup);
  //Atualiza o número de doses
  lcd.setCursor(19, 1);
  lcd.print(doses);
  //Atualiza se o quantidade de água
  lcd.setCursor(5, 2);
  lcd.print(100*doses);
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

void Tela_pagamento_aguardando()
{
  lcd.setCursor(0, 0);
  lcd.print("#==================#");
  lcd.setCursor(0, 1);
  lcd.print("Aguardando Pagamento");
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