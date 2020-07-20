---
id: transformer
title: Transformador
sidebar_label: Transformador
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

>Aparelho estático com dois ou mais enrolamentos que, por indução eletromagnética, transforma um sistema de tensão e corrente alternada em outro sistema de tensão e corrente geralmente de valores diferentes e na mesma frequência com o objetivo de transmitir energia elétrica. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=421-01-01).

Transformadores com quaisquer conexões e defasagens entre as tensões primárias e secundárias podem ser inseridos no PSP-UFU. Para isso, os transformadores devem ser modelados representando seu tape ($t$) e defasagem ($\phi$), utilizando um transformador ideal de relação de transformação $\overline{t}:1$ em série com sua impedância ($\overline{y}_T = r_T + jx_T$), em que $\overline{t}$ é um número complexo ($t\angle\phi$)

:::caution Atenção
O modelo utilizado no PSP-UFU **não considera** o ramo magnetizante ou impedâncias mútuas entre as fases.
:::

A figura abaixo mostra o modelo do transformador implementado no PSP-UFU:

<img src={useBaseUrl("images/transformerModel.svg")} alt="Modelo PI da linha" title="Modelo PI da linha" />

Em que:
- $\bold{r_T}$	é a resistência total do transformador;
- $\bold{x_T}$	é a reatância indutiva total do transformador;
- $\bold{\overline{t}}$	é a o tape complexo do transformador.

A relação entre as tensões e correntes das barras genéricas $i$ e $j$ é obtida pela equação matricial:

$$
\begin{bmatrix}
\dot{I}_{ij}\\
\dot{I}_{ji}
\end{bmatrix}
=
\begin{bmatrix}
\displaystyle \frac{\overline{y}_T}{t^2} & -\displaystyle \frac{\overline{y}_T}{\overline{t}^*}\\
-\displaystyle \frac{\overline{y}_T}{\overline{t}} & \overline{y}_T
\end{bmatrix}
\times
\begin{bmatrix}
\dot{V}_{i}\\
\dot{V}_{j}
\end{bmatrix}
$$

Assim como nas [linhas](line), o transformador de dois enrolamentos deve ser inserido entre barras, porém não há a necessidade das tensões nominais desses barramentos serem idênticas.

:::caution Atenção!
A primeira barra selecionada será o lado **primário**, o qual é indicado por um círculo. Para alterar o lado primário após a inserção basta desconectar os nós dos transformadores e reconectá-los alternando as barras, utilizando as [ferramentas CAD](cadTools#conexão-e-desconexão-de-elemetos-na-barra).
:::

## Formulário de edição dos transformadores

A imagem abaixo apresenta o formulário de inserção/alteração de dados dos transformadores:

<img src={useBaseUrl("images/trafoForm.png")} alt="Formulário dos transformadores no PSP-UFU" title="Formulário dos transformadores no PSP-UFU" />

Esse formulário é subdividido em dois contextos distintos:
- **Geral**: no qual são inseridas informações gerais dos transformadores, informações do fluxo de carga, sua conexão e defasagem;
- **Falta**: local onde as impedâncias de sequência zero e impedâncias de aterramento são inseridas;

Além desses dois contextos, pode ser observado o botão "Estabilidade" na parte inferior direita do formulário. Esse formulário, comum a vários outros elementos, permite a inserção e/ou remoção do transformador durante o estudo de [estabilidade](stability).

<img src={useBaseUrl("images/trafoSw.png")} alt="Formulário de chaveamento do transformador" title="Formulário de chaveamento do transformador" />

---
### Geral

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Tensão nominal
Campo de informação *não editável* que apresenta a tensão primária e secundária do transformador. Para alterar esse campo é necessário editar o campo correspondente dos [barramento](bus#geral) conectados.

#### Tensão base
Tensão utilizada para converter os parâmetros de impedância inseridos em $\Omega$ para $p.u.$ As tensões selecionadas por uma caixa de escolha são editadas nos formulários dos [barramentos](bus#geral) conectados.

:::info Informação
A tensão base é ignorada caso os dados inseridos estejam em $p.u.$
:::

#### Potência nominal
Potência nominal da linha, inserida em MVA, kVA ou VA.

Esse campo é especialmente importante caso a opção "Utilizar a potência nominal como base" esteja marcada.

#### Impedância do transformador
Impedância série do transformador (resistência e reatância indutiva), inserida em $p.u.$ ou $\Omega$. Caso as impedâncias sejam inseridas em $\Omega$ será utilizada a tensão base selecionada para conversão para $p.u.$

#### Conexão
Conexão do transformador utilizada para cálculo das correntes de sequência zero nas faltas desbalanceadas com presença de terra (fase-terra e fase-fase-terra).

As seguintes conexões estão disponíveis:
1. $Y_{aterrado}-Y_{aterrado}$
2. $Y-Y_{aterrado}$
3. $Y_{aterrado}-Y$
4. $Y-Y$
5. $\Delta-Y_{aterrado}$
6. $\Delta-Y$
7. $Y_{aterrado}-\Delta$
8. $Y-\Delta$
9. $\Delta-\Delta$

#### TAP
Tape do transformador **em relação ao primário**.

:::caution Atenção!
O tape no PSP-UFU deve ser inserido pela relação entre a tensão primária nominal e a tensão de tape $\left(\text{TAP} = \frac{V_{nominal}}{V_{tape}}\right)$.
:::

#### Defasagem
Defasagem do transformador em relação ao primário, em graus. Esse valor independe do tipo de conexão, portanto para conexões diferentes de $Y-Y$(aterrado ou não) e $\Delta-\Delta$, deve-se inserir o ângulo de defasagem correto.

#### Utilizar potência nominal como base
Caso essa opção seja marcada, o programa irá utilizar a potência nominal do transformador como base para a conversão das unidades, caso contrário será usada a [potência base do sistema](simulationConfig).

---
### Falta

#### Impedância de sequência zero do transformador
Esses parâmetros são necessários para o correto cálculo das correntes de [falta desbalanceadas](fault) (fase-fase, fase-fase-terra e fase-terra) e devem ser inseridos em $p.u.$

:::warning Cuidado!
A não inserção desses dados acarretarão em resultados incorretos para [faltas desbalanceadas](fault).
:::

#### Impedâncias de aterramento
As impedâncias de aterramento são somente utilizadas para conexão $Y_{aterrado}$ e devem ser inseridas em $p.u.$

---
### Botão Estabilidade
O botão "Estabilidade" irá abrir um formulário, comum a vários outros elementos, que permite a inserção e/ou remoção do transformador durante o estudo de [estabilidade](stability).

Nesse formulário pode ser criada uma lista genérica de inserções e remoções da linha no tempo, personalizada por um contexto de propriedades de chaveamento que são editados o tipo de chaveamento (inserção ou remoção) e o instante (em segundos) do evento. Essas propriedades são atribuídas e retiradas da lista genérica por meio dos botões "Adicionar" e "Remover", respectivamente.