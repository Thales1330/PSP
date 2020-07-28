---
id: limiter
title: Limitador absoluto
sidebar_label: Limitador absoluto
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Os blocos de limite absoluto cumprem a função de restringir os valores de entrada de acordo com os limites máximos e mínimos inseridos pelo usuário. Caso a entrada ultrapasse algum dos limites impostos, a saída será igual a esse valor limite até que a entrada retorne para a faixa permitida.

As restrições máximas e mínimas de valores dos sistemas de controle são modeladas utilizando um Limitador, cuja implementação é bastante simples e segue a seguinte expressão:
$$
y_n = 
\begin{cases}
L_{sup}{,}	& \text{se } u_n > L_{sup} \\
u_n{,}		& \text{se } L_{inf} \le u_n \le L_{sup}\\
L_{inf}{,}	& \text{se } u_n < L_{inf}
\end{cases}
$$
Em que:
- $y_n$	Éé o valor de saída atual do bloco
- $L_{sup}$ e $L_{inf}$	são os limites superior e inferior, respectivamente
- $u_n$	é o valor de entrada atual do bloco

## Formulário de edição do bloco Limitador
A figura abaixo apresenta o formulário de edição de dados do bloco limitador.

<div><center><img src={useBaseUrl("images/limiterForm.png")} alt="Formulário de edição de dados do bloco limitador no PSP-UFU" title="Formulário de edição de dados do bloco limitador no PSP-UFU" /></center></div>

Esse bloco não linear é definido pelo **limite superior e inferior**, inseridos pelo usuário.

:::info Informação
Tais blocos são bastante encontrados nos sistemas de controle para limitar a atuação do controlador, além de representar limites físicos de modelos implementados, como, por exemplo, valores máximos de corrente de excitação ou potência mecânica fornecida pelas turbinas.
:::