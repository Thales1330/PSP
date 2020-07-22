const remarkMath = require("remark-math");
const rehypeKatex = require("rehype-katex");

module.exports = {
  title: 'PSP-UFU',
  tagline: 'Power System Platform of Federal University of Uberlândia',
  url: 'https://thales1330.github.io',
  baseUrl: '/PSP/',
  favicon: 'img/favicon.ico',
  organizationName: 'thales1330', // Usually your GitHub org/user name.
  projectName: 'PSP', // Usually your repo name.
  themeConfig: {
    navbar: {
      title: 'PSP-UFU',
      logo: {
        alt: 'PSP-UFU logo',
        src: 'img/logo.svg',
      },
      links: [
        {
          to: 'docs/',
          activeBasePath: 'docs',
          label: 'User Guide',
          position: 'left',
        },
        {to: 'blog', label: 'Changelog', position: 'left'},
        {
          href: 'https://github.com/Thales1330/PSP',
          label: 'PSP-UFU Source Code',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Guide',
          items: [
            {
              label: 'PSP-UFU Guide',
              to: 'docs/',
            },
			{
              label: 'Code Documentation',
              to: 'https://thales1330.github.io/PSP/doxygen/html/index.html',
            },
          ],
        },
        {
          title: 'Community',
          items: [
            {
              label: 'Ask a question',
              href: 'https://github.com/Thales1330/PSP/issues',
            },
            {
              label: 'Twitter',
              href: 'https://twitter.com/PspUfu',
            },
          ],
        },
        {
          title: 'Site Image Credit',
          items: [
			{
				label: 'Images designed by upklyak / slidesgo / macrovector / Freepik',
				href: 'http://www.freepik.com/',
			},
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Thales Lima Oliveira. Built with Docusaurus.`,
    },
	algolia: {
      apiKey: 'api-key',
      indexName: 'index-name',
      appId: 'app-id', // Optional, if you run the DocSearch crawler on your own
      algoliaOptions: {}, // Optional, if provided by Algolia
    },
  },
  presets: [
    [
      '@docusaurus/preset-classic',
      {
        docs: {
          // It is recommended to set document id as docs home page (`docs/` path).
          homePageId: 'aboutPSP',
          sidebarPath: require.resolve('./sidebars.js'),
          // Please change this to your repo.
          editUrl:
            'https://github.com/Thales1330/PSP/tree/master/docusaurus',
          remarkPlugins: [remarkMath],
          rehypePlugins: [[rehypeKatex, {strict: false}]],
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          editUrl:
            'https://github.com/Thales1330/PSP/tree/master/docs',
        },
        theme: {
          customCss: require.resolve('./src/css/custom.css'),
        },
      },
    ],
  ],
};
