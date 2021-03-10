---
id: heatmap
title: Mapa de Tensão
sidebar_label: Mapa de Tensão
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

A ferramenta Mapa de Tensão utiliza o concento de [mapa de calor](https://en.wikipedia.org/wiki/Heat_map) para exibir os valores de tensão de todas as barras do sistema por meio de cores.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/heatmap.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/heatmap.webm")} type="video/webm" />
</video>

:::tip Dica
Essa ferramenta é útil para identificação de barras com sub e sobretensões no sistema de potência, assim como o efeito da inserção / remoção de elementos do sistema.
:::

A legenda que correlaciona os valores do módulo da tensão com as cores são exibidas à esquerda da área de trabalho.

## Habilitando a ferramenta Mapa de Tensão

A ferramenta pode ser habilitada ou desabilitada no [submenu Ribbon **Ferramentas**](mainScreen#menu-ribbon) clicando no botão **Mapa de tensão** ou pela tecla de atalho "Ctrl + Shift + H".

As configurações da ferramenta podem ser também acessadas no [submenu Ribbon **Ferramentas**](mainScreen#menu-ribbon) clicando no botão **Configurações do projeto**

<div><center><img src={useBaseUrl("images/heatmapForm.png")} alt="Configurações da ferramenta Mapa de Tensão" title="Configurações da ferramenta Mapa de Tensão" /></center></div>

:::info Informação
Na versão atual do programa, somente as opções do mapa de tensão são exibidas nas configurações de projeto.
:::

Os limites superiores e inferiores, em $p.u.$ são editados nesse formulário.

Alternativamente, o programa pode calcular automaticamente os limites. Nesse caso, o limite superior será a maior tensão do circuito e o limite inferior será a menor tensão do circuito.

:::warning Cuidado!
O cálculo automático dos limites pode prejudicar a identificação de barras com sub e sobretensões no sistema.
:::