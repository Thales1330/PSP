(window.webpackJsonp=window.webpackJsonp||[]).push([[16],{140:function(e,a,o){"use strict";o.d(a,"a",(function(){return p})),o.d(a,"b",(function(){return u}));var t=o(0),r=o.n(t);function n(e,a,o){return a in e?Object.defineProperty(e,a,{value:o,enumerable:!0,configurable:!0,writable:!0}):e[a]=o,e}function i(e,a){var o=Object.keys(e);if(Object.getOwnPropertySymbols){var t=Object.getOwnPropertySymbols(e);a&&(t=t.filter((function(a){return Object.getOwnPropertyDescriptor(e,a).enumerable}))),o.push.apply(o,t)}return o}function s(e){for(var a=1;a<arguments.length;a++){var o=null!=arguments[a]?arguments[a]:{};a%2?i(Object(o),!0).forEach((function(a){n(e,a,o[a])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(o)):i(Object(o)).forEach((function(a){Object.defineProperty(e,a,Object.getOwnPropertyDescriptor(o,a))}))}return e}function c(e,a){if(null==e)return{};var o,t,r=function(e,a){if(null==e)return{};var o,t,r={},n=Object.keys(e);for(t=0;t<n.length;t++)o=n[t],a.indexOf(o)>=0||(r[o]=e[o]);return r}(e,a);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);for(t=0;t<n.length;t++)o=n[t],a.indexOf(o)>=0||Object.prototype.propertyIsEnumerable.call(e,o)&&(r[o]=e[o])}return r}var d=r.a.createContext({}),m=function(e){var a=r.a.useContext(d),o=a;return e&&(o="function"==typeof e?e(a):s(s({},a),e)),o},p=function(e){var a=m(e.components);return r.a.createElement(d.Provider,{value:a},e.children)},b={inlineCode:"code",wrapper:function(e){var a=e.children;return r.a.createElement(r.a.Fragment,{},a)}},l=r.a.forwardRef((function(e,a){var o=e.components,t=e.mdxType,n=e.originalType,i=e.parentName,d=c(e,["components","mdxType","originalType","parentName"]),p=m(o),l=t,u=p["".concat(i,".").concat(l)]||p[l]||b[l]||n;return o?r.a.createElement(u,s(s({ref:a},d),{},{components:o})):r.a.createElement(u,s({ref:a},d))}));function u(e,a){var o=arguments,t=a&&a.mdxType;if("string"==typeof e||t){var n=o.length,i=new Array(n);i[0]=l;var s={};for(var c in a)hasOwnProperty.call(a,c)&&(s[c]=a[c]);s.originalType=e,s.mdxType="string"==typeof e?e:t,i[1]=s;for(var d=2;d<n;d++)i[d]=o[d];return r.a.createElement.apply(null,i)}return r.a.createElement.apply(null,o)}l.displayName="MDXCreateElement"},141:function(e,a,o){"use strict";o.d(a,"b",(function(){return n})),o.d(a,"a",(function(){return i}));var t=o(16),r=o(142);function n(){const{siteConfig:{baseUrl:e="/",url:a}={}}=Object(t.default)();return{withBaseUrl:(o,t)=>function(e,a,o,{forcePrependBaseUrl:t=!1,absolute:n=!1}={}){if(!o)return o;if(o.startsWith("#"))return o;if(Object(r.b)(o))return o;if(t)return a+o;const i=o.startsWith(a)?o:a+o.replace(/^\//,"");return n?e+i:i}(a,e,o,t)}}function i(e,a={}){const{withBaseUrl:o}=n();return o(e,a)}},142:function(e,a,o){"use strict";function t(e){return!0===/^(\w*:|\/\/)/.test(e)}function r(e){return void 0!==e&&!t(e)}o.d(a,"b",(function(){return t})),o.d(a,"a",(function(){return r}))},88:function(e,a,o){"use strict";o.r(a),o.d(a,"frontMatter",(function(){return s})),o.d(a,"metadata",(function(){return c})),o.d(a,"toc",(function(){return d})),o.d(a,"default",(function(){return p}));var t=o(3),r=o(7),n=(o(0),o(140)),i=o(141),s={id:"mainScreen",title:"Tela Principal",sidebar_label:"Tela Principal"},c={unversionedId:"mainScreen",id:"mainScreen",isDocsHomePage:!1,title:"Tela Principal",description:"Organiza\xe7\xe3o da tela principal",source:"@site/i18n/pt/docusaurus-plugin-content-docs/current/mainScreen.md",slug:"/mainScreen",permalink:"/PSP/pt/docs/mainScreen",editUrl:"https://github.com/Thales1330/PSP/tree/master/docusaurus/docs/mainScreen.md",version:"current",sidebar_label:"Tela Principal",sidebar:"docs",previous:{title:"Instala\xe7\xe3o",permalink:"/PSP/pt/docs/installation"},next:{title:"Ferramentas CAD",permalink:"/PSP/pt/docs/cadTools"}},d=[{value:"Organiza\xe7\xe3o da tela principal",id:"organiza\xe7\xe3o-da-tela-principal",children:[{value:"Menu Ribbon",id:"menu-ribbon",children:[]},{value:"\xc1rea de Trabalho",id:"\xe1rea-de-trabalho",children:[]},{value:"Barra de Status",id:"barra-de-status",children:[]}]},{value:"Configura\xe7\xf5es gerais",id:"configura\xe7\xf5es-gerais",children:[{value:"Idioma",id:"idioma",children:[]},{value:"Renderiza\xe7\xe3o",id:"renderiza\xe7\xe3o",children:[]}]},{value:"Arquivos de projeto do PSP-UFU",id:"arquivos-de-projeto-do-psp-ufu",children:[{value:"Novo projeto",id:"novo-projeto",children:[]},{value:"Salvar e Salvar como...",id:"salvar-e-salvar-como",children:[]},{value:"Abrir projeto",id:"abrir-projeto",children:[]},{value:"Importar projeto",id:"importar-projeto",children:[]}]}],m={toc:d};function p(e){var a=e.components,o=Object(r.a)(e,["components"]);return Object(n.b)("wrapper",Object(t.a)({},m,o,{components:a,mdxType:"MDXLayout"}),Object(n.b)("h2",{id:"organiza\xe7\xe3o-da-tela-principal"},"Organiza\xe7\xe3o da tela principal"),Object(n.b)("p",null,"A tela principal do PSP-UFU \xe9 dividida em tr\xeas se\xe7\xf5es: ",Object(n.b)("em",{parentName:"p"},Object(n.b)("a",{parentName:"em",href:"mainScreen#menu-ribbon"},'Menu "Ribbon"')),", ",Object(n.b)("em",{parentName:"p"},Object(n.b)("a",{parentName:"em",href:"mainScreen#%C3%A1rea-de-trabalho"},"\xc1rea de Trabalho"))," e ",Object(n.b)("em",{parentName:"p"},Object(n.b)("a",{parentName:"em",href:"mainScreen#barra-de-status"},"Barra de Status")),"."),Object(n.b)("div",null,Object(n.b)("center",null,Object(n.b)("img",{src:Object(i.a)("images/mainScreen.svg"),alt:"Tela principal",title:"Tela principal do PSP-UFU no Windows"}))),Object(n.b)("p",null,"Resumidamente, no ",Object(n.b)("strong",{parentName:"p"},"Menu Ribbon")," s\xe3o acessadas as ferramentas do PSP-UFU; na ",Object(n.b)("strong",{parentName:"p"},"\xc1rea de Trabalho")," s\xe3o criados os diagramas unifilares de pot\xeancia; e na ",Object(n.b)("strong",{parentName:"p"},"Barra de Status")," s\xe3o exibidas importantes informa\xe7\xf5es acerca do estado da \xc1rea de Trabalho."),Object(n.b)("h3",{id:"menu-ribbon"},"Menu Ribbon"),Object(n.b)("blockquote",null,Object(n.b)("p",{parentName:"blockquote"},"Em inform\xe1tica o ribbon (faixa, em ingl\xeas) \xe9 um formato de apresenta\xe7\xe3o de interface baseada na GUI onde a barra de ferramentas \xe9 mostrada atrav\xe9s de uma barra mais larga com icones maiores possibilitando o uso dos aplicativos por dispositivos ",Object(n.b)("em",{parentName:"p"},"touch screen"),".\n",Object(n.b)("a",{parentName:"p",href:"https://pt.wikipedia.org/wiki/Ribbon_(inform%C3%A1tica)"},"Wikipedia"))),Object(n.b)("p",null,"O menu Ribbon \xe9 composto de tr\xeas submenus: \u201cArquivo\u201d, \u201cFerramentas\u201d e \u201cSimula\xe7\xe3o\u201d."),Object(n.b)("div",null,Object(n.b)("center",null,Object(n.b)("img",{src:Object(i.a)("images/menuFile.png"),alt:"Menu Arquivo",title:"Menu Arquivo"}))),Object(n.b)("p",null,"Em ",Object(n.b)("strong",{parentName:"p"},"Arquivo")," est\xe3o as ferramentas gerais do programa, as quais permitem ao usu\xe1rio criar e salvar novos projetos, al\xe9m daquelas que possibilitam carregar os projetos j\xe1 existentes e importar arquivos de outros programas. Esse submenu tamb\xe9m possui op\xe7\xf5es gerais do programa e acesso a informa\xe7\xf5es sobre o programa."),Object(n.b)("div",null,Object(n.b)("center",null,Object(n.b)("img",{src:Object(i.a)("images/menuTools.png"),alt:"Menu Ferramentas",title:"Menu Ferramentas"}))),Object(n.b)("p",null,"Em ",Object(n.b)("strong",{parentName:"p"},"Ferramentas")," est\xe3o presentes os itens espec\xedficos, os quais est\xe3o relacionados \xe0 cria\xe7\xe3o e manipula\xe7\xe3o da rede el\xe9trica de pot\xeancia, relat\xf3rios tabulares de sa\xedda de dados e acesso ao formul\xe1rio de cria\xe7\xe3o de gr\xe1ficos no tempo. Algumas configura\xe7\xf5es espec\xedficas do projeto ativo s\xe3o acessadas por esse menu."),Object(n.b)("div",null,Object(n.b)("center",null,Object(n.b)("img",{src:Object(i.a)("images/menuSimulation.png"),alt:"Menu Simula\xe7\xe3o",title:"Menu Simula\xe7\xe3o"}))),Object(n.b)("p",null,"Finalmente, o submenu ",Object(n.b)("strong",{parentName:"p"},"Simula\xe7\xe3o")," proporciona o acesso do usu\xe1rio aos c\xe1lculos realizados pelo ",Object(n.b)("em",{parentName:"p"},"software")," e suas configura\xe7\xf5es, al\xe9m da ferramenta de \u201csolu\xe7\xe3o cont\xednua\u201d, a qual resolve os c\xe1lculos est\xe1ticos (fluxo de pot\xeancia, curto-circuito e harm\xf4nicos) ap\xf3s quaisquer mudan\xe7as na rede de pot\xeancia, como remo\xe7\xe3o de componente ou altera\xe7\xe3o de seus dados. Todas as configura\xe7\xf5es de simula\xe7\xe3o do projeto ativo s\xe3o acessadas por esse menu."),Object(n.b)("h3",{id:"\xe1rea-de-trabalho"},"\xc1rea de Trabalho"),Object(n.b)("p",null,"Na \xe1rea de trabalho est\xe3o presentes os projetos em execu\xe7\xe3o, os quais s\xe3o divididos por abas identificadas pelo nome do projeto (caso j\xe1 esteja gravado no disco). \xc9 nessa \xe1rea que s\xe3o inseridos e exclu\xeddos os elementos el\xe9tricos, cujas ferramentas s\xe3o acessadas por meio do ",Object(n.b)("a",{parentName:"p",href:"mainScreen#menu-ribbon"},"Menu Ribbon"),"."),Object(n.b)("p",null,"As edi\xe7\xf5es gr\xe1ficas dos elementos, como posi\xe7\xe3o, conex\xe3o e tamanho dos barramentos, s\xe3o realizadas por meio da a\xe7\xe3o ",Object(n.b)("em",{parentName:"p"},"drag-and-drop")," (arrastar e soltar) com a utiliza\xe7\xe3o do mouse. A edi\xe7\xe3o dos dados el\xe9tricos \xe9 feita por meio de formul\xe1rios de dados acessados clicando duas vezes sobre os elementos."),Object(n.b)("div",null,Object(n.b)("center",null,Object(n.b)("img",{src:Object(i.a)("images/workspace.png"),alt:"\xc1rea de Trabalho",title:"\xc1rea de Trabalho"}))),Object(n.b)("h3",{id:"barra-de-status"},"Barra de Status"),Object(n.b)("p",null,"A barra de status \xe9 respons\xe1vel por informa\xe7\xf5es interessantes sobre as circunst\xe2ncias atuais do programa: modo de opera\xe7\xe3o do mouse (editar, mover, arrastar), zoom aplicado e posi\xe7\xe3o do mouse na \xe1rea de trabalho, al\xe9m de dados sobre as a\xe7\xf5es dos usu\xe1rios, como: inser\xe7\xe3o de elementos, informa\xe7\xe3o sobre copiar e colar, etc."),Object(n.b)("div",{className:"admonition admonition-tip alert alert--success"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"12",height:"16",viewBox:"0 0 12 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M6.5 0C3.48 0 1 2.19 1 5c0 .92.55 2.25 1 3 1.34 2.25 1.78 2.78 2 4v1h5v-1c.22-1.22.66-1.75 2-4 .45-.75 1-2.08 1-3 0-2.81-2.48-5-5.5-5zm3.64 7.48c-.25.44-.47.8-.67 1.11-.86 1.41-1.25 2.06-1.45 3.23-.02.05-.02.11-.02.17H5c0-.06 0-.13-.02-.17-.2-1.17-.59-1.83-1.45-3.23-.2-.31-.42-.67-.67-1.11C2.44 6.78 2 5.65 2 5c0-2.2 2.02-4 4.5-4 1.22 0 2.36.42 3.22 1.19C10.55 2.94 11 3.94 11 5c0 .66-.44 1.78-.86 2.48zM4 14h5c-.23 1.14-1.3 2-2.5 2s-2.27-.86-2.5-2z"}))),"Dica")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},Object(n.b)("strong",{parentName:"p"},"Sempre fique atendo \xe0 barra de status"),", pois s\xe3o fornecidas informa\xe7\xf5es importantes acerca da opera\xe7\xe3o do programa. Caso tenha alguma d\xfavida siga as instru\xe7\xf5es apresentadas nesse componente."))),Object(n.b)("h2",{id:"configura\xe7\xf5es-gerais"},"Configura\xe7\xf5es gerais"),Object(n.b)("p",null,"As configura\xe7\xf5es gerais do programa s\xe3o acessadas no submenu Ribbon ",Object(n.b)("strong",{parentName:"p"},"Arquivo"),". Essas configura\xe7\xf5es s\xe3o aplicadas para todos os projetos e permanecem gravadas no disco."),Object(n.b)("div",null,Object(n.b)("center",null,Object(n.b)("img",{src:Object(i.a)("images/generalSettings.png"),alt:"Configura\xe7\xf5es gerais",title:"Configura\xe7\xf5es gerais"}))),Object(n.b)("div",{className:"admonition admonition-warning alert alert--danger"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"12",height:"16",viewBox:"0 0 12 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M5.05.31c.81 2.17.41 3.38-.52 4.31C3.55 5.67 1.98 6.45.9 7.98c-1.45 2.05-1.7 6.53 3.53 7.7-2.2-1.16-2.67-4.52-.3-6.61-.61 2.03.53 3.33 1.94 2.86 1.39-.47 2.3.53 2.27 1.67-.02.78-.31 1.44-1.13 1.81 3.42-.59 4.78-3.42 4.78-5.56 0-2.84-2.53-3.22-1.25-5.61-1.52.13-2.03 1.13-1.89 2.75.09 1.08-1.02 1.8-1.86 1.33-.67-.41-.66-1.19-.06-1.78C8.18 5.31 8.68 2.45 5.05.32L5.03.3l.02.01z"}))),"Cuidado")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},"Algumas configura\xe7\xf5es de seguran\xe7a do seu computador (principalmente em sistemas Windows) podem gerar uma mensagem de erro ao confirmar as altera\xe7\xf5es das configura\xe7\xf5es gerais."),Object(n.b)("p",{parentName:"div"},"Para resolver esse problema basta ",Object(n.b)("strong",{parentName:"p"},"executar o PSP-UFU como administrador")," (clicar com bot\xe3o direito no atalho do programa e posteriormente em executar como administrador). Altere novamente as configura\xe7\xf5es e reinicie o programa."))),Object(n.b)("h3",{id:"idioma"},"Idioma"),Object(n.b)("p",null,"Atualmente os seguintes idiomas est\xe3o dispon\xedveis no PSP-UFU:"),Object(n.b)("ul",null,Object(n.b)("li",{parentName:"ul"},"Ingl\xeas"),Object(n.b)("li",{parentName:"ul"},"Portugu\xeas")),Object(n.b)("div",{className:"admonition admonition-info alert alert--info"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"14",height:"16",viewBox:"0 0 14 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M7 2.3c3.14 0 5.7 2.56 5.7 5.7s-2.56 5.7-5.7 5.7A5.71 5.71 0 0 1 1.3 8c0-3.14 2.56-5.7 5.7-5.7zM7 1C3.14 1 0 4.14 0 8s3.14 7 7 7 7-3.14 7-7-3.14-7-7-7zm1 3H6v5h2V4zm0 6H6v2h2v-2z"}))),"Informa\xe7\xe3o")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},"O programa deve ser reiniciado para surtir efeito da altera\xe7\xe3o do idioma."))),Object(n.b)("h3",{id:"renderiza\xe7\xe3o"},"Renderiza\xe7\xe3o"),Object(n.b)("p",null,"Define como os elementos gr\xe1ficos do ",Object(n.b)("a",{parentName:"p",href:"powerEditor"},"editor de pot\xeancia")," e do ",Object(n.b)("a",{parentName:"p",href:"controlEditor"},"editor de controle")," s\xe3o desenhados na tela. Atualmente duas op\xe7\xf5es est\xe3o dispon\xedveis:"),Object(n.b)("ul",null,Object(n.b)("li",{parentName:"ul"},Object(n.b)("a",{parentName:"li",href:"https://www.opengl.org/about/"},"OpenGL")),Object(n.b)("li",{parentName:"ul"},Object(n.b)("a",{parentName:"li",href:"https://docs.wxwidgets.org/3.0/classwx_graphics_context.html"},"Device Context"))),Object(n.b)("div",{className:"admonition admonition-warning alert alert--danger"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"12",height:"16",viewBox:"0 0 12 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M5.05.31c.81 2.17.41 3.38-.52 4.31C3.55 5.67 1.98 6.45.9 7.98c-1.45 2.05-1.7 6.53 3.53 7.7-2.2-1.16-2.67-4.52-.3-6.61-.61 2.03.53 3.33 1.94 2.86 1.39-.47 2.3.53 2.27 1.67-.02.78-.31 1.44-1.13 1.81 3.42-.59 4.78-3.42 4.78-5.56 0-2.84-2.53-3.22-1.25-5.61-1.52.13-2.03 1.13-1.89 2.75.09 1.08-1.02 1.8-1.86 1.33-.67-.41-.66-1.19-.06-1.78C8.18 5.31 8.68 2.45 5.05.32L5.03.3l.02.01z"}))),"Cuidado!")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},'o OpenGL pode n\xe3o ser suportado pelo seu computador. Nesse caso, ao criar um novo projeto a tela de trabalho n\xe3o \xe9 exibida ou \xe9 exibida uma tela preta. A solu\xe7\xe3o \xe9 alterar o renderizador para "Device Context". '),Object(n.b)("p",{parentName:"div"},"Caso apare\xe7a uma mensagem de erro, execute o programa como administrador e repita o processo."))),Object(n.b)("div",{className:"admonition admonition-info alert alert--info"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"14",height:"16",viewBox:"0 0 14 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M7 2.3c3.14 0 5.7 2.56 5.7 5.7s-2.56 5.7-5.7 5.7A5.71 5.71 0 0 1 1.3 8c0-3.14 2.56-5.7 5.7-5.7zM7 1C3.14 1 0 4.14 0 8s3.14 7 7 7 7-3.14 7-7-3.14-7-7-7zm1 3H6v5h2V4zm0 6H6v2h2v-2z"}))),"Informa\xe7\xe3o")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},"O programa deve ser reiniciado para surtir efeito de altera\xe7\xe3o do renderizador."))),Object(n.b)("h2",{id:"arquivos-de-projeto-do-psp-ufu"},"Arquivos de projeto do PSP-UFU"),Object(n.b)("p",null,"As op\xe7\xf5es de cria\xe7\xe3o, grava\xe7\xe3o e abertura de projetos no disco, assim como importa\xe7\xe3o de arquivos de outros programas est\xe3o presentes no submenu Ribbon ",Object(n.b)("strong",{parentName:"p"},"Arquivo"),"."),Object(n.b)("h3",{id:"novo-projeto"},"Novo projeto"),Object(n.b)("p",null,"A cria\xe7\xe3o de um novo projeto \xe9 realizada clicando no bot\xe3o ",Object(n.b)("strong",{parentName:"p"},"Novo projeto")," no ",Object(n.b)("a",{parentName:"p",href:"mainScreen#menu-ribbon"},"submenu ",Object(n.b)("strong",{parentName:"a"},"Arquivo")),". Essa a\xe7\xe3o cria um sistema em branco na \xe1rea de trabalho, local onde \xe9 poss\xedvel inserir os elementos el\xe9tricos por meio do ",Object(n.b)("a",{parentName:"p",href:"mainScreen#menu-ribbon"},"submenu ",Object(n.b)("strong",{parentName:"a"},"Ferramentas"))," ou pelas ",Object(n.b)("a",{parentName:"p",href:"powerEditor#teclas-de-atalho"},"teclas de atalho"),"."),Object(n.b)("p",null,"Esse sistema em branco pode ser ciado utilizando o ",Object(n.b)("a",{parentName:"p",href:"powerEditor"},"Editor de Pot\xeancia"),"."),Object(n.b)("h3",{id:"salvar-e-salvar-como"},"Salvar e Salvar como..."),Object(n.b)("p",null,'A op\xe7\xe3o "Salvar" sobrep\xf5e as altera\xe7\xf5es realizadas no projeto aberto e grava no disco. A op\xe7\xe3o "Salvar como..." cria um novo arquivo e grava o projeto no disco com o aux\xedlio de uma janela de sele\xe7\xe3o de pasta (e defini\xe7\xe3o do nome do arquivo).'),Object(n.b)("div",{className:"admonition admonition-info alert alert--info"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"14",height:"16",viewBox:"0 0 14 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M7 2.3c3.14 0 5.7 2.56 5.7 5.7s-2.56 5.7-5.7 5.7A5.71 5.71 0 0 1 1.3 8c0-3.14 2.56-5.7 5.7-5.7zM7 1C3.14 1 0 4.14 0 8s3.14 7 7 7 7-3.14 7-7-3.14-7-7-7zm1 3H6v5h2V4zm0 6H6v2h2v-2z"}))),"Informa\xe7\xe3o")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},'Para projetos que est\xe3o sendo gravados pela primeira vez a op\xe7\xe3o "Salvar" se comporta de forma id\xeantica \xe0 op\xe7\xe3o "Salvar como...".'))),Object(n.b)("div",{className:"admonition admonition-tip alert alert--success"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"12",height:"16",viewBox:"0 0 12 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M6.5 0C3.48 0 1 2.19 1 5c0 .92.55 2.25 1 3 1.34 2.25 1.78 2.78 2 4v1h5v-1c.22-1.22.66-1.75 2-4 .45-.75 1-2.08 1-3 0-2.81-2.48-5-5.5-5zm3.64 7.48c-.25.44-.47.8-.67 1.11-.86 1.41-1.25 2.06-1.45 3.23-.02.05-.02.11-.02.17H5c0-.06 0-.13-.02-.17-.2-1.17-.59-1.83-1.45-3.23-.2-.31-.42-.67-.67-1.11C2.44 6.78 2 5.65 2 5c0-2.2 2.02-4 4.5-4 1.22 0 2.36.42 3.22 1.19C10.55 2.94 11 3.94 11 5c0 .66-.44 1.78-.86 2.48zM4 14h5c-.23 1.14-1.3 2-2.5 2s-2.27-.86-2.5-2z"}))),"Dica")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},"O PSP-UFU grava os arquivos com a extens\xe3o ",Object(n.b)("em",{parentName:"p"},".psp"),". Esses arquivos nada mais s\xe3o que arquivos de texto utilizando a ",Object(n.b)("a",{parentName:"p",href:"https://en.wikipedia.org/wiki/Markup_language"},"linguagem de marca\xe7\xe3o")," XML (",Object(n.b)("a",{parentName:"p",href:"https://en.wikipedia.org/wiki/XML"},"eXtensible Markup Language"),"), que define uma s\xe9rie de regras de formata\xe7\xe3o dos dados de forma que eles s\xe3o tanto leg\xedveis por humanos quanto por m\xe1quinas."),Object(n.b)("p",{parentName:"div"},"Portanto, os dados el\xe9tricos contidos neles podem ser facilmente identificados e alterados, caso necess\xe1rio."))),Object(n.b)("h3",{id:"abrir-projeto"},"Abrir projeto"),Object(n.b)("p",null,"Essa op\xe7\xe3o abre os projetos gravados no disco por meio com o aux\xedlio de uma janela de sele\xe7\xe3o de arquivos. "),Object(n.b)("h3",{id:"importar-projeto"},"Importar projeto"),Object(n.b)("p",null,"O PSP-UFU permite a importa\xe7\xe3o de arquivos dos seguintes programas:"),Object(n.b)("ul",null,Object(n.b)("li",{parentName:"ul"},Object(n.b)("a",{parentName:"li",href:"http://www.cepel.br/pt_br/produtos/programas-computacionais-por-categoria/menu/anarede-analise-de-redes-eletricas.htm"},"ANAREDE")),Object(n.b)("li",{parentName:"ul"},Object(n.b)("a",{parentName:"li",href:"https://matpower.org/"},"Matpower"))),Object(n.b)("p",null,"Para importa\xe7\xe3o dos arquivos do ",Object(n.b)("strong",{parentName:"p"},"ANAREDE")," s\xe3o utilizados tanto o arquivo de dados el\xe9tricos (",Object(n.b)("em",{parentName:"p"},".pwf"),") quanto de dados gr\xe1ficos dos elementos (",Object(n.b)("em",{parentName:"p"},".lst"),")."),Object(n.b)("p",null,"Uma vez que o arquivo do ",Object(n.b)("strong",{parentName:"p"},"Matpower")," (",Object(n.b)("em",{parentName:"p"},".m"),") n\xe3o possui dados gr\xe1ficos dos elementos el\xe9tricos, o diagrama unifilar \xe9 automaticamente gerado pelo PSP-UFU ao import\xe1-lo."),Object(n.b)("h4",{id:"gera\xe7\xe3o-autom\xe1tica-do-layout-de-diagramas-unifilares"},"Gera\xe7\xe3o autom\xe1tica do ",Object(n.b)("em",{parentName:"h4"},"layout")," de diagramas unifilares"),Object(n.b)("p",null,"Para criar o ",Object(n.b)("em",{parentName:"p"},"layout")," autom\xe1tico, utilizou-se a teoria dos grafos baseada no posicionamento direcionado \xe0 for\xe7a e aplica\xe7\xe3o de grafos com arestas ponderadas. De acordo com a teoria dos grafos, o sistema \xe9 modelado como v\xe9rtices e arestas. Nesse contexto, os barramentos s\xe3o os v\xe9rtices e os ramos do sistema (linhas e transformadores) s\xe3o as arestas. As localiza\xe7\xf5es de elementos de deriva\xe7\xe3o, assim como os n\xf3s dos elementos, s\xe3o automaticamente controladas pelo PSP-UFU."),Object(n.b)("p",null,"A ferramenta de ",Object(n.b)("em",{parentName:"p"},"layout")," autom\xe1tico \xe9 composta por um processo iterativo e os resultados s\xe3o mais refinados com um n\xfamero maior de itera\xe7\xf5es. Os melhores resultados s\xe3o obtidos utilizando o n\xfamero de itera\xe7\xf5es igual ou superior a cinco vezes o n\xfamero de barramentos do sistema."),Object(n.b)("div",{className:"admonition admonition-caution alert alert--warning"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"16",height:"16",viewBox:"0 0 16 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M8.893 1.5c-.183-.31-.52-.5-.887-.5s-.703.19-.886.5L.138 13.499a.98.98 0 0 0 0 1.001c.193.31.53.501.886.501h13.964c.367 0 .704-.19.877-.5a1.03 1.03 0 0 0 .01-1.002L8.893 1.5zm.133 11.497H6.987v-2.003h2.039v2.003zm0-3.004H6.987V5.987h2.039v4.006z"}))),"Aten\xe7\xe3o!")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},"O ",Object(n.b)("em",{parentName:"p"},"layout")," autom\xe1tico ainda est\xe1 em desenvolvimento e algumas melhorias podem ser feitas para evitar cruzamentos e sobreposi\xe7\xf5es, por\xe9m, em seu estado atual, \xe9 totalmente funcional e satisfat\xf3rio."))),Object(n.b)("div",{className:"admonition admonition-tip alert alert--success"},Object(n.b)("div",{parentName:"div",className:"admonition-heading"},Object(n.b)("h5",{parentName:"div"},Object(n.b)("span",{parentName:"h5",className:"admonition-icon"},Object(n.b)("svg",{parentName:"span",xmlns:"http://www.w3.org/2000/svg",width:"12",height:"16",viewBox:"0 0 12 16"},Object(n.b)("path",{parentName:"svg",fillRule:"evenodd",d:"M6.5 0C3.48 0 1 2.19 1 5c0 .92.55 2.25 1 3 1.34 2.25 1.78 2.78 2 4v1h5v-1c.22-1.22.66-1.75 2-4 .45-.75 1-2.08 1-3 0-2.81-2.48-5-5.5-5zm3.64 7.48c-.25.44-.47.8-.67 1.11-.86 1.41-1.25 2.06-1.45 3.23-.02.05-.02.11-.02.17H5c0-.06 0-.13-.02-.17-.2-1.17-.59-1.83-1.45-3.23-.2-.31-.42-.67-.67-1.11C2.44 6.78 2 5.65 2 5c0-2.2 2.02-4 4.5-4 1.22 0 2.36.42 3.22 1.19C10.55 2.94 11 3.94 11 5c0 .66-.44 1.78-.86 2.48zM4 14h5c-.23 1.14-1.3 2-2.5 2s-2.27-.86-2.5-2z"}))),"Dica")),Object(n.b)("div",{parentName:"div",className:"admonition-content"},Object(n.b)("p",{parentName:"div"},"Uma vez que o Matpower pode converter os formatos CDF (",Object(n.b)("em",{parentName:"p"},"Common Data Format"),") e PSS/E RAW para arquivos ",Object(n.b)("em",{parentName:"p"},".m"),", estes podem ser importados de forma indireta para o PSP-UFU."))))}p.isMDXComponent=!0}}]);