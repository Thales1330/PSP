---
id: mainScreen
title: Tela Principal
sidebar_label: Tela Principal
---
import useBaseUrl from "@docusaurus/useBaseUrl";

## Organização da tela principal
A tela principal do PSP-UFU é dividida em três seções: *[Menu "Ribbon"](mainScreen#menu-ribbon)*, *[Área de Trabalho](mainScreen#área-de-trabalho)* e *[Barra de Status](mainScreen#barra-de-status)*.

<div><center><img src={useBaseUrl("images/mainScreen.svg")} alt="Tela principal" title="Tela principal do PSP-UFU no Windows" /></center></div>

Resumidamente, no **Menu Ribbon** são acessadas as ferramentas do PSP-UFU; na **Área de Trabalho** são criados os diagramas unifilares de potência; e na **Barra de Status** são exibidas importantes informações acerca do estado da Área de Trabalho.


### Menu Ribbon
>Em informática o ribbon (faixa, em inglês) é um formato de apresentação de interface baseada na GUI onde a barra de ferramentas é mostrada através de uma barra mais larga com icones maiores possibilitando o uso dos aplicativos por dispositivos *touch screen*.
[Wikipedia](https://pt.wikipedia.org/wiki/Ribbon_(inform%C3%A1tica))

O menu Ribbon é composto de três submenus: “Arquivo”, “Ferramentas” e “Simulação”.

<div><center><img src={useBaseUrl("images/menuFile.png")} alt="Menu Arquivo" title="Menu Arquivo" /></center></div>

Em **Arquivo** estão as ferramentas gerais do programa, as quais permitem ao usuário criar e salvar novos projetos, além daquelas que possibilitam carregar os projetos já existentes e importar arquivos de outros programas. Esse submenu também possui opções gerais do programa e acesso a informações sobre o programa.

<div><center><img src={useBaseUrl("images/menuTools.png")} alt="Menu Ferramentas" title="Menu Ferramentas" /></center></div>

Em **Ferramentas** estão presentes os itens específicos, os quais estão relacionados à criação e manipulação da rede elétrica de potência, relatórios tabulares de saída de dados e acesso ao formulário de criação de gráficos no tempo. Algumas configurações específicas do projeto ativo são acessadas por esse menu.

<div><center><img src={useBaseUrl("images/menuSimulation.png")} alt="Menu Simulação" title="Menu Simulação" /></center></div>

Finalmente, o submenu **Simulação** proporciona o acesso do usuário aos cálculos realizados pelo *software* e suas configurações, além da ferramenta de “solução contínua”, a qual resolve os cálculos estáticos (fluxo de potência, curto-circuito e harmônicos) após quaisquer mudanças na rede de potência, como remoção de componente ou alteração de seus dados. Todas as configurações de simulação do projeto ativo são acessadas por esse menu.

### Área de Trabalho
Na área de trabalho estão presentes os projetos em execução, os quais são divididos por abas identificadas pelo nome do projeto (caso já esteja gravado no disco). É nessa área que são inseridos e excluídos os elementos elétricos, cujas ferramentas são acessadas por meio do [Menu Ribbon](mainScreen#menu-ribbon).

As edições gráficas dos elementos, como posição, conexão e tamanho dos barramentos, são realizadas por meio da ação *drag-and-drop* (arrastar e soltar) com a utilização do mouse. A edição dos dados elétricos é feita por meio de formulários de dados acessados clicando duas vezes sobre os elementos.

<div><center><img src={useBaseUrl("images/workspace.png")} alt="Área de Trabalho" title="Área de Trabalho" /></center></div>

### Barra de Status
A barra de status é responsável por informações interessantes sobre as circunstâncias atuais do programa: modo de operação do mouse (editar, mover, arrastar), zoom aplicado e posição do mouse na área de trabalho, além de dados sobre as ações dos usuários, como: inserção de elementos, informação sobre copiar e colar, etc.

:::tip Dica
**Sempre fique atendo à barra de status**, pois são fornecidas informações importantes acerca da operação do programa. Caso tenha alguma dúvida siga as instruções apresentadas nesse componente.
:::

## Configurações gerais
As configurações gerais do programa são acessadas no submenu Ribbon **Arquivo**. Essas configurações são aplicadas para todos os projetos e permanecem gravadas no disco.

<div><center><img src={useBaseUrl("images/generalSettings.png")} alt="Configurações gerais" title="Configurações gerais" /></center></div>

:::warning Cuidado
Algumas configurações de segurança do seu computador (principalmente em sistemas Windows) podem gerar uma mensagem de erro ao confirmar as alterações das configurações gerais.

Para resolver esse problema basta **executar o PSP-UFU como administrador** (clicar com botão direito no atalho do programa e posteriormente em executar como administrador). Altere novamente as configurações e reinicie o programa.
:::

### Idioma
Atualmente os seguintes idiomas estão disponíveis no PSP-UFU:
- Inglês
- Português

:::info Informação
O programa deve ser reiniciado para surtir efeito da alteração do idioma.
:::

### Renderização
Define como os elementos gráficos do [editor de potência](powerEditor) e do [editor de controle](controlEditor) são desenhados na tela. Atualmente duas opções estão disponíveis:
- [OpenGL](https://www.opengl.org/about/)
- [Device Context](https://docs.wxwidgets.org/3.0/classwx_graphics_context.html)

:::warning Cuidado!
o OpenGL pode não ser suportado pelo seu computador. Nesse caso, ao criar um novo projeto a tela de trabalho não é exibida ou é exibida uma tela preta. A solução é alterar o renderizador para "Device Context". 

Caso apareça uma mensagem de erro, execute o programa como administrador e repita o processo.
:::

:::info Informação
O programa deve ser reiniciado para surtir efeito de alteração do renderizador.
:::

## Arquivos de projeto do PSP-UFU
As opções de criação, gravação e abertura de projetos no disco, assim como importação de arquivos de outros programas estão presentes no submenu Ribbon **Arquivo**.

### Novo projeto
A criação de um novo projeto é realizada clicando no botão **Novo projeto** no [submenu **Arquivo**](mainScreen#menu-ribbon). Essa ação cria um sistema em branco na área de trabalho, local onde é possível inserir os elementos elétricos por meio do [submenu **Ferramentas**](mainScreen#menu-ribbon) ou pelas [teclas de atalho](powerEditor#teclas-de-atalho).

Esse sistema em branco pode ser ciado utilizando o [Editor de Potência](powerEditor).

### Salvar e Salvar como...
A opção "Salvar" sobrepõe as alterações realizadas no projeto aberto e grava no disco. A opção "Salvar como..." cria um novo arquivo e grava o projeto no disco com o auxílio de uma janela de seleção de pasta (e definição do nome do arquivo).

:::info Informação
Para projetos que estão sendo gravados pela primeira vez a opção "Salvar" se comporta de forma idêntica à opção "Salvar como...".
:::

:::tip Dica
O PSP-UFU grava os arquivos com a extensão *.psp*. Esses arquivos nada mais são que arquivos de texto utilizando a [linguagem de marcação](https://en.wikipedia.org/wiki/Markup_language) XML ([eXtensible Markup Language](https://en.wikipedia.org/wiki/XML)), que define uma série de regras de formatação dos dados de forma que eles são tanto legíveis por humanos quanto por máquinas.

Portanto, os dados elétricos contidos neles podem ser facilmente identificados e alterados, caso necessário.
:::

### Abrir projeto
Essa opção abre os projetos gravados no disco por meio com o auxílio de uma janela de seleção de arquivos. 

### Importar projeto
O PSP-UFU permite a importação de arquivos dos seguintes programas:
- [ANAREDE](http://www.cepel.br/pt_br/produtos/programas-computacionais-por-categoria/menu/anarede-analise-de-redes-eletricas.htm)
- [Matpower](https://matpower.org/)

Para importação dos arquivos do **ANAREDE** são utilizados tanto o arquivo de dados elétricos (*.pwf*) quanto de dados gráficos dos elementos (*.lst*).

Uma vez que o arquivo do **Matpower** (*.m*) não possui dados gráficos dos elementos elétricos, o diagrama unifilar é automaticamente gerado pelo PSP-UFU ao importá-lo.

#### Geração automática do *layout* de diagramas unifilares
Para criar o *layout* automático, utilizou-se a teoria dos grafos baseada no posicionamento direcionado à força e aplicação de grafos com arestas ponderadas. De acordo com a teoria dos grafos, o sistema é modelado como vértices e arestas. Nesse contexto, os barramentos são os vértices e os ramos do sistema (linhas e transformadores) são as arestas. As localizações de elementos de derivação, assim como os nós dos elementos, são automaticamente controladas pelo PSP-UFU.

A ferramenta de *layout* automático é composta por um processo iterativo e os resultados são mais refinados com um número maior de iterações. Os melhores resultados são obtidos utilizando o número de iterações igual ou superior a cinco vezes o número de barramentos do sistema.

:::caution Atenção!
O *layout* automático ainda está em desenvolvimento e algumas melhorias podem ser feitas para evitar cruzamentos e sobreposições, porém, em seu estado atual, é totalmente funcional e satisfatório.
:::

:::tip Dica
Uma vez que o Matpower pode converter os formatos CDF (*Common Data Format*) e PSS/E RAW para arquivos *.m*, estes podem ser importados de forma indireta para o PSP-UFU.
:::