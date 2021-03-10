import React from 'react';
import clsx from 'clsx';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import Translate, {translate} from '@docusaurus/Translate';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import useBaseUrl from '@docusaurus/useBaseUrl';
import styles from './styles.module.css';

const features = [
  {
    title: <Translate>Complete Platform</Translate>,
    imageUrl: 'img/complete-platform.svg',
    description: (
      <>
        <Translate>PSP-UFU is a</Translate> <b><Translate>cross-platform, multilingual, Free and Open-Source Software (FOSS)</Translate></b> <Translate>with</Translate> <b><Translate>advanced GUI (Graphical User Interface) features</Translate></b> <Translate>and</Translate> <b><Translate>CAD (Computer-Aided Design) tools</Translate></b> <Translate>for electrical power system studies</Translate>.
      </>
    ),
  },
  {
    title: <><Translate>Advanced CAD Features</Translate></>,
    imageUrl: 'img/advanced-cad.svg',
    description: (
      <>
        <Translate>The software allows the construction of any</Translate> <b><Translate>electric transmission network</Translate></b> <Translate>and</Translate> <b><Translate>control systems</Translate></b> <Translate>through the deployment of visual elements</Translate>.
      </>
    ),
  },
  {
    title: <><Translate>Easy Visualization</Translate></>,
    imageUrl: 'img/visualization.svg',
    description: (
      <>
        <Translate>For the visualization of results, the program offers linked text elements in the main screen, voltage heatmap display, and also table and graph editors.</Translate>
      </>
    ),
  },
  {
    title: <><Translate>Application</Translate></>,
    imageUrl: 'img/application.svg',
    description: (
      <>
        <Translate>The PSP-UFU aims to provide efficient computer simulation tools for</Translate> <b><Translate>research and education purposes</Translate></b>, <Translate>in addition to</Translate> <b><Translate>industrial applications</Translate></b> <Translate>in electrical power systems</Translate>.
      </>
    ),
  },
];

const studies = [
	{
    title: <><Translate>Power Flow</Translate></>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li>Newton-Raphson</li>
		    <li>Gauss-Seidel</li>
		    <li><Translate>Hybrid Newton-Gauss</Translate></li>
		    <li><Translate>Three-phase induction motors included</Translate></li>
      </ul>
    ),
  },
  {
    title: <><Translate>Short-Circuit calculation</Translate></>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li><Translate>Balanced</Translate></li>
		    <li><Translate>Unbalanced</Translate></li>
		    <li><Translate>Short-Circuit power in all system buses</Translate></li>
      </ul>
    ),
  },
  {
    title: <><Translate>Harmonics</Translate></>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li><Translate>Harmonic voltages and THD (Total Harmonic Distortion) calculation</Translate></li>
		    <li><Translate>Frequency scan</Translate></li>
      </ul>
    ),
  },
  {
    title: <><Translate>Transient and Dynamic Stability</Translate></>,
    //imageUrl: 'img/undraw_docusaurus_react.svg',
    description: (
      <ul>
        <li><Translate>Several synchronous machine models automatically selected</Translate></li>
		    <li><Translate>Three-phase induction motors</Translate></li>
		    <li><Translate>User-defined machine controls, exciters and prime moves created using block diagrams (Exciters, AVR, PSS, Hydro and Thermal turbines, Speed Governor, etc.)</Translate></li>
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
      description="<Translate>Power System Platform</Translate>">
      <header className={clsx('hero hero--primary', styles.heroBanner)}>
        <div className="container">
          <h1 className="hero__title">{siteConfig.title}</h1>
          <p className="hero__subtitle"><Translate>Power System Platform of Federal University of Uberlândia</Translate></p>
		  
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
              <Translate>Download</Translate>
            </Link>
          </div>
		  <div className={styles.buttons}>
			<Link
              className={clsx(
                'button button--outline button--secondary button--lg',
                styles.getStarted,
              )}
              to={useBaseUrl('docs/')}>
              <Translate>User Guide</Translate>
            </Link>
          </div>
		  <div className={styles.buttons}>
			<Link
              className={clsx(
                'button button--outline button--secondary button--lg',
                styles.getStarted,
              )}
              to={useBaseUrl('https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/Thales1330/PSP/tree/master/Project/data/samples')}>
              <Translate>Samples</Translate>
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
		<h2><Translate>Studies Performed</Translate></h2>
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
		<h2><Translate>Published Papers</Translate></h2>
			<Translate>Further details can be found in the published papers:</Translate>
			<blockquote cite="https://doi.org/10.1002/2050-7038.12185">
			Oliveira, T. L., Guimarães, G. C., & Silva, L. R. C. (2019). PSP-UFU: An open-source, graphical, and multiplatform software for power system studies. <i>International Transactions on Electrical Energy Systems</i>, e12185. doi: <a href="https://doi.org/10.1002/2050-7038.12185">10.1002/2050-7038.12185</a>
			</blockquote>
			<blockquote cite="https://doi.org/10.1177/0020720919879058">
			Oliveira, T. L., Guimarães, G. C., Silva, L. R., & Rezende, J. O. (2019). Power system education and research applications using free and open-source, graphical and multiplatform PSP-UFU software. <i>The International Journal of Electrical Engineering & Education</i>, 0020720919879058. doi: <a href="https://doi.org/10.1177/0020720919879058">10.1177/0020720919879058</a>
			</blockquote>
		</div>
		
		<div className="container">
		<h2><Translate>Code Documentation</Translate></h2>
		<p>
			<Translate>All detailed descriptions of the source-code can be found at</Translate> <b><a href="https://thales1330.github.io/PSP/doxygen/html/index.html"><Translate>Online Documentation</Translate></a></b>, <Translate>generated by</Translate> <a href="http://www.doxygen.org/">Doxygen</a>.
		</p>
		</div>
		
		<div className="container">
		<h2><Translate>Screenshots</Translate></h2>
			<center>
			<iframe src="gallery.html" style={{position: 'relative', width: '100%', height: '56.25vw', display: 'block', overflow: 'hidden', border: 'none'}}> 
				<Translate>Your browser doesn’t support the this feature.</Translate> 
			</iframe>
			</center>

		</div>
      </main>
    </Layout>
  );
}

export default Home;
