"use strict";(self.webpackChunkpsp_ufu=self.webpackChunkpsp_ufu||[]).push([[9947],{5748:(e,o,a)=>{a.r(o),a.d(o,{assets:()=>l,contentTitle:()=>t,default:()=>p,frontMatter:()=>d,metadata:()=>n,toc:()=>c});var r=a(4848),s=a(8453),i=a(6025);const d={id:"graphViewer",title:"Visualizador de Gr\xe1ficos",sidebar_label:"Visualizador de Gr\xe1ficos"},t=void 0,n={id:"graphViewer",title:"Visualizador de Gr\xe1ficos",description:"Para exibi\xe7\xe3o dos resultados originados dos c\xe1lculos de estabilidade e varredura de frequ\xeancias, desenvolveu-se um visualizador de gr\xe1ficos, como mostra a figura abaixo:",source:"@site/docs/graphViewer.md",sourceDirName:".",slug:"/graphViewer",permalink:"/PSP/docs/graphViewer",draft:!1,unlisted:!1,editUrl:"https://github.com/Thales1330/PSP/tree/master/docusaurus/docs/graphViewer.md",tags:[],version:"current",frontMatter:{id:"graphViewer",title:"Visualizador de Gr\xe1ficos",sidebar_label:"Visualizador de Gr\xe1ficos"},sidebar:"docs",previous:{title:"Relat\xf3rios Tabulares",permalink:"/PSP/docs/tabularReport"},next:{title:"Mapa de Tens\xe3o",permalink:"/PSP/docs/heatmap"}},l={},c=[{value:"Cria\xe7\xe3o, manipula\xe7\xe3o e navega\xe7\xe3o de curvas",id:"cria\xe7\xe3o-manipula\xe7\xe3o-e-navega\xe7\xe3o-de-curvas",level:2}];function u(e){const o={a:"a",admonition:"admonition",em:"em",h2:"h2",p:"p",strong:"strong",table:"table",tbody:"tbody",td:"td",th:"th",thead:"thead",tr:"tr",...(0,s.R)(),...e.components};return(0,r.jsxs)(r.Fragment,{children:[(0,r.jsx)("link",{rel:"stylesheet",href:(0,i.A)("katex/katex.min.css")}),"\n",(0,r.jsxs)(o.p,{children:["Para exibi\xe7\xe3o dos resultados originados dos c\xe1lculos de ",(0,r.jsx)(o.a,{href:"stability",children:"estabilidade"})," e ",(0,r.jsx)(o.a,{href:"harmonics",children:"varredura de frequ\xeancias"}),", desenvolveu-se um visualizador de gr\xe1ficos, como mostra a figura abaixo:"]}),"\n",(0,r.jsx)("div",{children:(0,r.jsx)("center",{children:(0,r.jsx)("img",{src:(0,i.A)("images/graphViewer.png"),alt:"Visualizador de gr\xe1ficos do PSP-UFU",title:"Visualizador de gr\xe1ficos do PSP-UFU"})})}),"\n",(0,r.jsx)(o.h2,{id:"cria\xe7\xe3o-manipula\xe7\xe3o-e-navega\xe7\xe3o-de-curvas",children:"Cria\xe7\xe3o, manipula\xe7\xe3o e navega\xe7\xe3o de curvas"}),"\n",(0,r.jsxs)(o.p,{children:["Tal visualizador utiliza a biblioteca ",(0,r.jsx)(o.a,{href:"https://wxmathplot.sourceforge.io/",children:"wxMathPlot"}),", e permite ao usu\xe1rio imprimir todas as curvas no tempo originadas dos resultados de ",(0,r.jsx)(o.a,{href:"stability",children:"estabilidade"})," e ",(0,r.jsx)(o.a,{href:"harmonics",children:"varredura de frequ\xeancias"}),", acessados em dados hier\xe1rquicos em forma de \xe1rvore, identificados pelo tipo e nome do elemento de pot\xeancia, al\xe9m do tipo de curva."]}),"\n",(0,r.jsx)(o.admonition,{title:"Informa\xe7\xe3o",type:"info",children:(0,r.jsxs)(o.p,{children:["As curvas podem ser plotadas com ",(0,r.jsx)(o.strong,{children:"clique duplo"}),' sobre ela ou ao selecionar a caixa "Plotar" nas propriedades do tra\xe7o.']})}),"\n",(0,r.jsx)(o.p,{children:"Os gr\xe1ficos impressos podem ser caracterizados pela edi\xe7\xe3o de suas propriedades do tra\xe7o, ou seja, pode-se alterar a cor, espessura e tipo de tra\xe7o (seis tipos dispon\xedveis). Por padr\xe3o o eixo vertical \xe9 o tempo (em estabilidade) ou frequ\xeancia (varredura de frequ\xeancias), por\xe9m o usu\xe1rio pode alterar para qualquer curva dispon\xedvel."}),"\n",(0,r.jsx)(o.p,{children:"V\xe1rias propriedades gerais do gr\xe1fico podem ser editadas: t\xedtulo do gr\xe1fico, r\xf3tulos dos eixos x e y, margens e limites inferiores e superiores de ambos os eixos."}),"\n",(0,r.jsxs)(o.p,{children:["O menu ",(0,r.jsx)(o.strong,{children:"Visualiza\xe7\xe3o"})," permite a inser\xe7\xe3o de exibi\xe7\xe3o de legenda, linhas de grade e coordenadas do ponteiro do mouse (utilizada para destacar pontos na curva), al\xe9m da possibilidade de alternar o tema de impress\xe3o entre escuro e claro, facilitando a visualiza\xe7\xe3o ou exporta\xe7\xe3o do gr\xe1fico."]}),"\n",(0,r.jsxs)(o.p,{children:["No menu ",(0,r.jsx)(o.strong,{children:"Arquivo"}),", pode-se salvar o gr\xe1fico na forma de imagem (",(0,r.jsx)(o.em,{children:".png"}),", ",(0,r.jsx)(o.em,{children:".jpg"})," ou ",(0,r.jsx)(o.em,{children:".bmp"}),") ou envi\xe1-lo diretamente para a \xe1rea de transfer\xeancia. No mesmo menu tamb\xe9m \xe9 permitido a exporta\xe7\xe3o no formato ",(0,r.jsx)(o.em,{children:(0,r.jsx)(o.a,{href:"https://en.wikipedia.org/wiki/Comma-separated_values",children:"CSV (Comma-separated Values)"})}),", o qual poder\xe1 ser utilizado em outros visualizadores de gr\xe1ficos, como o Excel ou o ",(0,r.jsx)(o.a,{href:"http://www.gnuplot.info/",children:"gnuplot"}),"."]}),"\n",(0,r.jsx)(o.p,{children:"A \xe1rea de plotagem permite a navega\xe7\xe3o utilizando ferramentas de zoom, por \xe1rea de sele\xe7\xe3o e comandos de mouse, e arrasto das curvas impressas."}),"\n",(0,r.jsx)(o.admonition,{title:"Dica",type:"tip",children:(0,r.jsx)(o.p,{children:"Ao clicar com o bot\xe3o direito sobre a \xe1rea de plotagem as op\xe7\xf5es de manipula\xe7\xe3o s\xe3o exibidas por meio de um menu de contexto."})}),"\n",(0,r.jsxs)(o.p,{children:["Os seguintes ",(0,r.jsx)(o.strong,{children:"comandos de mouse"})," s\xe3o utilizados para navega\xe7\xe3o das curvas:"]}),"\n",(0,r.jsxs)(o.table,{children:[(0,r.jsx)(o.thead,{children:(0,r.jsxs)(o.tr,{children:[(0,r.jsx)(o.th,{children:"Comando"}),(0,r.jsx)(o.th,{children:"A\xe7\xe3o"})]})}),(0,r.jsxs)(o.tbody,{children:[(0,r.jsxs)(o.tr,{children:[(0,r.jsx)(o.td,{children:"Bot\xe3o esquerdo pressionado + Selecionar \xe1rea"}),(0,r.jsx)(o.td,{children:"Seleciona a \xe1rea de zoom"})]}),(0,r.jsxs)(o.tr,{children:[(0,r.jsx)(o.td,{children:"Bot\xe3o direito pressionado + Mover mouse"}),(0,r.jsx)(o.td,{children:"Arrasta a curva"})]}),(0,r.jsxs)(o.tr,{children:[(0,r.jsx)(o.td,{children:"Roda do mouse"}),(0,r.jsx)(o.td,{children:"Arrasto vertical"})]}),(0,r.jsxs)(o.tr,{children:[(0,r.jsx)(o.td,{children:"Roda do mouse + Shift"}),(0,r.jsx)(o.td,{children:"Arrasto horizontal"})]}),(0,r.jsxs)(o.tr,{children:[(0,r.jsx)(o.td,{children:"Roda do mouse + Ctrl"}),(0,r.jsx)(o.td,{children:"Mais/menos zoom"})]})]})]})]})}function p(e={}){const{wrapper:o}={...(0,s.R)(),...e.components};return o?(0,r.jsx)(o,{...e,children:(0,r.jsx)(u,{...e})}):u(e)}},8453:(e,o,a)=>{a.d(o,{R:()=>d,x:()=>t});var r=a(6540);const s={},i=r.createContext(s);function d(e){const o=r.useContext(i);return r.useMemo((function(){return"function"==typeof e?e(o):{...o,...e}}),[o,e])}function t(e){let o;return o=e.disableParentContext?"function"==typeof e.components?e.components(s):e.components||s:d(e.components),r.createElement(i.Provider,{value:o},e.children)}}}]);