---
id: text
title: Texto Vinculado
sidebar_label: Texto Vinculado
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

O elemento de texto vinculado (ao elemento de potência) pode ser inserido no editor de [elementos de potência](powerEditor). Esse elemento de texto pode ser vinculado a inúmeros resultados e parâmetros do sistema de potência.

Esse elemento é útil para identificação e análise dos resultados de elementos de potência.

:::tip Dica
Outra aplicação importante é a atualização de seu valor ao aplicar a “solução contínua” no projeto, auxiliando na visualização das mudanças de estado da rede elétrica.

Também pode-se utilizar os resultados apresentados por esses elementos juntamente com o diagrama unifilar na criação de relatórios do projeto.
:::

<div><center><img src={useBaseUrl("images/textExp.png")} alt="Exemplo do elemento de texto no PSP-UFU" title="Exemplo de elemento de texto no PSP-UFU" /></center></div>

## Inserindo elementos de texto no PSP-UFU
Por meio do submenu **Ferramentas** pode-se inserir os elementos de texto, como é apresentado na imagem abaixo.

<div><center><img src={useBaseUrl("images/addTextElement.svg")} alt="Adicionar elementos de texto" title="Adicionar elementos de texto" /></center></div>

Ao selecionar o botão "Texto" ou utilizar a **tecla de atalho "A"**, o usuário deve clicar na posição desejada da [área de trabalho](powerEditor). Com isso, será adicionado um elemento temporário com o valor "Texto". Para parametrizar esse elemento clique duas vezes sobre ele.

:::caution Atenção
O elemento de texto, assim como os [elementos de controle](controlEditor) não exibem automaticamente o formulário de edição de dados ao inseri-lo pela primeira vez.
:::

## Formulário de edição de dados do elemento de texto
A figura abaixo apresenta o formulário de edição de dados desse componente gráfico.

<div><center><img src={useBaseUrl("images/textForm.png")} alt="Formulário do elemento de texto no PSP-UFU" title="Formulário do elemento de texto no PSP-UFU" /></center></div>

Sua parametrização é realizada por campos que associam o componente aos elementos elétricos de potência, além do formato dos resultados numéricos. Os campos são liberados a medida que os dados anteriores são inseridos e uma mensagem de erro é apresentada ao usuário caso algum parâmetro necessário não seja preenchido.

O processo de associação se inicia na escolha do tipo de elemento de potência será vinculado ao texto. Em seguida é exibida uma lista com o nome de todos os componentes previamente inseridos no editor de elementos de potência do tipo definido no campo anterior, associando, assim, o texto ao elemento.

Em “Tipo de texto” define-se qual será a saída apresentada. Tal campo depende do tipo de elemento selecionado, como mostra a tabela abaixo. Alguns outros dados são necessários para os elementos de ramo, como barra de origem e destino, com o intuito de representar o fluxo correto de potência e corrente. Finalmente o usuário deve inserir a quantidade de casas decimais para dados numéricos. A pré-visualização pode ser observada no formulário.

|Elemento de potência|Tipo de texto|Unidades disponíveis|
|:------------------:|-------------|--------------------|
|[Barra](bus)|Nome<br/>Tensão<br/>Ângulo<br/>Corrente de falta<br/>Tensão de falta<br/>Nível de curto-circuito<br/>THD de tensão|-<br/>$p.u.$, V, kV<br/>Graus, Radianos<br/>$p.u.$, A, kA<br/>$p.u.$, V, kV<br/>$p.u.$, VA, kVA, MVA<br/>%|
|[Gerador](syncGenerator)|Nome<br/>Potência ativa<br/>Potência reativa<br/>Corrente de falta|-<br/>$p.u.$, W, kW, MW<br/>$p.u.$, var, kvar, Mvar<br/>$p.u.$, A, kA|
|[Linha](line) e [transformador](transformer)|Nome<br/>Fluxo de potência ativo<br/>Fluxo de potência reativo<br/>Perdas<br/>Corrente<br/>Corrente de falta|-<br/>$p.u.$, W, kW, MW<br/>$p.u.$, var, kvar, Mvar<br/>$p.u.$, W, kW, MW<br/>$p.u.$, A, kA<br/>$p.u.$, A, kA|
|[Carga](load), [Motor de indução](indMotor)<br/>e [Compensador síncrono](syncMotor)|Nome<br/>Potência ativa<br/>Potência reativa|-<br/>$p.u.$, W, kW, MW<br/>$p.u.$, var, kvar, Mvar|
|[Capacitor](capacitor) e [Indutor](inductor)|Nome<br/>Potência reativa|-<br/>$p.u.$, var, kvar, Mvar|

:::info Informação
Uma vez inseridos, esses elementos de texto podem ser manipulados da [mesma forma que os elementos de potência](cadTools).
:::