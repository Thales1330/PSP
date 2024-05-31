---
id: simulationConfig
title: Configurações da Simulação
sidebar_label: Configurações da Simulação
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

As parametrizações e configurações das simulações são acessadas no [menu Simulação](mainScreen#ribbon-menu) clicando no ícone do botão **Configurações de simulação**.

<div><center><img src={useBaseUrl("images/menuSimulationSettings.svg")} alt="Menu Simulação" title="Menu Simulação" /></center></div>

## Formulário de edição das configurações de simulação

A imagem abaixo apresenta o formulário de edição das configurações de simulação:

<div><center><img src={useBaseUrl("images/simulationConfigForm.png")} alt="Configurações de simulação" title="Configurações de simulação" /></center></div>

Esse formulário é subdividido em quatro contextos distintos:
- **Geral**: no qual são inseridas informações gerais e comuns a várias simulações;
- **Fluxo de carga**: local onde é selecionado o [método de solução numérica](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu), assim como seus parâmetros;
- **Estabilidade**: contendo opções de simulação no tempo e parâmetros do [método de integração](stability#integração-numérica);
- **Carga ZIP**: contém as opções gerais das [cargas do tipo ZIP](load#carga-no-estudo-de-estabilidade) no estudo de [estabilidade](stability).

---
<Tabs
  groupId="simulationConfigs-tabs"
  defaultValue="general"
  values={[
    {label: 'Geral', value: 'general'},
    {label: 'Fluxo de carga', value: 'powerFlow'},
    {label: 'Estabilidade', value: 'stability'},
    {label: 'Carga ZIP', value: 'zipLoad'},
  ]
}>
<TabItem value="general">

#### Potência base
Potência base do sistema utilizada para conversão dos dados reais em $p.u.$ e vice-versa. Pode ser inserido em VA, kVA ou MVA.

:::tip Dica
A potência base de cada elemento pode ser distinta da potência base do sistema. Para isso, basta marcar a opção "Utilizar potência nominal \[*do elemento*\] como base", presente em seus respectivos [formulários de edição da dados](powerEditor).
:::

#### Frequência do sistema
Define a frequência nominal do sistema.

:::warning Cuidado!
Ao alterar a frequência nominal atente-se ao campo "[Frequência de circuito aberto](syncGenerator#frequência-de-circuito-aberto)" das máquinas síncronas.
:::

#### Cálculo contínuo
Habilita ou desabilita o cálculo contínuo para os cálculos de [curto-circuito](fault), [nível de curto-circuito](fault) e [distorções harmônicas](harmonics). O cálculo contínuo para o [fluxo de carga](powerFlow) é sempre habilitado.

:::tip Dica
Para habilitar o cálculo contínuo aperte o botão **Habilitar solução** presente no [menu Simulação](mainScreen#ribbon-menu).

Com essa opção, os cálculos estáticos selecionados são automaticamente realizados ao modificar quaisquer parâmetros da rede, como dados elétricos e acionamento dos disjuntores dos elementos (remoção ou inserção).
:::

</TabItem>
<TabItem value="powerFlow">

#### Método de solução
Define o método de solução numérica para o estudo de [fluxo de carga](powerFlow). Na versão atual, está presente no PSP-UFU os seguintes métodos:
- [Gauss-Seidel](powerFlow#gauss-seidel)
- [Newton-Raphson](powerFlow#newton-raphson)
- [Gauss-Newton híbrido](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu)

#### Fator de aceleração
Fator utilizado para acelerar a convergência nos métodos de Gauss. A utilização do fator de aceleração é realizada por meio da equação:
$$
\dot{V}_{i_{AC}}^{(v + 1)} = \alpha \left( \dot{V}_{i}^{(v + 1)}- \dot{V}_{i_{AC}}^{(v)} \right) + \dot{V}_{i_{AC}}^{(v)}
$$
Em que:
- $\dot{V}_{i_{AC}}^{(v + 1)}$ é a tensão complexa da iteração atual com fator de aceleração aplicado
- $\dot{V}_{i}^{(v + 1)}$ é a tensão complexa da iteração atual
- $\dot{V}_{i_{AC}}^{(v)}$ é a tensão complexa da iteração anterior com fator de aceleração aplicado
- $\alpha$ é o fator de aceleração

Essa opção é habilitada somente para os métodos de [Gauss-Seidel](powerFlow#gauss-seidel) e [Gauss-Newton híbrido](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu).

#### Tolerância
É o valor de erro entre as iterações (em $p.u.$) no qual é obtida a convergência do método numérico.

Para o método de [Gauss-Seidel](powerFlow#gauss-seidel) o erro é calculado pelo maior valor do módulo da diferença entre a tensão na iteração anterior e atual ($e = \left| V_{i}^{(v + 1)} - V_{i}^{(v)} \right|$); enquanto que no método de [Newton-Raphson](powerFlow#newton-raphson) esse erro é o módulo do maior valor de ajuste de potência ($|\Delta P|$ ou $|\Delta Q|$).

#### Iterações máx
É o número máximo de iterações que um método poderá alcançar. Caso esse valor seja alcançado, o cálculo de [fluxo de carga](powerFlow) é interrompido com erro.

#### Ângulo da barra de referência
É o valor do ângulo da [barra de referência](powerFlow), em graus.

#### Inércia do Newton
É o valor que multiplica as correções de potência. Valores maiores que $1{,}0$ incrementam a correção e menores que $1{,}0$ diminuem a correção.

:::info Informação
Valores menores que $1{,}0$ *podem* auxiliar na convergência, porém aumentam o número de iterações e consequentemente o tempo de processamento do método.
:::

Essa opção é habilitada somente para o método de [Newton-Raphson](powerFlow#newton-raphson).

#### Tolerância do Gauss
Define a tolerância do Gauss-Seidel para o método híbrido. Essa opção é habilitada somente para o método de [Gauss-Newton híbrido](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu).

Uma vez que o Newton-Raphson é consideravelmente sensível às condições iniciais, as primeiras iterações são calculadas utilizado o Gauss-Seidel até que o erro fique menor que a tolerância especificada no campo “Tolerância do Gauss”. A partir desse ponto o cálculo é realizado utilizando o método de Newton-Raphson até que se obtenha a convergência com um erro menor que a tolerância estipulada no campo “Tolerância”.

:::info Informação
Tal opção auxilia na convergência de sistemas impossíveis de resolver utilizando Newton-Raphson convencional.
:::

</TabItem>
<TabItem value="stability">

#### Passo de integração
Define o passo de integração para o método de integração [Trapezoidal Implícito](stability#integração-numérica).

:::caution Atenção!
Valores muito elevados poderão gerar erros de simulação, enquanto valores muito pequenos aumentarão significativamente o tempo de processamento do cálculo de estabilidade.
:::

#### Tempo de simulação
Define o tempo total de simulação, em segundos.

#### Tolerância
Tolerância do processo iterativo (em $p.u.$) do método de integração [Trapezoidal Implícito](stability#integração-numérica).

#### Iterações máx
Número máximo de iterações do processo iterativo do método de integração [Trapezoidal Implícito](stability#integração-numérica). Caso o número de iterações ultrapasse esse valor, o cálculo de estabilidade será encerrado com erro.

#### Razão de passo dos controles
Define a quantidade passos de integração que o sistema de controle será submetido para $1$ passo de integração do sistema de potência.

Por exemplo, se o passo de integração for definido em $0{,}01~s$ e a razão de passo como $10$, o valor efetivo do passo de integração para os sistemas de controle será de $0{,}001~s$ (10 vezes menor).

:::info Informação
A utilização desse campo se justifica em situações de grandes diferenças nas constantes de tempo das máquinas síncronas (geralmente maiores) e dos sistemas de controle (geralmente menores).
:::

:::tip Dica
Um valor de razão de passo de controles igual a $1$ não irá alterar o passo de integração do sistema de controle.
:::

#### Tempo de impressão
É o intervalo de tempo o qual o programa irá armazenar os dados para impressão em [gráficos no tempo](graphViewer).

:::warning Cuidado!
Valores muito pequenos (menores que $0{,}001~s$) podem aumentar significativamente o tempo de processamento do cálculo de estabilidade.

O tempo de impressão **deve ser maior ou igual** ao passo de integração.
:::

</TabItem>
<TabItem value="zipLoad">

#### Utilizar composição geral para todas as cargas do sistema
Habilita a composição da [carga ZIP](load#carga-no-estudo-de-estabilidade) para todas as cargas que **não** possuem parametrização individual.

#### Composição da carga (potências ativa e reativa)
Define a composição, em porcentagem, das parcelas de Impedância, Corrente e Potência constantes da carga. Essa composição pode ser implementada individualmente para a potência ativa e reativa.

:::caution Atenção!
A soma das parcelas da carga ZIP para uma potência deve ser 100%, caso contrário será exibida uma mensagem de erro.
:::

#### Subtensão a qual as cargas serão modeladas como impedância constante
Define o valor percentual da tensão ($V_{low}$) que, para valores inferiores a esse, a carga passa a ser considerada como impedância constante pura.

:::info Informação
Cargas de corrente e potência constantes possuem problemas em tensões muito baixas. À medida que a tensão diminui as correntes dessas cargas não reduz, resultando em perda de precisão e problemas na convergência de processos iterativos. Para contornar esse problema utiliza-se uma tensão pré-definida ($V_{low}$), a qual as cargas (ou parcelas) de corrente e potência constantes são modeladas como impedância constante.
:::

</TabItem>
</Tabs>

---