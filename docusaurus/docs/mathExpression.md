---
id: mathExpression
title: Expressão Matemática
sidebar_label: Expressão Matemática
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

O bloco de expressão matemática utiliza a biblioteca externa [fparser](http://warp.povusers.org/FunctionParser/) e permite ao usuário inserir expressões matemáticas genéricas de qualquer complexidade combinadas a estruturas condicionais em um diagrama de controle, aumentando significativamente a capacidade de generalização do programa.

## Sintaxe das expressões matemáticas
As sintaxes das operações (para expressões `A` e `B`) são apresentadas na tabela abaixo:

|Operador|Expressão|
|--------|---------|
|`( )`	|Expressão em parênteses primeiro
|`A unit`	|Um multiplicador de unidade (se um estiver sido adicionado)
|`A^B`	|Exponenciação (`A` elevado à potência `B`)
|`-A`	|Operação unária de oposto
|`!A`	|Operação lógica unária de negação (resulta em $1$ se `int(A)` é $0$, senão $0$)
|`A*B` `A/B` `A%B`	|Multiplicação, divisão e módulo
|`A+B` `A-B`	|Adição e subtração
|`A=B` `A<B` `A<=B` `A!=B` `A>B` `A>=B`	|Comparação entre `A` e `B` (resulta em $1$ ou $0$)
|`A&B`	|Resulta em $1$ se `int(A)` e `int(B)` forem diferentes de $0$, senão $0$
|`A`\|`B`	|Resulta em $1$ se `int(A)` ou `int(B)` forem diferentes de $0$, senão $0$

As **funções matemáticas** suportadas pela biblioteca, as quais podem ser utilizadas na construção de diagrama de blocos, são descritas na tabela abaixo:

|Expressão|Descrição
|---------|---------|
|`abs(A)`|Valor absoluto de `A`. Para números reais, se `A` é negativo, retorna `–A`, senão retorna `A`. Para números complexos a expressão é equivalente a `hypot(real(x),imag(x))`.
|`acos(A)`|Arco cosseno de `A`. Retorna o ângulo em radianos.
|`acosh(A)`|O mesmo que `acos()` mas para cosseno hiperbólico
|`arg(A)`|Ângulo de fase de um número complexo `A`.
|`asin(A)`|Arco seno de `A`. Retorna o ângulo em radianos.
|`asinh(A)`|Mesmo que `asin()`, mas para seno hiperbólico
|`atan(A)`|Arco tangente de `A`. Retorna o ângulo em radianos.
|`atan2(A,B)`|Arco tangente de `A/B`, a qual os sinais dos dois argumentos para determinar o quadrante do resultado. Retorna a solução de do seguinte sistema: `hypot(A,B)*sin(x)=A, hypot(A,B)*cos(x)=B`. O valor retornado varia de $-\pi$ a $\pi$.
|`atanh(A)`|Mesmo que `atan()`, mas para tangente hiperbólica.
|`cbrt(A)`|Raiz cúbica de `A`.
|`conj(A)`|Conjugado complexo de `A`.
|`ceil(A)`|Teto de `A`. Arredonda para o próximo maior inteiro.
|`cos(A)`|Cosseno de `A`. Retorna o ângulo em radianos.
|`cosh(A)`|Mesmo que `cos()`, mas para cosseno hiperbólico
|`cot(A)`|Cotangente de `A`. Retorna o ângulo em radianos.
|`csc(A)`|Cossecante de `A`. Retorna o ângulo em radianos.
|`exp(A)`|Exponencial de `A`. Retorna o valor de e elevado a potência A.
|`exp2(A)`|Exponencial de `A` na base $2$.
|`floor(A)`|Piso de `A`. Arredonda para o próximo menor inteiro.
|`hypot(A,B)`|Função de distância Euclidiana.
|`if(A,B,C)`|Se `int(A)` é diferente de $0$, retorna o valor de `B`, senão retorna `C`.
|`imag(A)`|Retorna a parte imaginária do número complexo `A`.
|`int(A)`|Arredonda `A` para o inteiro mais próximo.
|`log(A)`|Logaritmo natural (base $e$) de `A`.
|`log2(A)`|Logaritmo na base $2$ de `A`.
|`log10(A)`|Logaritmo na base $10$ de A.
|`max(A,B)`|Se `A>B`, o resultado é `A`, senão é `B`.
|`min(A,B)`|Se `A<B`, o resultado é `A`, senão é `B`.
|`polar(A,B)`|Retorna o número complexo de magnitude `A` e ângulo de fase `B` (em radianos).
|`pow(A,B)`|Exponenciação (`A` elevado à potência `B`)
|`real(A,B)`|Retorna a parte real do número complexo `A`.
|`sec(A)`|Secante de `A`.
|`sin(A)`|Seno de `A`. Retorna o ângulo em radianos.
|`sinh(A)`|Mesmo que `sin()`, mas para seno hiperbólico.
|`sqrt(A)`|Raiz quadrada de `A`.
|`tan(A)`|Tangente de `A`. Retorna o ângulo em radianos.
|`tanh(A)`|Mesmo que `tan()`, mas para tangente hiperbólica.
|`trunc(A)`|Valor truncado de `A`. Retorna o número inteiro de `A` sem a parcela fracionada.

É possível atribuir novas variáveis utilizando a seguinte sintaxe: 

``` cpp
<nome da variável> := <expressão>; <função>
```

Por exemplo:
``` cpp
comprimento := sqrt(x*x+y*y); 2*comprimento*sin(comprimento)
```

:::tip Dica
O caractere de espaço e de nova linha são ignorados na interpretação da expressão, portanto para maior organização, o código anterior pode ser escrito da seguinte forma:
``` cpp
comprimento := sqrt(x*x+y*y);
2*comprimento*sin(comprimento)
```
:::

:::warning Cuidado!
Note que a expressão que determina o valor de saída do bloco **não possui** o caractere `;` em seu final.
:::

## Formulário de edição de dados do bloco de Expressão Matemática
O formulário de inserção e edição das entradas do bloco e da expressão matemática genérica, assim como ferramentas de auxílio de sua construção é apresentado na figura abaixo.

<div><center><img src={useBaseUrl("images/mathExpressionForm.png")} alt="Formulário de edição de dados do bloco de expressão matemática no PSP-UFU" title="Formulário de edição de dados do bloco de expressão matemática no PSP-UFU" /></center></div>

No campo de “Variáveis de entrada” é inserida uma lista com os nomes das entradas separados por espaços. Um número qualquer de entrada pode ser definido nessa lista esses nomes são apresentados no ícone gráfico presente no editor de controle, além de serem destacados na expressão inserida pelo usuário. O número de entradas e saídas se comporta de forma semelhante aos [blocos somadores](sum).

Abaixo do campo das variáveis de entrada está presente o local para inserção da expressão matemática. A sintaxe da expressão inserida pelo usuário possui realce (por meio de diferentes formas e cores da fonte) para números, operadores, variáveis de entrada, funções e constantes, facilitando a criação, manipulação e identificação de erros de digitação e lógica.

:::info Informação
Como ferramenta de auxílio ao usuário foi desenvolvida uma verificação da expressão inserida.
:::

Tal ferramenta irá encontrar erros e indicará ao usuário qual o tipo do erro, além da sua localização, destacando-o. A figura anterior exemplifica a identificação automática do erro pelo PSP-UFU, assim como sua posição na expressão inserida.

## Exemplo de Expressão Matemática
A corrente de campo em $p.u.$ pode ser estimada de forma aproximada utilizando as potências ativa ($P$) e reativa ($Q$), assim como as reatâncias transitórias de eixo direto ($x_d$) e em quadratura ($x_q$) e o módulo da tensão terminal ($V$) da máquina:
$$
i_f \approx \sqrt{ \left( V + Q' \right)^2 + P^2} + \left(\frac{x_d}{x_q} +1{,}0 \right) \times \frac{Q' \left( V + Q' \right) + {P'}^2}{\sqrt{\left( V + Q'\right)^2 + P^2}}
$$
Em que:
- $P' = x_q \times \frac{P}{V}$
- $Q' = x_q \times \frac{Q}{V}$

Primeiramente, deve-se inserir no campo "variáveis de entrada" as grandezas fornecidas pelo PSP-UFU. Nesse caso, como pode ser observado no bloco de [entrada / saída](io), todas as variáveis necessárias são fornecidas: $P$, $Q$ e $V$. Essas variáveis devem ser inseridas separadas por espaço: `p q v`.

Utilizando três blocos de [entrada / saída](io) pode-se fornecer tais dados ao conectá-los ao bloco de expressão matemática.

As reatâncias transitórias de eixo direto ($x_d$) e em quadratura ($x_q$) podem ser definidas diretamente na expressão matemática:
``` cpp
xd := 0.146;
xq := 0.0969;
```

Os valores de $P'$ e $Q'$ podem ser também calculados:
``` cpp
yp := xq * p / v;
yq := xq * q / v;
```

Com todos os dados necessários pode-se calcular a corrente de campo:
``` cpp
i_f := sqrt((v + yq)^2 + p^2) + (xd / xq + 1.0) *
       ((yq * (v + yq) + yp^2) / sqrt((v + yq)^2 + p^2));
```

Finalmente, pode-se definir a saída do bloco (não inserindo `;`):
``` cpp
i_f
```

Portanto, para as entradas definidas como `p q v`, a expressão matemática total para cálculo da corrente de campo será:
``` cpp
xd := 0.146;
xq := 0.0969;
yp := xq * p / v;
yq := xq * q / v;
i_f := sqrt((v + yq)^2 + p^2) + (xd / xq + 1.0) *
       ((yq * (v + yq) + yp^2) / sqrt((v + yq)^2 + p^2));
i_f
```

## Referências
- NIEMINEN, J.; YLILUOMA, J. Function Parser for C++ v4.5.2. Disponível em: \<http://warp.povusers.org/FunctionParser/\>. 