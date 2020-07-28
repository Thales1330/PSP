---
id: syncMotor
title: Compensador Síncrono
sidebar_label: Compensador Síncrono
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

> Uma máquina síncrona operando sem carga mecânica e fornecendo ou absorvendo energia reativa. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=411-34-03).

## Compensador Síncrono no PSP-UFU
O compensador síncrono nada mais é que um motor síncrono operando sem carga em seu eixo com o intuito de fornecer ou absorver potência reativa na rede. Em relação ao [fluxo de carga](powerFlow), no PSP-UFU esse elemento tem comportamento idêntico ao [gerador síncrono](syncGenerator), porém a sua potência ativa é inserida nos algoritmos de solução com sinal negativo.

Sua parametrização para o estudo de [fluxo de carga](powerFlow), [falta](fault) e [harmônicos](harmonics) é idêntica ao [gerador síncrono](syncGenerator).

:::warning Cuidado!
O compensador síncrono **não foi implementado no estudo de [estabilidade](stability)** e sua presença no circuito pode causar erros durante os cálculos.

Não inclua esse elemento para os estudos de [estabilidade](stability). Versões futuras do PSP-UFU irão contemplar esse elemento para esse estudo.
:::

:::tip Dica
É possível utilizar um [gerador síncrono](syncGenerator) com potência ativa *nula* ou *negativa* para representar o compensador síncrono nos estudos de [estabilidade](stability).
:::

## Formulário de edição dos compensadores síncronos
A imagem abaixo apresenta o formulário de inserção/alteração de dados dos compensadores síncronos:

<div><center><img src={useBaseUrl("images/syncMotorForm.png")} alt="Formulário dos compensadores síncronos no PSP-UFU" title="Formulário dos compensadores síncronos no PSP-UFU" /></center></div>


<Tabs
  groupId="syncMotor-tabs"
  defaultValue="general"
  values={[
    {label: 'Geral', value: 'general'},
    {label: 'Falta', value: 'fault'},
  ]
}>
<TabItem value="general">

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Potência nominal
Potência nominal do compensador, inserida em MVA, kVA ou VA.

Esse campo é especialmente importante caso a opção "Utilizar a potência nominal como base" esteja marcada.

#### Potências ativa e reativa
Potências ativa (inserida em W, kW, MW ou p.u.) e reativa (inserida em var, kvar, Mvar ou p.u.) do compensador.

Caso a barra conectada seja PV o valor de potência reativa será ignorado e caso seja de referência ambos os valores inseridos serão desprezados.

:::caution Atenção!
Caso mais de um compensador esteja conectado na mesma barra, os valores de potência reativa (nas barras de referência e PV) e ativa (nas barras de referência) são igualmente distribuídas, respeitando os limites individuais de potência reativa.
:::

#### Potências reativas máxima e mínima
Limites de potência reativa máxima e mínima do compensador para controle de tensão em barras PV. Caso esses valores sejam ultrapassados, o reativo gerado pela unidade será limitado ao valor inserido e a barra conectada será transformada em PQ, não controlando a tensão estabelecida.

#### Utilizar potência nominal como base
Caso essa opção seja marcada, o programa irá utilizar a potência nominal do compensador como base para a conversão das unidades, inclusive aqueles no formulário de estabilidade, caso contrário será usada a [potência base do sistema](simulationConfig).

</TabItem>
<TabItem value="fault">

#### Impedâncias de sequência
Valores de resistência e reatância para cálculo das correntes de falta. São inseridos dados de sequência positiva, negativa e zero.

#### Impedância de aterramento
Valores utilizados para o cálculo das correntes de falta do tipo fase-terra e fase-fase-terra. Caso o neutro do gerador não seja aterrado, o valor inserido nesse campo é ignorado.

#### Neutro aterrado
Indica se o neutro do gerador é aterrado.

</TabItem>
</Tabs>