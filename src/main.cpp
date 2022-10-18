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
#define SENSOR_COPO 1
#define BUTTON_CONTINUE 12
#define BUTTON_LEITE 13

// Outputs
#define BOMBA_AGUA 33
#define LED_LEITE 15

/*=============== Funções =================*/
void PreparaShake();
void ApagaPedido();
void AddPedido(int sup);

/*=============== Variáveis Globais =================*/
float preco_total = 0; // Valor total do pedido
float preco_sup[4] = {1.5, 1.2, 1, 2}; // Preço de cada dose de suplemento
int doses = 0; // Números de doses de suplemento do pedido
int sup_id = NULL;

const int button_sup[4] = {25, 26, 27, 14}; // Portas dos botões de seleção de suplementos
int button_sup_state[4] = {0, 0, 0, 0}; // Armazenar estado dos botões de seleção de suplementos
const int led_sup_state[4] = {17, 16, 4, 2}; // Portas dos leds que indicam se o botão foi pressionado
const int motor[5] = {36, 39, 34, 35, 32}; // Portas de saida dos relês de ativição dos motores

int continue_pressed = 0; // controle do botão de continue

void setup()
{
  /*=============== Display LCD =================*/
  lcd.begin();
  lcd.backlight();
  lcd.print("Bem Vindo");  
  lcd.setCursor(0, 1);
  lcd.print("Press Continue...");

  /*=============== Inputs/Outputs =================*/
  pinMode(SENSOR_COPO, INPUT);
  pinMode(BUTTON_CONTINUE, INPUT_PULLDOWN);
  pinMode(BUTTON_LEITE, INPUT_PULLDOWN);
  
  for (int i = 0; i<4; i++){
    pinMode(button_sup[i], INPUT_PULLDOWN);
    pinMode(led_sup_state[i], OUTPUT);
  }

  for (int i = 0; i<5; i++)
    pinMode(motor[i], OUTPUT);  
  
  pinMode(BOMBA_AGUA, OUTPUT);
  pinMode(LED_LEITE, OUTPUT);
  
}

void loop()
{
  /*=============== estado Machine =================*/
  switch (estado)
  {
  case ESTADO_INICIO:
    // while (digitalRead(BUTTON_CONTINUE) == LOW)
    if(1)
    {
      lcd.setCursor(0, 1);
      lcd.print("Press Continue...");
      delay(2000);
    }
    lcd.clear();
    estado = ESTADO_SUPLEMENTO;
    break;

  case ESTADO_SUPLEMENTO:

    while (continue_pressed == 0 || preco_total == 0) // só avança de estado se algum produto foi selecionado e 'Continuar' foi pressionado
    {
      lcd.setCursor(0, 0);
      lcd.print("Preco: ");
      lcd.setCursor(0, 1);
      lcd.print("Sup:   Ds: ");
      

      if (continue_pressed == 0) // Verifica se Continue foi pressionado e armazena na variável de controle
        continue_pressed = digitalRead(BUTTON_CONTINUE);

      for (int i = 0; i < 4; i++) // loop que monitora os 4 botões de suplemento
      {
        if(digitalRead(button_sup[i])==HIGH){
          AddPedido(i);

          lcd.setCursor(7,0);
          lcd.print(preco_total);
          lcd.setCursor(5,1);
          lcd.print(sup_id);
          lcd.setCursor(11,1);
          lcd.print(doses);
          delay(400);
        }
      }
    }
    continue_pressed = 0; // reset var de controle continue
    estado = ESTADO_BASE; // Avança de estado
    break;

  case ESTADO_BASE:
    if (1)
    {
      estado = ESTADO_PAGAMENTO;
    }
    break;

  case ESTADO_PAGAMENTO:
    if (1)
    {
      estado = ESTADO_PREPARO;
    }
    break;

  case ESTADO_PREPARO:
    if (digitalRead(SENSOR_COPO) == HIGH) // Prepara o Shake somente se identificar o copo
    {
      
    }
    break;
  }
}

void ApagaPedido(){ // Apaga o pedido
  for (int i = 0; i<4; i++)
    digitalWrite(led_sup_state[i], LOW);
    preco_total = 0;
    doses = 0;
    sup_id = NULL;
}

void AddPedido(int i) // Adicionar o suplemento selecionado
{
  if (digitalRead(led_sup_state[i]) == LOW)
  { // Se for um suplemento não selecionado anteriormente, o pedido é apagado (reset)
    // e inicia-se um novo pedido com o suplemento em questão
    ApagaPedido();
    digitalWrite(led_sup_state[i], HIGH);
    preco_total = preco_sup[i];
    doses += 1;
    sup_id = i;
  }
  else if(doses<5){ // Se o suplemento ja foi selecionado durante o pedido, apenas adiciona-se uma dose a mais (limite 5 doses)
    preco_total += preco_sup[i];
    doses += 1;
  }
  else // Caso o limite de 5 doses seja ultrapassado, apaga o pedido
    ApagaPedido();
}

void PreparaShake()
{
  if (digitalRead(led_sup_state[0]) == 1)
  {
    motor[0] == HIGH;
  }

  else if (digitalRead(led_sup_state[1]) == 1)
  {
    motor[1] == HIGH;
  }

  else if (digitalRead(led_sup_state[2]) == 1)
  {
    motor[2] == HIGH;
  }

  else if (digitalRead(led_sup_state[3]) == 1)
  {
    motor[3] == HIGH;
  }

  if (digitalRead(LED_LEITE) == HIGH)
  {
    motor[4] == HIGH;
  }
}


