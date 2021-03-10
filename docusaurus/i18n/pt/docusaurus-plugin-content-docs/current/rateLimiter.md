---
id: rateLimiter
title: Limitador de taxa
sidebar_label: Limitador de taxa
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Os blocos de limite de taxa têm como objetivo conter a taxa de crescimento ou decrescimento da sua entrada a valores inseridos pelo usuário. A taxa de variação é calculada a partir dos valores de entrada atual e passado e, caso seja violada, será definida uma saída que respeite essas taxas (de crescimento ou decrescimento).

A taxa ($r$) é calculada de acordo com a equação:
$$
r = \frac{u_n - y_{n-1}}{h}
$$
Em que:
- $u_n$	é a entrada atual do bloco
- $y_{n-1}$	é o valor de saída anterior do bloco
- $h$	é o passo de integração

Obtida a taxa, o bloco irá resultar em um valor calculado pelas seguintes condições:
$$
\begin{cases}
h \times R_{cre}{,}	& \text{se } r > R_{cre} \\
u_n{,}		& \text{se } R_{dec} \le r \le R_{cre}\\
h \times R_{dec}{,}	& \text{se } r < R_{dec}
\end{cases}
$$
Em que:
- $R_{cre}$ e $R_{dec}$	são as taxas de crescimento e decaimento, respectivamente

## Formulário de edição de dados do Limitador de taxa
A figura abaixo apresenta o formulário de edição de dados do bloco limitador de taxa.

<div><center><img src={useBaseUrl("images/rateLimitForm.png")} alt="Formulário de edição de dados do bloco limitador de taxa no PSP-UFU" title="Formulário de edição de dados do bloco limitador de taxa no PSP-UFU" /></center></div>

Esse bloco é definido pelo **limite superior (taxa de crescimento máxima) e inferior (taxa de decrescimento máxima)**, inseridos pelo usuário.

:::info Informação
Esses blocos são úteis para limitar variações bruscas na entrada que possam comprometer o controle ou limites físicos de modelos de turbina, por exemplo.
:::