---
id: line
title: Linha
sidebar_label: Linha
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

>Um meio de transmissão fabricado usado para transmitir energia eletromagnética entre dois pontos com um mínimo de radiação. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=704-02-02).

## Linha no PSP-UFU
As linhas no PSP-UFU são modelos $\pi$ equilibradas. Podem ser utilizadas como linhas de transmissão e distribuição de sistemas elétricos de potência.

A figura abaixo mostra o modelo $\pi$ implementado no PSP-UFU:

<div><center><img src={useBaseUrl("images/lineModel.svg")} alt="Modelo PI da linha" title="Modelo PI da linha" /></center></div>

Em que:
- $\bold{r_L}$	é a resistência da linha;
- $\bold{x_L}$	é a reatância indutiva da linha;
- $\bold{b_{L}^{d}}$	é a susceptância capacitiva *shunt* da linha.

:::caution Atenção
O modelo utilizado no PSP-UFU é **equilibrado** e **não possui impedâncias mútuas** entre as fases.
:::

A linha pode ser inseridas com pontos de ancoragem, ou "nós", para maior personalização gráfica do elemento, como apresentado nas [Ferramentas CAD](cadTools#line).

:::warning Cuidado!
A linha deve ser inserida entre duas barras de **mesma tensão nominal**. Caso você tente inseri-la entre barras de tensão diferentes, uma mensagem de erro será exibida.
:::

:::tip Dica
Um arranjo série de uma [linha](line) sem as susceptâncias *shunt*, um [barramento](bus) e um [capacitor](capacitor) pode ser utilizado para fabricar um filtro passivo nos [estudos harmônicos](harmonics). Em versões futuras, um elemento de filtro passivo será implementado no PSP-UFU.
:::

## Formulário de edição das linhas

A imagem abaixo apresenta o formulário de inserção/alteração de dados das linhas de transmissão:

<div><center><img src={useBaseUrl("images/lineForm.png")} alt="Formulário das linhas no PSP-UFU" title="Formulário das linhas no PSP-UFU" /></center></div>

Esse formulário é subdividido em dois contextos distintos:
- **Geral**: no qual são inseridas informações gerais da linha e informações do fluxo de carga;
- **Falta**: local onde as impedâncias de sequência zero são inseridas.

:::info Informação
Os parâmetros necessários para construção da linha segundo seu modelo $\pi$ são inseridos na aba Geral, utilizados para construção da matriz admitância de sequência positiva e negativa.

Dados adicionais de impedâncias de sequência zero necessário para o cálculo de curtos-circuitos desbalanceados são editados na aba Falta, utilizados na construção da matriz admitância de sequência zero.
:::

Além desses dois contextos, pode ser observado o botão "Estabilidade" na parte inferior esquerda do formulário. Esse formulário, comum a vários outros elementos, permite a inserção e/ou remoção da linha durante o estudo de [estabilidade](stability).

<div><center><img src={useBaseUrl("images/lineSw.png")} alt="Formulário de chaveamento da linha" title="Formulário de chaveamento da linha" /></center></div>

<Tabs
  groupId="line-tabs"
  defaultValue="general"
  values={[
    {label: 'Geral', value: 'general'},
    {label: 'Falta', value: 'fault'},
    {label: 'Botão Estabilidade', value: 'stability'},
  ]
}>
<TabItem value="general">

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Tensão nominal
Campo de informação *não editável* que apresenta a tensão nominal da linha. Para alterar esse campo é necessário editar o campo correspondente do [barramento](bus#nome) conectado.


#### Potência nominal
Potência nominal da linha, inserida em MVA, kVA ou VA.

Esse campo é especialmente importante caso a opção "Utilizar a potência nominal como base" esteja marcada.

#### Parâmetros do modelo $\pi$
Resistência ($r_L$), Reatância indutiva ($x_L$) série e Susceptância capacitiva shunt **total** ($2b_{L}^{d}$) presentes no modelo $\pi$ da linha.

Esses parâmetros são inseridos em $p.u.$, $\Omega$ ou $\Omega/km$ ($S$ ou $S/km$ para $b_{L}^{d}$).

#### Comprimento da linha
Utilizado para calcular o valor dos parâmetros da linha inseridos em $\Omega/km$ (ou $S/km$).

:::info Informação
O comprimento da linha é ignorado caso não sejam utilizadas as unidades por quilometro.
:::

#### Utilizar potência nominal como base
Caso essa opção seja marcada, o programa irá utilizar a potência nominal da linha como base para a conversão das unidades, caso contrário será usada a [potência base do sistema](simulationConfig).

</TabItem>
<TabItem value="fault">

#### Parâmetros do modelo $\pi$ de sequência zero
Esses parâmetros são necessários para o correto cálculo das correntes de [falta desbalanceadas](fault) (fase-fase, fase-fase-terra e fase-terra) e devem ser inseridos em $p.u.$

:::warning Cuidado!
A não inserção desses dados acarretarão em resultados incorretos para [faltas desbalanceadas](fault).
:::

</TabItem>
<TabItem value="stability">

O botão "Estabilidade" irá abrir um formulário, comum a vários outros elementos, que permite a inserção e/ou remoção da linha durante o estudo de [estabilidade](stability).

Nesse formulário pode ser criada uma lista genérica de inserções e remoções da linha no tempo, personalizada por um contexto de propriedades de chaveamento que são editados o tipo de chaveamento (inserção ou remoção) e o instante (em segundos) do evento. Essas propriedades são atribuídas e retiradas da lista genérica por meio dos botões "Adicionar" e "Remover", respectivamente.

</TabItem>
</Tabs>
