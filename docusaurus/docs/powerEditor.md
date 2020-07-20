---
id: powerEditor
title: Editor de Potência
sidebar_label: Editor de Potência
---

## Novo projeto
A criação de um novo projeto é realizada clicando no item **Novo projeto** no [submenu **Arquivo**](mainScreen#menu-ribbon). Essa ação cria um sistema em branco na área de trabalho, local onde é possível inserir os elementos elétricos por meio do [submenu **Ferramentas**](mainScreen#menu-ribbon) ou pelas [teclas de atalho](powerEditor#teclas-de-atalho).

Abaixo são apresentados os elementos elétricos de potência que podem ser inseridos no PSP-UFU:
- [Barramento](bus)
- Elementos de ramo
	- [Linha](line)
	- [Transformador](transformer)
- Elementos Shunt
	- [Carga](load)
	- [Capacitor](capacitor)
	- [Indutor](inductor)
	- [Fonte de Corrente Harmônica](harmSource)
- Máquinas
	- [Gerador Síncrono](syncGenerator)
	- [Compensador Síncrono](syncMotor)
	- [Motor de Indução](indMotor)


![Elementos de potência][powerElements]
[powerElements]: /PSP/images/powerElements.svg 'Elementos de potência'

Todos os elementos inseridos pode ser manipulados e editados utilizando as [Ferramentas CAD](cadTools).

## Inserindo elementos
Por meio do submenu **Ferramentas** pode-se inserir os elementos de potência, como é apresentado na imagem abaixo.

![Adicionar elementos de potência][addPowerElements]
[addPowerElements]: /PSP/images/addPowerElements.png 'Adicionar elementos de potência'

No caso de inserção de um barramento, o usuário deve apenas clicar na posição desejada, já para os outros componentes será solicitado que o usuário selecione em uma ou duas barras, o qual o novo componente será conectado.

Sempre que são inseridos novos elementos, um formulário de dados é exibido e pode-se editar os dados elétricos e acessar formulários adicionais, como dados de estabilidade.

:::caution Atenção
O primeiro elemento que a ser inserido no projeto **deve** ser um barramento. Os demais elementos elétricos devem ser conectados em uma ou mais barras.
:::

Além dos elementos de potência pode-se inserir os elementos de *texto vinculado*, os quais estão associados a uma grandeza que se deseja vizualizar diretamente na [Área de Trabalho](mainScreen#área-de-trabalho).

:::warning Cuidado!
Ao inserir um barramento, a mesma irá acompanhar o ponteiro do mouse até que você clique na posição desejada. Para os demais elementos **deve-se clicar em uma mais barras** para exibir o elemento inserido.

**Sempre fique atento às instruções da [Barra de Status](mainScreen#barra-de-status).**
:::

<video autoPlay loop muted controls>
  <source src= "/PSP/videos/timelapseBuild.mp4" type="video/mp4" />
</video>

### Teclas de atalho
É possível inserir todos os elementos de potência por meio de teclas de atalho, facilitando e agilizando a criação dos diagramas unifilares no PSP-UFU. A tabela abaixo apresenta tais atalhos:

| Elemento             | Atalho    | Elemento             | Atalho    |
| -------------------- | :-------: | -------------------- | :-------: |
| Barramento           | B         | Carga                | Shift + L |
| Linha                | L         | Capacitor            | Shift + C |
| Transformador        | T         | Indutor              | Shift + I |
| Gerador              | G         | Corrente harmônica   | Shift + H |
| Motor de indução     | I         | Texto                | A         |
| Compensador síncrono | K         |

## Editando dados elétricos
Ao inserir um elemento, seu respectivo formulário de edição é exibido para inserção dos dados elétricos d elemento. Todos os dados podem ser **alterados** ao **clicar duas vezes sobre o elemento** ou por opção do **menu de contexto** exibido ao clicar com o botão direito sobre o elemento.

Cada formulário de edição de dados elétricos dos elementos possuem suas particularidades e são descritos em documentação específica: [Barramento](bus), [Linha](line), [Transformador](transformer), [Carga](load), [Capacitor](capacitor), [Indutor](inductor), [Fonte de Corrente Harmônica](harmSource), [Gerador Síncrono](syncGenerator), [Compensador Síncrono](syncMotor), [Motor de Indução](indMotor).