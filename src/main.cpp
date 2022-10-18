#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inicialização Display LCD

/*=========== Máquina de estados ============*/
typedef enum
{
  ESTADO_INICIO = 0,
  ESTADO_SUPLEMENTO,
  ESTADO_BASE,
  ESTADO_PAGAMENTO,
  ESTADO_PREPARO
} Estados;

static Estados estado = ESTADO_INICIO; // Verificar se esta declarada no local certo

/*=============== Inputs/Outputs =================*/
// Inputs:
#define BUTTON_CONTINUE 25
#define BUTTON_SUP1 26
#define BUTTON_SUP2 27
#define BUTTON_SUP3 14
#define BUTTON_SUP4 12
#define BUTTON_LEITE 13
#define SENSOR_COPO 1

// Outputs - Relês:
#define MOTOR1 36
#define MOTOR2 39
#define MOTOR3 34
#define MOTOR4 35
#define MOTOR5 32
#define BOMBA_AGUA 33

// Outputs - LEDs:
#define LED_SUP1 17
#define LED_SUP2 16
#define LED_SUP3 4
#define LED_SUP4 2
#define LED_LEITE5 15

/*=============== Funções =================*/
void PreparaShake();

/*=============== Variáveis Globais =================*/
const int b[5] = {1, 2, 3, 4, 5};  // Buttons pins
const int motor[5] = {6, 7, 8, 9, 10}; // rele pins
int bs[5] = {0, 0, 0, 0, 0};           // Buttons Estados
int p[5] = {0, 0, 0, 0, 0};            // powders control

float preco_total = 0; // Valor total do pedido

void setup()
{
  /*=============== Display LCD =================*/
  lcd.begin();
  lcd.backlight();

  /*=============== Inputs/Outputs =================*/
  pinMode(BUTTON_CONTINUE, INPUT);
  pinMode(BUTTON_SUP1, INPUT);
  pinMode(BUTTON_SUP2, INPUT);
  pinMode(BUTTON_SUP3, INPUT);
  pinMode(BUTTON_SUP4, INPUT);
  pinMode(BUTTON_LEITE, INPUT);
  pinMode(SENSOR_COPO, INPUT);

  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(MOTOR3, OUTPUT);
  pinMode(MOTOR4, OUTPUT);
  pinMode(MOTOR5, OUTPUT);
  pinMode(BOMBA_AGUA, OUTPUT);

  pinMode(LED_SUP1, OUTPUT);
  pinMode(LED_SUP2, OUTPUT);
  pinMode(LED_SUP3, OUTPUT);
  pinMode(LED_SUP4, OUTPUT);
  pinMode(LED_LEITE5, OUTPUT);
}

void loop()
{
  /*=============== estado Machine =================*/
  switch (estado)
  {
  case ESTADO_INICIO:
    while (BUTTON_CONTINUE == LOW)
    {
      lcd.print("Bem Vindo");
      lcd.setCursor(0, 1);
      lcd.print("Pressione Continuar...");
      delay(100);
    }
    estado = ESTADO_SUPLEMENTO;
    break;

  case ESTADO_SUPLEMENTO:
    while (BUTTON_CONTINUE == LOW)
    {
    }
    break;

  case ESTADO_BASE:
    if (1)
    {
    }
    break;

  case ESTADO_PAGAMENTO:
    if (1)
    {
    }
    break;

  case ESTADO_PREPARO:
    if (1)
    {
    }
    break;
  }
}

void PreparaShake()
{
  if (p[1] == 1)
  {
    motor[0] == HIGH;
  }

  else if (p[2] == 1)
  {
    motor[1] == HIGH;
  }

  else if (p[3] == 1)
  {
    motor[2] == HIGH;
  }

  else if (p[4] == 1)
  {
    motor[3] == HIGH;
  }

  else if (p[5] == 1)
  {
    motor[4] == HIGH;
  }
}
