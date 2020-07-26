---
id: inductor
title: Indutor
sidebar_label: Indutor
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

>Um reator destinado à conexão de derivação em uma rede para compensar a corrente capacitiva. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=421-01-03).

## Indutor no PSP-UFU
O elemento indutor (ou reator) representa, geralmente, reator *shunt* no circuito do PSP-UFU.

Os capacitores e reatores exigem somente sua potência reativa como parâmetro. Esses elementos passivos são somente inseridos na matriz admitância cuja impedância é calculada a partir de sua potência e tensão nominais.

## Formulário de edição dos indutores
A imagem abaixo apresenta o formulário de inserção/alteração de dados dos indutores:

<div><center><img src={useBaseUrl("images/indForm.png")} alt="Formulário dos indutores no PSP-UFU" title="Formulário dos indutores no PSP-UFU" /></center></div>

Além do único contexto geral, pode ser observado o botão "Estabilidade" na parte inferior esquerda do formulário. Esse formulário, comum a vários outros elementos, permite a inserção e/ou remoção do indutor durante o estudo de [estabilidade](stability).

<div><center><img src={useBaseUrl("images/indSw.png")} alt="Formulário de chaveamento do indutor" title="Formulário de chaveamento do indutor" /></center></div>

---
### Geral

#### Nome
Identificação do elemento elétrico. Podem ser inseridos quaisquer números de caracteres no padrão [Unicode](https://pt.wikipedia.org/wiki/Unicode).

Todos os componentes de potência do PSP-UFU possuem esse campo.

#### Potência reativa
Os capacitores e reatores exigem somente sua potência reativa como parâmetro, inserida em Mvar, kvar, var ou $p.u.$ (na base de potência do sistema). 

---
### Botão Estabilidade
O botão "Estabilidade" irá abrir um formulário, comum a vários outros elementos, que permite a inserção e/ou remoção do indutor durante o estudo de [estabilidade](stability).

Nesse formulário pode ser criada uma lista genérica de inserções e remoções da linha no tempo, personalizada por um contexto de propriedades de chaveamento que são editados o tipo de chaveamento (inserção ou remoção) e o instante (em segundos) do evento. Essas propriedades são atribuídas e retiradas da lista genérica por meio dos botões "Adicionar" e "Remover", respectivamente.