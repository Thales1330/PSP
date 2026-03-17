import React from 'react';
import Dropdown from 'react-bootstrap/Dropdown';
import clsx from 'clsx';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import Translate, {translate} from '@docusaurus/Translate';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import useBaseUrl from '@docusaurus/useBaseUrl';
import styles from './styles.module.css';

const features = [
  {
    title: <Translate>Complete Platform for Research & Industry</Translate>,
    imageUrl: 'img/applicationv2.png',
    description: (
      <>
        <Translate>A</Translate> <b><Translate>complete</Translate></b>, <b><Translate>cross-platform</Translate></b>, <b><Translate>multilingual</Translate></b> and <b><Translate>open-source environment</Translate></b> <Translate>for electrical power system simulation, supporting research, education, and real-world industrial applications.</Translate>
      </>
    ),
  },
  {
    title: <><Translate>Advanced CAD Features</Translate></>,
    imageUrl: 'img/advancedCAD.png',
    description: (
      <>
        <Translate>The software allows the construction of any</Translate> <b><Translate>electric transmission network</Translate></b> <Translate>and</Translate> <b><Translate>control systems</Translate></b> <Translate>through the deployment of visual elements</Translate>.
      </>
    ),
  },
  {
    title: <><Translate>Easy Visualization</Translate></>,
    imageUrl: 'img/easyView.png',
    description: (
      <>
        <Translate>For the visualization of results, the program offers linked text elements in the main screen, voltage heatmap display, and also table and graph editors.</Translate>
      </>
    ),
  },
  {
    title: <><Translate>Power Flow</Translate></>,
    imageUrl: 'img/powerFlow.png',
    description: (
      <>
        <Translate>
          Steady-state analysis of electrical networks using robust numerical methods.
        </Translate>
        <ul>
          <li>Newton-Raphson</li>
          <li>Gauss-Seidel</li>
          <li><Translate>Hybrid Newton-Gauss</Translate></li>
          <li><Translate>Three-phase induction motors supported</Translate></li>
        </ul>
      </>
    ),
  },
  {
    title: <><Translate>Short-Circuit & Harmonics</Translate></>,
    imageUrl: 'img/faultHarm.png',
    description: (
      <>
        <Translate>
         Fault analysis and power quality evaluation for system protection and design.
        </Translate>
        <ul>
          <li><Translate>Balanced faults</Translate></li>
          <li><Translate>Unbalanced faults</Translate></li>
          <li><Translate>Short-circuit power in all buses</Translate></li>
          <li><Translate>Harmonic voltages and THD calculation</Translate></li>
          <li><Translate>Frequency scan</Translate></li>
        </ul>
      </>
    ),
  },
  {
    title: <><Translate>Transient & Dynamic Stability</Translate></>,
    imageUrl: 'img/stability.png',
    description: (
      <>
        <Translate>
         Time-domain simulation of system dynamics under disturbances and control actions.
        </Translate>
        <ul>
          <li><Translate>Automatic selection of synchronous machine models</Translate></li>
          <li><Translate>Three-phase induction motors</Translate></li>
          <li><Translate>User-defined control systems via block diagrams</Translate></li>
          <li><Translate>Exciters, AVR, PSS, turbines and speed governors</Translate></li>
        </ul>
      </>
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

      <img
        src={useBaseUrl('/images/logo.svg')}
        alt={siteConfig.title}
        className={styles.heroLogo}
      />

      <p style={{textAlign:"center"}}>
        <iframe
          src="https://ghbtns.com/github-btn.html?user=thales1330&repo=psp&type=star&count=true&size=large"
          frameBorder="0"
          scrolling="0"
          width="130"
          height="30"
          title="GitHub">
        </iframe>
      </p>

      <div style={{marginTop:"20px"}}>

<h2
  style={{
    textAlign:"center",
    marginBottom:"15px",
    fontWeight:"700"
  }}
>
⚡ <Translate>Download</Translate> ⚡
</h2>

<div
  style={{
    display:"flex",
    justifyContent:"center",
    gap:"40px",
    flexWrap:"wrap"
  }}
>

{/* WINDOWS */}
<div
  style={{
    background:"rgba(255, 255, 255, 0.4)",
    borderRadius:"14px",
    padding:"28px",
    width:"250px",
    textAlign:"center",
    boxShadow:"0 8px 25px rgba(0,0,0,0.15)",
    transition:"transform 0.15s ease"
  }}
  onMouseEnter={(e)=>e.currentTarget.style.transform="translateY(-4px)"}
  onMouseLeave={(e)=>e.currentTarget.style.transform="translateY(0)"}
>

<img
  src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/windows8/windows8-original.svg"
  width="42"
/>

<h3 style={{marginTop:"10px"}}>Windows</h3>

<div style={{display:"flex", flexDirection:"column", gap:"12px", marginTop:"18px"}}>

<a
  className="button button--primary"
  href="https://github.com/Thales1330/PSP/releases/latest/download/PSP-UFU_x86_32_VC.exe">
  Installer (.exe)
</a>

<a
  className="button button--outline button--secondary button--sm"
  href="https://github.com/Thales1330/PSP/releases/latest/download/PSP-UFU_x86_32_VC.zip">
  Portable (.zip)
</a>

</div>

</div>


{/* LINUX */}
<div
  style={{
    background:"rgba(255, 255, 255, 0.4)",
    borderRadius:"14px",
    padding:"28px",
    width:"250px",
    textAlign:"center",
    boxShadow:"0 8px 25px rgba(0,0,0,0.15)",
    transition:"transform 0.15s ease"
  }}
  onMouseEnter={(e)=>e.currentTarget.style.transform="translateY(-4px)"}
  onMouseLeave={(e)=>e.currentTarget.style.transform="translateY(0)"}
>

<img
  src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/linux/linux-original.svg"
  width="42"
/>

<h3 style={{marginTop:"10px"}}>Linux</h3>

<div style={{display:"flex", flexDirection:"column", gap:"12px", marginTop:"18px"}}>

<a
  className="button button--primary"
  href="https://github.com/Thales1330/PSP/releases/latest/download/PSP-UFU-x86_64.deb">
  Ubuntu / Debian (.deb)
</a>

<a
  className="button button--outline button--secondary button--sm"
  href="https://github.com/Thales1330/PSP/releases/latest/download/PSP-UFU-x86_64.AppImage">
  AppImage (portable)
</a>

</div>

</div>

</div>

<div
  style={{
    display:"flex",
    justifyContent:"center",
    gap:"15px",
    marginTop:"30px",
    flexWrap:"wrap"
  }}
>

<Link
  className={clsx(
    'button button--outline button--secondary button--lg',
    styles.getStarted,
  )}
  to={useBaseUrl('docs/')}>
  <Translate>User Guide</Translate>
</Link>

<Link
  className={clsx(
    'button button--outline button--secondary button--lg',
    styles.getStarted,
  )}
  to={useBaseUrl('https://raw.githubusercontent.com/Thales1330/PSP/master/docs/downloads/samples.zip')}>
  <Translate>Samples</Translate>
</Link>

</div>

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
		<h2><Translate>Published Papers</Translate></h2>
			<Translate>Further details can be found in the published papers:</Translate>
			<blockquote cite="https://doi.org/10.1002/2050-7038.12185">
			Oliveira, T. L., Guimarães, G. C., & Rosenthal, J. C. S. (2019). PSP-UFU: An open-source, graphical, and multiplatform software for power system studies. <i>International Transactions on Electrical Energy Systems</i>, e12185. doi: <a href="https://doi.org/10.1002/2050-7038.12185">10.1002/2050-7038.12185</a>
      </blockquote>
			<blockquote cite="https://doi.org/10.1177/0020720919879058">
			Oliveira, T. L., Guimarães, G. C., Rosenthal, J. C. S., & Rezende, J. O. (2019). Power system education and research applications using free and open-source, graphical and multiplatform PSP-UFU software. <i>The International Journal of Electrical Engineering & Education</i>, 0020720919879058. doi: <a href="https://doi.org/10.1177/0020720919879058">10.1177/0020720919879058</a>
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
