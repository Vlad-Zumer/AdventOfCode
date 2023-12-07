fs = require('fs');


/**
 * @typedef Card
 * @type {object}
 * @property {number} id - the game number
 * @property {number[]} winningNumbers - the winning numbers on the card
 * @property {number[]} cardNumbers - the actual numbers
 * @property {number} matches - the number of numbers matching on the card
*/

/**
 * @param {string} gameString
 * @returns {Card} 
 */
function parseGame(gameString) {

    let aux = gameString.split(":");
    const gameId = Number(aux[0].split(" ").filter(el => el.trim().length > 0)[1].trim());

    aux = aux[1].split("|");
    const winningNumbers = aux[0].trim().split(" ").filter(el => el.trim().length > 0).map(el => Number(el.trim()));
    const cardNumbers = aux[1].trim().split(" ").filter(el => el.trim().length > 0).map(el => Number(el.trim()));

    return game = {
        id: gameId,
        winningNumbers: winningNumbers,
        cardNumbers: cardNumbers,
        matches: winningNumbers.filter(el => cardNumbers.includes(el)).length,
        dbg: gameString
    }
}


/**@type {string} */
// const fileContent = fs.readFileSync('test.txt', 'utf8');
const fileContent = fs.readFileSync('input.txt', 'utf8');

let gameList = fileContent.trim()
    .split("\n")
    .map(el => parseGame(el.trim()));

// for (const game of gameList) {
//     console.log(`Card ${game.id}: ${game.winningNumbers} | ${game.cardNumbers}`);
// }

let p1 = gameList.map(el => el.matches)
    .filter(el => el > 0)
    .map(el => 2 ** (el - 1))
    .reduce((acc, curr) => acc + curr);

let cards = Array(gameList.length).fill(1);

for (const game of gameList) {
    const gameIndex = game.id - 1;
    for (let index = gameIndex+1; index < gameIndex + 1 + game.matches; index++) {
        cards[index] += cards[gameIndex];
    }
}

console.log(`P1: ${p1}`);
console.log(`P2: ${cards.reduce((acc, curr)=> acc+curr)}`);