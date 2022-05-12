const { Parser } = require('../src/Parser');
const parser = new Parser();

const program = `
/* hello
world */
"hello";
`;

const ast = parser.parse(program);

console.log(JSON.stringify(ast, null, 4));
