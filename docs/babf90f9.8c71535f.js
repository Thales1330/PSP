(window.webpackJsonp=window.webpackJsonp||[]).push([[40],{139:function(e,t,r){"use strict";r.r(t),r.d(t,"frontMatter",(function(){return c})),r.d(t,"metadata",(function(){return u})),r.d(t,"rightToc",(function(){return s})),r.d(t,"default",(function(){return p}));var n=r(2),a=r(6),o=(r(0),r(161)),i=r(162),c={id:"rateLimiter",title:"Limitador de taxa",sidebar_label:"Limitador de taxa"},u={id:"rateLimiter",isDocsHomePage:!1,title:"Limitador de taxa",source:"@site/docs\\rateLimiter.md",permalink:"/PSP/docs/rateLimiter",editUrl:"https://github.com/Thales1330/PSP/tree/master/docusaurus/docs/rateLimiter.md",sidebar_label:"Limitador de taxa",sidebar:"docs",previous:{title:"Limitador absoluto",permalink:"/PSP/docs/limiter"},next:{title:"Constante",permalink:"/PSP/docs/const"}},s=[],l={rightToc:s};function p(e){var t=e.components,r=Object(a.a)(e,["components"]);return Object(o.b)("wrapper",Object(n.a)({},l,r,{components:t,mdxType:"MDXLayout"}),Object(o.b)("link",{rel:"stylesheet",href:Object(i.a)("katex/katex.min.css")}))}p.isMDXComponent=!0},160:function(e,t,r){"use strict";var n=r(0),a=r(35);t.a=function(){return Object(n.useContext)(a.a)}},161:function(e,t,r){"use strict";r.d(t,"a",(function(){return p})),r.d(t,"b",(function(){return m}));var n=r(0),a=r.n(n);function o(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function i(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,n)}return r}function c(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?i(Object(r),!0).forEach((function(t){o(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):i(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function u(e,t){if(null==e)return{};var r,n,a=function(e,t){if(null==e)return{};var r,n,a={},o=Object.keys(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||(a[r]=e[r]);return a}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(a[r]=e[r])}return a}var s=a.a.createContext({}),l=function(e){var t=a.a.useContext(s),r=t;return e&&(r="function"==typeof e?e(t):c(c({},t),e)),r},p=function(e){var t=l(e.components);return a.a.createElement(s.Provider,{value:t},e.children)},f={inlineCode:"code",wrapper:function(e){var t=e.children;return a.a.createElement(a.a.Fragment,{},t)}},d=a.a.forwardRef((function(e,t){var r=e.components,n=e.mdxType,o=e.originalType,i=e.parentName,s=u(e,["components","mdxType","originalType","parentName"]),p=l(r),d=n,m=p["".concat(i,".").concat(d)]||p[d]||f[d]||o;return r?a.a.createElement(m,c(c({ref:t},s),{},{components:r})):a.a.createElement(m,c({ref:t},s))}));function m(e,t){var r=arguments,n=t&&t.mdxType;if("string"==typeof e||n){var o=r.length,i=new Array(o);i[0]=d;var c={};for(var u in t)hasOwnProperty.call(t,u)&&(c[u]=t[u]);c.originalType=e,c.mdxType="string"==typeof e?e:n,i[1]=c;for(var s=2;s<o;s++)i[s]=r[s];return a.a.createElement.apply(null,i)}return a.a.createElement.apply(null,r)}d.displayName="MDXCreateElement"},162:function(e,t,r){"use strict";r.d(t,"a",(function(){return o}));var n=r(160),a=r(163);function o(e,{forcePrependBaseUrl:t=!1,absolute:r=!1}={}){const{siteConfig:{baseUrl:o="/",url:i}={}}=Object(n.a)();if(!e)return e;if(t)return o+e;if(!Object(a.a)(e))return e;const c=o+e.replace(/^\//,"");return r?i+c:c}},163:function(e,t,r){"use strict";function n(e){return!1===/^(https?:|\/\/|mailto:|tel:)/.test(e)}r.d(t,"a",(function(){return n}))}}]);