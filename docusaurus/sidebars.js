module.exports = {
  docs: {
    'Introdução': [
		'aboutPSP',
		'installation',
		'mainScreen',
		'cadTools'
	],
		
    'Editor de Elementos de Potência': [
		'powerEditor',
		{
			type: 'category',
			label: 'Elementos de Potência',
			items: ['bus', 'line', 'transformer', 'load', 'capacitor', 'inductor', 'harmSource', 'syncGenerator', 'syncMotor', 'indMotor'],
		},
	],
    'Editor de Elementos de Controle': [
	'controlEditor',
		{
			type: 'category',
			label: 'Elementos de Controle',
			items: ['io', 'transferFunction', 'sum', 'multiplier', 'divider', 'exponential', 'limiter', 'rateLimiter', 'const', 'gain', 'mathExpression'],
		},
	],
    'Simulações': [
		'powerFlow',
		'fault',
		'harmonics',
		'stability',
		'simulationConfig',
	],
	'Visualização dos Dados' : [
		'text',
		'tabularReport',
		'graphViewer',
		'heatmap'
	],
  },
};
