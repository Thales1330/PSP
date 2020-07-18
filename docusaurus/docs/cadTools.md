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

>Desenho assistido por computador (DAC; em inglês: *computer aided design* - CAD) é o nome genérico de sistemas computacionais (*software*) utilizados pela engenharia, geologia, geografia, arquitetura e design para facilitar o projeto e desenho técnicos. No caso do *design*, este pode estar ligado especificamente a todas as suas vertentes (produtos como vestuário, eletroeletrônicos, automobilísticos, etc.), de modo que os jargões de cada especialidade são incorporados na interface de cada programa. [Wikipedia](https://pt.wikipedia.org/wiki/Desenho_assistido_por_computador#cite_note-1)

Todos os objetos criados contêm vários atributos gráficos, como: posição, orientação, cor, tamanho, etc. Para modificá-los foram inseridas algumas ferramentas intuitivas e de fácil utilização, as quais são: [**selecionar**](cadTools#selecionar), [**mover**](cadTools#mover), [**girar**](cadTools#girar), [**arrastar**](cadTools#arrastar), [**zoom**](cadTools#zoom), [**encaixar**](cadTools#encaixar), [**copiar, colar**](cadTools#copiar-e-colar) e [**excluir**](cadTools#excluir).

Tais ferramentas permitem a **personalização**, **navegação** e auxiliam a **criação** de redes elétricas de potência, assim como diagramas de controle na plataforma.

### Selecionar
A seleção de um único objeto é realizada clicando com o botão esquerdo do mouse sobre o elemento, que será identificado pelo contorno em <Highlight color="#80bfff">azul</Highlight> do objeto.

Para a seleção de vários objetos simultaneamente é necessário clicar com o botão esquerdo do mouse em um local vazio da tela e arrastá-lo, criando assim um **retângulo de seleção** que irá acompanhar o ponteiro do mouse enquanto o botão esquerdo estiver pressionado. Ao soltá-lo todos os objetos que intercederem o retângulo serão selecionados.

Mantendo pressionado a tecla Control (Ctrl) do teclado pode-se manter seleções anteriores enquanto novos elementos são selecionados.

Para desmarcar todos os objetos basta clicar em uma área sem objetos.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/select.mp4" type="video/mp4" />
</video>

### Mover
Um elemento poderá ser movido para qualquer local por meio da ação *drag-and-drop* (clicando e arrastando) ou pela ferramenta mover.

Ao habilitar o “modo mover”, movendo todos os componentes selecionados à medida que a posição do mouse é alterada. Para desabilitar esse modo basta pressionar a tecla Escape do teclado.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/move.mp4" type="video/mp4" />
</video>

### Girar
Alguns objetos poderão ser rotacionados através da ferramenta girar, acessado pela tecla de atalho “R”, girando o objeto no sentido horário, ou “Shift + R”, para o sentido anti-horário (essa opção irá afetar todos os objetos selecionados).

Ao utilizar essa ferramenta os objetos irão girar 45º a partir do seu ponto de origem. Para atingir a angulação pretendida basta repetir o processo de rotação.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/rotate.mp4" type="video/mp4" />
</video>

### Arrastar
Três ferramentas muito úteis na navegação do circuito, principalmente em grandes redes, são: arrastar, zoom e encaixar.

A ferramenta arrastar possibilita mover todo o circuito **preservando as posições relativas entre os elementos**. Esta ferramenta pode ser acessada pelo submenu “Ferramentas” ou **mantendo pressionada o scroll do mouse**, ativando assim o “modo arrastar”. Ao ativá-lo todo o circuito será movido clicando e arrastando na direção desejada.

Para sair do “modo arrastar” basta pressionar a tecla Escape do teclado.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/drag.mp4" type="video/mp4" />
</video>

### Zoom
O zoom será aplicado ao utilizar o **scroll do mouse** para mais ou menos zoom. A ferramenta zoom irá aproximar ou distanciar o circuito a partir da posição do ponteiro do mouse na tela para encontrar objetos ou destaca-los.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/zoom.mp4" type="video/mp4" />
</video>

### Encaixar
A ferramenta encaixar irá mover o circuito e aplicar o zoom necessário para que todos os componentes da rede sejam exibidos na tela. Esta ferramenta pode ser acessada através do submenu “Ferramentas” ou pela combinação de teclas “Shift + F”.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/fit.mp4" type="video/mp4" />
</video>

### Copiar e Colar
Duas ferramentas importantes durante o processo de criação da rede elétrica de potência no *software* são copiar e colar. Qualquer circuito criado no PSP-UFU, poderá ser duplicado completamente ou parcialmente no mesmo projeto ou em múltiplos através dessas ferramentas.

Para copiar basta selecionar os elementos que se deseja duplicar e acessar a ferramenta no submenu “Ferramentas” ou pressionando a combinação das teclas “Ctrl + C”, então os dados do circuito serão copiados para a área de transferência. Ao copiar um circuito ele poderá ser colado no mesmo projeto ou em um projeto distinto, utilizando o mesmo submenu ou pressionando a combinação das teclas “Ctrl + V”.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/copyPaste.mp4" type="video/mp4" />
</video>

### Excluir
Qualquer objeto poderá ser excluído do projeto por meio da ferramenta deletar, presente no menu Ribbon, ou pela tecla de atalho Delete.

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/delete.mp4" type="video/mp4" />
</video>