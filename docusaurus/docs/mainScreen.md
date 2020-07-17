---
id: mainScreen
title: Tela Principal
sidebar_label: Tela Principal
---
## Organização da tela principal
A tela principal do PSP-UFU é dividida em três seções: *[Menu "Ribbon"](mainScreen#menu-ribbon)*, *[Área de Trabalho](mainScreen#área-de-trabalho)* e *[Barra de Status](mainScreen#barra-de-status)*.

![Tela Principal][mainScreen]
[mainScreen]: /PSP/images/mainScreen.svg 'Tela principal do PSP-UFU no Windows'

Resumidamente, no **Menu Ribbon** são acessadas as ferramentas do PSP-UFU; na **Área de Trabalho** são criados os diagramas unifilares de potência; e na **Barra de Status** são exibidas importantes informações acerca do estado da Área de Trabalho.


### Menu Ribbon
>Em informática o ribbon (faixa, em inglês) é um formato de apresentação de interface baseada na GUI onde a barra de ferramentas é mostrada através de uma barra mais larga com icones maiores possibilitando o uso dos aplicativos por dispositivos *touch screen*.
[Wikipedia](https://pt.wikipedia.org/wiki/Ribbon_(inform%C3%A1tica))

O menu Ribbon é composto de três submenus: “Arquivo”, “Ferramentas” e “Simulação”.

Em **Arquivo** estão as ferramentas gerais do programa, as quais permitem ao usuário criar e salvar novos projetos, além daquelas que possibilitam carregar os projetos já existentes e importar arquivos de outros programas. Esse submenu também possui opções gerais do programa e acesso a informações sobre o programa.

![Menu Arquivo][menuFile]
[menuFile]: /PSP/images/menuFile.png 'Menu Arquivo'

Em **Ferramentas** estão presentes os itens específicos, os quais estão relacionados à criação e manipulação da rede elétrica de potência, relatórios tabulares de saída de dados e acesso ao formulário de criação de gráficos no tempo. Algumas configurações específicas do projeto ativo são acessadas por esse menu.

![Menu Ferramentas][menuTools]
[menuTools]: /PSP/images/menuTools.png 'Menu Ferramentas'

Finalmente, o submenu **Simulação** proporciona o acesso do usuário aos cálculos realizados pelo *software* e suas configurações, além da ferramenta de “solução contínua”, a qual resolve os cálculos estáticos (fluxo de potência, curto-circuito e harmônicos) após quaisquer mudanças na rede de potência, como remoção de componente ou alteração de seus dados. Todas as configurações de simulação do projeto ativo são acessadas por esse menu.

![Menu Simulation][menuSimulation]
[menuSimulation]: /PSP/images/menuSimulation.png 'Menu Simulação'

### Área de Trabalho
Na área de trabalho estão presentes os projetos em execução, os quais são divididos por abas identificadas pelo nome do projeto (caso já esteja gravado no disco). É nessa área que são inseridos e excluídos os elementos elétricos, cujas ferramentas são acessadas por meio do [Menu Ribbon](mainScreen#menu-ribbon).

As edições gráficas dos elementos, como posição, conexão e tamanho dos barramentos, são realizadas por meio da ação *drag-and-drop* (arrastar e soltar) com a utilização do mouse. A edição dos dados elétricos é feita por meio de formulários de dados acessados clicando duas vezes sobre os elementos.

![Área de Trabalho][workspace]
[workspace]: /PSP/images/workspace.png 'Área de Trabalho'

### Barra de Status
A barra de status é responsável por informações interessantes sobre as circunstâncias atuais do programa: modo de operação do mouse (editar, mover, arrastar), zoom aplicado e posição do mouse na área de trabalho, além de dados sobre as ações dos usuários, como: inserção de elementos, informação sobre copiar e colar, etc.