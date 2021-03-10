---
id: exponential
title: Exponencial
sidebar_label: Exponencial
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

A função exponencial dos elementos de controle realiza essa operação com uma entrada real, obedecendo a expressão:
$$
y_n = \bold{A} e^{\bold{B} u_n}
$$
Em que:
- $u_n$ e $y_n$	é a entrada e a saída do bloco exponencial, respectivamente;
- $\bold{A}$ e $\bold{B}$	são constantes definidas pelo usuário.

## Formulário de edição de dados do bloco Exponencial
A figura abaixo apresenta o formulário de edição de dados do bloco exponencial.

<div><center><img src={useBaseUrl("images/expForm.png")} alt="Formulário de edição de dados do bloco exponencial no PSP-UFU" title="Formulário de edição de dados do bloco exponencial no PSP-UFU" /></center></div>

Esse bloco não linear é definido pelas constantes A e B, inseridas pelo usuário.

:::info Informação
Tais blocos são úteis na representação de não linearidades, como, por exemplo, modelagem da saturação de máquinas de corrente contínuas presentes em alguns reguladores automáticos de tensão.
:::
