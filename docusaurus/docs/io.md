---
id: io
title: Entrada / Saída
sidebar_label: Entrada / Saída
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

As entradas e saída são variáveis relacionadas às máquinas síncronas. Cada tipo de entrada e saída está relacionada a um regulador da máquina: **Regulador de Tensão (AVR, do inglês *Automatic Voltage Regulator*)** e **Regulador de Velocidade (RV)**.

:::warning Cuidado!
O sistema de controle deve ter **ao menos uma entrada e uma saída**.
:::

As entradas e saídas do sistema de controle são definidas por esses blocos, os quais são distintos para cada tipo de escopo. A figura abaixo apresenta um exemplo de um formulário de edição das entradas e saídas de um AVR:

<div><center><img src={useBaseUrl("images/ioForm.png")} alt="Formulário de edição de entradas e saídas de sistemas de controle de um AVR" title="Formulário de edição de entradas e saídas de sistemas de controle de um AVR" /></center></div>

## Variáveis de entrada e saída

O usuário deve sinalizar o tipo de bloco entre entrada e saída e, então, selecionar em uma lista a variável desejada. As seguintes variáveis de controle estão atualmente disponíveis no programa:

- **Tensão terminal** (*entrada: AVR*): Módulo da tensão no barramento da máquina síncrona, em $p.u.$, variável no tempo. Essa variável é normalmente utilizada no cálculo do erro da tensão de referência do AVR;
- **Velocidade** (*entrada: AVR e RV*): Velocidade da máquina síncrona, em rad/s, variável no tempo. Normalmente utilizada no cálculo do erro de velocidade nos reguladores de velocidade, além de entrada do PSS em AVRs;
- **Potência ativa e reativa** (*entrada: AVR*): Potência ativa fornecida pela máquina síncrona, em $p.u.$, variável no tempo. Normalmente utilizada como entrada do PSS (potência ativa) e controle de sub e sobrecorrente de excitação nos AVRs;
- **Tensão terminal inicial** (*entrada: AVR*): Módulo da tensão no barramento da máquina síncrona prévia ao estudo dinâmico originado do fluxo de carga, em $p.u.$, fixo no tempo. Essa variável está normalmente associada à referência de tensão do AVR;
- **Velocidade inicial** (*entrada: AVR e RV*): Velocidade do sistema (2πf_ref), definida nas opções de simulação, em rad/s, fixa no tempo. Normalmente utiliza-se essa variável como referência de velocidade em RVs e normalização da velocidade;
- **Potência mecânica inicial** (*entrada: RV*): Potência mecânica inicial, calculada após a inicialização das máquinas síncronas com os dados originados do fluxo de carga, em $p.u.$, fixa no tempo. Normalmente é utilizada como referência de potência mecânica nos reguladores de velocidade;
- **Variação de velocidade e potência ativa** (*entrada: AVR*): Cálculo da variação dessas entradas entre os passos de integração normalizada pelo passo de integração, conforme a equação:
	$$
	\Delta x = \frac{x_n-x_{n-1}}{h}
	$$
	Em que:
	- $\Delta x$	é a variação da entrada (velocidade ou potência ativa);
	- $x_n$ e $x_{n-1}$	é a variável no passo atual e anterior, respectivamente;
	- $h$	é o passo de integração.
	
	:::note Nota
	A normalização é necessária para a correta utilização da razão de passo de controle, definida nas [configurações de simulação](simulationConfig).
	:::
- **Tensão de campo** (*saída: AVR*): Define a tensão aplicada ao campo na máquina síncrona, em $p.u.$ Utilizada como saída dos AVRs, controlando principalmente tensão no barramento conectado e/ou fator de potência da máquina;
- **Potência mecânica** (*saída: RV*): Define qual a potência mecânica aplicada no eixo da máquina síncrona. Utilizada como saída dos reguladores de velocidade, controlando principalmente a potência ativa injetada pela máquina e sua frequência.
