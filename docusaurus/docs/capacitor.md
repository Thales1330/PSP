---
id: capacitor
title: Capacitor
sidebar_label: Capacitor
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

>Um dispositivo constituído essencialmente por dois eletrodos separados por um dielétrico. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=436-01-03).

## Capacitor no PSP-UFU
O elemento capacitor representa, geralmente, um banco de capacitores *shunt* no circuito do PSP-UFU.

:::tip Dica
Um arranjo série de uma [linha](line) sem as susceptâncias *shunt*, um [barramento](bus) e um [capacitor](capacitor) pode ser utilizado para fabricar um filtro passivo nos [estudos harmônicos](harmonics). Em versões futuras, um elemento de filtro passivo será implementado no PSP-UFU.
:::

Os capacitores e reatores exigem somente sua potência reativa como parâmetro. Esses elementos passivos são somente inseridos na matriz admitância cuja impedância é calculada a partir de sua potência e tensão nominais.

## Formulário de edição dos capacitores
A imagem abaixo apresenta o formulário de inserção/alteração de dados dos capacitores:

<img src={useBaseUrl("images/capacitorForm.png")} alt="Formulário dos capacitores no PSP-UFU" title="Formulário dos capacitores no PSP-UFU" />

Além do único contexto geral, pode ser observado o botão "Estabilidade" na parte inferior esquerda do formulário. Esse formulário, comum a vários outros elementos, permite a inserção e/ou remoção do capacitor durante o estudo de [estabilidade](stability).

<img src={useBaseUrl("images/swCapacitor.png")} alt="Formulário de chaveamento do capacitor" title="Formulário de chaveamento do capacitor" />

---
### Geral

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Potência reativa
Os capacitores e reatores exigem somente sua potência reativa como parâmetro, inserida em Mvar, kvar, var ou $p.u.$ (na base de potência do sistema). 

---
### Botão Estabilidade
O botão "Estabilidade" irá abrir um formulário, comum a vários outros elementos, que permite a inserção e/ou remoção do transformador durante o estudo de [estabilidade](stability).

Nesse formulário pode ser criada uma lista genérica de inserções e remoções da linha no tempo, personalizada por um contexto de propriedades de chaveamento que são editados o tipo de chaveamento (inserção ou remoção) e o instante (em segundos) do evento. Essas propriedades são atribuídas e retiradas da lista genérica por meio dos botões "Adicionar" e "Remover", respectivamente.