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
	'doc2'
	],
    'Simulações': [
		'powerFlow'
	],
  },
};
