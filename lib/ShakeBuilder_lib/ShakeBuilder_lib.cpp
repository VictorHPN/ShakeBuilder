#include "ShakeBuilder_lib.h"
#include <Arduino.h>
#include <Wire.h>

/*=============== Variáveis Globais =================*/
// Preço de cada dose de suplemento:
float preco_suplemento[4] = {1, 2, 3, 4};
float preco_leite = 10;
int tempo_dose = 4000; // Tempo que cada motor gira para servir cada dose de suplemento
int tempo_dose_leite = 5000; // Tempo que cada motor gira para servir uma dose de leite
int tempo_dose_agua = 3000; // Tempo que cada motor gira para servir a quantidade de água referente a uma dose de suplemento

Shake::Shake()
{
    preco_total = 0;
    doses = 0;
    id_suplemento = -1; //'Vazio' -> nenhum seleciondado
    leite_st = false;
}

// set and gets:
float Shake::get_preco_total()
{

    return this->preco_total;
}

int Shake::get_doses()
{
    return this->doses;
}

int Shake::get_id_suplemento()
{
    return this->id_suplemento;
}

bool Shake::get_leite_st()
{
    return this->leite_st;
}

void Shake::set_preco_total(float preco_t)
{
    this->preco_total = preco_t;
}

void Shake::set_doses(int d)
{
    this->doses = d;
}

void Shake::set_id_suplemento(int id)
{
    this->id_suplemento = id;
}

void Shake::set_leite_st(bool state)
{
    this->leite_st = state;
}

void Shake::Atualiza_Preco()
{ // Apaga o pedido
    this->preco_total = (preco_suplemento[id_suplemento]) * (this->doses);
    if (this->leite_st == true)
    { // Se o pedido conter LEITE, adicion-se o preço do leite ao total
        this->preco_total += preco_leite;
    }
}

void Shake::Apaga_Pedido()
{
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(led_state[i], LOW);
    }
    this->preco_total = 0;
    this->doses = 0;
    this->id_suplemento = -1;
}

void Shake::Adiciona_ao_Pedido(int id) // Adicionar o suplemento selecionado -> id = id do botão pressionado
{
    // Coleta se o produto selecionado já foi selecionado ou não, e armazena
    int est_produto_selecionado = digitalRead(led_state[id]);
    if (id == 4) // Se o botão do Leite foi selecionado...
    {
        if (this->leite_st == false) // Se leite ja estiver ativo -> desativa
        {
            this->leite_st = true;
            digitalWrite(led_state[id], HIGH);
        }
        else // Se leite ja estiver desativado -> ativa
        {
            this->leite_st = false;
            digitalWrite(led_state[id], LOW);
        }
    }
    else if (est_produto_selecionado == LOW)
    { // Se for um suplemento não selecionado anteriormente, o pedido é apagado (reset)
      // e inicia-se um novo pedido com o suplemento em questão
        Apaga_Pedido();
        digitalWrite(led_state[id], HIGH);
        this->doses += 1;
        this->id_suplemento = id;
    }
    else if (doses < 5)
    { // Se o suplemento ja foi selecionado durante o pedido, apenas adiciona-se 
      // uma dose a mais (limite 5 doses)
        this->doses += 1;
    }
    else
    { // Caso o limite de 5 doses seja ultrapassado, apaga o pedido
        Apaga_Pedido();
        return; // Termina o método já que o pedido foi resetado;
    }
    Atualiza_Preco(); // Atualiza o preco_total do pedido ao fim da adição
    return;
}

void Shake::Prepara_Shake()
{
    for (int i = 0; i<4; i++) 
    {
        if (digitalRead(led_state[0]) == HIGH)
        {
            int tempo_operacao = (this->doses)*tempo_dose; // Calcula o tempo 
            digitalWrite(motor[i], HIGH); // Ativa motor do suplemento escolhido
            delay(tempo_operacao);        // até servir todas as doses,
            digitalWrite(motor[i], LOW);  // e então desliga o motor,
            break; // e continua para as próxima etapa do preparo
        }
    }
    if (this->leite_st == true) // Verifica se deve adicionar leite
    {
        digitalWrite(motor[4], HIGH); // Ativa motor do leite
        delay(tempo_dose_leite);            // até servir uma dose,
        digitalWrite(motor[4], LOW);  // e então desliga o motor,
    }
    // Servindo a água:
    int tempo_operacao = (this->doses)*tempo_dose_agua; // Calcula o tempo
    digitalWrite(BOMBA_AGUA, HIGH); // Ativa a bomba de água
    delay(tempo_operacao);          // até servir todas as doses,
    digitalWrite(BOMBA_AGUA, LOW);  // e então desliga o motor,
}