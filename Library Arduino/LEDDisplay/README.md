Este projeto contém driver para Módulos LED 8x8 controlados via MAX722xx. Ele permite que você crie uma exibição de tamanho personalizado que é limitada apenas pelo próprio hardware. Os tamanhos vertical e horizontal podem conter até 256 módulos, mas antes de atingir esse limite você ficaria sem linhas Slave Select para controlar os chips MAX, ou ficaria limitado pela quantidade de RAM. O fato é: você pode controlar uma quantidade razoável de chips MAX e criar uma exibição de tamanho personalizado;)

Eu testei toda a ideia em um display que consiste em 8 módulos de LED na horizontal e 3 na posição vertical. Isso nos dá 24 módulos que contêm 1536 LEDs (8 * 8 * 3 * 8).

# Hardware
Os esquemas Fritzing estão aqui: [/doc/fritzing] (doc/fritzing)

Primeiro vamos começar com o controlador, na verdade qualquer Arduino funcionará, usei o Mega devido ao grande número de pinos de saída digital. Você também pode usar o Nano com o registrador de deslocamento e alterar a forma de endereçar as linhas Select Slave em * Display :: send (...) *.

Você precisará de uma fonte de alimentação extra para acionar os LEDs - assumindo que usará mais de uma matriz de LED.

## Módulo de matriz de LED único de condução
Eu usei o MAX7219 e o 788BS LED Matrix, este é o único com ânodo comum. O esquema abaixo ilustra a fiação dos LEDs, MAX e Arduino:

Este é equivalente, mas em vez de LEDs únicos, temos layout de PIN do Módulo de LED:

Pode acontecer, que o seu Módulo LED tenha cátodo comum, neste caso você tem que religar a conexão ao chip MAX. Você apenas tem que ter em mente que o MAX tem dois conjuntos de pinos, que são relevantes aqui: Dig0-Dig7 devem ser conectados aos cátodos (-) e SegA-SegG aos ânodos (+). Além disso, essa mudança irá trocar linhas com colunas dentro do módulo de LED de seno.

## Conectando todos os LEDs Matrix juntos
No capítulo anterior, vimos como conectar um módulo de LED único com o chip MAX. Agora, conectaremos vários módulos de LED em um grande display.
Abaixo está a tela física que usei para testes e exemplos. Cada Módulo de LED possui uma etiqueta indicando sua posição e a linha Selecionar Escravo.

Aqui está a fiação:

Cada conector de 3 pinos no esquema acima simboliza um módulo descrito no capítulo anterior (LED Matrix + MAX72xx), agora conectamos todos esses módulos juntos.

Todos os chips MAX722xx compartilham linhas MOSI e SCK comuns, MISO não é usado, cada chip ocupa uma linha Slave Select separada.

A posição da matriz de LED no esquema acima corresponde diretamente à sua localização no display físico que usei para teste. Além disso, cada módulo possui uma descrição indicando sua posição e linha Select Slave, por exemplo: * (2,1) SS: 35 * nos dá o segundo módulo na terceira linha (contando a partir do zero) e PIN: 35 no Arduino para a linha Select Slave.

#Programas
## Compilando
Estamos usando bibliotecas padrão do Arduino, então elas já estão disponíveis, a única exceção é [ArdLog]  Você deve importar este LIB para o seu IDE.

## Comunicação com MAX72xxx
Estamos usando a biblioteca SPI padrão e a linha Select Slave no chip MAX para endereçamento. MAX é configurado no modo LED Matrix - portanto, não há nada de especial. O método de configuração pode ser encontrado em: * Display :: setup () *

## Configurando as coisas
A principal classe de nosso interesse será o * Display * - é responsável pela configuração dos chips MAX e fornece API para pintura.

Antes de começarmos a pintar, é necessário configurar as coisas. O código abaixo cria um array 2D contendo linhas Select Slave e inicializa a exibição. O display em si consiste em 3 linhas, cada uma com 8 módulos de LED. Obviamente, você pode escolher qualquer tamanho responsável, mas vou ficar com este.

O layout da matriz 2D mencionada corresponde à exibição física: cada Módulo de LED tem um chip MAX dedicado e cada chip tem uma linha Select Slave dedicada. A primeira dimensão da nossa matriz indica a linha física no display, a segunda dimensão indica o Módulo LED dentro desta linha e o próprio valor contém o número PIN para a linha Selecionar Escravo.
