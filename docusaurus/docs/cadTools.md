---
id: cadTools
title: Ferramentas CAD
sidebar_label: Ferramentas CAD
---
export const Highlight = ({children, color}) => ( <span style={{
      backgroundColor: color,
      borderRadius: '2px',
      color: '#000',
      padding: '0.2rem',
    }}>{children}</span> );

import useBaseUrl from "@docusaurus/useBaseUrl";

>Desenho assistido por computador (DAC; em inglês: *computer aided design* - CAD) é o nome genérico de sistemas computacionais (*software*) utilizados pela engenharia, geologia, geografia, arquitetura e design para facilitar o projeto e desenho técnicos. No caso do *design*, este pode estar ligado especificamente a todas as suas vertentes (produtos como vestuário, eletroeletrônicos, automobilísticos, etc.), de modo que os jargões de cada especialidade são incorporados na interface de cada programa. [Wikipedia](https://pt.wikipedia.org/wiki/Desenho_assistido_por_computador)

## Ferramentas do menu

Todos os objetos criados contêm vários atributos gráficos, como: posição, orientação, cor, tamanho, etc. Para modificá-los foram inseridas algumas ferramentas intuitivas e de fácil utilização, as quais são: [**selecionar**](cadTools#selecionar), [**mover**](cadTools#mover), [**girar**](cadTools#girar), [**arrastar**](cadTools#arrastar), [**zoom**](cadTools#zoom), [**encaixar**](cadTools#encaixar), [**copiar, colar**](cadTools#copiar-e-colar) e [**excluir**](cadTools#excluir).

Essas ferramentas podem ser acessadas por suas teclas de atalho assim como pelo [submenu Ferramentas](mainScreen#menu-ribbon).

Tais ferramentas permitem a **personalização**, **navegação** e auxiliam a **criação** de redes elétricas de potência, assim como diagramas de controle na plataforma.

### Selecionar
A seleção de um único objeto é realizada clicando com o botão esquerdo do mouse sobre o elemento, que será identificado pelo contorno em <Highlight color="#80bfff">azul</Highlight> do objeto.

Para a seleção de vários objetos simultaneamente é necessário clicar com o botão esquerdo do mouse em um local vazio da tela e arrastá-lo, criando assim um **retângulo de seleção** que irá acompanhar o ponteiro do mouse enquanto o botão esquerdo estiver pressionado. Ao soltá-lo todos os objetos que intercederem o retângulo serão selecionados.

Mantendo pressionado a tecla Control (Ctrl) do teclado pode-se manter seleções anteriores enquanto novos elementos são selecionados.

Para desmarcar todos os objetos basta clicar em uma área sem objetos.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/select.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/select.webm")} type="video/webm" />
</video>

### Mover
Um elemento poderá ser movido para qualquer local por meio da ação *drag-and-drop* (clicando e arrastando) ou pela ferramenta mover.

Ao habilitar o “modo mover”, movendo todos os componentes selecionados à medida que a posição do mouse é alterada. Para desabilitar esse modo basta pressionar a tecla Escape do teclado.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/move.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/move.webm")} type="video/webm" />
</video>

### Girar
Alguns objetos poderão ser rotacionados através da ferramenta girar, acessado pela tecla de atalho “R”, girando o objeto no sentido horário, ou “Shift + R”, para o sentido anti-horário (essa opção irá afetar todos os objetos selecionados). O elemento também pode ser rotacionado pelo [submenu Ferramentas](mainScreen#menu-ribbon) ou ao acessar o menu de contexto clicando com o botão direito sobre o elemento.

:::caution Atenção!
O menu de contexto somente será exibido se o elemento estiver selecionado.
:::

Ao utilizar essa ferramenta os objetos irão girar 45º a partir do seu ponto de origem. Para atingir a angulação pretendida basta repetir o processo de rotação.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/rotate.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/rotate.webm")} type="video/webm" />
</video>

### Arrastar
Três ferramentas muito úteis na navegação do circuito, principalmente em grandes redes, são: arrastar, zoom e encaixar.

A ferramenta arrastar possibilita mover todo o circuito **preservando as posições relativas entre os elementos**. Esta ferramenta pode ser acessada pelo submenu “Ferramentas” ou **mantendo pressionada o *scroll* do mouse**, ativando assim o “modo arrastar”. Ao ativá-lo todo o circuito será movido clicando e arrastando na direção desejada.

Para sair do “modo arrastar” basta pressionar a tecla Escape do teclado.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/drag.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/drag.webm")} type="video/webm" />
</video>

### Zoom
O zoom será aplicado ao utilizar o **scroll do mouse** para mais ou menos zoom. A ferramenta zoom irá aproximar ou distanciar o circuito a partir da posição do ponteiro do mouse na tela para encontrar objetos ou destaca-los.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/zoom.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/zoom.webm")} type="video/webm" />
</video>

### Encaixar
A ferramenta encaixar irá mover o circuito e aplicar o zoom necessário para que todos os componentes da rede sejam exibidos na tela. Esta ferramenta pode ser acessada através do submenu “Ferramentas” ou pela combinação de teclas “Shift + F”.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/fit.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/fit.webm")} type="video/webm" />
</video>

### Copiar e Colar
Duas ferramentas importantes durante o processo de criação da rede elétrica de potência no *software* são copiar e colar. Qualquer circuito criado no PSP-UFU poderá ser duplicado completamente ou parcialmente no mesmo projeto ou em múltiplos através dessas ferramentas.

Para copiar basta selecionar os elementos que se deseja duplicar e acessar a ferramenta no submenu “Ferramentas” ou pressionando a combinação das teclas “Ctrl + C”, então os dados do circuito serão copiados para a área de transferência. Ao copiar um circuito ele poderá ser colado no mesmo projeto ou em um projeto distinto, utilizando o mesmo submenu ou pressionando a combinação das teclas “Ctrl + V”.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/copyPaste.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/copyPaste.webm")} type="video/webm" />
</video>

### Excluir
Qualquer objeto poderá ser excluído do projeto por meio da ferramenta deletar, presente no menu Ribbon, pela tecla de atalho Delete ou pelo acesso ao menu de contexto clicando com o botão direito sobre o elemento selecionado.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/delete.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/delete.webm")} type="video/webm" />
</video>


## Personalização gráfica os elementos
Alguns elementos permitem sua personalização gráfica, como as barras e linhas.

### Barra
No caso dos barramentos, pode-se alterar o **comprimento da barra** clicando e arrastando no *pickbox* exibido ao posicionar o ponteiro do mouse sobre esse elemento selecionado.

### Linha
As linhas podem ser inseridas com **"nós" (pontos de ancoragem)**, personalizando a disposição das linhas no diagrama unifilar. Esses nós são inseridos durante o processo de criação do elemento, anteriormente à seleção da segunda barra.

Após a sua inserção, novos nós podem ser anexados pelo menu de contexto acessado ao clicar com o botão direito sobre a linha selecionada. Sua remoção também é alcançada por uma opção do menu de contexto.

Assim como nos barramentos, a alteração da posição dos nós é obtida clicando e arrastando o *pickbox* exibido ao posicionar o ponteiro do mouse sobre esse elemento selecionado.

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/graphicPropsChange.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/graphicPropsChange.webm")} type="video/webm" />
</video>

## Conexão e desconexão de elemetos na barra
Os elementos podem ser desconectados e reconectados às barras após sua inserção somente pela ferramenta *drag-and-drop*. Para isso, o elemento deve ser selecionado e o seu nó de conexão deve ser arrastado para o local requerido.

:::warning Cuidado!
O elemento a ser desconectado/reconectado **deve ser previamente selecionado** para efetuar a operação. Caso contrário a barra será movida ou nenhuma operação será realizada.
:::

No caso de desconexão, o elemento será automaticamente removido da simulação, sendo indicado pela sua cor (o elemento passa a ter cor <Highlight color="#7f7f7f">cinza</Highlight>).

:::caution Atenção!
Ao reconectar o elemento ao circuito, deve-se **atentar para estado do seu "disjuntor"**, indicado por um quadrado próximo ao seu nó de conexão. Um elemento reconectado volta ao circuito com seu disjuntor aberto (<Highlight color="#ff1919">vermelho</Highlight>), sendo necessário clicar sobre o mesmo a fim de inserir o elemento no circuito.
:::

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/disconnect.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/disconnect.webm")} type="video/webm" />
</video>