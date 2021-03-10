---
id: harmSource
title: Fonte de Corrente Harmônica
sidebar_label: Fonte de Corrente Harmônica
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

> (**Fonte de perturbação harmônica**) Parte do sistema elétrico de potência ou das instalações a ele conectadas, que causa distorção harmônica da forma de onda da corrente e / ou tensão. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=614-01-16).

## Fonte de corrente harmônica no PSP-UFU
As fontes de corrente harmônica são responsáveis pela injeção de correntes harmônicas e são utilizadas pela ferramenta de cálculo de [distorções harmônicas](harmonics). Uma lista de correntes harmônicas pode ser inserida em um mesmo elemento, conforme é exibido na o formulário de edição de dados.

:::caution Atenção!
A presença da fonte de corrente harmônica **não é considerada** nos estudos de [fluxo de carga](powerFlow), [curto-circuito](fault) e [estabilidade](stability).
:::

## Formulário de edição das fontes de corrente harmônica
A imagem abaixo apresenta o formulário de inserção/alteração de dados da fonte de corrente harmônica:

<div><center><img src={useBaseUrl("images/harmSourceForm.png")} alt="Formulário dos indutores no PSP-UFU" title="Formulário dos indutores no PSP-UFU" /></center></div>

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

#### Ordem
Especifica a ordem harmônica a ser inserida na lista de correntes harmônicas do elemento. Caso a ordem harmônica já exista na lista, ela será substituída pelos novos parâmetros.

#### Corrente
Módulo da corrente harmônica.

#### Unidade
Unidade do módulo de corrente harmônica, em A ou $p.u.$

#### Ângulo
Ângulo da corrente harmônica, em graus.

#### Adicionar / Remover
Insere ou modifica a nova ordem harmônica ao clicar em “Adicionar”. Retira a ordem harmônica selecionada da lista ao clicar em “Remover”.
