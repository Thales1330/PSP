---
id: indMotor
title: Motor de Indução
sidebar_label: Motor de Indução
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

> Uma máquina assíncrona da qual apenas um enrolamento é energizado. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=411-31-10).

## Motor de indução trifásico no PSP-UFU
No PSP-UFU, os motores de indução são contemplados nos estudos de [fluxo de carga](powerFLow) e no estudo de [estabilidade](stability).

:::info Informação
Os dados de estabilidade da máquina de indução são utilizados em conjunto com o fluxo de carga, **calculando de forma correta a potência reativa das máquinas** e consequentemente a tensão no barramento conectado.
:::

:::warning Cuidado!
Ao marcar a opção "Calcular a potência reativa no fluxo de carga", devem ser inseridos os dados corretos na aba "Estabilidade", caso contrário, será atribuída uma potência reativa incorreta no motor.
:::

### Motor de indução trifásico no cálculo do fluxo de carga
A figura abaixo apresenta o modelo do motor de indução trifásico (MIT) de gaiola simples.

<div><center><img src={useBaseUrl("images/indMotorModel.svg")} alt="Circuito equivalente do motor de indução" title="Circuito equivalente do motor de indução" /></center></div>

As potências ativa e reativa podem ser calculadas em relação às variáveis e parâmetros do motor em $p.u.$ como:

$$
P = \frac{V^2 \left\{ \left( \frac{r_2}{s} \right) \left[\left(\frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]+K_1 \left[\left(\frac{r_2}{s} \right) \left(x_m+x_1 \right) + r_1 K_1 \right] \right\} }{\left[\left( \frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]^2+\left[\left( \frac{r_2}{s} \right)(x_m+x_1 )+r_1 K_1 \right]^2}\\
Q = \frac{-V^2 \left\{ K_1 \left[\left(\frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]- \left( \frac{r_2}{s} \right) \left[\left(\frac{r_2}{s} \right) \left(x_m+x_1 \right) + r_1 K_1 \right] \right\} }{\left[\left( \frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]^2+\left[\left( \frac{r_2}{s} \right)(x_m+x_1 )+r_1 K_1 \right]^2}\\
~\\
K_1=x_2+x_m
$$

Como pode ser observado nas equações acima, existem quatro variáveis e somente duas equações. Na prática, as variáveis podem ser reduzidas a três, uma vez que o módulo da tensão (V) é obtido nos resultados do [fluxo de potência](powerFlow). Para resolver as equações é necessário definir uma variável adicional. A variável escolhida como fixa no PSP-UFU é a potência ativa (P), por fornecer resultados numericamente corretos e adequados para motores em situações de estabilidade.

Portanto, nesse modelo estático, a potência ativa é mantida constante durante o cálculo do fluxo de carga e o escorregamento (s) é atualizado em cada iteração. A equação da potência ativa pode ser reescrita em relação ao escorregamento:

$$
\left(\frac{r_2}{s} \right)^2 A + \left(\frac{r_2}{s} \right) B + C = 0
$$
Em que:
$$
A = P \left( r_1^2 + K_3^2 \right) - V^2 r_1\\
B = 2P(r_1 K_2 + K_3 K_4) - V^2 \left(K_2 + K_1 K_3 \right)\\
C = P \left( K_2^2 + K_4^2 \right) - V^2 K_1 K_4\\
K_2 = -x_1 K_1 -x_2 x_m\\
K_3 = x_m + x_1\\
K_4 = r_1 K_1
$$

Esse modelo pode ser inserido na solução do [fluxo de carga](powerFlow) seguindo os seguintes passos:

1. As constantes $K_1$ a $K_4$ são inicialmente calculadas . Esses valores são mantidos constantes durante toda a solução;
2. Em cada iteração são calculados os coeficientes $A$, $B$ e $C$ utilizando o valor atualizado de $V$;
3. A equação quadrática é resolvida e dois valores de $\left(\frac{r_2}{s} \right)$ são obtidos, em que o maior deles é escolhido por estar na região estável da característica toque-escorregamento do motor;
4. Utilizando o novo valor de $\left(\frac{r_2}{s} \right)$, a potência reativa ($Q$) é obtida. O vetor de potências é então atualizado e os procedimentos convencionais de solução do fluxo de potência são realizados.

Os passos de 2 a 4 são repetidos até que se obtenha a convergência.

:::caution Atenção!
No PSP-UFU, os motores de indução não são considerados no cálculo de curto-circuito.
:::

### Motor de indução trifásico no estudo de estabilidade
Uma importante carga dinâmica são os motores de indução, uma vez que correspondem a uma parcela significativa das cargas presentes no sistema elétrico. O [modelo da máquina de indução apresentado anteriormente](indMotor#motor-de-indução-trifásico-no-cálculo-do-fluxo-de-carga), a qual pode ser utilizada tanto como motor quanto como gerador, é bem estabelecida na literatura.

Como descrito na [seção anterior](indMotor#motor-de-indução-trifásico-no-cálculo-do-fluxo-de-carga), a inicialização dessa máquina é realizada em conjunto com o fluxo de potência, visto que a potência reativa exigida pela máquina de indução é dependente dos parâmetros do motor, assim como a tensão do seu barramento. Essa abordagem é necessária, pois métodos convencionais conduzem a resultados errôneos em sistemas altamente carregados.

É necessário expressar a equação de movimento da máquina de indução em termos de torque e não potência, como é realizado com as máquinas síncronas. A simetria do rotor também faz com que a posição angular não seja importante e o escorregamento ($s$) é utilizado no lugar da velocidade ($\omega$), em que:

$$
s = frac{\Omega_0 - \omega}{\Omega_0}
$$

Desprezando as perdas por atrito e ventilação e a potência no eixo suave, as equações mecânicas do motor são expressas da seguinte forma:

$$
T_m = A -Bs + Cs^2\\
T_e = \frac{Re\left\{ \dot{E}\dot{I}^* \right\}}{\Omega_b}\\
\frac{ds}{dt} = \frac{\left( T_m - T_e \right)}{2H}
$$
Em que:
- $T_m$ é o torque mecânico;
- $T_e$ é o torque elétrico;
- $H$ é a inércia do conjunto motor - carga mecânica

Os termos $A$, $B$ e $C$ são termos que definem o comportamento do torque mecânico da carga de acordo com o escorregamento. O torque mecânico normalmente varia com a **velocidade**, podendo ser expressa proporcionalmente com a seguinte equação quadrática:

$$
T_m \propto a + b\omega + c\omega^2
$$
Em que:
$$
\begin{cases}
	A \propto a + b + c \\
	B \propto b + 2c \\
	C \propto c
\end{cases}
$$

As equações elétricas do motor de indução de gaiola simples são baseadas no circuito equivalente da [figura anterior](indMotor#motor-de-indução-trifásico-no-cálculo-do-fluxo-de-carga). De forma semelhante ao modelo transitório da máquina síncrona, o motor de indução pode ser modelado pelo circuito equivalente de Thevenin de tensão transitória $E'$ atrás de uma resistência do estator $r_1$ e uma reatância transitória $x'$. A reatância transitória aparente de rotor bloqueado é dada por:
$$
x' = x_1 + \frac{x_2 x_m}{x_2 + x_m}
$$

A constante de tempo de circuito aberto do rotor ($T_0'$) é:
$$
T_0' = \frac{x_2 + x_m}{\Omega_b r_2}
$$

E a reatância de circuito aberto é:
$$
x_0 = x_1 + x_m
$$

Uma vez que as reatâncias não são afetadas pela posição do rotor, as EADs do motor de indução podem ser expressar diretamente por componentes reais ($r$) e imaginárias ($m$). Portanto, a descrição completa desse modelo é representada pelo seguinte sistema de equações algébrico-diferenciais:
$$
V_r - E_r' = r_1 I_r - x' I_m\\
V_m - E_m' = r_1 I_m - x' I_m\\
\frac{dE_r'}{dt} = \Omega_b s E_m' - \frac{E_r' + \left( x_0 - x' \right) I_m}{T_0'}\\
\frac{dE_m'}{dt} = \Omega_b s E_r' - \frac{E_m' i \left( x_0 - x' \right) I_r}{T_0'}
$$

## Formulário de edição dos geradores síncronos
A imagem abaixo apresenta o formulário de inserção/alteração de dados dos motores de indução:

<div><center><img src={useBaseUrl("images/indMotorForm.png")} alt="Formulário dos motores de indução no PSP-UFU" title="Formulário dos motores de indução no PSP-UFU" /></center></div>

No formulário pode ser observado o botão "Chaveamento" na parte inferior esquerda do formulário. Esse formulário, comum a vários outros elementos, permite a inserção e/ou remoção do motor durante o estudo de [estabilidade](stability).

<div><center><img src={useBaseUrl("images/indMotorSw.png")} alt="Formulário de chaveamento do motores de indução" title="Formulário de chaveamento do motores de indução" /></center></div>

---
### Geral

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Potência nominal
Potência nominal do gerador, inserida em MVA, kVA ou VA.

Esse campo é especialmente importante caso a opção "Utilizar a potência nominal como base" esteja marcada.

#### Potências ativa e reativa
Potências ativa (inserida em W, kW, MW ou $p.u.$) e reativa (inserida em var, kvar, Mvar ou $p.u.$) do motor.

:::caution Atenção!
Caso a opção “Calcular a potência reativa no fluxo de carga” esteja ativada, o campo de potência reativa é desativado para edição.
:::

#### Calcular a potência reativa no fluxo de carga
Caso essa opção seja marcada, o programa irá utilizar os dados fornecidos no formulário de estabilidade para calcular a potência reativa do motor durante o processo iterativo do fluxo de carga.

:::warning Cuidado!
Caso essa opção não seja utilizada o motor será considerado uma [carga de potência constante](load) no estudo de [fluxo de carga](powerFlow).

A não utilização dessa opção poderá gerar erros de regime permamente no estudo de [estabilidade](stability).
:::

Utilizar potência nominal como base
Caso essa opção seja marcada, o programa irá utilizar a potência nominal do gerador como base para a conversão das unidades, inclusive aqueles no formulário de estabilidade, caso contrário será usada a potência base do sistema.

---
### Estabilidade

#### Imprimir dados da máquina de indução
Exibe os dados do gerador síncrono nos gráficos no tempo. Os seguintes dados são exibidos:
- tensão terminal
- potências ativa e reativas
- corrente, torque elétrico
- torque mecânico
- velocidade
- escorregamento

#### Inércia
Inércia do conjunto motor-carga, em s.

#### Resistência e reatância do estator
Resistência e reatância do estator do motor de indução, em $p.u.$

#### Resistência e reatância do rotor
Resistência e reatância do rotor do motor de indução, em $p.u.$

#### Reatância de magnetização
Reatância do ramo de magnetização do motor de indução, em $p.u.$

#### Utilizar fator de gaiola
Permite a inserção de um fator de gaiola ($K_g$) no motor de indução. Tal fator eleva o valor da resistência do rotor em condições de grande escorregamento, como ocorre em motores de gaiola dupla ou gaiola profunda. A resistência do rotor segue a seguinte equação:

$$
r_2 = r_{2_0} \left( 1+K_g s \right)
$$
Em que:
- $r_{2_0}$ é a resistência do rotor inserida no formulário.

#### Característica da carga
Descreve a característica do torque da carga mecânica no eixo do motor seguindo a equação do torque mecânico.
$$
T_m \propto \bold{a} + \bold{b}\omega + \bold{c}\omega^2
$$

:::warning Cuidado!
A soma dos coeficientes ($a + b + c$) deve ser unitária.
:::

---
### Botão Chaveamento
O botão "Chaveamento" irá abrir um formulário, comum a vários outros elementos, que permite a inserção e/ou remoção do gerador durante o estudo de [estabilidade](stability).

Nesse formulário pode ser criada uma lista genérica de inserções e remoções da linha no tempo, personalizada por um contexto de propriedades de chaveamento que são editados o tipo de chaveamento (inserção ou remoção) e o instante (em segundos) do evento. Essas propriedades são atribuídas e retiradas da lista genérica por meio dos botões "Adicionar" e "Remover", respectivamente.

## Referências
1. SÁNCHEZ, J. C.; OLIVARES, T. I. A.; ORTIZ, G. R.; VEGA, D. R. Induction Motor Static Models for Power Flow and Voltage Stability Studies. In: IEEE Power and Energy Society General Meeting, 2012, San Diego. doi: https://doi.org/10.1109/PESGM.2012.6345618
2. IEEE Std 399-1997. IEEE Recommended Practice for Industrial and Commercial Power Systems Analysis (Brown Book). IEEE, New York, ago. 1998. doi: https://doi.org/10.1109/IEEESTD.1998.88568
3. MILANO, F. Power System Modelling and Scripting. London: Springer, 2010. doi: https://doi.org/10.1007/978-3-642-13669-6
4. ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286