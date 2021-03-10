---
id: sum
title: Somador
sidebar_label: Somador
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Como o próprio nome sugere, os somadores são elementos de controle capazes de somar qualquer quantidade de valores reais.

## Formulário de edição de dados do somador

A quantidade de entradas, assim como seus sinais são definidas pelo usuário, como mostra a figura abaixo.

<div><center><img src={useBaseUrl("images/sumForm.png")} alt="Formulário de edição de dados do somador no PSP-UFU" title="Formulário de edição de dados do somador no PSP-UFU" /></center></div>

No seu único campo “**Sinais**” é inserida uma lista de sinais **separados por espaços** contendo os símbolos “+” para uma entrada positiva e “-” para negativa.

:::info Informação
O usuário pode inserir duas ou mais entradas de sinais no bloco.
:::

Uma vez inseridas novas entradas, nós correspondentes serão incluídos no elemento gráfico do somador, o qual poderá ser conectado por meio das linhas de conexão.

De forma semelhante, o usuário poderá excluir entradas de sinais pré-existentes, as quais serão removidas no ícone gráfico pela remoção do nó e da linha (caso exista alguma conectada).
