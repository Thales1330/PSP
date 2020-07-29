---
id: powerFlow
title: Fluxo de Potência
sidebar_label: Fluxo de Potência
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

Um estudo fundamental no planejamento da expansão e operação de um sistema elétrico é o **fluxo de potência (ou fluxo de carga)** uma vez que a operação satisfatória desse sistema depende do conhecimento dos efeitos da interligação, de novas cargas, de novas centrais geradoras e de novas linhas antes que elas sejam instaladas. Esse estudo tem como objetivo encontrar o fluxo de potência nos ramos e as tensões nodais do Sistema Elétrico de Potência (SEP) impostas pela geração e a carga.

As equações formuladas a partir dos modelos dos elementos presentes no estudo de fluxo de carga são não lineares e não possuem solução analítica explícita. Portanto é necessária a utilização de métodos numéricos iterativos para solução do problema de fluxo de carga.

## Formulação do problema de fluxo de carga
O problema de fluxo de carga pode ser representado por um sistema de equações e inequações algébricas não-lineares que correspondem às leis de Kirchhoff e a um conjunto de restrições de operação impostos pelos componentes de uma rede elétrica.
Na formulação do problema a cada barra da rede são associadas quatro variáveis, sendo que duas delas surgem como dados e duas como incógnitas (em uma barra de índice $i$):
- $V_i$	é a magnitude da tensão na barra $i$;
- $θ_i$	é o ângulo da tensão na barra $i$;
- $P_i$	é a injeção líquida de potência ativa na barra $i$;
- $Q_i$	é a injeção líquida de potência reativa na barra $i$.

Relativo às variáveis que são incógnitas e os dados do sistema, podem ser definidos **três tipos de barras**:
- **Barra PQ**:	$P_i$ e $Q_i$ são dados, $V_i$ e $θ_i$ são calculados;
- **Barra PV**:	$P_i$ e $V_i$ são dados, $Q_i$ e $θ_i$ são calculados;
- **Barra de Referência**	$V_i$ e $θ_i$ são dados, $P_i$ e $Q_i$ são calculados.

:::info Informação
As **barras PQ** geralmente são representadas pelos barramentos de carga, os quais não há geração e controle de tensão.

As **barras PV** se caracterizam pelo controle de tensão mediante a injeção ou absorção de potência reativa por meio do controle da excitação de uma máquina síncrona.

A **barra de Referência** (ou de folga, de oscilação) tem como função, assim como o próprio nome diz, servir de referência de tensão e ângulo do sistema. Essa barra é necessariamente geradora, uma vez que ela é responsável pelo equilíbrio do balanço de potência do sistema.
:::

:::caution Atenção!
O tipo de barra deve ser definido no [elemento barramento](bus).

Note que **o sistema deve possuir somente uma barra de referência**.
:::

Como mencionado anteriormente, as equações são não-lineares e a solução analítica não é prática. As soluções dessas equações seguem **[processos iterativos](https://en.wikipedia.org/wiki/Iterative_method)**, em que são atribuídos valores estimados (ou iniciais) para as barras com tensões desconhecidas e, baseado na potência ativa e reativa e módulo da tensão especificados, calcula-se por meio das equações previamente apresentadas as novas tensões complexas em cada nó do sistema.

Na sequência, esse conjunto de valores para as tensões em cada barra é utilizado para novamente calcular outro grupo de tensões. Cada cálculo de um novo conjunto de tensões é chamado iteração. O processo iterativo é repetido até que as mudanças em todas as barras sejam menores do que um valor pré-estipulado, obtendo assim a convergência.

## Execução do fluxo de carga no PSP-UFU
Após a construção do diagrama unifilar no [editor de potência](powerEditor), a execução do fluxo de carga é realizada no [menu Simulação](mainScreen#menu-ribbon) clicando no botão **Fluxo de carga**.

<div><center><img src={useBaseUrl("images/menuSimulationPF.svg")} alt="Menu Simulação" title="Menu Simulação" /></center></div>

:::tip Dica
Caso o fluxo de carga tenha sido executado com sucesso, as setas de potência serão exibidas, a barra de status indicará sucesso na operação e os [elementos de texto](text) serão atualizados.
:::

**Outra possibilidade** é a execução por meio do cálculo contínuo, também presente no [menu Simulação](mainScreen#menu-ribbon) e seu acionamento é realizado co clicar no botão **Habilitar solução**. Com essa opção, os cálculos estáticos selecionados nas [configurações de simulação](simulationConfig) são automaticamente realizados ao modificar quaisquer parâmetros da rede, como dados elétricos e acionamento dos disjuntores dos elementos (remoção ou inserção).

:::caution Atenção!
Evite construir o circuito com o cálculo contínuo habilitado, uma vez que configurações temporárias podem levar a erros de execução da simulação.

Para desabilitar o cálculo contínuo clique no botão **Desabilitar solução**.
:::

### Erros comuns na execução do fluxo de carga
A seguir são apresentados os erros mais comuns relacionados ao fluxo de carga.

#### A seguinte mensagem de erro é exibida: "O número máximo de iterações foi alcançado"
Essa mensagem de erro é exibida quando o método de solução numérica selecionado nas [configurações de simulação](simulationConfig) atinge o número máximo de iterações inserido. As seguintes situações podem ocasionar esse erro:
- **Os parâmetros do circuito estão incorretos**. caso parâmetros muito fora dos valores padrão dos elementos elétricos sejam inseridos, o cálculo de fluxo de carga pode divergir. Verifique se os dados foram inseridos corretamente.
- **Algum elemento possui parâmetros discrepantes dos demais**. Caso um dos elementos inseridos possua um valor de impedância muito distinto dos demais, como por exemplo uma linha com impedância muito elevada ou uma carga muito pequena, pode levar à divergência do método numérico. Nesse caso, reconsidere a necessidade de representação desses elementos no circuito e alterne entre os métodos numéricos de solução disponíveis.
- **O número máximo de iterações está muito baixo**. Alguns circuitos exigem um número maior de iterações, portanto altere o valor do máximo de iterações nas [configurações de simulação](simulationConfig). Também tente alternar entre os métodos numéricos de solução disponíveis.
- **Os parâmetros de simulação estão inadequados**. Caso um parâmetro do método de solução esteja inadequado, como fator de aceleração ou tolerância, o cálculo pode não alcançar a convergência. Altere esses parâmetros nas [configurações de simulação](simulationConfig).

#### Os dados de saída são exibidos como "NaN" ou "nan"
Isso ocorre devido a erros de operações matemáticas nos cálculos de fluxo de carga. "NaN" significa *Not a Number*.
- **Algum barramento está isolado**. Esse erro é bastante comum e pode ocorrer ao inserir um barramento sem conectá-lo ao sistema ou ao remover os elementos de ramo que conectam uma barra ao sistema. A solução é eliminar essa barra do diagrama.
- **Os parâmetros do circuito estão incorretos**. caso parâmetros muito fora dos valores padrão dos elementos elétricos sejam inseridos, o cálculo de fluxo de carga pode divergir. Verifique se os dados foram inseridos corretamente.
- **Algum elemento possui parâmetros discrepantes dos demais**. Caso um dos elementos inseridos possua um valor de impedância muito distinto dos demais, como por exemplo uma linha com impedância muito elevada ou uma carga muito pequena, pode levar à divergência do método numérico. Nesse caso, reconsidere a necessidade de representação desses elementos no circuito e alterne entre os métodos numéricos de solução disponíveis.

## Métodos de solução numérica do fluxo de carga no PSP-UFU
Os métodos implementados no programa para solução do problema de fluxo de carga no PSP-UFU são **Gauss-Seidel (GS)** e **Newton-Raphson (NR)**. Além desses métodos clássicos, um método híbrido pode ser utilizado (definido nas [configurações de simulação](simulationConfig)), em que é utilizado inicialmente o GS e na sequência o NR, aumentando a chance de convergência do NR.

### Gauss-Seidel
O método de Gauss-Seidel tem sido bastante utilizado nas últimas décadas para solução do problema de fluxo de carga, uma vez que não há a necessidade de fatorar a matrizes, reduzindo o esforço computacional. Atualmente, restrições computacionais são menos problemáticas e outros métodos são normalmente escolhidos, porém o Gauss-Seidel ainda possui valor didático e, visto que o PSP-UFU também possui fins educacionais, optou-se pela implementação desse método.

Para iniciar as iterações do método são necessários valores iniciais para as tensões ($\dot{V}_i^0$) que devem ser calculadas. Normalmente para barras do tipo PQ tem-se $\dot{V}_i^0=1{,}0+j0{,}0~p.u.$ e para barras do tipo PV $\dot{V}_i^0=V_i^{esp}+j0{,}0$ [p.u.], em que $V_i^{esp}$ é o módulo da tensão especificada para a barra PV. A barra de referência tem o módulo e ângulo de tensão fixos e não participam no processo iterativo.

O método de Gauss-Seidel mostra um **número excessivo de iterações** e, com o intuito reduzi-los, multiplicam-se as correções de tensões por uma constante. Essa operação amplia o valor da correção, trazendo a tensão para mais perto do valor do valor final. Os multiplicadores que realizam essa convergência melhorada são chamados de fatores de aceleração. Para qualquer sistema existem valores ótimos para os fatores de aceleração e uma escolha inadequada pode resultar em uma convergência mais lenta ou torná-la impossível. Normalmente é utilizado um fator de aceleração igual a 1,6, valor definido como padrão no programa. A utilização do fator de aceleração é realizada por meio da equação:
$$
\dot{V}_{i_{AC}}^{(v + 1)} = \alpha \left( \dot{V}_{i}^{(v + 1)}- \dot{V}_{i_{AC}}^{(v)} \right) + \dot{V}_{i_{AC}}^{(v)}
$$
Em que:
- $\dot{V}_{i_{AC}}^{(v + 1)}$ é a tensão complexa da iteração atual com fator de aceleração aplicado
- $\dot{V}_{i}^{(v + 1)}$ é a tensão complexa da iteração atual
- $\dot{V}_{i_{AC}}^{(v)}$ é a tensão complexa da iteração anterior com fator de aceleração aplicado
- $\alpha$ é o fator de aceleração

O fluxograma abaixo demonstra como o método de Gauss-Seidel foi implementado no PSP-UFU:

<div><center><img src={useBaseUrl("images/gaussSeidel.svg")} alt="Método numérico de Gauss-Seidel para fluxo de carga" title="Método numérico de Gauss-Seidel para fluxo de carga" /></center></div>

### Newton-Raphson
O método de Newton-Raphson (também conhecido como método de Newton ou Newton-Fourier) para solução do fluxo de carga é descrito em vários livros e artigos. Atualmente é o algoritmo mais utilizado para solução do fluxo de carga. Para casos bem condicionados, esse método geralmente converge em 4 a 5 iterações, porém existe a possibilidade da técnica contornar o ponto da solução sem nunca atingi-la, o que também justifica implementação do método de Gauss-Seidel no *software*.

A expansão da [série de Taylor](https://en.wikipedia.org/wiki/Taylor_series) para uma função de duas ou mais variáveis é a base do método de Newton-Raphson para resolver o problema do fluxo de carga. Fazendo a expansão em série de Taylor, para duas equações e duas incógnitas ($f_1\left( x_{1}, x_{2} \right)$ e $f_2\left( x_{1}, x_{2} \right)$), sem listar as derivadas parciais maiores que 1 na forma matricial, obtêm-se:
$$
\begin{bmatrix}
K_1 - f_1\left( x_{1}^{(0)}, x_{2}^{(0)} \right)\\
K_2 - f_2\left( x_{1}^{(0)}, x_{2}^{(0)} \right)
\end{bmatrix}
=
\begin{bmatrix}
\displaystyle\frac{\partial f_1}{\partial x_1} & \displaystyle\frac{\partial f_1}{\partial x_2}\\
\displaystyle\frac{\partial f_2}{\partial x_1} & \displaystyle\frac{\partial f_2}{\partial x_2}
\end{bmatrix}
\begin{bmatrix}
\Delta x_{1}^{(0)}\\
\Delta x_{2}^{(0)}
\end{bmatrix}
$$
Em que:
- $K$ é o resultado da equação $f\left( x_{1}, x_{2} \right)$
- $x^{(0)}$ é a estimativa iniciais de $x$
- $\Delta x$ é o valor acrescido de $x^{(0)}$ que resulta em $x$, ou seja: $f\left( x_{1}, x_{2} \right) = f_1\left( x_{1}^{(0)} + \Delta x_1, x_{2}^{(0)} + \Delta x_2 \right)$

Essa expressão pode ser resumida em:
$$
\begin{bmatrix}
\Delta K_1^{(0)}\\
\Delta K_2^{(0)}
\end{bmatrix}
=
\begin{bmatrix}
J
\end{bmatrix}^{(0)}
\begin{bmatrix}
\Delta x_{1}^{(0)}\\
\Delta x_{2}^{(0)}
\end{bmatrix}
$$
Em que $[J]$ é a **Matriz Jacobiana**

Com a equação acima é possível calcular os valores de $\Delta x_{1}^{(0)}$ e $\Delta x_{2}^{(0)}$. Entretanto, esses valores somados às estimativas iniciais não determinam a solução correta, sendo necessário repetir o processo de determinação das constantes, formação da matriz jacobiana e solução da equação acima, o qual será refeito determinando novas estimativas $\Delta x_{1}^{(1)}$ e $\Delta x_{2}^{(1)}$.

Esse processo é repetido até que as correções se tornem tão pequenas que satisfaçam uma precisão escolhida.

Para aplicar o método de Newton-Raphson à solução das equações do fluxo de carga, utiliza-se as equações que representam as potências ativa e reativa injetadas em uma barra.

Assim como no método de Gauss-Seidel, a barra de referência é omitida da solução iterativa para determinar as tensões, pois a tensão complexa dessa barra é especificada. Como é conhecido o valor da potência ativa injetada ($P_i^{esp}$) nas barras do tipo PQ e PV, além da potência reativa injetada ($Q_i^{esp}$) nas barras PQ pode-se definir $P_i^{esp}$ e $Q_i^{esp}$ como os valores de $K$. Os valores estimados do módulo e ângulo da tensão correspondem aos valores estimados para $x_1$ e $x_2$.

O jacobiano consiste nas derivadas parciais de $P_i$ e $Q_i$ em relação a cada uma das variáveis das equações injeção de potência líquida na barra $i$. A matriz coluna formada por $\Delta x_{1}$ e $\Delta x_{2}$ corresponde às correções de ângulo ($\Delta \theta_i$) e módulo ($\Delta V_i$) das tensões de barra.

Com isso pode-se escrever a equação matricial de um sistema de $n$ barras, em que a barra número 1 corresponde à barra de referência e as barras de número 2 a n são barras do tipo PQ:
$$
\begin{array}{r}
\text{NPQ} + \text{NPV}
\begin{cases}
~\\
~
\end{cases}\\
~\\
\text{NPQ}
\begin{cases}
~\\
~
\end{cases}
\end{array}
\begin{bmatrix}
\Delta P_2\\
\vdots\\
\Delta P_n\\
\Delta Q_2\\
\vdots\\
\Delta Q_n\\
\end{bmatrix}
=
\begin{bmatrix}
\displaystyle\frac{\partial P_2}{\partial \theta_2} & \dots & \displaystyle\frac{\partial P_2}{\partial \theta_n} & \displaystyle\frac{\partial P_2}{\partial V_2} & \dots & \displaystyle\frac{\partial P_2}{\partial V_n}\\
\vdots & \ddots & \vdots & \vdots & \ddots & \vdots\\
\displaystyle\frac{\partial P_n}{\partial \theta_2} & \dots & \displaystyle\frac{\partial P_n}{\partial \theta_n} & \displaystyle\frac{\partial P_n}{\partial V_2} & \dots & \displaystyle\frac{\partial P_n}{\partial V_n}\\
\displaystyle\frac{\partial Q_2}{\partial \theta_2} & \dots & \displaystyle\frac{\partial Q_2}{\partial \theta_n} & \displaystyle\frac{\partial Q_2}{\partial V_2} & \dots & \displaystyle\frac{\partial Q_2}{\partial V_n}\\
\vdots & \ddots & \vdots & \vdots & \ddots & \vdots\\
\displaystyle\frac{\partial Q_n}{\partial \theta_2} & \dots & \displaystyle\frac{\partial Q_n}{\partial \theta_n} & \displaystyle\frac{\partial Q_n}{\partial V_2} & \dots & \displaystyle\frac{\partial Q_n}{\partial V_n}
\end{bmatrix}
\begin{bmatrix}
\Delta \theta_2\\
\vdots\\
\Delta \theta_2\\
\Delta V_2\\
\vdots\\
\Delta V_n\\
\end{bmatrix}
$$
Em que:
- $\text{NPQ}$ é o número de barras PQ
- $\text{NPV}$ é o número de barras PV

O processo iterativo se inicia calculando as potências ativas ($P_i^{calc}$) para as barras PQ e PV e as potências reativas ($Q_i^{calc}$) para as barras PQ, ambas as equações utilizando as estimativas iniciais das tensões complexas. Calcula-se, então, as correções de potência ($\Delta P$ e $\Delta Q$):
$$
\Delta P = P_i^{esp} - P_i^{calc}\\
\Delta Q = Q_i^{esp} - Q_i^{calc}
$$

O passo seguinte é a formação da matriz jacobiana. Com isso é possível calcular as correções de módulo e ângulo das tensões de todas as barras (com exceção da barra de referência). Para tanto, no PSP-UFU utiliza-se o método de [Eliminação Gaussiana](https://en.wikipedia.org/wiki/Gaussian_elimination) e em sequência a substituição regressiva. Esse procedimento diminui o esforço computacional, uma vez que a inversão da matriz jacobiana em todas as iterações é evitada.

Com as correções de módulo e ângulo das tensões das barras calculados, aplicam-se as seguintes equações:
$$
	\theta_i^{(v+1)}=\theta_i^{(v)}+\Delta \theta_i^{(v)}\\
	V_i^{(v+1)}=V_i^{(v)}+\Delta V_i^{(v)}
$$

O processo é então reiniciado e será repetido até que se obtenha a convergência, quando as correções se tornam tão pequenas que satisfaçam uma tolerância pré-estipulada.

O fluxograma abaixo mostra o método de Newton-Raphson para solução do fluxo de carga implementado.

<div><center><img src={useBaseUrl("images/newtonRaphson.svg")} alt="Método numérico de Newton-Raphson para fluxo de carga" title="Método numérico de Newton-Raphson para fluxo de carga" /></center></div>

## Controles e limites em um problema de fluxo de carga
Nas barras de [geração](syncGenerator) e também naquelas em que se encontra um [compensador síncrono](syncMotor) conectado, o controle da magnitude da tensão no barramento é realizado por meio do ajuste da corrente de campo das máquinas síncronas, as quais podem operar sobrexcitadas (injetando reativos) ou subexcitadas (absorvendo reativos). Os valores limites de potência reativa que podem ser injetadas ou absorvidas dependem da máquina síncrona em estudo. Esses limites são incluídos no fluxo de carga com a criação de dois novos parâmetros, a potência reativa máxima ($Q_i^{máx}$) e potência reativa mínima ($Q_i^{min}$), sendo esses valores a soma dos limites individuais das máquinas em uma mesma barra genérica $i$.

A manutenção da potência reativa dentro dos limites é realizada pela troca do tipo de barra, ou seja, as barras violadoras que controlam a tensão (PV), passam a ser barras de carga (PQ), cuja potência é fixada como o limite que seria ultrapassado ($Q_i^{máx}$ ou $Q_i^{min}$) e a tensão deixa de ser controlada partir de então.

A verificação de violação e troca de tipo de barra pode ser realizada a cada iteração ou ao final da convergência do cálculo. No PSP-UFU foi implementada a última estratégia, uma vez que separa os conceitos de cálculo e de verificação de limites, tornando mais fácil o desenvolvimento de novos métodos numéricos e limites. Nessa abordagem, ao ajustar o sistema para a nova situação não violadora o cálculo iterativo deve ser retomado até que obtenha novamente a convergência.

## Referências
- MONTICELLI, A. J. Fluxo de Carga em Redes de Energia Elétrica. São Paulo: Edgar Blücher, 1983.
- STEVENSON JR.; WILLIAN, D. Elementos de Análise de Sistemas de Potência. 2ª ed. São Paulo: McGraw-Hill, 1986.
- MILANO, F. Power System Modelling and Scripting. London: Springer, 2010. doi: https://doi.org/10.1007/978-3-642-13669-6
- ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286
- TINNEY, W. F.; HART, C. E. Power Flow Solution by Newton’s Method. IEEE Transaction on Power Apparatus and Systems, v. PAS-86, n. 11, nov. 1967. doi: https://doi.org/10.1109/TPAS.1967.291823