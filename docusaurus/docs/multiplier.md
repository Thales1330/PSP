---
id: multiplier
title: Multiplicador
sidebar_label: Multiplicador
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Esse elemento é bastante simples e cumpre com a função matemáticas de seu nome.

Possui duas entradas de valores reais e sua saída é computada na sequência do nó superior pelo inferior, ou seja, a saída será o valor presente no nó 1 multiplicado pelo nó 2.

:::caution Atenção!
Nós não conectados no multiplicador são assumidos com o valor unitário e para o divisor é considerado valor nulo.
:::