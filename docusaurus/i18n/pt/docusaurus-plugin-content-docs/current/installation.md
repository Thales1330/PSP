---
id: installation
title: Instalação
sidebar_label: Instalação
---
import useBaseUrl from "@docusaurus/useBaseUrl";

## Download

O PSP-UFU pode ser baixado em sua [*última versão* no repositório PSP-UFU no GitHub](https://github.com/Thales1330/PSP/releases/latest).

Ao acessar o link, role a página até encontrar **Assets**:

<div><center><img src={useBaseUrl("images/download.png")} alt="Assets download" title="Informações da Figura" /></center></div>

Como o PSP-UFU é um software multiplataforma, é necessário baixar o arquivo correto:
- Para o sistema operacional [Windows](installation#windows) baixe os arquivos *.exe* (recomendado) ou *.zip*.
- Para sistemas [Linux](installation#linux) baseados em Debian (por exemplo, Ubuntu, Mint) *64 bits* baixe o pacote de instalação *.deb*.

:::tip Dica
Se você estiver interessado em contribuir com o PSP-UFU construindo-o para outros sistemas operacionais (principalmente macOS!) Entre em contato conosco pelo [GitHub](https://github.com/Thales1330/PSP/issues) ou pelo [Twitter/X](https://twitter.com/PspUfu).
:::

A seguir estão as instruções passo a passo para instalar os sistemas operacionais [Windows](installation#windows) e [Linux](installation#linux). Também são apresentadas soluções para os erros mais recorrentes.

## Windows
Após baixar o arquivo *.exe*, prossiga com cada etapa do instalador. O executável solicitará automaticamente a instalação do **Microsoft Visual C++ Redistributable** e, caso não esteja instalado em seu computador, realizará os procedimentos de instalação.

Se o executável não abrir, baixe o arquivo *.zip* e descompacte-o em uma pasta no seu sistema. Nesse caso, é necessário baixar e instalar manualmente o [**Microsoft Visual C++ Redistributable 32 bits**](https://aka.ms/vs/16/release/vc_redist.x86.exe).

Após a conclusão da instalação, abra o PSP-UFU para verificar a instalação correta. O executável está presente na pasta "*\<caminho_instalação\>/PSP-UFU/bin/PSP-UFU.exe*".

### Problemas recorrentes do Windows
- Ao baixar o navegador você pode sinalizar **falsamente** que se trata de um arquivo perigoso, basta clicar em "Manter arquivo";
- Ao abrir o instalador, o Windows sinaliza novamente que o arquivo é perigoso, basta clicar em “Executar mesmo assim”;
- Ao abrir o programa e alterar o idioma, é exibida uma mensagem de erro. Para resolver esse problema, basta executar o PSP-UFU como administrador (clicar com o botão direito no atalho do programa e depois executar como administrador). Mude novamente o idioma e reinicie o programa;
- Ao criar um novo projeto, a tela de trabalho não é exibida ou é exibida uma tela preta. Vá para as configurações gerais e altere o renderizador para "Contexto do dispositivo". Se aparecer uma mensagem de erro, execute o programa como administrador e repita o processo. Reinicie o programa.


## Linux
A instalação no sistema Linux é facilmente realizada pelo pacote de instalação automática *.deb*.
:::caution Atenção!
Observe que esta versão para Linux está desatualizada e alguns recursos do programa podem não estar presentes.

Se você quiser contribuir com o projeto e construir a versão atual no sistema Linux baseado em Debian ou outras distribuições, entre em contato conosco através do [GitHub](https://github.com/Thales1330/PSP/issues) ou [Twitter/X]( https://twitter.com/PspUfu).
:::

### Problemas recorrentes do Linux
Pode ser necessário adicionar o [diretório de bibliotecas wxWidgets](https://wiki.wxwidgets.org/Compiling_and_getting_started#Running_wxWidgets_projects) nas variáveis ​​de ambiente.
Para fazer isso, siga **UM** dos métodos apresentados abaixo:

#### *Método I (recomendado)*
Digite o seguinte comando no arquivo **~ / .bashrc**:
```shell
echo "exportar LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> ~/.bashrc
```

#### *Método II (não recomendado)*
:::warning Cuidado!
Utilizando este método, os passos abaixo devem ser executados **toda vez** que você quiser abrir o PSP-UFU.
:::
1. Abra o terminal e acesse o diretório onde está instalado o PSP-UFU:
```shell
cd/usr/local/bin
```
2. Insira as bibliotecas compartilhadas de wxWidgets nas variáveis ​​de ambiente:
```shell
exportar LD_LIBRARY_PATH = $ LD_LIBRARY_PATH: /usr/local/lib
```
3. Em seguida, execute o PSP-UFU na **mesma seção** do terminal:
```shell
./PSP-UFU
```

:::warning Atenção!
Verifique se o computador possui as unidades de vídeo instaladas corretamente, caso contrário o programa não exibirá a área de trabalho para criar e editar diagramas unifilares ou diagramas de blocos de controle.
:::