---
id: powerEditor
title: Editor de Potência
sidebar_label: Editor de Potência
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

O Editor de Elementos de Potência no PSP-UFU permite a inserção, manipulação e edição de elementos elétricos de potência por meio de um diagrama unifilar.

>(**Diagrama unifilar**) Um diagrama do sistema no qual as linhas polifásicas são representados por sua única linha equivalente. [*tradução livre* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=601-02-04).

:::info Informação
Todos os elementos inseridos pode ser manipulados e editados utilizando as [Ferramentas CAD](cadTools).
:::

## Novo projeto
A criação de um novo projeto é realizada clicando no item **Novo projeto** no [submenu **Arquivo**](mainScreen#ribbon-menu). Essa ação cria um sistema em branco na área de trabalho, local onde é possível inserir os elementos elétricos por meio do [submenu **Ferramentas**](mainScreen#ribbon-menu) ou pelas [teclas de atalho](powerEditor#teclas-de-atalho).

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
	- Transitório Eletromagnético (ATP)
- Máquinas
	- [Gerador Síncrono](syncGenerator)
	- [Compensador Síncrono](syncMotor)
	- [Motor de Indução](indMotor)

<div><center><img src={useBaseUrl("images/powerElements.svg")} alt="Elementos de potência" title="Elementos de potência" /></center></div>

## Inserindo elementos
Por meio do submenu **Ferramentas** pode-se inserir os elementos de potência, como é apresentado na imagem abaixo.

<div><center><img src={useBaseUrl("images/addPowerElements.png")} alt="Adicionar elementos de potência" title="Adicionar elementos de potência" /></center></div>

No caso de inserção de um barramento, o usuário deve apenas clicar na posição desejada, já para os outros componentes será solicitado que o usuário selecione em uma ou duas barras, o qual o novo componente será conectado.

Sempre que são inseridos novos elementos, um formulário de dados é exibido e pode-se editar os dados elétricos e acessar formulários adicionais, como dados de estabilidade.

:::caution Atenção
O primeiro elemento que a ser inserido no projeto **deve** ser um barramento. Os demais elementos elétricos devem ser conectados em uma ou mais barras.
:::

Além dos elementos de potência pode-se inserir os elementos de *[texto vinculado](text)*, os quais estão associados a uma grandeza que se deseja vizualizar diretamente na [Área de Trabalho](mainScreen#workspace).

:::warning Cuidado!
Ao inserir um barramento, a mesma irá acompanhar o ponteiro do mouse até que você clique na posição desejada. Para os demais elementos **deve-se clicar em uma mais barras** para exibir o elemento inserido.

**Sempre fique atento às instruções da [Barra de Status](mainScreen#status-bar).**
:::

<video autoPlay loop muted playsInline controls>
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
| Motor de indução     | I         | Transitório Eletromagnético | Shift + E |
| Compensador síncrono | K         |  Texto                | A         |

## Editando dados elétricos
Ao inserir um elemento, seu respectivo formulário de edição é exibido para inserção dos dados elétricos d elemento. Todos os dados podem ser **alterados** ao **clicar duas vezes sobre o elemento** ou por opção do **menu de contexto** exibido ao clicar com o botão direito sobre o elemento.

Cada formulário de edição de dados elétricos dos elementos possuem suas particularidades e são descritos em documentação específica: [Barramento](bus), [Linha](line), [Transformador](transformer), [Carga](load), [Capacitor](capacitor), [Indutor](inductor), [Fonte de Corrente Harmônica](harmSource), [Gerador Síncrono](syncGenerator), [Compensador Síncrono](syncMotor), [Motor de Indução](indMotor).