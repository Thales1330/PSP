---
id: installation
title: Installation
sidebar_label: Installation
---
import useBaseUrl from "@docusaurus/useBaseUrl";

## Download

The PSP-UFU can be downloaded from the [*last release* of the PSP-UFU repository on GitHub](https://github.com/Thales1330/PSP/releases/latest).

When accessing the link, scroll down the page until you find the * Assets *:

<div><center><img src={useBaseUrl("images/download.png")} alt="Assets download" title="Informações da Figura" /></center></div>

As the PSP-UFU is a multiplatform software, you must download the correct file:
- For the operating system [Windows](installation#windows) download the files *.exe* (recommended) or *.zip*.
- For [Linux](installation#linux) systems based on Debian (for example, Ubuntu, Mint) *64 bits* download the installation package *.deb*.

:::tip Tip
If you are interested in contributing to PSP-UFU by building it for other operating systems (mainly macOS!) Contact us at [GitHub](https://github.com/Thales1330/PSP/issues) or [Twitter](https://twitter.com/PspUfu).
:::

Na sequência são apresentados o passo-a-passo para instalação dos SOs [Windows](installation#windows) e [Linux](installation#linux). Também são apresentadas as soluções para os erros mais recorrentes.

## Windows
Após baixar o arquivo *.exe* avance em cada passo do instalador. O executável irá solicitar automaticamente a instalação do **Microsoft Visual C++ Redistribuível** e, caso não esteja instalado no seu computador, realize os procedimentos do programa.

Caso o executável falhe em abrir, baixe o arquivo *.zip* e descompacte em alguma pasta do seu sistema. Nesse caso é necessário baixar e instalar manualmente o [**Microsoft Visual C++ Redistribuível 32 bits**](https://aka.ms/vs/16/release/vc_redist.x86.exe).

Após a conclusão da instalação abra o PSP-UFU para verificar a correta instalação. O executável está presente na pasta "*<local de instalação>/PSP-UFU/bin/PSP-UFU.exe*".

### Problemas recorrentes no Windows
- Ao baixar o navegador pode **falsamente** sinalizar que é um arquivo perigoso, basta clicar em "Manter arquivo";
- Ao abrir o instalador, novamente o Windows sinaliza que o arquivo é perigoso, basta clicar em "Executar assim mesmo";
- Ao abrir o programa e alterar o seu idioma, uma mensagem de erro é exibida. Para resolver esse problema basta executar o PSP-UFU como administrador (clicar com botão direito no atalho do programa e posteriormente em executar como administrador). Altere novamente o idioma e reinicie o programa;
- Ao criar um novo projeto a tela de trabalho não é exibida ou é exibida uma tela preta. Vá nas opções gerais e altere o renderizador para "Device Context". Caso apareça uma mensagem de erro, execute o programa como administrador e repita o processo. Reinicie o programa.


## Linux
A instalação no sistema Linux é facilmente realizado pelo pacote de instalação automático *.deb*.
:::caution Atenção!
Note que essa versão para Linux está defasada e algumas funcionalidades do programa podem não estar presentes.

Caso queira contribuir com o projeto e construir a versão atual no sistema Linux baseado em Debian ou em outras distribuições entre em contato pelo [GitHub](https://github.com/Thales1330/PSP/issues) ou [Twitter](https://twitter.com/PspUfu).
:::

### Problemas recorrentes no Linux
Pode ser necessário adicionar o [diretório das bibliotecas do wxWidgets](https://wiki.wxwidgets.org/Compiling_and_getting_started#Running_wxWidgets_projects) nas variáveis de ambiente.
Para isso, siga **UM** dos métodos apresentados abaixo:

#### *Método I (recomendado)*
Insira o seguinte comando no arquivo **~/.bashrc**:
```shell
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> ~/.bashrc
```

#### *Método II (não recomendado)*
:::warning Cuidado!
Utilizando esse método os passos abaixo devem ser executados **todas as vezes** que você quiser abrir o PSP-UFU.
:::
1. Abra o terminal e acesse o diretório onde está instalado o PSP-UFU:
```shell
cd /usr/local/bin
```
2. Insira as bibliotecas compartilhadas do wxWidgets nas variáveis de ambiente:
```shell
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```
3. Então, execute o PSP-UFU na **mesma seção** do terminal:
```shell
./PSP-UFU
```

:::caution Atenção!
Verifique se o computador possui as unidades de vídeo instaladas corretamente, caso contrário, o programa não exibirá a área de trabalho para criar e editar diagramas de linha única ou diagramas de blocos de controle.
:::
