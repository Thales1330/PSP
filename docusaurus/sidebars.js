module.exports = {
  docs: {
    'Introduction': [
		'aboutPSP',
		'installation',
		'mainScreen',
		'cadTools'
	],
		
    'Power Element Editor': [
		'powerEditor',
		{
			type: 'category',
			label: 'Power Elements',
			items: ['bus', 'line', 'transformer', 'load', 'capacitor', 'inductor', 'harmSource', 'syncGenerator', 'syncMotor', 'indMotor'],
		},
	],
    'Control Element Editor': [
	'controlEditor',
		{
			type: 'category',
			label: 'Control Elements',
			items: ['io', 'transferFunction', 'sum', 'multiplier', 'divider', 'exponential', 'limiter', 'rateLimiter', 'const', 'gain', 'mathExpression'],
		},
	],
    'Simulations': [
		'powerFlow',
		'fault',
		'harmonics',
		'stability',
		'simulationConfig',
	],
	'Data Visualization' : [
		'text',
		'tabularReport',
		'graphViewer',
		'heatmap'
	],
  },
};
