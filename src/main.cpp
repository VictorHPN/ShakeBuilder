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
#define GPIO_CONTINUE 1
#define GPIO_CANCEL 2
#define GPIO_SUP1 3
#define GPIO_SUP2 4
#define GPIO_SUP3 5
#define GPIO_SUP4 6
#define GPIO_MILK
#define GPIO_SENSOR

//Outputs:
#define GPIO_MOTOR1
#define GPIO_MOTOR2
#define GPIO_MOTOR3
#define GPIO_MOTOR4
#define GPIO_MOTOR5

/*=============== Global Variables =================*/
float Order_Price = 0; //Valor total do pedido
int Supplement_order{4}; //Vetor 

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
      while (!GPIO_CONTINUE)
      {
        //lcd print 'Bem Vindo'
      }
      state = STE_SUPPLEMENT;
      break;

    case STE_SUPPLEMENT:
      if (1)
      {
        
      }
      break;

    case STE_BASE:
      if (1)
      {
        
      }
      break;

    case STE_PAYMENT:
      if (1)
      {
        
      }
      break;

    case STE_PREPARE:
      if (1)
      {
        
      }
      break;
  }
}