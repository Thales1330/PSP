---
id: bus
title: Barramento
sidebar_label: Barramento
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

>Condutor de baixa impedância ao qual vários circuitos elétricos podem ser conectados em pontos separados.
Nota - Em muitos casos, o barramento consiste em uma barra. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=151-12-30).

## Barramento no PSP-UFU
O elemento **barramento**, ou simplesmente **barra**, é um conector ou nó do diagrama unifilar do PSP-UFU. Essa barra pode representar um PAC (Ponto de Acoplamento Comum), um poste de distribuição, uma subestação, um barramento da subestação, entre inúmeros outros tipos pontos de análise e conexão entre elementos.

## Formulário de edição dos barramentos
:::caution Atenção!
O barramento deve ser o **primeiro elemento elétrico a ser inserido no diagrama de potência**, uma vez que os demais componentes de potência são conectados nele.
:::

A imagem abaixo apresenta o formulário de inserção/alteração de dados das barras:

<img src={useBaseUrl("images/busForm.png")} alt="Formulário dos barramentos no PSP-UFU" title="Formulário dos barramentos no PSP-UFU" />

Esse formulário é subdividido em quatro contextos distintos:
- **Geral**: no qual são inseridas informações gerais da barra e informações do fluxo de carga;
- **Falta**: local onde o curto-circuito *shunt* deve ser inserido;
- **Estabilidade**: contendo opções de visualização de dados da barra em gráficos no tempo e inserção de faltas trifásicas no cálculo de estabilidade transitória;
- **Qualidade de energia**: contém a opção de de visualização da impedância harmônica vista pela barra.

---
### Geral

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Tensão nominal
Utilizado para cálculo da impedância base de alguns elementos conectados, além do cálculo da relação de transformação dos transformadores conectados entre barras. Sua unidade pode ser selecionada, sendo expressa em V ou em kV.

A modificação desse parâmetro irá alterar toda a tensão do trecho conectado por linhas elétricas, sendo emitido um alerta ao usuário.

#### Tensão controlada
Caracteriza o barramento como **barra de tensão controlada ([Barra PV](powerFlow))**,

:::caution Atenção!
Essa opção é somente válida caso alguma máquina síncrona esteja conectado, caso contrário esse valor será ignorado. Caso o limite de potência reativa da máquina síncrona conectada seja ultrapassado esse valor também é ignorado.
:::

O valor poderá ser inserido em p.u. ou em volts (ou kV caso a tensão nominal esteja nesta unidade).

#### Barra de referência
Caracteriza o barramento como **barra de referência ([Barra de oscilação](powerFlow))**. Essa opção é somente válida caso esteja conectado um gerador síncrono, caso contrário uma mensagem de erro será exibida ao usuário ao realizar algum dos cálculos do programa.

---
### Falta

#### Inserir falta

Indica se existe um curto-circuito na barra nos estudos de [curto-circuito](fault).

:::warning Cuidado!
Nota-se que essa opção irá inserir uma falta no cálculo de [curto-circuito](fault) e **não será considerado no estudo de estabilidade**.
:::

#### Tipo de falta
Seleciona o tipo de falha shunt da barra:
- trifásico para a terra
- fase-fase
- fase-fase-terra
- fase-terra

#### Local da falta
Seleciona a fase em que se situa a falta (ou combinação delas no caso de falha entre duas fases), sendo essa opção desabilitada para o tipo de falta trifásica.

#### Resistência e reatância de falta
Representam a impedância da falta. Curtos-circuitos francos (sem impedância de falta) são representados inserindo valor zero em ambos os campos.

---
### Estabilidade

#### Imprimir dados
Mostra os dados da barra editada no gráfico gerado pelo estudo de estabilidade transitória. Os seguintes dados são exibidos:
- módulo da tensão do barramento
- ângulo da tensão do barramento.

#### Inserir falta
Insere uma falta trifásica na barra no cálculo de [estabilidade](stability).
:::warning Cuidado!
Nota-se que essa opção irá inserir uma falta no cálculo de [estabilidade](stability) e **não será considerado no estudo de [curto-circuito](fault)**.
:::

#### Tempo
Instante no tempo ($t_{falta}$) em que ocorre a falta durante os estudos de [estabilidade](stability). Esse campo e os próximos são somente acessados caso seja inserida uma falta no estudo de estabilidade.

#### Duração da falta
Duração da falta inserida ($\Delta t_{falta}$) no estudo de estabilidade. A falta irá iniciar no instante $t_{falta}$ e será interrompida em $t_{falta}+\Delta t_{falta}$.

#### Resistência e reatância de falta
Impedância de falta nos estudos de estabilidade. Uma falta franca é representada pela inserção de um valor de impedância shunt de valor muito baixo ($j10^{-5}  p.u.$).

---
### Qualidade de energia

#### Imprimir impedância harmônica da barra
Imprime a impedância harmônica vista por essa barra referente ao estudo de [Resposta na Frequência](harmonics).
