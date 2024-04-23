"use strict";(self.webpackChunkpsp_ufu=self.webpackChunkpsp_ufu||[]).push([[7431],{3905:function(e,t,a){a.d(t,{Zo:function(){return p},kt:function(){return k}});var r=a(7294);function n(e,t,a){return t in e?Object.defineProperty(e,t,{value:a,enumerable:!0,configurable:!0,writable:!0}):e[t]=a,e}function o(e,t){var a=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),a.push.apply(a,r)}return a}function i(e){for(var t=1;t<arguments.length;t++){var a=null!=arguments[t]?arguments[t]:{};t%2?o(Object(a),!0).forEach((function(t){n(e,t,a[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(a)):o(Object(a)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(a,t))}))}return e}function l(e,t){if(null==e)return{};var a,r,n=function(e,t){if(null==e)return{};var a,r,n={},o=Object.keys(e);for(r=0;r<o.length;r++)a=o[r],t.indexOf(a)>=0||(n[a]=e[a]);return n}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(r=0;r<o.length;r++)a=o[r],t.indexOf(a)>=0||Object.prototype.propertyIsEnumerable.call(e,a)&&(n[a]=e[a])}return n}var s=r.createContext({}),d=function(e){var t=r.useContext(s),a=t;return e&&(a="function"==typeof e?e(t):i(i({},t),e)),a},p=function(e){var t=d(e.components);return r.createElement(s.Provider,{value:t},e.children)},m="mdxType",u={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},c=r.forwardRef((function(e,t){var a=e.components,n=e.mdxType,o=e.originalType,s=e.parentName,p=l(e,["components","mdxType","originalType","parentName"]),m=d(a),c=n,k=m["".concat(s,".").concat(c)]||m[c]||u[c]||o;return a?r.createElement(k,i(i({ref:t},p),{},{components:a})):r.createElement(k,i({ref:t},p))}));function k(e,t){var a=arguments,n=t&&t.mdxType;if("string"==typeof e||n){var o=a.length,i=new Array(o);i[0]=c;var l={};for(var s in t)hasOwnProperty.call(t,s)&&(l[s]=t[s]);l.originalType=e,l[m]="string"==typeof e?e:n,i[1]=l;for(var d=2;d<o;d++)i[d]=a[d];return r.createElement.apply(null,i)}return r.createElement.apply(null,a)}c.displayName="MDXCreateElement"},5146:function(e,t,a){a.r(t),a.d(t,{assets:function(){return m},contentTitle:function(){return d},default:function(){return f},frontMatter:function(){return s},metadata:function(){return p},toc:function(){return u}});var r=a(7462),n=a(3366),o=(a(7294),a(3905)),i=a(4996),l=["components"],s={id:"powerEditor",title:"Editor de Pot\xeancia",sidebar_label:"Editor de Pot\xeancia"},d=void 0,p={unversionedId:"powerEditor",id:"powerEditor",title:"Editor de Pot\xeancia",description:"O Editor de Elementos de Pot\xeancia no PSP-UFU permite a inser\xe7\xe3o, manipula\xe7\xe3o e edi\xe7\xe3o de elementos el\xe9tricos de pot\xeancia por meio de um diagrama unifilar.",source:"@site/docs/powerEditor.md",sourceDirName:".",slug:"/powerEditor",permalink:"/PSP/docs/powerEditor",draft:!1,editUrl:"https://github.com/Thales1330/PSP/tree/master/docusaurus/docs/powerEditor.md",tags:[],version:"current",frontMatter:{id:"powerEditor",title:"Editor de Pot\xeancia",sidebar_label:"Editor de Pot\xeancia"},sidebar:"docs",previous:{title:"CAD Tools",permalink:"/PSP/docs/cadTools"},next:{title:"Barramento",permalink:"/PSP/docs/bus"}},m={},u=[{value:"Novo projeto",id:"novo-projeto",level:2},{value:"Inserindo elementos",id:"inserindo-elementos",level:2},{value:"Teclas de atalho",id:"teclas-de-atalho",level:3},{value:"Editando dados el\xe9tricos",id:"editando-dados-el\xe9tricos",level:2}],c={toc:u},k="wrapper";function f(e){var t=e.components,a=(0,n.Z)(e,l);return(0,o.kt)(k,(0,r.Z)({},c,a,{components:t,mdxType:"MDXLayout"}),(0,o.kt)("link",{rel:"stylesheet",href:(0,i.Z)("katex/katex.min.css")}),(0,o.kt)("p",null,"O Editor de Elementos de Pot\xeancia no PSP-UFU permite a inser\xe7\xe3o, manipula\xe7\xe3o e edi\xe7\xe3o de elementos el\xe9tricos de pot\xeancia por meio de um diagrama unifilar."),(0,o.kt)("blockquote",null,(0,o.kt)("p",{parentName:"blockquote"},"(",(0,o.kt)("strong",{parentName:"p"},"Diagrama unifilar"),") Um diagrama do sistema no qual as linhas polif\xe1sicas s\xe3o representados por sua \xfanica linha equivalente. ",(0,o.kt)("a",{parentName:"p",href:"http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=601-02-04"},(0,o.kt)("em",{parentName:"a"},"tradu\xe7\xe3o livre")," - IEC 60050"),".")),(0,o.kt)("admonition",{title:"Informa\xe7\xe3o",type:"info"},(0,o.kt)("p",{parentName:"admonition"},"Todos os elementos inseridos pode ser manipulados e editados utilizando as ",(0,o.kt)("a",{parentName:"p",href:"cadTools"},"Ferramentas CAD"),".")),(0,o.kt)("h2",{id:"novo-projeto"},"Novo projeto"),(0,o.kt)("p",null,"A cria\xe7\xe3o de um novo projeto \xe9 realizada clicando no item ",(0,o.kt)("strong",{parentName:"p"},"Novo projeto")," no ",(0,o.kt)("a",{parentName:"p",href:"mainScreen#menu-ribbon"},"submenu ",(0,o.kt)("strong",{parentName:"a"},"Arquivo")),". Essa a\xe7\xe3o cria um sistema em branco na \xe1rea de trabalho, local onde \xe9 poss\xedvel inserir os elementos el\xe9tricos por meio do ",(0,o.kt)("a",{parentName:"p",href:"mainScreen#menu-ribbon"},"submenu ",(0,o.kt)("strong",{parentName:"a"},"Ferramentas"))," ou pelas ",(0,o.kt)("a",{parentName:"p",href:"powerEditor#teclas-de-atalho"},"teclas de atalho"),"."),(0,o.kt)("p",null,"Abaixo s\xe3o apresentados os elementos el\xe9tricos de pot\xeancia que podem ser inseridos no PSP-UFU:"),(0,o.kt)("ul",null,(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"bus"},"Barramento")),(0,o.kt)("li",{parentName:"ul"},"Elementos de ramo",(0,o.kt)("ul",{parentName:"li"},(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"line"},"Linha")),(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"transformer"},"Transformador")))),(0,o.kt)("li",{parentName:"ul"},"Elementos Shunt",(0,o.kt)("ul",{parentName:"li"},(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"load"},"Carga")),(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"capacitor"},"Capacitor")),(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"inductor"},"Indutor")),(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"harmSource"},"Fonte de Corrente Harm\xf4nica")))),(0,o.kt)("li",{parentName:"ul"},"M\xe1quinas",(0,o.kt)("ul",{parentName:"li"},(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"syncGenerator"},"Gerador S\xedncrono")),(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"syncMotor"},"Compensador S\xedncrono")),(0,o.kt)("li",{parentName:"ul"},(0,o.kt)("a",{parentName:"li",href:"indMotor"},"Motor de Indu\xe7\xe3o"))))),(0,o.kt)("div",null,(0,o.kt)("center",null,(0,o.kt)("img",{src:(0,i.Z)("images/powerElements.svg"),alt:"Elementos de pot\xeancia",title:"Elementos de pot\xeancia"}))),(0,o.kt)("h2",{id:"inserindo-elementos"},"Inserindo elementos"),(0,o.kt)("p",null,"Por meio do submenu ",(0,o.kt)("strong",{parentName:"p"},"Ferramentas")," pode-se inserir os elementos de pot\xeancia, como \xe9 apresentado na imagem abaixo."),(0,o.kt)("div",null,(0,o.kt)("center",null,(0,o.kt)("img",{src:(0,i.Z)("images/addPowerElements.png"),alt:"Adicionar elementos de pot\xeancia",title:"Adicionar elementos de pot\xeancia"}))),(0,o.kt)("p",null,"No caso de inser\xe7\xe3o de um barramento, o usu\xe1rio deve apenas clicar na posi\xe7\xe3o desejada, j\xe1 para os outros componentes ser\xe1 solicitado que o usu\xe1rio selecione em uma ou duas barras, o qual o novo componente ser\xe1 conectado."),(0,o.kt)("p",null,"Sempre que s\xe3o inseridos novos elementos, um formul\xe1rio de dados \xe9 exibido e pode-se editar os dados el\xe9tricos e acessar formul\xe1rios adicionais, como dados de estabilidade."),(0,o.kt)("admonition",{title:"Aten\xe7\xe3o",type:"caution"},(0,o.kt)("p",{parentName:"admonition"},"O primeiro elemento que a ser inserido no projeto ",(0,o.kt)("strong",{parentName:"p"},"deve")," ser um barramento. Os demais elementos el\xe9tricos devem ser conectados em uma ou mais barras.")),(0,o.kt)("p",null,"Al\xe9m dos elementos de pot\xeancia pode-se inserir os elementos de ",(0,o.kt)("em",{parentName:"p"},(0,o.kt)("a",{parentName:"em",href:"text"},"texto vinculado")),", os quais est\xe3o associados a uma grandeza que se deseja vizualizar diretamente na ",(0,o.kt)("a",{parentName:"p",href:"mainScreen#%C3%A1rea-de-trabalho"},"\xc1rea de Trabalho"),"."),(0,o.kt)("admonition",{title:"Cuidado!",type:"warning"},(0,o.kt)("p",{parentName:"admonition"},"Ao inserir um barramento, a mesma ir\xe1 acompanhar o ponteiro do mouse at\xe9 que voc\xea clique na posi\xe7\xe3o desejada. Para os demais elementos ",(0,o.kt)("strong",{parentName:"p"},"deve-se clicar em uma mais barras")," para exibir o elemento inserido."),(0,o.kt)("p",{parentName:"admonition"},(0,o.kt)("strong",{parentName:"p"},"Sempre fique atento \xe0s instru\xe7\xf5es da ",(0,o.kt)("a",{parentName:"strong",href:"mainScreen#barra-de-status"},"Barra de Status"),"."))),(0,o.kt)("video",{autoPlay:!0,loop:!0,muted:!0,playsInline:!0,controls:!0},(0,o.kt)("source",{src:"/PSP/videos/timelapseBuild.mp4",type:"video/mp4"})),(0,o.kt)("h3",{id:"teclas-de-atalho"},"Teclas de atalho"),(0,o.kt)("p",null,"\xc9 poss\xedvel inserir todos os elementos de pot\xeancia por meio de teclas de atalho, facilitando e agilizando a cria\xe7\xe3o dos diagramas unifilares no PSP-UFU. A tabela abaixo apresenta tais atalhos:"),(0,o.kt)("table",null,(0,o.kt)("thead",{parentName:"table"},(0,o.kt)("tr",{parentName:"thead"},(0,o.kt)("th",{parentName:"tr",align:null},"Elemento"),(0,o.kt)("th",{parentName:"tr",align:"center"},"Atalho"),(0,o.kt)("th",{parentName:"tr",align:null},"Elemento"),(0,o.kt)("th",{parentName:"tr",align:"center"},"Atalho"))),(0,o.kt)("tbody",{parentName:"table"},(0,o.kt)("tr",{parentName:"tbody"},(0,o.kt)("td",{parentName:"tr",align:null},"Barramento"),(0,o.kt)("td",{parentName:"tr",align:"center"},"B"),(0,o.kt)("td",{parentName:"tr",align:null},"Carga"),(0,o.kt)("td",{parentName:"tr",align:"center"},"Shift + L")),(0,o.kt)("tr",{parentName:"tbody"},(0,o.kt)("td",{parentName:"tr",align:null},"Linha"),(0,o.kt)("td",{parentName:"tr",align:"center"},"L"),(0,o.kt)("td",{parentName:"tr",align:null},"Capacitor"),(0,o.kt)("td",{parentName:"tr",align:"center"},"Shift + C")),(0,o.kt)("tr",{parentName:"tbody"},(0,o.kt)("td",{parentName:"tr",align:null},"Transformador"),(0,o.kt)("td",{parentName:"tr",align:"center"},"T"),(0,o.kt)("td",{parentName:"tr",align:null},"Indutor"),(0,o.kt)("td",{parentName:"tr",align:"center"},"Shift + I")),(0,o.kt)("tr",{parentName:"tbody"},(0,o.kt)("td",{parentName:"tr",align:null},"Gerador"),(0,o.kt)("td",{parentName:"tr",align:"center"},"G"),(0,o.kt)("td",{parentName:"tr",align:null},"Corrente harm\xf4nica"),(0,o.kt)("td",{parentName:"tr",align:"center"},"Shift + H")),(0,o.kt)("tr",{parentName:"tbody"},(0,o.kt)("td",{parentName:"tr",align:null},"Motor de indu\xe7\xe3o"),(0,o.kt)("td",{parentName:"tr",align:"center"},"I"),(0,o.kt)("td",{parentName:"tr",align:null},"Texto"),(0,o.kt)("td",{parentName:"tr",align:"center"},"A")),(0,o.kt)("tr",{parentName:"tbody"},(0,o.kt)("td",{parentName:"tr",align:null},"Compensador s\xedncrono"),(0,o.kt)("td",{parentName:"tr",align:"center"},"K"),(0,o.kt)("td",{parentName:"tr",align:null}),(0,o.kt)("td",{parentName:"tr",align:"center"})))),(0,o.kt)("h2",{id:"editando-dados-el\xe9tricos"},"Editando dados el\xe9tricos"),(0,o.kt)("p",null,"Ao inserir um elemento, seu respectivo formul\xe1rio de edi\xe7\xe3o \xe9 exibido para inser\xe7\xe3o dos dados el\xe9tricos d elemento. Todos os dados podem ser ",(0,o.kt)("strong",{parentName:"p"},"alterados")," ao ",(0,o.kt)("strong",{parentName:"p"},"clicar duas vezes sobre o elemento")," ou por op\xe7\xe3o do ",(0,o.kt)("strong",{parentName:"p"},"menu de contexto")," exibido ao clicar com o bot\xe3o direito sobre o elemento."),(0,o.kt)("p",null,"Cada formul\xe1rio de edi\xe7\xe3o de dados el\xe9tricos dos elementos possuem suas particularidades e s\xe3o descritos em documenta\xe7\xe3o espec\xedfica: ",(0,o.kt)("a",{parentName:"p",href:"bus"},"Barramento"),", ",(0,o.kt)("a",{parentName:"p",href:"line"},"Linha"),", ",(0,o.kt)("a",{parentName:"p",href:"transformer"},"Transformador"),", ",(0,o.kt)("a",{parentName:"p",href:"load"},"Carga"),", ",(0,o.kt)("a",{parentName:"p",href:"capacitor"},"Capacitor"),", ",(0,o.kt)("a",{parentName:"p",href:"inductor"},"Indutor"),", ",(0,o.kt)("a",{parentName:"p",href:"harmSource"},"Fonte de Corrente Harm\xf4nica"),", ",(0,o.kt)("a",{parentName:"p",href:"syncGenerator"},"Gerador S\xedncrono"),", ",(0,o.kt)("a",{parentName:"p",href:"syncMotor"},"Compensador S\xedncrono"),", ",(0,o.kt)("a",{parentName:"p",href:"indMotor"},"Motor de Indu\xe7\xe3o"),"."))}f.isMDXComponent=!0}}]);