
//+================= PROJETO INTEGRADOR - MÁQUINA DE VENDAS =================+
//|ALUNOS: JOÃO VICTOR ZACHÊO, JULIA ALVES DOS SANTOS E LAURA DALFOVO        |
//|TURMA: TÉCNICO INTEGRADO EM ELETROELETRÔNICA, MÓDULO 8                    |
//|ORIENTADOR: NEURY BOARETTO                                                |
//|DATA: 21/06/2023                                                          |
//+==========================================================================+

//============== DEFINIÇÃO DE BIBLIOTECAS, VARIÁVEIS E PINAGENS ==============
// INCLUSÃO DE BIBLIOTECAS
#include <Keypad.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Password.h>
#include <Stepper.h>

int verifica1; //VERIFICA EXECUÇÃO DE ETAPA DO PROGRAMA
int verifica2; //VERIFICA EXECUÇÃO DE ETAPA DO PROGRAMA

Password senha = Password("1234"); //DEFINE A SENHA DA MÁQUINA

#define SS_PIN 53 //PINO SDA
#define RST_PIN 2 //PINO DE RESET

MFRC522 rfid(SS_PIN, RST_PIN); //PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS RFID

LiquidCrystal lcd(46, 44, 42, 40, 38, 36); //PINAGEM DISPLAY LCD

const int pinoLedVerde = 3; //PINO REFERENTE AO LED VERDE
const int pinoLedVermelho = 4; //PINO REFERENTE AO LED VERMELHO

char tecla_pressionada; //VARIÁVEL QUE GUARDA TECLA PRESSIONADA NO TECLADO

const int stepsPerRevolution = 64; //NÚMERO DE PASSOS POR VOLTA
Stepper motor1(stepsPerRevolution, 43,47,45,49); //INICIALIZA O MOTOR 1
Stepper motor2(stepsPerRevolution, 41,37,39,35); //INICIALIZA O MOTOR 2
Stepper motor3(stepsPerRevolution, 27,31,29,33); //INICIALIZA O MOTOR 3
Stepper motor4(stepsPerRevolution, 8,10,9,11); //INICIALIZA O MOTOR 4

//PROPRIEDADES DA MÁQUINA
int qtdProdutos = 4;
char gavetas[4] = {'1', '2', '3', '4'};

//CÓDIGO DAS TAG ACEITAS
String tags[4] = {"37:63:6A:B5", "F3:88:68:1A", "F3:F1:6F:1A", "33:E5:69:1A"};

const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 3; //QUANTIDADE DE COLUNAS DO TECLADO

//CONSTRUÇÃO DA MATRIZ DE CARACTERES DO TECLADO
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte PinosqtdLinhas[qtdLinhas] = {22, 24, 26, 28}; //PINOS UTILIZADOS PELAS LINHAS DO TECLADO
byte PinosqtdColunas[qtdColunas] = {30, 32, 34}; //PINOS UTILIZADOS PELAS COLUNAS DO TECLADO

//INICIALIZAÇÃO DO TECLADO
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas);


//================= INÍCIO DO PROGRAMA ==================
//-------------- INICIALIZAÇÃO DO PROGRAMA --------------
void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  
  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522
  
  meuteclado.setDebounceTime(5); //DEFINE TEMPO DE REGISTRO DO TECLADO
  
  motor1.setSpeed(400); //VELOCIDADE DO MOTOR 1
  motor2.setSpeed(400); //VELOCIDADE DO MOTOR 2
  motor3.setSpeed(400); //VELOCIDADE DO MOTOR 3
  motor4.setSpeed(400); //VELOCIDADE DO MOTOR 4
  
  pinMode(pinoLedVerde, OUTPUT); //DEFINE O LED COMO SAÍDA
  pinMode(pinoLedVermelho, OUTPUT); //DEFINE O LED COMO SAÍDA

  digitalWrite(pinoLedVerde, LOW); //LED INICIA DESLIGADO
  digitalWrite(pinoLedVermelho, LOW); //LED INICIA DESLIGADO

  //INICIALIZAÇÃO DA INTERFACE
  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.print("Maquina de");
  lcd.setCursor(5, 1);
  lcd.print("vendas");
  Serial.println();
  Serial.println("======= MÁQUINA DE VENDAS =======");
  Serial.println();
  delay(2000);
}//-------- FIM DA INICIALIZAÇÃO DO PROGRAMA ----------

//----------- INÍCIO DO LOOP DO PROGRAMA --------------
void loop(){
  verifica1 = 0; //DEFINE STATUS DA VARIÁVEL
  verifica2 = 0; //DEFINE STATUS DA VARIÁVEL

  //MENSAGEM INICIAL
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Insira o");
  lcd.setCursor(5, 1);
  lcd.print("cartao");
  Serial.println("Insira o cartão");
  
  do{
    if(meuteclado.getKeys()){
      if(meuteclado.isPressed('*')){ //SE A TECLA * FOR PRESSIONADA
        leituraAbastecimento();      //CHAMA A FUNÇÃO RESPONSÁVEL PELO ABASTECIMENTO
      }
    }

    verifica1 = leituraRfid(); //CHAMA A FUNÇÃO RESPONSÁVEL PELA VALIDAÇÃO DA TAG RFID (0 - NÃO LIDO, 1 - APROVADO, 2 - NEGADO)
  }while(verifica1 == 0);      //ENQUANTO CARTÃO NÃO FOR APROVADO

  if (verifica1 == 1){ //SE CARTÃO FOR APROVADO

    //MENSAGEM PARA INSERÇÃO DE SENHA
    lcd.clear();
    lcd.print("Insira a senha:");
    lcd.setCursor(0, 1);
    lcd.print("*-Reset|#-Enter");
    Serial.println("");
    Serial.println("Insira a senha:  Pressione # para confirmar ou * para apagar");
    
    while (verifica2 == 0) {
      verifica2 = leituraSenha(); //CHAMA A FUNÇÃO RESPONSÁVEL PELA VALIDAÇÃO DE SENHA (0 - NÃO LIDO, 1 - APROVADO, 2 - NEGADO)
    }
  }

  if (verifica1 == 1 && verifica2 == 1) { //SE O CARTÃO E A SENHA FOREM APROVADOS
    
    digitalWrite(pinoLedVerde, HIGH); //LIGA O LED VERDE

    //MENSAGEM DE AUTORIZAÇÃO
    Serial.println("Acesso Autorizado");
    Serial.println();
    lcd.clear();
    lcd.print("Ola funcionario!");
    Serial.println("Olá Funcionário!");
    Serial.println();
    delay(2000);

    do{
      //MESAGEM DE SELEÇÃO DE PRODUTO
      lcd.clear();
      lcd.print("Selecione o item");
      lcd.setCursor(0, 1);
      lcd.print("#/* - Cancelar");
      Serial.println("Selecione o item desejado");
      Serial.println("Pressione # ou * para cancelar");
  
      tecla_pressionada = meuteclado.waitForKey(); //ESPERA TECLA SER PRESSIONADA
      
      if(tecla_pressionada == '#' || tecla_pressionada == '*'){ //SE # OU * FOR PRESSIONADO
        //MENSAGEM DE CANCELAMENTO DE OPERAÇÃO
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Operacao");
        lcd.setCursor(3, 1);
        lcd.print("cancelada");
        Serial.println("Operação Cancelada");
        delay(1500);
      }else{ //SE OUTRA TECLA FOR PRESSIONADA
       //MENSAGEM MOSTRANDO ITEM SELECIONADO
       Serial.print("Item : ");
       Serial.println(tecla_pressionada);
       lcd.clear();
       lcd.print("Item: ");
       lcd.print(tecla_pressionada);
       delay(1500);
        
       if (verificaProduto(tecla_pressionada) == false) { //SE O PRODUTO SELECIONADO NÃO EXISTIR
         //MENSAGEM DE PRODUTO INVÁLIDO
         lcd.clear();
         lcd.print("Produto Invalido");
         Serial.println();
         Serial.println("Produto Inválido");
         delay(1500); 
         Serial.println();
        
       }else { //SE O PRODUTO SELECIONADO EXISTER
         //MENSAGEM DE DISPENSA DE PRODUTO
         delay(2000);
         lcd.clear();
         lcd.setCursor(1, 0);
         lcd.print("Em andamento...");
         Serial.println();
         Serial.println("Em andamento...");
        
         dispensaProduto(tecla_pressionada); //CHAMA A FUNÇÃO RESPONSÁVEL PELA DISPENSA DE PRODUTOS
         delay(1500);

         //MENSAGEM FINAL
         lcd.clear();
         lcd.setCursor(3, 0);
         lcd.print("Aproveite!");
         Serial.println();
         Serial.println("Aproveite!");
         Serial.println();
        }
     }
    }while(tecla_pressionada != '*' && //ENQUANTO A OPERAÇÃO NÃO É CANCELADA
           tecla_pressionada != '#' &&
           verificaProduto(tecla_pressionada) == false); //OU PRODUTO É INVÁLIDO
    
    delay(3000);
    digitalWrite(pinoLedVerde, LOW); //DESLIGA O LED VERDE
    
  }else { //SE CARTÃO OU SENHA FOREM NEGADOS
    delay(1000);
    digitalWrite(pinoLedVermelho, HIGH); //LIGA O LED VERMELHO
    
    //MENSAGEM DE ACESSO NEGADO
    Serial.println("Acesso Negado");
    Serial.println();
    lcd.clear();
    lcd.print(" Acesso Negado!");
    delay(1500);
    
    digitalWrite(pinoLedVermelho, LOW); //DESLIGA O LED VERDE
    lcd.clear();
  }
}//-------------- FIM DO LOOP DO PROGRAMA --------------


//============== FUNÇÕES DO PROGRAMA ==============

/***1. FUNÇÃO DE VALIDAÇÃO DE PRODUTO***/
bool verificaProduto(char codProduto) {
  for (int i = 0; i < qtdProdutos; i++) {
    if (codProduto == gavetas[i]) {
      return true;
    }
  }
  return false;
}
/***------------------------------------------***/

/***2. FUNÇÃO DE VALIDAÇÃO DE INSERÇÃO DE TAG***/
bool verificaTag(String codTag) {
  for (int i = 0; i < qtdProdutos; i++) {
    if (codTag == tags[i]) {
      return true;
    }
  }
  return false;
}
/***------------------------------------------***/

/***3. FUNÇÃO DE VALIDAÇÃO DA TAG RFID***/
int leituraRfid() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) //SE O CARTÃO PRESENTE NO LEITOR NÃO FOR DIFERENTE DO ÚLTIMO CARTÃO LIDO
    return 0; //RETORNA PARA LER NOVAMENTE

  //INICIO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }
  strID.toUpperCase();
  //FIM DO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA

  //TAGS ACEITAS = 37:63:6A:B5 / F3:88:68:1A / F3:F1:6F:1A / 33:E5:69:1A
  //TAGS NEGADAS = BA:74:7D:7F / 13:0F:66:1A
  if (verificaTag(strID) == true) { //SE O CARTÃO FOR APROVADO
    //MENSAGEM DE CARTÃO APROVADO
    delay(1000);
    Serial.println("Cartão Válido!");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Cartao Valido!");
    delay(2000);

    lcd.clear();
    Serial.println("Retire o cartão");
    lcd.setCursor(4, 0);
    lcd.print("Retire o");
    lcd.setCursor(5, 1);
    lcd.print("cartao");
    delay(2500);
    return 1;

  } else { //SE O CARTÃO FOR NEGADO
    delay(1000);
    return 2;
  }

  rfid.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
  rfid.PCD_StopCrypto1(); //PARADA DA CRIPTOGRAFIA NO PCD
}
/***------------------------------------------***/

/***4. FUNÇÃO DE LEITURA DE TECLAS PRESSIONADAS PARA A SENHA***/
int leituraSenha() {
  char key = meuteclado.getKey();
  String passcode = ""; //DEFINE STATUS INICIAL DA VARIÁVEL
  
  do {
    key = meuteclado.waitForKey(); //ESPERA TECLA SER PRESSIONADA
    Serial.print(key);

    if (key != '*' && key != '#'){ //SE A TECLA PRESSIONADA FOR DIFERENTE DE # E *
      //INDICA, NA INTERFACE, QUE SENHA ESTÁ SENDO DIGITADA
      passcode = passcode + '*';
      senha.append(key);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(passcode);
    }

    if(key == '*'){ //SE A TECLA PRESSIONADA FOR *
      //RESETA A SENHA NA INTERFACE
      passcode = "";
      senha.reset();
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("*-Reset|#-Enter");
    }

    if (key == '#') { //SE A TECLA PRESSIONADA FOR #
      lcd.clear();
      
      if (verificasenha() == true){ //ENVIA SENHA PARA VERIFICAÇÃO
        passcode = ""; //LIMPA DADO DA VARIÁVEL
        return 1; //SENHA APROVADA
        
      } else {
        passcode = ""; //LIMPA DADO DA VARIÁVEL
        return 2; //SENHA NEGADA
      }
    }
  } while (key != '#'); //ENQUANTO SENHA NÃO FOI ENVIADA
}
/***------------------------------------------***/

/***5. FUNÇÃO DE VERIFICAÇÃO DE SENHA***/
bool verificasenha(){
  //MENSAGEM DE VERIFICAÇÃO
  lcd.clear();
  Serial.println("");
  Serial.print("Verificando, aguarde... ");
  lcd.setCursor(2, 0);
  lcd.print("Verificando");
  lcd.setCursor(3, 1);
  lcd.print("Aguarde...");
  delay(2000);
  
  if (senha.evaluate()){ //SE A SENHA FOR APROVADA
    senha.reset();
    
    //MENSAGEM DE SENHA APROVADA
    lcd.clear();
    lcd.setCursor(1, 0);
    Serial.println("Senha correta!");
    lcd.print("Senha correta!");
    delay(2000);
    return true;
    
  }else{ //SE A SENHA FOR NEGADA
    senha.reset();
    
    //MENSAGEM DE SENHA NEGADA
    lcd.clear();
    lcd.setCursor(1, 0);
    Serial.println("Senha Invalida!");
    lcd.print("Senha Invalida!");
    delay(2000);
    return false;
  }
}
/***------------------------------------------***/

/***6. FUNÇÃO DE DISPENSA DE PRODUTO***/
void dispensaProduto(char codProduto){
  switch(codProduto){ //VERIFICA CÓDIGO DIGITADO
    case '1': for(int i = 0; i < 32; i++){
                motor1.step(-stepsPerRevolution); //GIRA O MOTOR 1 NO SENTIDO ANTI-HORÁRIO
              }
              desligaMotor(43,47,45,49);
              break;
    case '2': for(int i = 0; i < 32; i++){
                motor2.step(-stepsPerRevolution); //GIRA O MOTOR 2 NO SENTIDO ANTI-HORÁRIO
              }
              desligaMotor(41,37,39,35);
              break;
    case '3': for(int i = 0; i < 32; i++){
                motor3.step(-stepsPerRevolution); //GIRA O MOTOR 2 NO SENTIDO ANTI-HORÁRIO
              }
              desligaMotor(27,31,29,33);
              break;
    case '4': for(int i = 0; i < 32; i++){
                motor4.step(-stepsPerRevolution); //GIRA O MOTOR 2 NO SENTIDO ANTI-HORÁRIO
              }
              desligaMotor(8,10,9,11);
              break;
    default:  break;
  }
}
/***------------------------------------------***/

/***7. FUNÇÃO DE ABASTECIMENTO DA MÁQUINA***/
void leituraAbastecimento(){
  char key; //VARIÁVEL DE LEITURA DE TECLA
  char key2; //VARIÁVEL DE LEITURA DE TECLA
  
  //MENSAGEM DE ENTRADA NO MODO DE ABASTECIMENTO
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Modo");
  lcd.setCursor(1, 1);
  lcd.print("Abastecimento");
  Serial.println("Modo Abastecimento");
  delay(1500);

  //MENSAGEM DE SELEÇÃO DE CABINE
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Selecione a");
  lcd.setCursor(4, 1);
  lcd.print("cabine");
  Serial.println("Selecione a cabine");
  
  do {
    key = meuteclado.waitForKey(); //ESPERA TECLA SER PRESSIONADA
    char motor_selecionado; //VARIÁVEL QUE GUARDA A CABINE SELECIONADA

    if (key != '*' && key != '#'){  //SE A TECLA PRESSIONADA FOR DIFERENTE DE # E *
      motor_selecionado = key;
      
      //MENSAGEM DE CABINE SELECIONADA
      lcd.clear();
      lcd.print("Cabine: ");
      lcd.print(motor_selecionado);
      lcd.setCursor(0, 1);
      lcd.print("*-Reset|#-Enter");
      Serial.print("Cabine: ");
      Serial.println(motor_selecionado);

      key2 = meuteclado.waitForKey();//ESPERA TECLA SER PRESSIONADA
      
      if(key2 == '*'){  //SE A TECLA PRESSIONADA FOR *
        motor_selecionado = ' '; //LIMPA DADO DA VARIÁVEL
        key2 = ' '; //LIMPA DADO DA VARIÁVEL

        //MENSAGEM DE SELEÇÃO DE CABINE
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Selecione a");
        lcd.setCursor(4, 1);
        lcd.print("cabine");
        Serial.println("Selecione a cabine");
        
      }else if (key2 == '#'){ //SE A TECLA PRESSIONADA FOR #
        //MENSAGEM DE ABASTECIMENTO
        lcd.clear();
        lcd.print("Abastecendo...");
        
        switch(key){ //VERIFICA A CABINE SELECIONADA
          case '1': for(int i = 0; i < 32; i++){
                      motor1.step(stepsPerRevolution); //GIRA O MOTOR NO SENTIDO HORÁRIO
                    }
                    desligaMotor(43,47,45,49);
                    break;
          case '2': for(int i = 0; i < 32; i++){
                      motor2.step(stepsPerRevolution); //GIRA O MOTOR NO SENTIDO HORÁRIO
                    }
                    desligaMotor(41,37,39,35);
                    break;
          case '3': for(int i = 0; i < 32; i++){
                      motor3.step(stepsPerRevolution); //GIRA O MOTOR NO SENTIDO HORÁRIO
                    }
                    desligaMotor(27,31,29,33);
                    break;
          case '4': for(int i = 0; i < 32; i++){
                      motor4.step(stepsPerRevolution); //GIRA O MOTOR NO SENTIDO HORÁRIO
                    }
                    desligaMotor(8,10,9,11);
                    break;
          default:  break;
        }
     }
     //MENSAGEM DE SELEÇÃO DE CABINE
     lcd.clear();
     lcd.setCursor(2, 0);
     lcd.print("Selecione a");
     lcd.setCursor(4, 1);
     lcd.print("cabine");
     Serial.println("Selecione a cabine");
     
    } else if(key == '#'){ //SE A TECLA PRESSIONADA FOR #
      //MENSAGEM DE SAÍDA DO MODO DE ABASTECIMENTO
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Abastecimento");
      lcd.setCursor(2, 1);
      lcd.print("Finalizado");
      delay(2000);
      
      //MENSAGEM INICIAL
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Insira o");
      lcd.setCursor(5, 1);
      lcd.print("cartao");
      Serial.println("Insira o cartão");
    }
  } while (key != '#'); //ENQUANTO # NÃO FOR PRESSIONADO
}
/***------------------------------------------***/

/***8. FUNÇÃO DE DESLIGAMENTO DE MOTOR***/
void desligaMotor(int motor_pin1, int motor_pin2, int motor_pin3, int motor_pin4){
  digitalWrite(motor_pin1, LOW); //DESLIGA PINO 1 DO MOTOR
  digitalWrite(motor_pin2, LOW); //DESLIGA PINO 2 DO MOTOR
  digitalWrite(motor_pin3, LOW); //DESLIGA PINO 3 DO MOTOR
  digitalWrite(motor_pin4, LOW); //DESLIGA PINO 4 DO MOTOR
}
/***------------------------------------------***/