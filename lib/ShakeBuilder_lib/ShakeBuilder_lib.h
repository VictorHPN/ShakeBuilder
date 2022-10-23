#ifndef SHAKEBUILDER_LIB
#define SHAKEBUILDER_LIB

/*=============== Inputs/Outputs =================*/
// Inputs:
//Sensor do copo -> PREPARO
#define SENSOR_COPO 36 // VP
//Botões -> OPERAÇÃO
#define BUTTON_CONTINUE 39 // VN
#define BUTTON_SUP1 34 // D34
#define BUTTON_SUP2 35 // D35
#define BUTTON_SUP3 32 // D32
#define BUTTON_LEITE 33 // D33

// Outputs
//LEDs -> INDICADORES
#define LED_SUP1 17 // TX2
#define LED_SUP2 16 // RX2
#define LED_SUP3 4  // D4
#define LED_LEITE 2 // D2

//Motores e Bomba da Água -> PREPARO
#define MOTOR1 25 // D25
#define MOTOR2 26 // D26
#define MOTOR3 27 // D27
#define MOTOR_LEITE 14 // D14
#define BOMBA_AGUA 12 // D12

// Portas dos botões de seleção de suplementos e leite:
const int button_state[4] = { 
    BUTTON_SUP1, 
    BUTTON_SUP2, 
    BUTTON_SUP3,  
    BUTTON_LEITE
    };

// Portas dos leds que indicam se o botão foi pressionado:
const int led_state[4] = { 
    LED_SUP1,
    LED_SUP2,
    LED_SUP3,
    LED_LEITE
    };

// Portas de saida dos relês de ativição dos motores:
const int motor[5] = { 
    MOTOR1,
    MOTOR2,
    MOTOR3,
    MOTOR_LEITE
    };

class Shake{
    private:
        float preco_total; // Valor total do pedido
        int doses; // Números de doses de suplemento do pedido
        int id_suplemento; // identificador do suplemento
        bool leite_st; // leite selecionado ou não

    public:
        //Contrutor:
        Shake();
        //set and gets:
        float get_preco_total();
        int get_doses();
        int get_id_suplemento();
        bool get_leite_st();
        void set_preco_total(float preco_t);
        void set_doses(int d);
        void set_id_suplemento(int id);
        void set_leite_st(bool state);
        //Demais métodos:
        void Prepara_Shake();
        void Apaga_Sups_Escolhidos();
        void Adiciona_ao_Pedido(int sup);
        void Limpa_Objeto();
};

#endif //enfif SHAKEBUILDER_LIB