---
id: controlEditor
title: Editor de Controle
sidebar_label: Editor de Controle
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Os controles das [máquinas síncronas](syncGenerator) são criados, modificados, importados e exportados em um editor distinto dos elementos de potência, apresentado na figura abaixo.

<div><center><img src={useBaseUrl("images/controlEditor.png")} alt="Editor de elementos de controle do PSP-UFU" title="Editor de elementos de controle do PSP-UFU" /></center></div>

O acesso ao editor é realizado por meio de dois botões presentes no [formulário de edição dos dados de estabilidade dos geradores síncronos](syncGenerator#formulário-de-edição-dos-geradores-síncronos):
- [Editar AVR](syncGenerator#utilizar-avr-e-regulador-de-velocidade)
- [Editar regulador de velocidade](syncGenerator#utilizar-avr-e-regulador-de-velocidade)

:::caution Atenção!
No PSP-UFU a opção de editar o **AVR** engloba mais que somente o controle de tensão da máquina. Nele **deve** ser inserida a malha de controle da máquina assim como a **excitatriz da máquina síncrona**. Outras estratégias de controle (opcionais), como PSS (*Power System Stabilizer*) e/ou controles de sobre e sub excitação, são também implementadas em conjunto (como apresentado na figura anterior).
:::

:::caution Atenção!
Assim como no AVR, o **Regulador de Velocidade** engloba mais que a regulação primária da máquina. Nessa opção **deve** ser inserida ao menos a malha de controle da regulação primária de velocidade, assim como o **modelo da turbina**. Estratégias opcionais de controle da velocidade também são inseridas nessa opção.
:::

## Elementos de Controle
O acesso aos elementos de controle é realizado em uma janela (removível e encaixável) por meio de ícones relacionados.

Uma vez que o usuário clicar no ícone desejado, o elemento de controle acompanhará o ponteiro do mouse até ser efetivamente inserido na posição desejada ao clicar novamente na área de trabalho. Os elementos são então conectados por “linhas de conexão” inseridas ao clicar nos nós dos componentes previamente adicionados, permitindo a construção da rede de controle genérica.

As ferramentas de manipulação e navegação, como arrastar, mover e excluir são [herdadas](cadTools) do [editor de elementos de potência](powerEditor), possuindo comportamento idêntico. A **edição dos dados** dos componentes inseridos também é realizada com **duplo clique** sobre o elemento inserido, exibindo um formulário de edição de dados.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/timelapseControlEditor.mp4" type="video/mp4" />
  <source src= "/PSP/videos/timelapseControlEditor.webm" type="video/webm" />
</video>

Os seguintes blocos de controle estão presentes no PSP-UFU:
- [Entrada e Saída](io)
- [Função Transferência](transferFunction)
- Matemática
	- [Somador](sum)
	- [Multiplicador](multiplier)
	- [Divisor](divider)
	- [Exponencial](exponential)
- Limitadores
	- [Limitador absoluto](limiter)
	- [Limitador de taxa](rateLimiter)
- Constantes
	- [Constante](const)
	- [Ganho](gain)
- [Expressão Matemática](mathExpression)

## Inicialização do sistema de controle
Caso as entradas do sistema de controle apresentem valores diferentes de zero ou a primeira operação entre elas não resulte em uma saída nula, o sistema necessita de inicialização, de forma a adequar os valores de entradas e saída dos blocos elementares e dos vetores de estado das [funções transferências](transferFunction) presentes. Tal procedimento é realizado por meio da solução de toda rede de controle até que se obtenha sua convergência, ou seja, a diferença absoluta entre as mesmas saídas de uma solução anterior e uma atual deve ser nula ou muito próxima de zero.

:::info
O processo de **inicialização é realizada automaticamente** pelo PSP-UFU.

Uma vez que a inicialização é imposta pelas [entradas](io) do controle, erros de convergência podem ocorrer devido à má parametrização dos elementos dos sistemas de **potência e controle**.
:::

Para otimizar e melhorar a estabilidade do processo de inicialização utilizou-se um passo de integração variável dentro de limites, de forma que o passo aumenta em condições de diferenças menores entre as soluções do sistema de controle e diminui caso essa diferença se torne elevada. A implementação dessa abordagem reduziu significativamente o encerramento do processo com erro causado pela instabilidade numérica, além de acelerar a inicialização.

O fluxograma abaixo evidencia o processo de inicialização implementado no PSP-UFU:

<div><center><img src={useBaseUrl("images/controlInit.png")} alt="Estrutura da inicialização do sistema de controle" title="Estrutura da inicialização do sistema de controle" /></center></div>

## Teste do diagrama de controle
Na parte inferior esquerda do Editor de Controle está presente o botão "**Testar Sistema...**". O botão acessa o formulário de teste de controles, como é indicado na figura abaixo:

<div><center><img src={useBaseUrl("images/testControl.png")} alt="Formulário de teste de controles" title="Formulário de teste de controles" /></center></div>

Nesse formulário é possível inserir o comportamento de **todas as entradas do diagrama**:
- **Tipo de entrada**: Define um dos tipos de entradas possíveis no PSP-UFU: "Passo (*Step*)", Rampa (*Ramp*) ou Quadrática (*Quadratic*);
- **Tempo de início ($t_i$)**: Define o tempo de início na simulação da entrada definida. As entradas iniciais são sempre definidas como zero e assumem valores diferentes após o tempo de início;
- **Inclinação ($\alpha$)**: Valor relacionado ao tipo de entrada:
	- Para entrada do tipo "Passo" - O valor da inclinação define o valor final do passo:
	$$
	\begin{cases}
		\text{Se~} t < t_i \rightarrow f(t) = 0,0\\
		\text{Se~} t \ge t_i \rightarrow f(t) = \alpha
	\end{cases}
	$$
	- Para entrada do tipo "Rampa" - define o coeficiente de inclinação da reta:
	$$
	\begin{cases}
		\text{Se~} t < t_i \rightarrow f(t) = 0,0\\
		\text{Se~} t \ge t_i \rightarrow f(t) = \alpha \times t
	\end{cases}
	$$
	- Para entrada do tipo "Quadrática" - define o coeficiente de crescimento da curva:
	$$
	\begin{cases}
		\text{Se~} t < t_i \rightarrow f(t) = 0,0\\
		\text{Se~} t \ge t_i \rightarrow f(t) = \alpha \times t^2
	\end{cases}
	$$
	
- **Passo de integração ($h$)**: Passo do método de integração Trapezoidal Implícito;
- **Tempo de simulação ($t_f$)**: Tempo total de simulação.

Após a simulação os resultados são exibidos no [visualizador de gráficos](graphViewer).

:::caution Atenção!
A mesma parametrização de entrada é aplicada em todos os blocos de [entrada](io) do sistema.
:::

:::tip Dica
É possível aplicar **várias entradas distintas** (inclusive com condicionais!) utilizando o bloco de [expressão matemática](mathExpression).
:::

Utilizando o bloco de [expressão matemática](mathExpression) é possível inserir entradas complexas, como por exemplo testar o diagrama de blocos (AVR DC1C + PSS1A - [IEEE Std. 421.5-2016](https://doi.org/10.1109/IEEESTD.2016.7553421)) da figura abaixo:

<div><center><img src={useBaseUrl("images/exControl.svg")} alt="Sistema de controle sob teste" title="Sistema de controle sob teste" /></center></div>
<div><center><img src={useBaseUrl("images/compControl.svg")} alt="Comparação do sistema de controle sob teste" title="Comparação do sistema de controle sob teste" /></center></div>