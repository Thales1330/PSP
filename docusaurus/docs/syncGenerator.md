---
id: syncGenerator
title: Gerador Síncrono
sidebar_label: Gerador Síncrono
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

> Uma máquina de corrente alternada na qual a frequência das tensões geradas e a velocidade da máquina estão em uma proporção constante. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=411-31-08).

## Gerador Síncrono no PSP-UFU
Os geradores síncronos são a fonte de potência para o cálculo do fluxo de carga e estabilidade, além de serem um dos principais contribuintes para as correntes de falta.

:::caution Atenção!
Somente barramentos que possuem esse elemento conectado podem ser considerados [barras de referência](powerFlow). [Barras do tipo PV](powerFlow) devem conter um gerador síncrono ou um [motor síncrono (compensador síncrono)](syncMotor).
:::

O formulário de dados é dividido em dois, sendo o primeiro responsável pelos dados gerais, de [fluxo de carga](powerFlow) e de [falta](fault) e um segundo pelos dados de [estabilidade](stability) . Esse último também permite o acesso aos controles das máquinas síncronas manipulados pelo editor de controle.

:::info Informação
Os dados referentes às **impedâncias de sequência positiva do gerador síncrono** inseridas no [contexto (aba) "Falta"](syncGenerator#falta) são utilizados **tanto para os estudos de [curto-circuito](fault) quanto para os estudos de [harmônicos](harmonics)**.

Esses dados são ignorados nos estudos de [fluxo de carga](powerFlow) (*não são utilizado nesse estudo*) e [estabilidade](stability) (*são utilizados dados inseridos em formulário específico*).
:::

### Gerador Síncrono no fluxo de carga
O gerador síncrono é a fonte de potência do PSP-UFU no estudo de fluxo de carga. Seu comportamento difere de acordo com o tipo de barra conectada:
- [Barra de referência](powerFlow): Os dados de potência ativa e reativa inseridos são desprezados, uma vez que esse elemento será utilizado para completar o balanço de potência do estudo de fluxo d carga;
- [Barra PV](powerFlow): O dado de potência ativa é considerado, porém o dados de potência reativa são desprezados. O valor da potência reativa é utilizado para manter o módulo da tensão constante no barramento conectado;
- [Barra PQ](powerFlow): Para os geradores conectados a essa barra tanto a potência ativa quanto reativa inseridas são consideradas.

:::caution Atenção
Caso o limite de potência reativa seja excedido, o programa automaticamente transforma a [Barra PV](powerFlow) conectada em uma [Barra PQ](powerFlow), utilizando o valor limite de potência reativa que seria ultrapassado.
::: 

### Gerador Síncrono no estudo de curto-circuito
Enquanto os geradores no estudo de fluxo de carga são modelados somente por correntes injetadas nas barras, para o curto-circuito utiliza-se uma tensão atrás de uma impedância. A figura abaixo mostra o caminho da corrente e o circuito equivalente de cada sequência nos geradores.

<img src={useBaseUrl("images/sycGeneratorSeq.svg")} alt="Caminho das correntes e circuito equivalente: (a) sequência positiva; (b) sequência negativa; (c) sequência zero" title="Caminho das correntes e circuito equivalente: (a) sequência positiva; (b) sequência negativa; (c) sequência zero" />

As tensões geradas são somente de sequência positiva, uma vez que o gerador fornece sempre tensões trifásicas equilibradas. Portanto a rede de sequência positiva é composta de uma tensão pré-falta atrás de uma impedância de sequência positiva. As redes de sequência negativa e zero não contêm forças eletromotrizes, porém incluem as impedâncias do gerador de sequência negativa e zero.

A corrente que circula na impedância $\overline{z}_n$ entre o neutro e a terra é $3\dot{I}_{a0}$. Pela figura acima (c), observa-se que a queda de tensão de sequência zero do ponto a para terra ($\dot{V}_{a0}$) é:

$$
\dot{V}_{a0} = -3\dot{I}_{a0}\overline{z}_n - \dot{I}_{a0}\overline{z}_{g0}
$$

A rede de sequência zero, que é um circuito monofásico pelo qual se supõe que circule apenas corrente de sequência zero e deve, portanto, ter uma impedância definida pela seguinte equação:

$$
\overline{z}_{0} = 3\overline{z}_n + \overline{z}_{g0}
$$

:::tip Dica
Caso o gerador não seja aterrado, não circulará corrente de sequência zero por ele. Nesse caso, dependendo da conexão do transformador próximo ao gerador sem aterramento, a seguinte mensagem de erro pode ser exibida:

<p><center>"<i>Falha ao inverter a matriz admitância de sequência zero</i>"</center></p>

Isso ocorre porque a matriz admitância de sequência zero é singular. Para contornar esse problema escolha **uma** das duas soluções abaixo:
1. Marque a opção "Neutro aterrado" e insira um alto valor de reatância de aterramento ($j9999~p.u.$, por exemplo);
2. **Ou**, na barra do gerador, insira um [reator](inductor) de baixo valor de potência reativa ($1,0~var$, por exemplo).
:::

### Gerador Síncrono no estudo de estabilidade
A relação de valores observados em testes adequados (definidos na [IEEE Std. 115-2019](https://doi.org/10.1109/IEEESTD.2020.9050934)), denominados parâmetros padrões, são utilizados para modelar a máquina síncrona no estudo de estabilidade do PSP-UFU.

Os parâmetros da máquina síncrona que influenciam rapidamente no decaimento de valores são chamados **subtransitórios** (indicados por $''$), aqueles que influenciam mais lentamente são chamados **transitórios** (indicados por $'$) e, finalmente, aqueles que influenciam continuamente são chamados de parâmetros **síncronos** (sem indicação de sobrescrito).

Um conjunto de equações algébrico-diferenciais determinam o comportamento da máquina síncrona no estudo de estabilidade:

$$
E_{q}' - V_q = r_aI_q - x_{ds}'Id\\
E_{d}' - V_d = r_aI_d - x_{qs}'Iq\\
E_{q}'' - V_q = r_aI_q - x_{ds}''Id\\
E_{d}'' - V_d = r_aI_d - x_{qs}''Iq\\
~\\
\frac{dE_{q}'}{dt} = \frac{V_{fd} + \left( x_d - x_{d}' \right)I_d - s_d E_{q}'}{T_{d0}'}\\
\frac{dE_{d}'}{dt} = \frac{- \left( x_q - x_{q}' \right)I_q - s_q E_{d}'}{T_{q0}'}\\

\frac{dE_{q}''}{dt} = \frac{s_dE_{q}' + \left( x_{d}' - x_{d}'' \right)I_d - s_d E_{q}''}{T_{d0}''}\\
\frac{dE_{d}''}{dt} = \frac{s_dE_{d}' + \left( x_{q}' - x_{q}'' \right)I_q - s_d E_{d}''}{T_{q0}''}\\
~\\
\frac{d\omega}{dt} = \frac{\omega_r}{2H} \left[ P_m - P_e - D_a \left( \omega - \omega_r \right) \right]\\
\frac{d\delta}{dt} = \Omega_b\left( \omega - \omega_r \right)
$$

As duas últimas equações diferenciais são as equações mecânicas da máquina; e as demais são equações elétricas (consulte essa [tese](https://dx.doi.org/10.14393/ufu.te.2019.2444) para maiores detalhes acerca dos parâmetros dessas equações).

Utilizando as equações transitórias e subtransitórias podem-se definir cinco modelos de distintas complexidades.

:::caution Atenção!
O modelo da máquina síncrona é **selecionado automaticamente** de acordo com os dados fornecidos ao programa.
:::

Na sequência são apresentados tais modelos, incluídos os efeitos da saturação magnética, em conjunto com seus diagramas de blocos:

- **Modelo 1**: Corresponde a uma tensão constante atrás de uma reatância transitória de eixo direto ($x_{d}'$), não exigindo equações diferenciais;
- **Modelo 2**: São representados os efeitos transitórios de eixo direto, sendo necessária a solução de uma equação diferencial ($\frac{dE_{q}'}{dt}$), cujo diagrama de blocos é apresentado na figura abaixo:

<img src={useBaseUrl("images/model2SyncGenerator.svg")} alt="Diagrama de blocos do Modelo 2 das máquinas síncronas" title="Diagrama de blocos do Modelo 2 das máquinas síncronas" />

- **Modelo 3**: São representados os efeitos transitórios de eixo direto e em quadratura, exigindo duas equações diferenciais ($\frac{dE_{q}'}{dt}$ e $\frac{dE_{d}'}{dt}$), cujo diagrama de blocos é apresentado na figura abaixo:

<img src={useBaseUrl("images/model3SyncGenerator.svg")} alt="Diagrama de blocos do Modelo 3 das máquinas síncronas" title="Diagrama de blocos do Modelo 3 das máquinas síncronas" />

- **Modelo 4**: São representados os efeitos subtransitórios de eixo direto e em quadratura, sendo necessária a solução de três equações diferenciais ($\frac{dE_{q}'}{dt}$, $\frac{dE_{q}''}{dt}$ e $\frac{dE_{d}''}{dt}$), cujo diagrama de blocos é apresentado na figura abaixo:

<img src={useBaseUrl("images/model4SyncGenerator.svg")} alt="Diagrama de blocos do Modelo 4 das máquinas síncronas" title="Diagrama de blocos do Modelo 4 das máquinas síncronas" />

- **Modelo 5**: São representados os efeitos subtransitórios de eixo direto e em quadratura, sendo necessária a solução de quatro equações diferenciais ($\frac{dE_{q}'}{dt}$, $\frac{dE_{d}'}{dt}$, $\frac{dE_{q}''}{dt}$ e $\frac{dE_{d}''}{dt}$), cujo diagrama de blocos é apresentado na figura abaixo:

<img src={useBaseUrl("images/model5SyncGenerator.svg")} alt="Diagrama de blocos do Modelo 4 das máquinas síncronas" title="Diagrama de blocos do Modelo 4 das máquinas síncronas" />

:::note Nota
Em todos os modelos as equações diferenciais mecânicas são solucionadas.
:::

#### Barramento infinito
Algumas referências incluem um modelo sem equações diferenciais, em que a máquina é somente representada por uma tensão constante atrás de uma reatância transitória de eixo direto. Tal é utilizado na representação de um **barramento infinito**, o qual é normalmente constituído de um subsistema muito maior àquele simulado.

No PSP-UFU a **representação de um barramento infinito** pode ser obtido por meio da utilização de uma máquina representada pelo Modelo 1 cujo valor da constante de inércia (H) é infinito ou muito grande ($9999~s$, por exemplo) em relação às demais máquinas do sistema, e o valor de $x_{d}'$ deve ser um valor muito pequeno ($10^{-3}~p.u.$, por exemplo).

#### Centro de inércia
Normalmente utiliza-se a velocidade de referência como sendo a síncrona e portanto, nesse caso, $\omega_r = \omega_b = 1,0~p.u.$ Essa abordagem, adotada por vários livros de estabilidade, considera como referência uma máquina fictícia girando sempre na velocidade síncrona independente das perturbações aplicadas no sistema. No PSP-UFU foi implementado o conceito de centro de inércia (COI, do inglês, *Center of Inertia*), que constitui uma soma ponderada das velocidades das máquinas presentes no sistema:

$$
\omega_r=\frac{\left( \sum_{i=1}^{n} H_i \omega_i \right)}{\left( \sum_{i=1}^{n} H_i \right)}
$$
Em que:
$n$	é o número de máquinas síncronas **conectadas** no sistema.

A aplicação do COI resulta em dados de saída, como o ângulo do rotor, mais fáceis de serem analisados. Na implementação realizada no programa a utilização ou não desse recurso é opcional e pode ser [definida pelo usuário](simulationConfig).


## Formulário de edição dos geradores síncronos
A imagem abaixo apresenta o formulário de inserção/alteração de dados dos geradores síncronos:

<img src={useBaseUrl("images/syncGeneratorForm.png")} alt="Formulário dos geradores síncronos no PSP-UFU" title="Formulário dos geradores síncronos no PSP-UFU" />

Um segundo pelos dados de estabilidade, como mostra a figura abaixo, acessado ao clicar no botão "Estabilidade" do formulário principal. Nele é possível também acessar aos controles das máquinas síncronas manipulados pelo [editor de controle](controlEditor).

<img src={useBaseUrl("images/syncGeneratorStabForm.png")} alt="Formulário de estabilidade dos geradores síncronos no PSP-UFU" title="Formulário de estabilidade dos geradores síncronos no PSP-UFU" />

No formulário de estabilidade pode ser observado o botão "Chaveamento" na parte inferior esquerda do formulário. Esse formulário, comum a vários outros elementos, permite a inserção e/ou remoção do gerador durante o estudo de [estabilidade](stability).

<img src={useBaseUrl("images/syncGeneratorSw.png")} alt="Formulário de chaveamento do gerador síncrono" title="Formulário de chaveamento do gerador síncrono" />

---
### Geral

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Potência nominal
Potência nominal do gerador, inserida em MVA, kVA ou VA.

Esse campo é especialmente importante caso a opção "Utilizar a potência nominal como base" esteja marcada.

#### Potências ativa e reativa
Potências ativa (inserida em W, kW, MW ou p.u.) e reativa (inserida em var, kvar, Mvar ou p.u.) do gerador.

Caso a barra conectada seja PV o valor de potência reativa será ignorado e caso seja de referência ambos os valores inseridos serão desprezados.

:::caution Atenção!
Caso mais de um gerador esteja conectado na mesma barra, os valores de potência reativa (nas barras de referência e PV) e ativa (nas barras de referência) são igualmente distribuídas, respeitando os limites individuais de potência reativa.
:::

#### Potências reativas máxima e mínima
Limites de potência reativa máxima e mínima do gerador para controle de tensão em barras PV. Caso esses valores sejam ultrapassados, o reativo gerado pela unidade será limitado ao valor inserido e a barra conectada será transformada em PQ, não controlando a tensão estabelecida.

#### Utilizar potência nominal como base
Caso essa opção seja marcada, o programa irá utilizar a potência nominal do gerador como base para a conversão das unidades, inclusive aqueles no formulário de estabilidade, caso contrário será usada a [potência base do sistema](simulationConfig).

---
### Falta

#### Impedâncias de sequência
Valores de resistência e reatância para cálculo das correntes de falta. São inseridos dados de sequência positiva, negativa e zero.

#### Impedância de aterramento
Valores utilizados para o cálculo das correntes de falta do tipo fase-terra e fase-fase-terra. Caso o neutro do gerador não seja aterrado, o valor inserido nesse campo é ignorado.

#### Neutro aterrado
Indica se o neutro do gerador é aterrado.

---
### Botão Estabilidade

#### Imprimir dados da máquina síncrona
Exibe os dados do gerador síncrono nos gráficos no tempo. Os seguintes dados são exibidos:
- tensão terminal
- potências ativa e reativas
- potência mecânica
- frequência
- tensão de campo
- ângulo $\delta$.

#### Utilizar AVR e regulador de velocidade
Esses campos acionam ou inibem os controles da máquina síncrona, assim como modelos de turbina e excitatrizes.

Caso selecionadas os controles podem ser inseridos e modificados pelos botões “Editar AVR” e “Editar regulador de velocidade”, os quais acessam o [editor de controle do PSP-UFU](controlEditor).

#### Resistência de armadura
Resistência de armadura da máquina síncrona, em $p.u.$

#### Reatância de Potier
Reatância de Potier para cálculo da saturação da máquina, em $p.u.$

#### Fator de saturação
Valor utilizado no cálculo da curva de saturação.

Representa o valor (em $p.u.$) de corrente de campo necessária para atingir 1,2 p.u. de tensão terminal.

:::caution Atenção!
Esse valor deve ser **maior que 1,2**, ou irá gerar erros na simulação. Caso não seja informado, a saturação da máquina não é considerada nos cálculos.
:::

#### Reatâncias síncronas
Valores de reatância síncrona (regime permanente) da máquina. Os valores de eixo direto e em quadratura devem ser iguais ou muito próximos para representação de uma máquina de polos lisos, enquanto para polos salientes esses valores são distintos.

#### Reatâncias e constantes de tempo transitórias
Parâmetros transitórios da máquina síncrona em $p.u.$ ou segundos.

:::warning Cuidado!
O valor da reatância transitória de eixo direto deve ser diferente de zero ou levará o programa a erro.
:::

De acordo com a quantidade de parâmetros inseridos é definido internamente pelo programa qual o modelo a ser utilizado.

#### Reatâncias e constantes de tempo subtransitórias
Parâmetros subtransitórios da máquina síncrona em $p.u.$ ou segundos, representando em detalhes a presença de enrolamentos amortecedores. Assim como os dados transitórios, esses parâmetros definem o modelo da máquina.

---
### Botão Chaveamento
O botão "Chaveamento" irá abrir um formulário, comum a vários outros elementos, que permite a inserção e/ou remoção do gerador durante o estudo de [estabilidade](stability).

Nesse formulário pode ser criada uma lista genérica de inserções e remoções da linha no tempo, personalizada por um contexto de propriedades de chaveamento que são editados o tipo de chaveamento (inserção ou remoção) e o instante (em segundos) do evento. Essas propriedades são atribuídas e retiradas da lista genérica por meio dos botões "Adicionar" e "Remover", respectivamente.

## Referências
1. MILANO, F. Power System Modelling and Scripting. London: Springer, 2010. doi: https://doi.org/10.1007/978-3-642-13669-6
2. ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286
3. KUNDUR, P. Power System Stability and Control. McGraw-Hill, New York, 1994.
4. DOMMEL, H. W.; SATO, N. Fast Transient Stability Solutions. IEEE Transactions on Power Aparatus and Systems, v. PAS-91, n. 4, jul 1972, p. 1643-1650. doi: https://doi.org/10.1109/TPAS.1972.293341
5. IEEE Std 1110-2002 IEEE Guide for Synchronous Generator Modeling Practices and Applications in Power System Stability Analyses. IEEE, New York, nov. 2003. doi: https://doi.org/10.1109/IEEESTD.2003.94408
6. KIMBARK, E. W. Power System Stability: Volume III – Synchronous Machine. New York: Wiley-IEEE Press, 1995.