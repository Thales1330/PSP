---
id: fault
title: Curto-Circuito
sidebar_label: Curto-Circuito
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

O principal objetivo da análise de curto-circuito é o cálculo das correntes e tensões de falta para especificação de disjuntores, transformadores de corrente e a parametrização de relés de proteção. De 70 a 80% das faltas em linhas de transmissão são entre uma fase e terra, as quais ocorrem devido ao centelhamento de apenas uma fase da linha para a torre e então para a terra.O menor número de faltas, cerca de 5%, envolve todas as três fases, chamadas de faltas trifásicas. Os outros tipos de faltas envolvem duas fases e duas fases e a terra.

Todas essas falhas, exceto a trifásica, são assimétricas e causam desequilíbrio entre as fases.

## Cálculo de Curto-Circuito no PSP-UFU
O primeiro estágio do cálculo de curto-circuito é a determinação das tensões pré-falta, das potências de geração e cargas do sistema. Esses dados são obtidos por meio do estudo de [fluxo de carga](powerFlow).

:::info Informação
Atualmente, o PSP-UFU fornece resultado para os seguintes tipos de falta:
- Falta Trifásica (3F-T);
- Falta Fase-Terra (F-T);
- Falta Fase-Fase (F-F);
- Falta Fase-Fase-Terra (F-F-T).
:::

Os modelos dos elementos elétricos que constituem um sistema de potência para o estudo de curto-circuito são semelhantes aos do fluxo de carga, apresentando algumas divergências para as faltas desbalanceadas (F-T, F-F e F-F-T).

As faltas que ocorrem com maior frequência em sistemas de potência são assimétricas. Como qualquer falta assimétrica provoca fluxo de corrente desequilibrada é necessário empregar o método das componentes simétricas. Esse método permite o estudo de sistemas balanceados em conjunto cargas desbalanceadas.

## Execução do cálculo de curto-circuito no PSP-UFU
Existem duas formas de se calcular o curto-circuito no PSP-UFU:
- **Falta**: Calcula a falta inserida nas [barras](bus). Nesse tipo de cálculo é possível calcular faltas *shunt* nos [barramentos](bus) balanceadas e desbalanceadas.
- **Nível de curto-circuito**: Calcula o nível de curto-circuito (falta trifásica) em todos [barramentos](bus) do sistema.

Após a construção do diagrama unifilar no [editor de potência](powerEditor), a execução do cálculo de curto-circuito é realizada no [menu Simulação](mainScreen#menu-ribbon) clicando no botão **Falta**. Para calcular o nível de curto-circuito (falta trifásica) em todos [barramentos](bus) do sistema, basta clicar no botão **Nível de curto-circuito**.

<div><center><img src={useBaseUrl("images/menuSimulationFaulta.svg")} alt="Execução dos cálculos de curto-circuito" title="Execução dos cálculos de curto-circuito" /></center></div>

:::caution Atenção
É possível calcular as faltas sem a execução do [fluxo de carga](powerFlow), porém **não é recomendável**, visto que os valores das correntes de falta são significativamente alteradas.
:::

**Outra possibilidade** é a execução por meio do cálculo contínuo, também presente no [menu Simulação](mainScreen#menu-ribbon) e seu acionamento é realizado co clicar no botão **Habilitar solução**. Com essa opção, os cálculos estáticos selecionados nas [configurações de simulação](simulationConfig) são automaticamente realizados ao modificar quaisquer parâmetros da rede, como dados elétricos e acionamento dos disjuntores dos elementos (remoção ou inserção).

:::warning Cuidado!
Os cálculos de curtos-circuitos não são habilitados por padrão no cálculo contínuo e devem ser inseridos nas [configurações de simulação](simulationConfig).
:::

Os resultados do cálculo de curto-circuito são exibidos nos [elementos de texto vinculado](text), ao posicionar o mouse sobre os barramentos e em [relatórios tabulares](tabularReport).

### Erros comuns na execução do cálculo de curto-circuito
A seguir são apresentados os erros mais comuns relacionados ao calculo de curto-circuito.

#### A seguinte mensagem de erro é exibida: "Falha ao inverter a matriz admitância de sequência zero"
- **Impossibilidade de circulação da corrente de sequência zero**. Caso o gerador não seja aterrado, não circulará corrente de sequência zero por ele. Nesse caso, dependendo da conexão do transformador próximo ao gerador sem aterramento, a matriz admitância de sequência zero é singular. Para contornar esse problema escolha uma das duas soluções abaixo:
	- Marque a opção "Neutro aterrado" e insira um alto valor de reatância de aterramento ($j9999~p.u.$, por exemplo);
	- Ou, na barra do gerador, insira um reator de baixo valor de potência reativa ($1{,}0~var$, por exemplo).

## O cálculo de curto-circuito
Como já foi apresentado anteriormente, as faltas que ocorrem com maior frequência em sistemas de potência são assimétricas. Como qualquer falta assimétrica provoca fluxo de corrente desequilibrada é necessário empregar o método das componentes simétricas. Esse método permite o estudo de sistemas balanceados com cargas desbalanceadas.

### Método das componentes simétricas
Esse método proposto por C. L. Fortescue, permite definir um sistema de n fasores desbalanceados em n – 1 sistemas de n fases balanceados e um sistema de fase zero. O sistema de fase zero é definido por todas as fases de mesmo módulo e ângulo.
Para um sistema trifásico pode-se definir três componentes de sequência:
1.	Componentes de sequência positiva, constituindo em três fasores iguais em módulo, 120º defasados entre si, e tendo a mesma sequência de fase que os fasores originais;
2.	Componentes de sequência negativa, constituindo em três fasores iguais em módulo, 120º defasados entre si, e tendo a sequência de fase oposta à dos fasores originais.
3.	Componentes de sequência zero, constituindo em três fasores iguais em módulo e com defasagem nula entre si.

Com isso pode-se decompor as tensões de fase em componentes simétricas pelas seguintes equações:
$$
\begin{cases}
\dot{V}_a = \dot{V}_{a1} + \dot{V}_{a2} + \dot{V}_{a0}\\
\dot{V}_b = \dot{V}_{b1} + \dot{V}_{b2} + \dot{V}_{b0}\\
\dot{V}_c = \dot{V}_{c1} + \dot{V}_{c2} + \dot{V}_{c0}
\end{cases}
$$

A figura abaixo apresenta um exemplo de componentes simétricas e sua soma para obter os fasores desequilibrados.

<div><center><img src={useBaseUrl("images/fortescue.svg")} alt="Exemplo de componentes simétricas e sua soma para obter os fasores desequilibrados" title="Exemplo de componentes simétricas e sua soma para obter os fasores desequilibrados" /></center></div>

Para simplificar os cálculos adota-se um operador “$\overline{a}$”, com o intuito de indicar a rotação de um fasor. Tal operador é um número complexo de módulo unitário e ângulo de 120º:
$$
\overline{a} = 1 \angle 120^{\circ} = 1 e^{j2\pi/3} = -0{,}5 + j0{,}866
$$

Com isso pode-se utilizar as equações (de tensão apresentadas  em conjunto com o operador “$\overline{a}$” para construir a seguinte equação matricial:
$$
\begin{bmatrix}
\dot{V}_a\\
\dot{V}_b\\
\dot{V}_c
\end{bmatrix}
=
\overbrace{
\begin{bmatrix}
1 & 1 & 1\\
1 & \overline{a}^2 & \overline{a}\\
1 & \overline{a} & \overline{a}^2
\end{bmatrix}
}^{\left[ \bold{A} \right]}
\begin{bmatrix}
\dot{V}_{a0}\\
\dot{V}_{a1}\\
\dot{V}_{a2}
\end{bmatrix}
$$

Considerando a matriz quadrada da equação anterior sendo $\left[ \bold{A} \right]$, pode-se encontrar as componentes simétricas pré-multiplicando ambos os lados dessa mesma equação por $\left[ \bold{A} \right]^{-1}$.

Da mesma forma que no estudo de fluxo de carga, a representação dos elementos do sistema para o estudo de curto-circuito é realizada por meio de circuitos equivalentes inseridos na matriz admitância de barras. Nas faltas assimétricas (F-T, F-F e F-F-T) é necessário formar três matrizes admitância de sequência: positiva, negativa e zero.

:::info Informação
As informações a respeito das particularidades dos modelos para o estudo de curto-circuito são apresentados individualmente nos [elementos de potência](powerEditor#editando-dados-elétricos).
:::

### Equações do curto-circuito
Primeiramente será tratado o equacionamento para faltas balanceadas e então os estudos serão estendidos para as faltas desbalanceadas por meio da utilização do método das componentes simétricas.

#### Faltas balanceadas
Utiliza-se da matriz impedância de barras para o cálculo de curto-circuito, definida pela seguinte equação matricial:
$$
[\dot{V}] = [Z_{bus}][\dot{I}]
$$
Em que:
- $[Z_{bus}]$ é a inversa da matriz admitância de barras, chamada de matriz impedância de barras.

Por meio da expansão da equação anterior é possível calcular a corrente de falta trifásica na barra genérica $i$:
$$
\dot{I}_f = \frac{\dot{E}_i}{\overline{z}_{ii}+\overline{z}_{f}}
$$
Em que:
- $\dot{I}_f$ é a corrente de falta trifásica na barra $i$
- $\dot{E}_i$ é a tensão pré-falta na barra $i$
- $\overline{z}_{ii}$ é a impedância equivalente de Thevenin vista pela barra $i$, retirada da matriz impedância
- $\overline{z}_{f}$ é a impedância de falta

#### Faltas desbalanceadas
O desenvolvimento das equações do cálculo de curto-circuito para faltas desbalanceadas é realizado seguindo o seguinte procedimento:
1.	Definir os diagramas no ponto da falta, mostrando as conexões de todas fases para a falta. Assume-se que apenas impedâncias balanceadas estão presentes em ambos os lados do ponto da falta e o equivalente Thevenin até esse ponto é conhecido;
2.	Escrever as condições de contorno relacionando as tensões e corrente conhecidas para o tipo de falta estudada;
3.	Transformar as correntes e tensões do item 2 de a-b-c para o sistema de coordenadas 0-1-2;
4.	Encontrar a corrente do curto-circuito em estudo baseado no seguinte sistema de equações (para a fase A):
	$$
	\begin{cases}
	\dot{V}_{a1} = \dot{E}_a - \dot{I}_{a1} \overline{z}_1\\
	\dot{V}_{a2} = - \dot{I}_{a2} \overline{z}_2\\
	\dot{V}_{a0} = - \dot{I}_{a0} \overline{z}_0
	\end{cases}
	$$

A tabela abaixo apresenta as equações para as faltas desbalanceadas após a execução do procedimento apresenteado:

|**Falta**|**Seq. Positiva ($\dot{I}_{f}^{1}$)**|**Seq. Negativa ($\dot{I}_{f}^{2}$)**|**Seq. Zero ($\dot{I}_{f}^{0}$)**|
|-----|:-----------:|:-----------:|:-------:|
|**F-T**| $\dfrac{\dot{E}_i}{\overline{z}_{ii}^{1} + \overline{z}_{ii}^{2} + \overline{z}_{ii}^{0} + 3 \overline{z}_{f}}$ | $\dot{I}_{f}^{1}$ | $\dot{I}_{f}^{1}$|
|**F-F**| $\dfrac{\dot{E}_i}{\overline{z}_{ii}^{1} + \overline{z}_{ii}^{2} + \overline{z}_{f}}$ | $- \dot{I}_{f}^{1}$ | $0{,}0$|
|**F-F-T**| $\dfrac{\dot{E}_i \left( \overline{z}_{ii}^{2} + \overline{z}_{ii}^{0} + 3 \overline{z}_{f} \right)}{\overline{z}_{ii}^{1} \overline{z}_{ii}^{2} + 3 \overline{z}_{ii}^{2} \overline{z}_{f} + \overline{z}_{ii}^{2} \overline{z}_{ii}^{0} + 3 \overline{z}_{ii}^{1} \overline{z}_{f} + \overline{z}_{ii}^{1} \overline{z}_{ii}^{0} }$ | $- \dfrac{\dot{E}_i - \overline{z}_{ii}^{1} \dot{I}_{f}^{1}}{\overline{z}_{ii}^{2}}$ | $- \dfrac{\dot{E}_i - \overline{z}_{ii}^{1} \dot{I}_{f}^{1}}{\overline{z}_{ii}^{0} + 3 \overline{z}_{f}}$|

Para obter os valores em a-b-c é usada a equação matricial apresentada anteriormente, encerrando o cálculo de curto-circuito.

## Referências
1. ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286
1. STEVENSON JR.; WILLIAN, D. Elementos de Análise de Sistemas de Potência. 2ª ed. São Paulo: McGraw-Hill, 1986.
1. ANDERSON, P. M.; FOUAD, A. A. Power System Control and Stability. Wiley-IEEE Press, New York, 2002. doi: https://doi.org/10.1109/9780470545577
1. FORTESCUE, C. L. Method of Symmetrical Coordinates Applied to the Solution of Polyphase Networks. Trans. AIEE, v. 37, p.1027-1140, 1918. doi: https://doi.org/10.1109/T-AIEE.1918.4765570
1. ANDERSON, P. M. Analysis of faulted power systems. New York: IEEE Press, 1995.
