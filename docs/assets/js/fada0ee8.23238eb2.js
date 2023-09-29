"use strict";(self.webpackChunkpsp_ufu=self.webpackChunkpsp_ufu||[]).push([[8625],{3905:function(a,e,t){t.d(e,{Zo:function(){return i},kt:function(){return N}});var n=t(7294);function r(a,e,t){return e in a?Object.defineProperty(a,e,{value:t,enumerable:!0,configurable:!0,writable:!0}):a[e]=t,a}function m(a,e){var t=Object.keys(a);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(a);e&&(n=n.filter((function(e){return Object.getOwnPropertyDescriptor(a,e).enumerable}))),t.push.apply(t,n)}return t}function s(a){for(var e=1;e<arguments.length;e++){var t=null!=arguments[e]?arguments[e]:{};e%2?m(Object(t),!0).forEach((function(e){r(a,e,t[e])})):Object.getOwnPropertyDescriptors?Object.defineProperties(a,Object.getOwnPropertyDescriptors(t)):m(Object(t)).forEach((function(e){Object.defineProperty(a,e,Object.getOwnPropertyDescriptor(t,e))}))}return a}function o(a,e){if(null==a)return{};var t,n,r=function(a,e){if(null==a)return{};var t,n,r={},m=Object.keys(a);for(n=0;n<m.length;n++)t=m[n],e.indexOf(t)>=0||(r[t]=a[t]);return r}(a,e);if(Object.getOwnPropertySymbols){var m=Object.getOwnPropertySymbols(a);for(n=0;n<m.length;n++)t=m[n],e.indexOf(t)>=0||Object.prototype.propertyIsEnumerable.call(a,t)&&(r[t]=a[t])}return r}var p=n.createContext({}),l=function(a){var e=n.useContext(p),t=e;return a&&(t="function"==typeof a?a(e):s(s({},e),a)),t},i=function(a){var e=l(a.components);return n.createElement(p.Provider,{value:e},a.children)},c="mdxType",u={inlineCode:"code",wrapper:function(a){var e=a.children;return n.createElement(n.Fragment,{},e)}},d=n.forwardRef((function(a,e){var t=a.components,r=a.mdxType,m=a.originalType,p=a.parentName,i=o(a,["components","mdxType","originalType","parentName"]),c=l(t),d=r,N=c["".concat(p,".").concat(d)]||c[d]||u[d]||m;return t?n.createElement(N,s(s({ref:e},i),{},{components:t})):n.createElement(N,s({ref:e},i))}));function N(a,e){var t=arguments,r=e&&e.mdxType;if("string"==typeof a||r){var m=t.length,s=new Array(m);s[0]=d;var o={};for(var p in e)hasOwnProperty.call(e,p)&&(o[p]=e[p]);o.originalType=a,o[c]="string"==typeof a?a:r,s[1]=o;for(var l=2;l<m;l++)s[l]=t[l];return n.createElement.apply(null,s)}return n.createElement.apply(null,t)}d.displayName="MDXCreateElement"},5162:function(a,e,t){t.d(e,{Z:function(){return s}});var n=t(7294),r=t(6010),m={tabItem:"tabItem_Ymn6"};function s(a){var e=a.children,t=a.hidden,s=a.className;return n.createElement("div",{role:"tabpanel",className:(0,r.Z)(m.tabItem,s),hidden:t},e)}},4866:function(a,e,t){t.d(e,{Z:function(){return x}});var n=t(7462),r=t(7294),m=t(6010),s=t(2466),o=t(6550),p=t(1980),l=t(7392),i=t(12);function c(a){return function(a){var e,t;return null!=(e=null==(t=r.Children.map(a,(function(a){if(!a||(0,r.isValidElement)(a)&&(e=a.props)&&"object"==typeof e&&"value"in e)return a;var e;throw new Error("Docusaurus error: Bad <Tabs> child <"+("string"==typeof a.type?a.type:a.type.name)+'>: all children of the <Tabs> component should be <TabItem>, and every <TabItem> should have a unique "value" prop.')})))?void 0:t.filter(Boolean))?e:[]}(a).map((function(a){var e=a.props;return{value:e.value,label:e.label,attributes:e.attributes,default:e.default}}))}function u(a){var e=a.values,t=a.children;return(0,r.useMemo)((function(){var a=null!=e?e:c(t);return function(a){var e=(0,l.l)(a,(function(a,e){return a.value===e.value}));if(e.length>0)throw new Error('Docusaurus error: Duplicate values "'+e.map((function(a){return a.value})).join(", ")+'" found in <Tabs>. Every value needs to be unique.')}(a),a}),[e,t])}function d(a){var e=a.value;return a.tabValues.some((function(a){return a.value===e}))}function N(a){var e=a.queryString,t=void 0!==e&&e,n=a.groupId,m=(0,o.k6)(),s=function(a){var e=a.queryString,t=void 0!==e&&e,n=a.groupId;if("string"==typeof t)return t;if(!1===t)return null;if(!0===t&&!n)throw new Error('Docusaurus error: The <Tabs> component groupId prop is required if queryString=true, because this value is used as the search param name. You can also provide an explicit value such as queryString="my-search-param".');return null!=n?n:null}({queryString:t,groupId:n});return[(0,p._X)(s),(0,r.useCallback)((function(a){if(s){var e=new URLSearchParams(m.location.search);e.set(s,a),m.replace(Object.assign({},m.location,{search:e.toString()}))}}),[s,m])]}function k(a){var e,t,n,m,s=a.defaultValue,o=a.queryString,p=void 0!==o&&o,l=a.groupId,c=u(a),k=(0,r.useState)((function(){return function(a){var e,t=a.defaultValue,n=a.tabValues;if(0===n.length)throw new Error("Docusaurus error: the <Tabs> component requires at least one <TabItem> children component");if(t){if(!d({value:t,tabValues:n}))throw new Error('Docusaurus error: The <Tabs> has a defaultValue "'+t+'" but none of its children has the corresponding value. Available values are: '+n.map((function(a){return a.value})).join(", ")+". If you intend to show no default tab, use defaultValue={null} instead.");return t}var r=null!=(e=n.find((function(a){return a.default})))?e:n[0];if(!r)throw new Error("Unexpected error: 0 tabValues");return r.value}({defaultValue:s,tabValues:c})})),h=k[0],v=k[1],f=N({queryString:p,groupId:l}),w=f[0],b=f[1],x=(e=function(a){return a?"docusaurus.tab."+a:null}({groupId:l}.groupId),t=(0,i.Nk)(e),n=t[0],m=t[1],[n,(0,r.useCallback)((function(a){e&&m.set(a)}),[e,m])]),g=x[0],y=x[1],M=function(){var a=null!=w?w:g;return d({value:a,tabValues:c})?a:null}();return(0,r.useLayoutEffect)((function(){M&&v(M)}),[M]),{selectedValue:h,selectValue:(0,r.useCallback)((function(a){if(!d({value:a,tabValues:c}))throw new Error("Can't select invalid tab value="+a);v(a),b(a),y(a)}),[b,y,c]),tabValues:c}}var h=t(2389),v={tabList:"tabList__CuJ",tabItem:"tabItem_LNqP"};function f(a){var e=a.className,t=a.block,o=a.selectedValue,p=a.selectValue,l=a.tabValues,i=[],c=(0,s.o5)().blockElementScrollPositionUntilNextRender,u=function(a){var e=a.currentTarget,t=i.indexOf(e),n=l[t].value;n!==o&&(c(e),p(n))},d=function(a){var e,t=null;switch(a.key){case"Enter":u(a);break;case"ArrowRight":var n,r=i.indexOf(a.currentTarget)+1;t=null!=(n=i[r])?n:i[0];break;case"ArrowLeft":var m,s=i.indexOf(a.currentTarget)-1;t=null!=(m=i[s])?m:i[i.length-1]}null==(e=t)||e.focus()};return r.createElement("ul",{role:"tablist","aria-orientation":"horizontal",className:(0,m.Z)("tabs",{"tabs--block":t},e)},l.map((function(a){var e=a.value,t=a.label,s=a.attributes;return r.createElement("li",(0,n.Z)({role:"tab",tabIndex:o===e?0:-1,"aria-selected":o===e,key:e,ref:function(a){return i.push(a)},onKeyDown:d,onClick:u},s,{className:(0,m.Z)("tabs__item",v.tabItem,null==s?void 0:s.className,{"tabs__item--active":o===e})}),null!=t?t:e)})))}function w(a){var e=a.lazy,t=a.children,n=a.selectedValue,m=(Array.isArray(t)?t:[t]).filter(Boolean);if(e){var s=m.find((function(a){return a.props.value===n}));return s?(0,r.cloneElement)(s,{className:"margin-top--md"}):null}return r.createElement("div",{className:"margin-top--md"},m.map((function(a,e){return(0,r.cloneElement)(a,{key:e,hidden:a.props.value!==n})})))}function b(a){var e=k(a);return r.createElement("div",{className:(0,m.Z)("tabs-container",v.tabList)},r.createElement(f,(0,n.Z)({},a,e)),r.createElement(w,(0,n.Z)({},a,e)))}function x(a){var e=(0,h.Z)();return r.createElement(b,(0,n.Z)({key:String(e)},a))}},8690:function(a,e,t){t.r(e),t.d(e,{assets:function(){return c},contentTitle:function(){return l},default:function(){return k},frontMatter:function(){return p},metadata:function(){return i},toc:function(){return u}});var n=t(7462),r=t(3366),m=(t(7294),t(3905)),s=t(4996),o=(t(4866),t(5162),["components"]),p={id:"text",title:"Texto Vinculado",sidebar_label:"Texto Vinculado"},l=void 0,i={unversionedId:"text",id:"text",title:"Texto Vinculado",description:"O elemento de texto vinculado (ao elemento de pot\xeancia) pode ser inserido no editor de elementos de pot\xeancia. Esse elemento de texto pode ser vinculado a in\xfameros resultados e par\xe2metros do sistema de pot\xeancia.",source:"@site/docs/text.md",sourceDirName:".",slug:"/text",permalink:"/PSP/docs/text",draft:!1,editUrl:"https://github.com/Thales1330/PSP/tree/master/docusaurus/docs/text.md",tags:[],version:"current",frontMatter:{id:"text",title:"Texto Vinculado",sidebar_label:"Texto Vinculado"},sidebar:"docs",previous:{title:"Configura\xe7\xf5es da Simula\xe7\xe3o",permalink:"/PSP/docs/simulationConfig"},next:{title:"Relat\xf3rios Tabulares",permalink:"/PSP/docs/tabularReport"}},c={},u=[{value:"Inserindo elementos de texto no PSP-UFU",id:"inserindo-elementos-de-texto-no-psp-ufu",level:2},{value:"Formul\xe1rio de edi\xe7\xe3o de dados do elemento de texto",id:"formul\xe1rio-de-edi\xe7\xe3o-de-dados-do-elemento-de-texto",level:2}],d={toc:u},N="wrapper";function k(a){var e=a.components,t=(0,r.Z)(a,o);return(0,m.kt)(N,(0,n.Z)({},d,t,{components:e,mdxType:"MDXLayout"}),(0,m.kt)("link",{rel:"stylesheet",href:(0,s.Z)("katex/katex.min.css")}),(0,m.kt)("p",null,"O elemento de texto vinculado (ao elemento de pot\xeancia) pode ser inserido no editor de ",(0,m.kt)("a",{parentName:"p",href:"powerEditor"},"elementos de pot\xeancia"),". Esse elemento de texto pode ser vinculado a in\xfameros resultados e par\xe2metros do sistema de pot\xeancia."),(0,m.kt)("p",null,"Esse elemento \xe9 \xfatil para identifica\xe7\xe3o e an\xe1lise dos resultados de elementos de pot\xeancia."),(0,m.kt)("admonition",{title:"Dica",type:"tip"},(0,m.kt)("p",{parentName:"admonition"},"Outra aplica\xe7\xe3o importante \xe9 a atualiza\xe7\xe3o de seu valor ao aplicar a \u201csolu\xe7\xe3o cont\xednua\u201d no projeto, auxiliando na visualiza\xe7\xe3o das mudan\xe7as de estado da rede el\xe9trica."),(0,m.kt)("p",{parentName:"admonition"},"Tamb\xe9m pode-se utilizar os resultados apresentados por esses elementos juntamente com o diagrama unifilar na cria\xe7\xe3o de relat\xf3rios do projeto.")),(0,m.kt)("div",null,(0,m.kt)("center",null,(0,m.kt)("img",{src:(0,s.Z)("images/textExp.png"),alt:"Exemplo do elemento de texto no PSP-UFU",title:"Exemplo de elemento de texto no PSP-UFU"}))),(0,m.kt)("h2",{id:"inserindo-elementos-de-texto-no-psp-ufu"},"Inserindo elementos de texto no PSP-UFU"),(0,m.kt)("p",null,"Por meio do submenu ",(0,m.kt)("strong",{parentName:"p"},"Ferramentas")," pode-se inserir os elementos de texto, como \xe9 apresentado na imagem abaixo."),(0,m.kt)("div",null,(0,m.kt)("center",null,(0,m.kt)("img",{src:(0,s.Z)("images/addTextElement.svg"),alt:"Adicionar elementos de texto",title:"Adicionar elementos de texto"}))),(0,m.kt)("p",null,'Ao selecionar o bot\xe3o "Texto" ou utilizar a ',(0,m.kt)("strong",{parentName:"p"},'tecla de atalho "A"'),", o usu\xe1rio deve clicar na posi\xe7\xe3o desejada da ",(0,m.kt)("a",{parentName:"p",href:"powerEditor"},"\xe1rea de trabalho"),'. Com isso, ser\xe1 adicionado um elemento tempor\xe1rio com o valor "Texto". Para parametrizar esse elemento clique duas vezes sobre ele.'),(0,m.kt)("admonition",{title:"Aten\xe7\xe3o",type:"caution"},(0,m.kt)("p",{parentName:"admonition"},"O elemento de texto, assim como os ",(0,m.kt)("a",{parentName:"p",href:"controlEditor"},"elementos de controle")," n\xe3o exibem automaticamente o formul\xe1rio de edi\xe7\xe3o de dados ao inseri-lo pela primeira vez.")),(0,m.kt)("h2",{id:"formul\xe1rio-de-edi\xe7\xe3o-de-dados-do-elemento-de-texto"},"Formul\xe1rio de edi\xe7\xe3o de dados do elemento de texto"),(0,m.kt)("p",null,"A figura abaixo apresenta o formul\xe1rio de edi\xe7\xe3o de dados desse componente gr\xe1fico."),(0,m.kt)("div",null,(0,m.kt)("center",null,(0,m.kt)("img",{src:(0,s.Z)("images/textForm.png"),alt:"Formul\xe1rio do elemento de texto no PSP-UFU",title:"Formul\xe1rio do elemento de texto no PSP-UFU"}))),(0,m.kt)("p",null,"Sua parametriza\xe7\xe3o \xe9 realizada por campos que associam o componente aos elementos el\xe9tricos de pot\xeancia, al\xe9m do formato dos resultados num\xe9ricos. Os campos s\xe3o liberados a medida que os dados anteriores s\xe3o inseridos e uma mensagem de erro \xe9 apresentada ao usu\xe1rio caso algum par\xe2metro necess\xe1rio n\xe3o seja preenchido."),(0,m.kt)("p",null,"O processo de associa\xe7\xe3o se inicia na escolha do tipo de elemento de pot\xeancia ser\xe1 vinculado ao texto. Em seguida \xe9 exibida uma lista com o nome de todos os componentes previamente inseridos no editor de elementos de pot\xeancia do tipo definido no campo anterior, associando, assim, o texto ao elemento."),(0,m.kt)("p",null,"Em \u201cTipo de texto\u201d define-se qual ser\xe1 a sa\xedda apresentada. Tal campo depende do tipo de elemento selecionado, como mostra a tabela abaixo. Alguns outros dados s\xe3o necess\xe1rios para os elementos de ramo, como barra de origem e destino, com o intuito de representar o fluxo correto de pot\xeancia e corrente. Finalmente o usu\xe1rio deve inserir a quantidade de casas decimais para dados num\xe9ricos. A pr\xe9-visualiza\xe7\xe3o pode ser observada no formul\xe1rio."),(0,m.kt)("table",null,(0,m.kt)("thead",{parentName:"table"},(0,m.kt)("tr",{parentName:"thead"},(0,m.kt)("th",{parentName:"tr",align:"center"},"Elemento de pot\xeancia"),(0,m.kt)("th",{parentName:"tr",align:null},"Tipo de texto"),(0,m.kt)("th",{parentName:"tr",align:null},"Unidades dispon\xedveis"))),(0,m.kt)("tbody",{parentName:"table"},(0,m.kt)("tr",{parentName:"tbody"},(0,m.kt)("td",{parentName:"tr",align:"center"},(0,m.kt)("a",{parentName:"td",href:"bus"},"Barra")),(0,m.kt)("td",{parentName:"tr",align:null},"Nome",(0,m.kt)("br",null),"Tens\xe3o",(0,m.kt)("br",null),"\xc2ngulo",(0,m.kt)("br",null),"Corrente de falta",(0,m.kt)("br",null),"Tens\xe3o de falta",(0,m.kt)("br",null),"N\xedvel de curto-circuito",(0,m.kt)("br",null),"THD de tens\xe3o"),(0,m.kt)("td",{parentName:"tr",align:null},"-",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", V, kV",(0,m.kt)("br",null),"Graus, Radianos",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", A, kA",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", V, kV",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", VA, kVA, MVA",(0,m.kt)("br",null),"%")),(0,m.kt)("tr",{parentName:"tbody"},(0,m.kt)("td",{parentName:"tr",align:"center"},(0,m.kt)("a",{parentName:"td",href:"syncGenerator"},"Gerador")),(0,m.kt)("td",{parentName:"tr",align:null},"Nome",(0,m.kt)("br",null),"Pot\xeancia ativa",(0,m.kt)("br",null),"Pot\xeancia reativa",(0,m.kt)("br",null),"Corrente de falta"),(0,m.kt)("td",{parentName:"tr",align:null},"-",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", W, kW, MW",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", var, kvar, Mvar",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", A, kA")),(0,m.kt)("tr",{parentName:"tbody"},(0,m.kt)("td",{parentName:"tr",align:"center"},(0,m.kt)("a",{parentName:"td",href:"line"},"Linha")," e ",(0,m.kt)("a",{parentName:"td",href:"transformer"},"transformador")),(0,m.kt)("td",{parentName:"tr",align:null},"Nome",(0,m.kt)("br",null),"Fluxo de pot\xeancia ativo",(0,m.kt)("br",null),"Fluxo de pot\xeancia reativo",(0,m.kt)("br",null),"Perdas",(0,m.kt)("br",null),"Corrente",(0,m.kt)("br",null),"Corrente de falta"),(0,m.kt)("td",{parentName:"tr",align:null},"-",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", W, kW, MW",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", var, kvar, Mvar",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", W, kW, MW",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", A, kA",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", A, kA")),(0,m.kt)("tr",{parentName:"tbody"},(0,m.kt)("td",{parentName:"tr",align:"center"},(0,m.kt)("a",{parentName:"td",href:"load"},"Carga"),", ",(0,m.kt)("a",{parentName:"td",href:"indMotor"},"Motor de indu\xe7\xe3o"),(0,m.kt)("br",null),"e ",(0,m.kt)("a",{parentName:"td",href:"syncMotor"},"Compensador s\xedncrono")),(0,m.kt)("td",{parentName:"tr",align:null},"Nome",(0,m.kt)("br",null),"Pot\xeancia ativa",(0,m.kt)("br",null),"Pot\xeancia reativa"),(0,m.kt)("td",{parentName:"tr",align:null},"-",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", W, kW, MW",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", var, kvar, Mvar")),(0,m.kt)("tr",{parentName:"tbody"},(0,m.kt)("td",{parentName:"tr",align:"center"},(0,m.kt)("a",{parentName:"td",href:"capacitor"},"Capacitor")," e ",(0,m.kt)("a",{parentName:"td",href:"inductor"},"Indutor")),(0,m.kt)("td",{parentName:"tr",align:null},"Nome",(0,m.kt)("br",null),"Pot\xeancia reativa"),(0,m.kt)("td",{parentName:"tr",align:null},"-",(0,m.kt)("br",null),(0,m.kt)("span",{parentName:"td",className:"math math-inline"},(0,m.kt)("span",{parentName:"span",className:"katex"},(0,m.kt)("span",{parentName:"span",className:"katex-mathml"},(0,m.kt)("math",{parentName:"span",xmlns:"http://www.w3.org/1998/Math/MathML"},(0,m.kt)("semantics",{parentName:"math"},(0,m.kt)("mrow",{parentName:"semantics"},(0,m.kt)("mi",{parentName:"mrow"},"p"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},"."),(0,m.kt)("mi",{parentName:"mrow"},"u"),(0,m.kt)("mi",{parentName:"mrow",mathvariant:"normal"},".")),(0,m.kt)("annotation",{parentName:"semantics",encoding:"application/x-tex"},"p.u.")))),(0,m.kt)("span",{parentName:"span",className:"katex-html","aria-hidden":"true"},(0,m.kt)("span",{parentName:"span",className:"base"},(0,m.kt)("span",{parentName:"span",className:"strut",style:{height:"0.625em",verticalAlign:"-0.1944em"}}),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"p"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."),(0,m.kt)("span",{parentName:"span",className:"mord mathnormal"},"u"),(0,m.kt)("span",{parentName:"span",className:"mord"},"."))))),", var, kvar, Mvar")))),(0,m.kt)("admonition",{title:"Informa\xe7\xe3o",type:"info"},(0,m.kt)("p",{parentName:"admonition"},"Uma vez inseridos, esses elementos de texto podem ser manipulados da ",(0,m.kt)("a",{parentName:"p",href:"cadTools"},"mesma forma que os elementos de pot\xeancia"),".")))}k.isMDXComponent=!0}}]);