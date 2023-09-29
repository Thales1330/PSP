"use strict";(self.webpackChunkpsp_ufu=self.webpackChunkpsp_ufu||[]).push([[4946],{3905:function(e,a,r){r.d(a,{Zo:function(){return p},kt:function(){return f}});var t=r(7294);function o(e,a,r){return a in e?Object.defineProperty(e,a,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[a]=r,e}function n(e,a){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var t=Object.getOwnPropertySymbols(e);a&&(t=t.filter((function(a){return Object.getOwnPropertyDescriptor(e,a).enumerable}))),r.push.apply(r,t)}return r}function i(e){for(var a=1;a<arguments.length;a++){var r=null!=arguments[a]?arguments[a]:{};a%2?n(Object(r),!0).forEach((function(a){o(e,a,r[a])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):n(Object(r)).forEach((function(a){Object.defineProperty(e,a,Object.getOwnPropertyDescriptor(r,a))}))}return e}function s(e,a){if(null==e)return{};var r,t,o=function(e,a){if(null==e)return{};var r,t,o={},n=Object.keys(e);for(t=0;t<n.length;t++)r=n[t],a.indexOf(r)>=0||(o[r]=e[r]);return o}(e,a);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);for(t=0;t<n.length;t++)r=n[t],a.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(o[r]=e[r])}return o}var l=t.createContext({}),d=function(e){var a=t.useContext(l),r=a;return e&&(r="function"==typeof e?e(a):i(i({},a),e)),r},p=function(e){var a=d(e.components);return t.createElement(l.Provider,{value:a},e.children)},u="mdxType",m={inlineCode:"code",wrapper:function(e){var a=e.children;return t.createElement(t.Fragment,{},a)}},c=t.forwardRef((function(e,a){var r=e.components,o=e.mdxType,n=e.originalType,l=e.parentName,p=s(e,["components","mdxType","originalType","parentName"]),u=d(r),c=o,f=u["".concat(l,".").concat(c)]||u[c]||m[c]||n;return r?t.createElement(f,i(i({ref:a},p),{},{components:r})):t.createElement(f,i({ref:a},p))}));function f(e,a){var r=arguments,o=a&&a.mdxType;if("string"==typeof e||o){var n=r.length,i=new Array(n);i[0]=c;var s={};for(var l in a)hasOwnProperty.call(a,l)&&(s[l]=a[l]);s.originalType=e,s[u]="string"==typeof e?e:o,i[1]=s;for(var d=2;d<n;d++)i[d]=r[d];return t.createElement.apply(null,i)}return t.createElement.apply(null,r)}c.displayName="MDXCreateElement"},9045:function(e,a,r){r.r(a),r.d(a,{assets:function(){return u},contentTitle:function(){return d},default:function(){return g},frontMatter:function(){return l},metadata:function(){return p},toc:function(){return m}});var t=r(7462),o=r(3366),n=(r(7294),r(3905)),i=r(4996),s=["components"],l={id:"graphViewer",title:"Visualizador de Gr\xe1ficos",sidebar_label:"Visualizador de Gr\xe1ficos"},d=void 0,p={unversionedId:"graphViewer",id:"graphViewer",title:"Visualizador de Gr\xe1ficos",description:"Para exibi\xe7\xe3o dos resultados originados dos c\xe1lculos de estabilidade e varredura de frequ\xeancias, desenvolveu-se um visualizador de gr\xe1ficos, como mostra a figura abaixo:",source:"@site/docs/graphViewer.md",sourceDirName:".",slug:"/graphViewer",permalink:"/PSP/docs/graphViewer",draft:!1,editUrl:"https://github.com/Thales1330/PSP/tree/master/docusaurus/docs/graphViewer.md",tags:[],version:"current",frontMatter:{id:"graphViewer",title:"Visualizador de Gr\xe1ficos",sidebar_label:"Visualizador de Gr\xe1ficos"},sidebar:"docs",previous:{title:"Relat\xf3rios Tabulares",permalink:"/PSP/docs/tabularReport"},next:{title:"Mapa de Tens\xe3o",permalink:"/PSP/docs/heatmap"}},u={},m=[{value:"Cria\xe7\xe3o, manipula\xe7\xe3o e navega\xe7\xe3o de curvas",id:"cria\xe7\xe3o-manipula\xe7\xe3o-e-navega\xe7\xe3o-de-curvas",level:2}],c={toc:m},f="wrapper";function g(e){var a=e.components,r=(0,o.Z)(e,s);return(0,n.kt)(f,(0,t.Z)({},c,r,{components:a,mdxType:"MDXLayout"}),(0,n.kt)("link",{rel:"stylesheet",href:(0,i.Z)("katex/katex.min.css")}),(0,n.kt)("p",null,"Para exibi\xe7\xe3o dos resultados originados dos c\xe1lculos de ",(0,n.kt)("a",{parentName:"p",href:"stability"},"estabilidade")," e ",(0,n.kt)("a",{parentName:"p",href:"harmonics"},"varredura de frequ\xeancias"),", desenvolveu-se um visualizador de gr\xe1ficos, como mostra a figura abaixo:"),(0,n.kt)("div",null,(0,n.kt)("center",null,(0,n.kt)("img",{src:(0,i.Z)("images/graphViewer.png"),alt:"Visualizador de gr\xe1ficos do PSP-UFU",title:"Visualizador de gr\xe1ficos do PSP-UFU"}))),(0,n.kt)("h2",{id:"cria\xe7\xe3o-manipula\xe7\xe3o-e-navega\xe7\xe3o-de-curvas"},"Cria\xe7\xe3o, manipula\xe7\xe3o e navega\xe7\xe3o de curvas"),(0,n.kt)("p",null,"Tal visualizador utiliza a biblioteca ",(0,n.kt)("a",{parentName:"p",href:"https://wxmathplot.sourceforge.io/"},"wxMathPlot"),", e permite ao usu\xe1rio imprimir todas as curvas no tempo originadas dos resultados de ",(0,n.kt)("a",{parentName:"p",href:"stability"},"estabilidade")," e ",(0,n.kt)("a",{parentName:"p",href:"harmonics"},"varredura de frequ\xeancias"),", acessados em dados hier\xe1rquicos em forma de \xe1rvore, identificados pelo tipo e nome do elemento de pot\xeancia, al\xe9m do tipo de curva."),(0,n.kt)("admonition",{title:"Informa\xe7\xe3o",type:"info"},(0,n.kt)("p",{parentName:"admonition"},"As curvas podem ser plotadas com ",(0,n.kt)("strong",{parentName:"p"},"clique duplo"),' sobre ela ou ao selecionar a caixa "Plotar" nas propriedades do tra\xe7o.')),(0,n.kt)("p",null,"Os gr\xe1ficos impressos podem ser caracterizados pela edi\xe7\xe3o de suas propriedades do tra\xe7o, ou seja, pode-se alterar a cor, espessura e tipo de tra\xe7o (seis tipos dispon\xedveis). Por padr\xe3o o eixo vertical \xe9 o tempo (em estabilidade) ou frequ\xeancia (varredura de frequ\xeancias), por\xe9m o usu\xe1rio pode alterar para qualquer curva dispon\xedvel."),(0,n.kt)("p",null,"V\xe1rias propriedades gerais do gr\xe1fico podem ser editadas: t\xedtulo do gr\xe1fico, r\xf3tulos dos eixos x e y, margens e limites inferiores e superiores de ambos os eixos."),(0,n.kt)("p",null,"O menu ",(0,n.kt)("strong",{parentName:"p"},"Visualiza\xe7\xe3o")," permite a inser\xe7\xe3o de exibi\xe7\xe3o de legenda, linhas de grade e coordenadas do ponteiro do mouse (utilizada para destacar pontos na curva), al\xe9m da possibilidade de alternar o tema de impress\xe3o entre escuro e claro, facilitando a visualiza\xe7\xe3o ou exporta\xe7\xe3o do gr\xe1fico."),(0,n.kt)("p",null,"No menu ",(0,n.kt)("strong",{parentName:"p"},"Arquivo"),", pode-se salvar o gr\xe1fico na forma de imagem (",(0,n.kt)("em",{parentName:"p"},".png"),", ",(0,n.kt)("em",{parentName:"p"},".jpg")," ou ",(0,n.kt)("em",{parentName:"p"},".bmp"),") ou envi\xe1-lo diretamente para a \xe1rea de transfer\xeancia. No mesmo menu tamb\xe9m \xe9 permitido a exporta\xe7\xe3o no formato ",(0,n.kt)("em",{parentName:"p"},(0,n.kt)("a",{parentName:"em",href:"https://en.wikipedia.org/wiki/Comma-separated_values"},"CSV (Comma-separated Values)")),", o qual poder\xe1 ser utilizado em outros visualizadores de gr\xe1ficos, como o Excel ou o ",(0,n.kt)("a",{parentName:"p",href:"http://www.gnuplot.info/"},"gnuplot"),"."),(0,n.kt)("p",null,"A \xe1rea de plotagem permite a navega\xe7\xe3o utilizando ferramentas de zoom, por \xe1rea de sele\xe7\xe3o e comandos de mouse, e arrasto das curvas impressas."),(0,n.kt)("admonition",{title:"Dica",type:"tip"},(0,n.kt)("p",{parentName:"admonition"},"Ao clicar com o bot\xe3o direito sobre a \xe1rea de plotagem as op\xe7\xf5es de manipula\xe7\xe3o s\xe3o exibidas por meio de um menu de contexto.")),(0,n.kt)("p",null,"Os seguintes ",(0,n.kt)("strong",{parentName:"p"},"comandos de mouse")," s\xe3o utilizados para navega\xe7\xe3o das curvas:"),(0,n.kt)("table",null,(0,n.kt)("thead",{parentName:"table"},(0,n.kt)("tr",{parentName:"thead"},(0,n.kt)("th",{parentName:"tr",align:null},"Comando"),(0,n.kt)("th",{parentName:"tr",align:null},"A\xe7\xe3o"))),(0,n.kt)("tbody",{parentName:"table"},(0,n.kt)("tr",{parentName:"tbody"},(0,n.kt)("td",{parentName:"tr",align:null},"Bot\xe3o esquerdo pressionado + Selecionar \xe1rea"),(0,n.kt)("td",{parentName:"tr",align:null},"Seleciona a \xe1rea de zoom")),(0,n.kt)("tr",{parentName:"tbody"},(0,n.kt)("td",{parentName:"tr",align:null},"Bot\xe3o direito pressionado + Mover mouse"),(0,n.kt)("td",{parentName:"tr",align:null},"Arrasta a curva")),(0,n.kt)("tr",{parentName:"tbody"},(0,n.kt)("td",{parentName:"tr",align:null},"Roda do mouse"),(0,n.kt)("td",{parentName:"tr",align:null},"Arrasto vertical")),(0,n.kt)("tr",{parentName:"tbody"},(0,n.kt)("td",{parentName:"tr",align:null},"Roda do mouse + Shift"),(0,n.kt)("td",{parentName:"tr",align:null},"Arrasto horizontal")),(0,n.kt)("tr",{parentName:"tbody"},(0,n.kt)("td",{parentName:"tr",align:null},"Roda do mouse + Ctrl"),(0,n.kt)("td",{parentName:"tr",align:null},"Mais/menos zoom")))))}g.isMDXComponent=!0}}]);