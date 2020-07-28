---
id: divider
title: Divisor
sidebar_label: Divisor
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Esse elemento é bastante simples e cumpre com a função matemáticas de seu nome.

Possui duas entradas de valores reais e sua saída é computada na sequência do nó superior pelo inferior, ou seja, a saída será o valor presente no nó 1 dividido pelo nó 2.

:::caution Atenção!
Nós não conectados no divisor são considerados como valor nulo. Caso a divisão possua numerador zero, o valor da saída é definida como sendo $10^{15}$.
:::