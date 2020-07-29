---
id: harmonics
title: Harmônicos
sidebar_label: Harmônicos
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

Duas ferramentas relacionadas a harmônicos foram desenvolvidas no PSP-UFU:
- Distorção Harmônica Total de Tensão (THD, do inglês *Total Harmonic Distortion*);
- Resposta na Frequência.

A ferramenta de **[Distorção Harmônica Total](https://en.wikipedia.org/wiki/Total_harmonic_distortion)** calcula as tensões harmônicas causadas por [fontes de corrente harmônicas](harmSource), assim como o THD de todos os barramentos do sistema.

A ferramenta de **Resposta na Frequência** (ou análise de varredura de frequência) envolve a variação da impedância da rede em um espectro de frequências observado a partir de um certo barramento.

:::info Informação
A análise de varredura de frequência é amplamente usada no projeto de filtros harmônicos.
:::

:::tip Dica
Um arranjo série de uma [linha](line) sem as susceptâncias *shunt*, um [barramento](bus) e um [capacitor](capacitor) pode ser utilizado para fabricar um filtro passivo nos [estudos harmônicos](harmonics). Em versões futuras, um elemento de filtro passivo será implementado no PSP-UFU.
:::

:::caution Atenção!
Na versão atual do programa não são consideradas as alterações das resistências do sistema causado pelo efeito pelicular. Versões futuras irão contemplar tal característica.
:::

## Execução do cálculo de harmônicos no PSP-UFU
Após a construção do diagrama unifilar no [editor de potência](powerEditor), a execução do cálculo das distorções harmônicas é realizada no [menu Simulação](mainScreen#menu-ribbon) clicando no botão **Distorções Harmônicas**. Para acessar a ferramenta de de varredura de frequência, basta clicar no botão **Resposta na Frequência**.

<div><center><img src={useBaseUrl("images/menuSimulationHamonics.svg")} alt="Acesso às ferramentas de estudo harmônico" title="Acesso às ferramentas de estudo harmônico" /></center></div>

### Distorções Harmônicas
Ao clicar sobre o botão "Distorções Harmônicas" as distorções causadas pelas [fontes de corrente harmônica](harmSource) são calculadas em todos os barramentos do sistema.

:::caution Atenção!
Caso não forem inseridas [fontes de corrente harmônica](harmSource) no sistema de potência, a distorção de tensão de todas as barras será $0{,}0~\%$.
:::

**Outra possibilidade** é a execução por meio do cálculo contínuo, também presente no [menu Simulação](mainScreen#menu-ribbon) e seu acionamento é realizado co clicar no botão **Habilitar solução**. Com essa opção, os cálculos estáticos selecionados nas [configurações de simulação](simulationConfig) são automaticamente realizados ao modificar quaisquer parâmetros da rede, como dados elétricos e acionamento dos disjuntores dos elementos (remoção ou inserção).

:::warning Cuidado!
Os cálculos dos níveis de THD **não são habilitados por padrão** no cálculo contínuo e devem ser inseridos nas [configurações de simulação](simulationConfig).
:::

Os resultados das distorções harmônicas são exibidos nos [elementos de texto vinculados](text) e ao posicionar o mouse sobre um barramento.

### Resposta na Frequência
Ao clicar sobre o botão "Resposta na Frequência" será exibido um formulário para inserção dos parâmetros da ferramenta:

<div><center><img src={useBaseUrl("images/injHarmCurrent.png")} alt="Acesso à ferramentas resposta na frequência" title="Acesso à ferramentas resposta na frequência" /></center></div>

#### Frequência inicial
Define a frequência inicial da varredura.

#### Frequência final
Define a frequência final da varredura.

#### Passo de frequência
Define o passo de incremento da frequência. Passos menores irão gerar gráficos mais precisos, porém podem aumentar muito o tempo de execução.

#### Corrente injetada
Define o barramento o qual será injetada a corrente harmônica para análise.
:::tip Dica
Esse campo pode ser entendido como: "*A resposta das impedâncias harmônicas vistas pelos barramentos do sistema caso tenha uma fonte de corrente harmônica na barra selecionada*".
:::

#### Botão Executar
Ao clicar no botão executar a varredura será executada e os resultados exibidos no [vizualizador de gráficos](graphViewer), como mostra a imagem abaixo.

<div><center><img src={useBaseUrl("images/freqScanPlot.png")} alt="Exemplo de resposta na frequência" title="Exemplo de resposta na frequência" /></center></div>

:::warning Cuidado!
Para visualizar a varredura da impedância harmônica de um barramento, essa opção deve ser habilitada em seu [formulário de edição de dados](bus#imprimir-impedância-harmônica-da-barra).
:::

## Cálculos das ferramentas harmônicas

Ambos cálculos são realizados a partir da seguinte equação matricial:
$$
[\dot{I}]^h= [Y_{bus}]^h [\dot{V}]^h
$$
Em que:
- $[\dot{I}]^h$	Vetor das correntes harmônicas injetadas nas barras
- $[Y_{bus}]^h$	Matriz admitância harmônica de barras
- $[\dot{V}]^h$	Vetor das tensões harmônicas nas barras
- $h$	Ordem harmônica

O vetor das correntes harmônicas injetadas é definido no programa utilizando um elemento de potência, chamado “fonte de corrente harmônica”, em que o usuário pode criar uma lista de correntes injetadas (em A e/ou $p.u.$) em um barramento.

O programa define automaticamente as ordens harmônicas na simulação analisando todas as listas de fontes de corrente harmônicas previamente aos cálculos. Com isso são calculadas as admitâncias harmônicas necessárias de cada elemento pela multiplicação das reatâncias indutivas ($x_{l}^1$) e susceptâncias capacitivas ($b_{c}^1$) fundamentais de cada elemento pelas ordens harmônicas:
$$
x_{l}^h=h×x_l^1\\
b_{c}^h=h×b_c^1
$$

Uma vez calculadas as admitâncias harmônicas, são utilizados os mesmos modelos e algoritmos convencionais para construção da $Y_{bus}^h$.

A equação matricial é resolvida usando o método de eliminação gaussiana para evitar a inversão de cada matriz de admitância harmônica. Este procedimento torna o cálculo das tensões harmônicas computacionalmente eficiente.

Uma vez calculadas as tensões harmônicas, a THD em uma barra genérica $i$ pode ser definida por:
$$
\text{THD}_i=\dfrac{\sum_{h=2}^{n} V_{i}^h}{V_i^1}
$$

Na ferramenta de Resposta na Frequência uma corrente senoidal é injetada na barra em uma faixa de frequências e o conjunto de equações matriciais é usado para calcular a resposta da tensão. Este cálculo é repetido em etapas discretas cobrindo o espectro de frequência especificado.

O programa usa a matriz admitância de sequência positiva e uma corrente injetada de $1{,}0 p.u.$ também de sequência positiva na equação matricial para calcular diretamente as impedâncias em p.u.

## Referências
1. ARRILLAGA, J.; WATSON, N. R. Power System Harmonics. John Wiley & Sons; Chichester, 2003. doi: https://doi.org/10.1002/0470871229
