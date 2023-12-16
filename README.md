# Maquina de Vendas (Vending Machine)
Projeto Integrador para conclusão do Instituto Federal de Santa Catarina ([IFSC](https://www.ifsc.edu.br/web/campus-joinville))

A máquina foi construída a partir de materiais reutilizados, utilizando [Arduíno](https://www.arduino.cc) para o desenvolvimento do sistema.

<br>

<container style="display:flex; justify-content:center;">

<img src="https://imgur.com/iuMfTDc.png" alt="Imagem do projeto" style="height:200px;">

<details open style="margin-left:30px">
<summary><b>Menu</b></summary><br>

- [⚡ Circuito](#⚡-circuito)
- [⚙️ Funcionalidades](#⚙️-funcionalidades)
    - [🛡️ Segurança](#🛡️-segurança)
    - [🍫 Seleção de produtos](#🍫-seleção-de-produtos)
    - [🔃 Abastecimento](#🔃-abastecimento)
- [✒️ Autores](#✒️-autores)

</details>
</container>

<br>

## ⚡ Circuito

![circuito](https://imgur.com/odHPtoe.png)

**Componentes:**
|   |   |
|---|---|
| Arduino Mega | Teclado Matricial 3x4 |
| Leitor RFID | Display LCD 16x2 |
| Cartão de Proximidade RFID 13,56 kHz | LED 5mm difuso vermelho |
| Motores de Passo 28BYJ-48 | LED 5mm difuso verde |
| Drivers ULN2003 | Fonte de alimentação 9v |
| Resistores 330 ohms | Potenciômetro 10k |

## ⚙️ Funcionalidades

O projeto foi pensado para um ambiente empresarial, fazendo com que a máquina sirva de benefício aos funcionários. Por este motivo, ela conta com algumas funcionalidades voltadas para este contexto, além de 4 cabides para a disponibilização dos produtos.

### 🛡️ Segurança

A máquina possui um sistema de segurança onde o usuário deve inserir um cartão no leitor. Se o cartão for cadastrado, o sistema pede a senha de segurança para obter acesso a máquina. Caso o cartão não seja cadastrado ou a senha digitada for incorreta, o acesso é negado.

### 🍫 Seleção de produtos

Ao obter acesso a máquina, o usuário poderá selecionar o número da cabine onde se encontra o produto que deseja. Caso selecione uma cabine inexistente, o sistema pede para que digite outro número. O usuário também tem a opção de cancelar a opereção nesta função.

### 🔃 Abastecimento

Para facilitar o abastecimento de produtos, o sistema conta com uma função de abastecimento. Estando na tela inicial, ao pressionar ' * ' a função é ativada. Sendo assim, ao selecionar a cabine desejada, o sistema fará com que os produtos nesta cabine voltem para trás, liberando o primeiro espaço para o inserção de um novo produto. Ao finalizar, a tecla ' # ' deve ser pressionada para o encerramento da função.

## ✒️ Autores
| [<img src="https://avatars.githubusercontent.com/u/95360206?v=4" width=120 height=120 style="border-radius:50%"><br><sub>João Zachêo</sub>](https://github.com/joaozacheo) |  
| :---: |