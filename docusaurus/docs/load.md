---
id: load
title: Carga
sidebar_label: Carga
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

>1. a potência ativa, reativa ou aparente gerada, transmitida ou distribuída dentro de um sistema;
1. a potência demandada por um grupo de consumidores classificados de acordo com suas particularidades e características, por exemplo, carga de aquecimento, carga reativa diurna, etc. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=601-01-15).

## Carga no PSP-UFU

As cargas são os elementos de potência consumidores de potência genéricos. Nos estudos de fluxo de carga é possível modelar a carga como **potência ou impedância constante** e em estabilidade pode-se compor a carga na proporção desejada em três tipos (**carga ZIP**): potência constante, impedância constante e corrente constante.

As cargas de potência constante nos estudos de fluxo de carga são inseridos no vetor de potências e permanecem fixos durante toda a simulação.

Os elementos shunt, como reatores, banco de capacitores e cargas de impedância constante, são representados simplesmente por suas admitâncias em derivação. Como os parâmetros de entrada geralmente se constituem de suas potências ativa (P) e reativa (Q) nominais (no caso de reatores e capacitores, P=0), deve-se, portanto, encontrar sua admitância com a equação abaixo para inseri-la na matriz admitância:

$$
\overline{y} = \frac{P-jQ}{\dot{V}^2}
$$

:::caution Atenção!
Para os estudos de **curto-circuito** e **harmônicos** as cargas são modeladas como impedância constante, independentemente da sinalização indicada em seu formulário de edição de dados.
:::

### Carga no estudo de estabilidade
A modelagem das cargas do sistema de forma exata se torna impraticável em um problemas de estabilidade, visto à grande quantidade e variedade dos componentes envolvidos, além de apresentar modificações em suas composições causado por vários fatores, como tempo, condições climáticas e economia. Portanto várias aproximações devem ser utilizadas na formulação de um modelo de carga nos estudos aqui postos.

Uma forma interessante de se representar cargas dependentes da tensão são as cargas ZIP, as quais são funções quadráticas, possuindo três parcelas: impedância constante (Z), corrente constante (I) e potência constante (P). As potências ativa e reativa são obtidas pelas seguintes equações:
$$
P_L=P_{Z0} \left(\frac{V_i}{V_{i0}} \right)^2+P_{I0} \left(\frac{V_i}{V_i0} \right)+P_{P0}
$$
$$
Q_L=Q_{Z0} \left(\frac{V_i}{V_{i0}} \right)^2+Q_{I0} \left(\frac{V_i}{V_i0} \right)+Q_{P0}
$$

Em que:

$P_L$ e $Q_L$	são as potências ativa e reativa da carga ZIP, respectivamente, conectadas em uma barra genérica i;

$V_i$ é a tensão atual de uma barra genérica i;

$V_{i0}$ é a tensão inicial de uma barra genérica i, obtida do estudo de fluxo de carga;

$P_{Z0}$ e $Q_{Z0}$ são as parcelas de impedância constante para potências ativa e reativa, respectivamente;

$P_{I0}$ e $Q_{I0}$ são as parcelas de corrente constante para potências ativa e reativa, respectivamente;

$P_{P0}$ e $Q_{P0}$ são as parcelas de potência constante para potências ativa e reativa, respectivamente.

Os valores das parcelas de potência da carga ZIP são obtidos utilizando as potências da carga após a convergência do cálculo do fluxo de carga ($P_{L0}$, para a potência ativa e $Q_{L0}$, para a potência reativa), utilizando as seguintes expressões:

$$
\begin{cases}
P_{Z0}=\displaystyle \frac{k_{PZ}}{100} \frac{P_{L0}}{V_{i0}^2}\\
P_{I0}=\displaystyle \frac{k_{PI}}{100} \frac{P_{L0}}{V_{i0}}\\
P_{P0}=\displaystyle \frac{k_{PP}}{100} P_{L0}
\end{cases}
$$

Os valores de $k_{PZ}$, $k_{PI}$ e $k_{PP}$ representam a composição em impedância, corrente e potência constantes, respectivamente, da parcela ativa da carga, os quais são inseridos pelo usuário na forma de porcentagem, sendo a soma desses três valores necessariamente igual a 100%. Os parâmetros $k_{QZ}$, $k_{QI}$ e $k_{QP}$ podem ser interpretados de forma análoga, porém compõe a parcela reativa da carga.

O comportamento das potências, retiradas de simulações no PSP-UFU, de cada parcela que compõe a carga ZIP são evidenciadas na figura abaixo.

<img src={useBaseUrl("images/loadModel.svg")} alt="Comportamento da carga ZIP implementada no software: (a) curva de Tensão x Potência; (b) Curva de Tensão x Corrente" title="Comportamento da carga ZIP implementada no software: (a) curva de Tensão x Potência; (b) Curva de Tensão x Corrente" />

Cargas de corrente e potência constantes possuem problemas em tensões muito baixas. À medida que a tensão diminui as correntes dessas cargas não reduz, como pode ser observado na figura acima, resultando em perda de precisão e problemas na convergência de processos iterativos. Para contornar esse problema utiliza-se uma tensão pré-definida pelo usuário ($V_{low}$), a qual as cargas (ou parcelas) de corrente e potência constantes são modeladas como impedância constante, resultando no comportamento de potência e corrente apresentados na figura acima.

## Formulário de edição das cargas
A imagem abaixo apresenta o formulário de inserção/alteração de dados das cargas:

<img src={useBaseUrl("images/loadForm.png")} alt="Formulário das cargas no PSP-UFU" title="Formulário das cargas no PSP-UFU" />

Esse formulário é subdividido em dois contextos distintos:
- **Geral**: no qual são inseridas informações gerais das cargas, informações e o tipo de carga no fluxo de carga;
- **Estabilidade**: contendo opções de visualização de dados da carga em gráficos no tempo e opções de parametrização da carga ZIP.

Além desses dois contextos, pode ser observado o botão "Chaveamento" na parte inferior esquerda do formulário. Esse formulário, comum a vários outros elementos, permite a inserção e/ou remoção da carga durante o estudo de [estabilidade](stability).

<img src={useBaseUrl("images/loadSw.png")} alt="Formulário de chaveamento da carga" title="Formulário de chaveamento da carga" />

---
### Geral

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Potência ativa
Parcela de potência ativa da carga. Pode ser inserido em MW, kW W ou $p.u.$ (na base do sistema).

#### Potência reativa
Parcela de potência reativa da carga. Pode ser inserido em Mvar, kvar var ou $p.u.$ (na base do sistema).

#### Tipo de carga (fluxo de carga)
Tipo da carga para o estudo de fluxo de carga, podendo ser selecionado dois tipos: Potência constante e Impedância constante. As cargas de potência constantes são inseridas nos vetores de potência e permanecem invariantes no cálculo, enquanto que para as cargas de impedância constante determina-se o valor da impedância utilizando a potência e tensão nominal, a qual é inserida na matriz admitância.

---
### Estabilidade

#### Imprimir dados
Exibe os dados da carga nos gráficos no tempo.
Os seguintes dados são exibidos:
- módulo da tensão
- corrente
- potências ativa e reativa.

#### Utilizar composição de carga ZIP
Permite compor a carga ZIP para estudos de estabilidade. Caso essa opção não esteja selecionada é usada a composição global definida nas [configurações de simulação](simulationConfig).

:::caution Atenção
Por padrão, todas as cargas no PSP-UFU são definidas como impedância constante no estudo de estabilidade.
:::

:::tip Dica
Caso todas as barras possuírem o mesmo comportamento de carga ZIP, a parametrização poderá ser realizada nas [configurações de simulação](simulationConfig) para todas as cargas, sem a necessidade de parametrização individual.
:::

#### Composição de potência ativa e reativa
Valores da composição da carga ZIP, inseridas para potência ativa e reativa separadamente, em porcentagem. A soma da composição deve ser igual a 100%, caso contrário será exibida uma mensagem de erro ao usuário.

---
### Botão Chaveamento
O botão "Chaveamento" irá abrir um formulário, comum a vários outros elementos, que permite a inserção e/ou remoção do transformador durante o estudo de [estabilidade](stability).

Nesse formulário pode ser criada uma lista genérica de inserções e remoções da linha no tempo, personalizada por um contexto de propriedades de chaveamento que são editados o tipo de chaveamento (inserção ou remoção) e o instante (em segundos) do evento. Essas propriedades são atribuídas e retiradas da lista genérica por meio dos botões "Adicionar" e "Remover", respectivamente.
