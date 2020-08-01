---
id: graphViewer
title: Visualizador de Gráficos
sidebar_label: Visualizador de Gráficos
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Para exibição dos resultados originados dos cálculos de [estabilidade](stability) e [varredura de frequências](harmonics), desenvolveu-se um visualizador de gráficos, como mostra a figura abaixo:

<div><center><img src={useBaseUrl("images/graphViewer.png")} alt="Visualizador de gráficos do PSP-UFU" title="Visualizador de gráficos do PSP-UFU" /></center></div>

## Criação, manipulação e navegação de curvas
Tal visualizador utiliza a biblioteca [wxMathPlot](https://wxmathplot.sourceforge.io/), e permite ao usuário imprimir todas as curvas no tempo originadas dos resultados de [estabilidade](stability) e [varredura de frequências](harmonics), acessados em dados hierárquicos em forma de árvore, identificados pelo tipo e nome do elemento de potência, além do tipo de curva.

:::info Informação
As curvas podem ser plotadas com **clique duplo** sobre ela ou ao selecionar a caixa "Plotar" nas propriedades do traço.
:::

Os gráficos impressos podem ser caracterizados pela edição de suas propriedades do traço, ou seja, pode-se alterar a cor, espessura e tipo de traço (seis tipos disponíveis). Por padrão o eixo vertical é o tempo (em estabilidade) ou frequência (varredura de frequências), porém o usuário pode alterar para qualquer curva disponível.

Várias propriedades gerais do gráfico podem ser editadas: título do gráfico, rótulos dos eixos x e y, margens e limites inferiores e superiores de ambos os eixos.

O menu **Visualização** permite a inserção de exibição de legenda, linhas de grade e coordenadas do ponteiro do mouse (utilizada para destacar pontos na curva), além da possibilidade de alternar o tema de impressão entre escuro e claro, facilitando a visualização ou exportação do gráfico.

No menu **Arquivo**, pode-se salvar o gráfico na forma de imagem (*.png*, *.jpg* ou *.bmp*) ou enviá-lo diretamente para a área de transferência. No mesmo menu também é permitido a exportação no formato *[CSV (Comma-separated Values)](https://en.wikipedia.org/wiki/Comma-separated_values)*, o qual poderá ser utilizado em outros visualizadores de gráficos, como o Excel ou o [gnuplot](http://www.gnuplot.info/).

A área de plotagem permite a navegação utilizando ferramentas de zoom, por área de seleção e comandos de mouse, e arrasto das curvas impressas.

:::tip Dica
Ao clicar com o botão direito sobre a área de plotagem as opções de manipulação são exibidas por meio de um menu de contexto.
:::

Os seguintes **comandos de mouse** são utilizados para navegação das curvas:

|Comando|Ação|
|-------|----|
|Botão esquerdo pressionado + Selecionar área|Seleciona a área de zoom|
|Botão direito pressionado + Mover mouse|Arrasta a curva|
|Roda do mouse|Arrasto vertical|
|Roda do mouse + Shift|Arrasto horizontal|
|Roda do mouse + Ctrl|Mais/menos zoom|