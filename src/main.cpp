#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //Inicialização Display LCD

/*=========== State Machine ============*/
typedef enum {
  STE_BEGIN = 0, STE_SUPPLEMENT, STE_BASE, STE_PAYMENT, STE_PREPARE
} States;

static States state = STE_BEGIN; //Verificar se esta declarada no local certo

//Inputs:
#define GPIO_YES;
#define GPIO_NO
#define GPIO_CONTINUE
#define GPIO_CANCEL
#define GPIO_SUP1
#define GPIO_SUP2
#define GPIO_SUP3
#define GPIO_SUP4
#define GPIO_SUP5
#define GPIO_LED1
#define GPIO_LED2
#define GPIO_LED3
#define GPIO_LED4

bool Leite;

/*=============== Global Variables =================*/
float valor_total_compra = 0; //Valor total do pedido
int Supplement_order{5}; //Vetor 

void setup()
{
  /*=============== Display LCD =================*/
  lcd.begin();
  lcd.backlight();
  lcd.print("ESP32 - Projeto");
  lcd.setCursor(0, 1);
  lcd.print("com display!");

  /*=============== Máquina de Estados =================*/
  // ...
  // ...
  // ...

  /*=============== Display LCD =================*/

  /*=============== Display LCD =================*/

  /*=============== Display LCD =================*/

  /*=============== Display LCD =================*/
}

void loop()
{
  /*=============== State Machine =================*/
  switch (state)
  {
    case STE_BEGIN:
      if ()
      {
        
      }
      break;

    case STE_SUPPLEMENT:
      if ()
      {
        
      }
      break;

    case STE_BASE:
      if ()
      {
        
      }
      break;

    case STE_PAYMENT:
      if ()
      {
        
      }
      break;

    case STE_PREPARE:
      if ()
      {
        
      }
      break;
  }
}