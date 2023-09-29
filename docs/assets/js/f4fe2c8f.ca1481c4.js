"use strict";(self.webpackChunkpsp_ufu=self.webpackChunkpsp_ufu||[]).push([[5575],{3905:function(e,a,r){r.d(a,{Zo:function(){return c},kt:function(){return f}});var n=r(7294);function o(e,a,r){return a in e?Object.defineProperty(e,a,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[a]=r,e}function t(e,a){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);a&&(n=n.filter((function(a){return Object.getOwnPropertyDescriptor(e,a).enumerable}))),r.push.apply(r,n)}return r}function i(e){for(var a=1;a<arguments.length;a++){var r=null!=arguments[a]?arguments[a]:{};a%2?t(Object(r),!0).forEach((function(a){o(e,a,r[a])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):t(Object(r)).forEach((function(a){Object.defineProperty(e,a,Object.getOwnPropertyDescriptor(r,a))}))}return e}function s(e,a){if(null==e)return{};var r,n,o=function(e,a){if(null==e)return{};var r,n,o={},t=Object.keys(e);for(n=0;n<t.length;n++)r=t[n],a.indexOf(r)>=0||(o[r]=e[r]);return o}(e,a);if(Object.getOwnPropertySymbols){var t=Object.getOwnPropertySymbols(e);for(n=0;n<t.length;n++)r=t[n],a.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(o[r]=e[r])}return o}var l=n.createContext({}),u=function(e){var a=n.useContext(l),r=a;return e&&(r="function"==typeof e?e(a):i(i({},a),e)),r},c=function(e){var a=u(e.components);return n.createElement(l.Provider,{value:a},e.children)},d="mdxType",m={inlineCode:"code",wrapper:function(e){var a=e.children;return n.createElement(n.Fragment,{},a)}},p=n.forwardRef((function(e,a){var r=e.components,o=e.mdxType,t=e.originalType,l=e.parentName,c=s(e,["components","mdxType","originalType","parentName"]),d=u(r),p=o,f=d["".concat(l,".").concat(p)]||d[p]||m[p]||t;return r?n.createElement(f,i(i({ref:a},c),{},{components:r})):n.createElement(f,i({ref:a},c))}));function f(e,a){var r=arguments,o=a&&a.mdxType;if("string"==typeof e||o){var t=r.length,i=new Array(t);i[0]=p;var s={};for(var l in a)hasOwnProperty.call(a,l)&&(s[l]=a[l]);s.originalType=e,s[d]="string"==typeof e?e:o,i[1]=s;for(var u=2;u<t;u++)i[u]=r[u];return n.createElement.apply(null,i)}return n.createElement.apply(null,r)}p.displayName="MDXCreateElement"},5162:function(e,a,r){r.d(a,{Z:function(){return i}});var n=r(7294),o=r(6010),t={tabItem:"tabItem_Ymn6"};function i(e){var a=e.children,r=e.hidden,i=e.className;return n.createElement("div",{role:"tabpanel",className:(0,o.Z)(t.tabItem,i),hidden:r},a)}},4866:function(e,a,r){r.d(a,{Z:function(){return P}});var n=r(7462),o=r(7294),t=r(6010),i=r(2466),s=r(6550),l=r(1980),u=r(7392),c=r(12);function d(e){return function(e){var a,r;return null!=(a=null==(r=o.Children.map(e,(function(e){if(!e||(0,o.isValidElement)(e)&&(a=e.props)&&"object"==typeof a&&"value"in a)return e;var a;throw new Error("Docusaurus error: Bad <Tabs> child <"+("string"==typeof e.type?e.type:e.type.name)+'>: all children of the <Tabs> component should be <TabItem>, and every <TabItem> should have a unique "value" prop.')})))?void 0:r.filter(Boolean))?a:[]}(e).map((function(e){var a=e.props;return{value:a.value,label:a.label,attributes:a.attributes,default:a.default}}))}function m(e){var a=e.values,r=e.children;return(0,o.useMemo)((function(){var e=null!=a?a:d(r);return function(e){var a=(0,u.l)(e,(function(e,a){return e.value===a.value}));if(a.length>0)throw new Error('Docusaurus error: Duplicate values "'+a.map((function(e){return e.value})).join(", ")+'" found in <Tabs>. Every value needs to be unique.')}(e),e}),[a,r])}function p(e){var a=e.value;return e.tabValues.some((function(e){return e.value===a}))}function f(e){var a=e.queryString,r=void 0!==a&&a,n=e.groupId,t=(0,s.k6)(),i=function(e){var a=e.queryString,r=void 0!==a&&a,n=e.groupId;if("string"==typeof r)return r;if(!1===r)return null;if(!0===r&&!n)throw new Error('Docusaurus error: The <Tabs> component groupId prop is required if queryString=true, because this value is used as the search param name. You can also provide an explicit value such as queryString="my-search-param".');return null!=n?n:null}({queryString:r,groupId:n});return[(0,l._X)(i),(0,o.useCallback)((function(e){if(i){var a=new URLSearchParams(t.location.search);a.set(i,e),t.replace(Object.assign({},t.location,{search:a.toString()}))}}),[i,t])]}function v(e){var a,r,n,t,i=e.defaultValue,s=e.queryString,l=void 0!==s&&s,u=e.groupId,d=m(e),v=(0,o.useState)((function(){return function(e){var a,r=e.defaultValue,n=e.tabValues;if(0===n.length)throw new Error("Docusaurus error: the <Tabs> component requires at least one <TabItem> children component");if(r){if(!p({value:r,tabValues:n}))throw new Error('Docusaurus error: The <Tabs> has a defaultValue "'+r+'" but none of its children has the corresponding value. Available values are: '+n.map((function(e){return e.value})).join(", ")+". If you intend to show no default tab, use defaultValue={null} instead.");return r}var o=null!=(a=n.find((function(e){return e.default})))?a:n[0];if(!o)throw new Error("Unexpected error: 0 tabValues");return o.value}({defaultValue:i,tabValues:d})})),b=v[0],k=v[1],h=f({queryString:l,groupId:u}),g=h[0],y=h[1],P=(a=function(e){return e?"docusaurus.tab."+e:null}({groupId:u}.groupId),r=(0,c.Nk)(a),n=r[0],t=r[1],[n,(0,o.useCallback)((function(e){a&&t.set(e)}),[a,t])]),w=P[0],N=P[1],S=function(){var e=null!=g?g:w;return p({value:e,tabValues:d})?e:null}();return(0,o.useLayoutEffect)((function(){S&&k(S)}),[S]),{selectedValue:b,selectValue:(0,o.useCallback)((function(e){if(!p({value:e,tabValues:d}))throw new Error("Can't select invalid tab value="+e);k(e),y(e),N(e)}),[y,N,d]),tabValues:d}}var b=r(2389),k={tabList:"tabList__CuJ",tabItem:"tabItem_LNqP"};function h(e){var a=e.className,r=e.block,s=e.selectedValue,l=e.selectValue,u=e.tabValues,c=[],d=(0,i.o5)().blockElementScrollPositionUntilNextRender,m=function(e){var a=e.currentTarget,r=c.indexOf(a),n=u[r].value;n!==s&&(d(a),l(n))},p=function(e){var a,r=null;switch(e.key){case"Enter":m(e);break;case"ArrowRight":var n,o=c.indexOf(e.currentTarget)+1;r=null!=(n=c[o])?n:c[0];break;case"ArrowLeft":var t,i=c.indexOf(e.currentTarget)-1;r=null!=(t=c[i])?t:c[c.length-1]}null==(a=r)||a.focus()};return o.createElement("ul",{role:"tablist","aria-orientation":"horizontal",className:(0,t.Z)("tabs",{"tabs--block":r},a)},u.map((function(e){var a=e.value,r=e.label,i=e.attributes;return o.createElement("li",(0,n.Z)({role:"tab",tabIndex:s===a?0:-1,"aria-selected":s===a,key:a,ref:function(e){return c.push(e)},onKeyDown:p,onClick:m},i,{className:(0,t.Z)("tabs__item",k.tabItem,null==i?void 0:i.className,{"tabs__item--active":s===a})}),null!=r?r:a)})))}function g(e){var a=e.lazy,r=e.children,n=e.selectedValue,t=(Array.isArray(r)?r:[r]).filter(Boolean);if(a){var i=t.find((function(e){return e.props.value===n}));return i?(0,o.cloneElement)(i,{className:"margin-top--md"}):null}return o.createElement("div",{className:"margin-top--md"},t.map((function(e,a){return(0,o.cloneElement)(e,{key:a,hidden:e.props.value!==n})})))}function y(e){var a=v(e);return o.createElement("div",{className:(0,t.Z)("tabs-container",k.tabList)},o.createElement(h,(0,n.Z)({},e,a)),o.createElement(g,(0,n.Z)({},e,a)))}function P(e){var a=(0,b.Z)();return o.createElement(y,(0,n.Z)({key:String(a)},e))}},8311:function(e,a,r){r.r(a),r.d(a,{assets:function(){return p},contentTitle:function(){return d},default:function(){return k},frontMatter:function(){return c},metadata:function(){return m},toc:function(){return f}});var n=r(7462),o=r(3366),t=(r(7294),r(3905)),i=r(4996),s=r(4866),l=r(5162),u=["components"],c={id:"syncMotor",title:"Compensador S\xedncrono",sidebar_label:"Compensador S\xedncrono"},d=void 0,m={unversionedId:"syncMotor",id:"syncMotor",title:"Compensador S\xedncrono",description:"Uma m\xe1quina s\xedncrona operando sem carga mec\xe2nica e fornecendo ou absorvendo energia reativa. [tradu\xe7\xe3o livre - IEC 60050](",source:"@site/docs/syncMotor.md",sourceDirName:".",slug:"/syncMotor",permalink:"/PSP/docs/syncMotor",draft:!1,editUrl:"https://github.com/Thales1330/PSP/tree/master/docusaurus/docs/syncMotor.md",tags:[],version:"current",frontMatter:{id:"syncMotor",title:"Compensador S\xedncrono",sidebar_label:"Compensador S\xedncrono"},sidebar:"docs",previous:{title:"Gerador S\xedncrono",permalink:"/PSP/docs/syncGenerator"},next:{title:"Motor de Indu\xe7\xe3o",permalink:"/PSP/docs/indMotor"}},p={},f=[{value:"Compensador S\xedncrono no PSP-UFU",id:"compensador-s\xedncrono-no-psp-ufu",level:2},{value:"Formul\xe1rio de edi\xe7\xe3o dos compensadores s\xedncronos",id:"formul\xe1rio-de-edi\xe7\xe3o-dos-compensadores-s\xedncronos",level:2},{value:"Nome",id:"nome",level:4},{value:"Pot\xeancia nominal",id:"pot\xeancia-nominal",level:4},{value:"Pot\xeancias ativa e reativa",id:"pot\xeancias-ativa-e-reativa",level:4},{value:"Pot\xeancias reativas m\xe1xima e m\xednima",id:"pot\xeancias-reativas-m\xe1xima-e-m\xednima",level:4},{value:"Utilizar pot\xeancia nominal como base",id:"utilizar-pot\xeancia-nominal-como-base",level:4},{value:"Imped\xe2ncias de sequ\xeancia",id:"imped\xe2ncias-de-sequ\xeancia",level:4},{value:"Imped\xe2ncia de aterramento",id:"imped\xe2ncia-de-aterramento",level:4},{value:"Neutro aterrado",id:"neutro-aterrado",level:4}],v={toc:f},b="wrapper";function k(e){var a=e.components,r=(0,o.Z)(e,u);return(0,t.kt)(b,(0,n.Z)({},v,r,{components:a,mdxType:"MDXLayout"}),(0,t.kt)("link",{rel:"stylesheet",href:(0,i.Z)("katex/katex.min.css")}),(0,t.kt)("blockquote",null,(0,t.kt)("p",{parentName:"blockquote"},"Uma m\xe1quina s\xedncrona operando sem carga mec\xe2nica e fornecendo ou absorvendo energia reativa. ",(0,t.kt)("a",{parentName:"p",href:"http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=411-34-03"},(0,t.kt)("em",{parentName:"a"},"tradu\xe7\xe3o livre")," - IEC 60050"),".")),(0,t.kt)("h2",{id:"compensador-s\xedncrono-no-psp-ufu"},"Compensador S\xedncrono no PSP-UFU"),(0,t.kt)("p",null,"O compensador s\xedncrono nada mais \xe9 que um motor s\xedncrono operando sem carga em seu eixo com o intuito de fornecer ou absorver pot\xeancia reativa na rede. Em rela\xe7\xe3o ao ",(0,t.kt)("a",{parentName:"p",href:"powerFlow"},"fluxo de carga"),", no PSP-UFU esse elemento tem comportamento id\xeantico ao ",(0,t.kt)("a",{parentName:"p",href:"syncGenerator"},"gerador s\xedncrono"),", por\xe9m a sua pot\xeancia ativa \xe9 inserida nos algoritmos de solu\xe7\xe3o com sinal negativo."),(0,t.kt)("p",null,"Sua parametriza\xe7\xe3o para o estudo de ",(0,t.kt)("a",{parentName:"p",href:"powerFlow"},"fluxo de carga"),", ",(0,t.kt)("a",{parentName:"p",href:"fault"},"falta")," e ",(0,t.kt)("a",{parentName:"p",href:"harmonics"},"harm\xf4nicos")," \xe9 id\xeantica ao ",(0,t.kt)("a",{parentName:"p",href:"syncGenerator"},"gerador s\xedncrono"),"."),(0,t.kt)("admonition",{title:"Cuidado!",type:"warning"},(0,t.kt)("p",{parentName:"admonition"},"O compensador s\xedncrono ",(0,t.kt)("strong",{parentName:"p"},"n\xe3o foi implementado no estudo de ",(0,t.kt)("a",{parentName:"strong",href:"stability"},"estabilidade"))," e sua presen\xe7a no circuito pode causar erros durante os c\xe1lculos."),(0,t.kt)("p",{parentName:"admonition"},"N\xe3o inclua esse elemento para os estudos de ",(0,t.kt)("a",{parentName:"p",href:"stability"},"estabilidade"),". Vers\xf5es futuras do PSP-UFU ir\xe3o contemplar esse elemento para esse estudo.")),(0,t.kt)("admonition",{title:"Dica",type:"tip"},(0,t.kt)("p",{parentName:"admonition"},"\xc9 poss\xedvel utilizar um ",(0,t.kt)("a",{parentName:"p",href:"syncGenerator"},"gerador s\xedncrono")," com pot\xeancia ativa ",(0,t.kt)("em",{parentName:"p"},"nula")," ou ",(0,t.kt)("em",{parentName:"p"},"negativa")," para representar o compensador s\xedncrono nos estudos de ",(0,t.kt)("a",{parentName:"p",href:"stability"},"estabilidade"),".")),(0,t.kt)("h2",{id:"formul\xe1rio-de-edi\xe7\xe3o-dos-compensadores-s\xedncronos"},"Formul\xe1rio de edi\xe7\xe3o dos compensadores s\xedncronos"),(0,t.kt)("p",null,"A imagem abaixo apresenta o formul\xe1rio de inser\xe7\xe3o/altera\xe7\xe3o de dados dos compensadores s\xedncronos:"),(0,t.kt)("div",null,(0,t.kt)("center",null,(0,t.kt)("img",{src:(0,i.Z)("images/syncMotorForm.png"),alt:"Formul\xe1rio dos compensadores s\xedncronos no PSP-UFU",title:"Formul\xe1rio dos compensadores s\xedncronos no PSP-UFU"}))),(0,t.kt)(s.Z,{groupId:"syncMotor-tabs",defaultValue:"general",values:[{label:"Geral",value:"general"},{label:"Falta",value:"fault"}],mdxType:"Tabs"},(0,t.kt)(l.Z,{value:"general",mdxType:"TabItem"},(0,t.kt)("h4",{id:"nome"},"Nome"),(0,t.kt)("p",null,"Identifica\xe7\xe3o do elemento el\xe9trico. Podem ser inseridos quaisquer n\xfameros de caracteres no padr\xe3o ",(0,t.kt)("a",{parentName:"p",href:"https://pt.wikipedia.org/wiki/Unicode"},"Unicode"),"."),(0,t.kt)("p",null,"Todos os componentes de pot\xeancia do PSP-UFU possuem esse campo."),(0,t.kt)("h4",{id:"pot\xeancia-nominal"},"Pot\xeancia nominal"),(0,t.kt)("p",null,"Pot\xeancia nominal do compensador, inserida em MVA, kVA ou VA."),(0,t.kt)("p",null,'Esse campo \xe9 especialmente importante caso a op\xe7\xe3o "Utilizar a pot\xeancia nominal como base" esteja marcada.'),(0,t.kt)("h4",{id:"pot\xeancias-ativa-e-reativa"},"Pot\xeancias ativa e reativa"),(0,t.kt)("p",null,"Pot\xeancias ativa (inserida em W, kW, MW ou p.u.) e reativa (inserida em var, kvar, Mvar ou p.u.) do compensador."),(0,t.kt)("p",null,"Caso a barra conectada seja PV o valor de pot\xeancia reativa ser\xe1 ignorado e caso seja de refer\xeancia ambos os valores inseridos ser\xe3o desprezados."),(0,t.kt)("admonition",{title:"Aten\xe7\xe3o!",type:"caution"},(0,t.kt)("p",{parentName:"admonition"},"Caso mais de um compensador esteja conectado na mesma barra, os valores de pot\xeancia reativa (nas barras de refer\xeancia e PV) e ativa (nas barras de refer\xeancia) s\xe3o igualmente distribu\xeddas, respeitando os limites individuais de pot\xeancia reativa.")),(0,t.kt)("h4",{id:"pot\xeancias-reativas-m\xe1xima-e-m\xednima"},"Pot\xeancias reativas m\xe1xima e m\xednima"),(0,t.kt)("p",null,"Limites de pot\xeancia reativa m\xe1xima e m\xednima do compensador para controle de tens\xe3o em barras PV. Caso esses valores sejam ultrapassados, o reativo gerado pela unidade ser\xe1 limitado ao valor inserido e a barra conectada ser\xe1 transformada em PQ, n\xe3o controlando a tens\xe3o estabelecida."),(0,t.kt)("h4",{id:"utilizar-pot\xeancia-nominal-como-base"},"Utilizar pot\xeancia nominal como base"),(0,t.kt)("p",null,"Caso essa op\xe7\xe3o seja marcada, o programa ir\xe1 utilizar a pot\xeancia nominal do compensador como base para a convers\xe3o das unidades, inclusive aqueles no formul\xe1rio de estabilidade, caso contr\xe1rio ser\xe1 usada a ",(0,t.kt)("a",{parentName:"p",href:"simulationConfig"},"pot\xeancia base do sistema"),".")),(0,t.kt)(l.Z,{value:"fault",mdxType:"TabItem"},(0,t.kt)("h4",{id:"imped\xe2ncias-de-sequ\xeancia"},"Imped\xe2ncias de sequ\xeancia"),(0,t.kt)("p",null,"Valores de resist\xeancia e reat\xe2ncia para c\xe1lculo das correntes de falta. S\xe3o inseridos dados de sequ\xeancia positiva, negativa e zero."),(0,t.kt)("h4",{id:"imped\xe2ncia-de-aterramento"},"Imped\xe2ncia de aterramento"),(0,t.kt)("p",null,"Valores utilizados para o c\xe1lculo das correntes de falta do tipo fase-terra e fase-fase-terra. Caso o neutro do gerador n\xe3o seja aterrado, o valor inserido nesse campo \xe9 ignorado."),(0,t.kt)("h4",{id:"neutro-aterrado"},"Neutro aterrado"),(0,t.kt)("p",null,"Indica se o neutro do gerador \xe9 aterrado."))))}k.isMDXComponent=!0}}]);