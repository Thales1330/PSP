import React from 'react';
import clsx from 'clsx';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import useBaseUrl from '@docusaurus/useBaseUrl';
import styles from './styles.module.css';

const features = [
  {
    title: <>Complete Platform</>,
    imageUrl: 'img/complete-platform.svg',
    description: (
      <>
        PSP-UFU is a <b>cross-platform</b>, <b>multilingual</b>, <b>Free and Open-Source Software (FOSS)</b> with <b>advanced GUI (Graphical User Interface)</b> features and <b>CAD (Computer-Aided Design) tools</b> for electrical power system studies.
      </>
    ),
  },
  {
    title: <>Advanced CAD Features</>,
    imageUrl: 'img/advanced-cad.svg',
    description: (
      <>
        The software allows the construction of any electric <b>transmission network</b> and <b>control systems</b> through the deployment of visual elements.
      </>
    ),
  },
  {
    title: <>Easy Visualization</>,
    imageUrl: 'img/visualization.svg',
    description: (
      <>
        For the visualization of results, the program offers linked text elements in the main screen, voltage heatmap display, and also table and graph editors.
      </>
    ),
  },
  {
    title: <>Application</>,
    imageUrl: 'img/application.svg',
    description: (
      <>
        The PSP-UFU aims to provide efficient computer simulation tools for <b>research</b> and <b>education</b> purposes, in addition to <b>industrial applications</b> in electrical power systems.
      </>
    ),
  },
];

const studies = [
	{
    title: <>Power Flow</>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li>Newton-Raphson</li>
		<li>Gauss-Seidel</li>
		<li>Hybrid Newton-Gauss</li>
		<li>Three-phase induction motors included</li>
      </ul>
    ),
  },
  {
    title: <>Short-Circuit calculation</>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li>Balanced</li>
		<li>Unbalanced</li>
		<li>Short-Circuit power in all system buses</li>
      </ul>
    ),
  },
  {
    title: <>Harmonics</>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li>Harmonic voltages and THD (Total Harmonic Distortion) calculation</li>
		<li>Frequency scan</li>
      </ul>
    ),
  },
  {
    title: <>Transient and Dynamic Stability</>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li>Several synchronous machine models automatically selected</li>
		<li>Three-phase induction motors</li>
		<li>User-defined machine controls, exciters and prime moves created using block diagrams (Exciters, AVR, PSS, Hydro and Thermal turbines, Speed Governor, etc.)</li>
      </ul>
    ),
  },
];

function Feature({imageUrl, title, description}) {
  const imgUrl = useBaseUrl(imageUrl);
  return (
    <div className={clsx('col col--4', styles.feature)}>
      {imgUrl && (
        <div className="text--center">
          <img className={styles.featureImage} src={imgUrl} alt={title} />
        </div>
      )}
      <h3>{title}</h3>
      <p>{description}</p>
    </div>
  );
}

function Studies({imageUrl, title, description}) {
  const imgUrl = useBaseUrl(imageUrl);
  return (
    <div className={clsx('col col--4', styles.feature)}>
      {imgUrl && (
        <div className="text--center">
          <img className={styles.featureImage} src={imgUrl} alt={title} />
        </div>
      )}
      <h3>{title}</h3>
      {description}
    </div>
  );
}

function Home() {
  const context = useDocusaurusContext();
  const {siteConfig = {}} = context;
  return (
    <Layout
      title={`${siteConfig.title}`}
      description="Power System Platform">
      <header className={clsx('hero hero--primary', styles.heroBanner)}>
        <div className="container">
          <h1 className="hero__title">{siteConfig.title}</h1>
          <p className="hero__subtitle">{siteConfig.tagline}</p>
		  
		  <p><center>
		  <iframe src="https://ghbtns.com/github-btn.html?user=thales1330&repo=psp&type=star&count=true&size=large" frameborder="0" scrolling="0" width="130" height="30" title="GitHub"></iframe>
			</center></p>
		  <div className={styles.buttons}>
			<Link
              className={clsx(
                'button button--outline button--secondary button--lg',
                styles.getStarted,
              )}
              to={useBaseUrl('https://github.com/Thales1330/PSP/releases/latest')}>
              Download
            </Link>
          </div>
		  <div className={styles.buttons}>
			<Link
              className={clsx(
                'button button--outline button--secondary button--lg',
                styles.getStarted,
              )}
              to={useBaseUrl('docs/')}>
              User Guide
            </Link>
          </div>
		  <div className={styles.buttons}>
			<Link
              className={clsx(
                'button button--outline button--secondary button--lg',
                styles.getStarted,
              )}
              to={useBaseUrl('https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/Thales1330/PSP/tree/master/Project/data/samples')}>
              Samples
            </Link>
          </div>
        </div>
      </header>
      <main>
        {features && features.length > 0 && (
          <section className={styles.features}>
            <div className="container">
              <div className="row">
                {features.map((props, idx) => (
                  <Feature key={idx} {...props} />
                ))}
              </div>
            </div>
          </section>
        )}
		
		<div className="container">
		<h2>Studies Performed</h2>
		</div>
		
		{studies && studies.length > 0 && (
          <section className={styles.features}>
            <div className="container">
              <div className="row">
                {studies.map((props, idx) => (
                  <Studies key={idx} {...props} />
                ))}
              </div>
            </div>
          </section>
        )}
		
		<div className="container">
		<h2>Published Papers</h2>
			Further details can be found in the published papers:
			<blockquote cite="https://doi.org/10.1002/2050-7038.12185">
			Oliveira, T. L., Guimarães, G. C., & Silva, L. R. C. (2019). PSP-UFU: An open-source, graphical, and multiplatform software for power system studies. <i>International Transactions on Electrical Energy Systems</i>, e12185. doi: <a href="https://doi.org/10.1002/2050-7038.12185">10.1002/2050-7038.12185</a>
			</blockquote>
			<blockquote cite="https://doi.org/10.1177/0020720919879058">
			Oliveira, T. L., Guimarães, G. C., Silva, L. R., & Rezende, J. O. (2019). Power system education and research applications using free and open-source, graphical and multiplatform PSP-UFU software. <i>The International Journal of Electrical Engineering & Education</i>, 0020720919879058. doi: <a href="https://doi.org/10.1177/0020720919879058">10.1177/0020720919879058</a>
			</blockquote>
		</div>
		
		<div className="container">
		<h2>Code Documentation</h2>
		<p>
			All detailed descriptions of the source-code can be found at <b><a href="https://thales1330.github.io/PSP/doxygen/html/index.html">Online Documentation</a></b>, generated by <a href="http://www.doxygen.org/">Doxygen</a>.
		</p>
		</div>
		
		<div className="container">
		<h2>Screenshots</h2>
			<center>
			<iframe src="gallery.html" style={{position: 'relative', width: '100%', height: '56.25vw', display: 'block', overflow: 'hidden', border: 'none'}}> 
				Your browser doesn’t support the this feature. 
			</iframe>
			</center>

		</div>
      </main>
    </Layout>
  );
}

export default Home;
