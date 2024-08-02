WordCraft - User Guide and Technical Documentation

1. Game Overview:
WordCraft is an engaging word puzzle game where players rearrange words to form a target sentence. The game is played on a grid-based board with obstacles (walls) and a time limit. Players strategically move words around the board to recreate the target sentence in the correct order, either horizontally or vertically.

2. Code Structure:
The game is implemented using HTML, CSS (via Tailwind CSS), and JavaScript. The main components are:

a. wordcraft.html: Contains the game's HTML structure, layout, embedded CSS, and JavaScript code.
b. External libraries: 
   - Tailwind CSS (via CDN) for styling
   - canvas-confetti for celebratory animations
c. Internal CSS: Custom styles and animations defined within the <style> tag in wordcraft.html.
d. JavaScript: Inline scripts within wordcraft.html for game logic, event handling, and UI updates.

3. Game State:
The game state is managed through a `gameState` object, which includes:
- Current level (integer)
- Timer (integer, seconds elapsed)
- Target sentence (string)
- Words and their positions (array of objects)
- Grid size (object with rows and cols)
- Walls (array of objects with row and col properties)
- Moves (integer)
- Selection rectangle position (object with row and col)
- Seeded random number generator (RNG) (function)
- Selected word index (integer or null)
- Animation state (boolean)
- Completed levels (array of integers)
- Demo mode flag (boolean)

4. Key Functions:

a. initGame(isDemo): Initializes the game, generates a level, and starts the timer. Supports demo mode.
b. generateLevel(level): Creates a new level with random word positions and walls using a seeded RNG.
c. createGameBoard(): Renders the game board based on the current game state.
d. updateGameBoard(): Updates the visual representation of the game board.
e. moveSelectedWord(direction): Handles the logic for moving words on the board.
f. moveSelectedWordImmediate(direction): Moves the selected word immediately without animation.
g. checkLevelCompletion(): Verifies if the current word arrangement matches the target sentence.
h. completeLevel(): Handles level completion, scoring, and progression.
i. showLevelCompletionAnimation(): Displays an animation when a level is completed.
j. showGameCompletionAnimation(): Displays a celebratory animation when all levels are completed.
k. isPositionIsolated(position): Checks if a given position is isolated by walls or board edges.
l. animateWordMovement(wordIndex, startPos, endPos): Animates the movement of a word on the board.
m. startTimer(): Starts the game timer and updates the display.
n. stopTimer(): Stops the game timer.
o. toggleWordSelection(): Selects or deselects a word based on the selection rectangle position.
p. moveSelectionRectangle(direction): Moves the selection rectangle in the specified direction.
q. handleKeyDown(event): Handles keyboard input for game actions.
r. handleTouchStart(event), handleTouchMove(event), handleTouchEnd(event): Handle touch input for mobile devices.
s. updateGameInfo(): Updates the game information display (level, timer, moves, best moves).
t. updateTimerDisplay(): Updates the timer display.
u. updateMovesDisplay(): Updates the moves display.
v. detectGameMode(): Detects whether the game is being played on a desktop or mobile device.
w. startDemoTutorial(): Starts the demo tutorial for new players.
x. updateUIForGameMode(): Updates the UI based on the detected game mode.
y. handleCellClick(event): Handles click events on the game board cells.
z. addEventListeners(), removeEventListeners(): Add or remove event listeners for game actions.
aa. handleRestartLevel(event): Handles restarting the current level.
ab. handleSkipLevel(event): Handles skipping to the next level.
ac. highlightCorrectWords(sentence): Highlights correctly placed words in the target sentence.
ad. findCorrectWordPositions(): Finds the positions of correctly placed words.

5. Game Flow:
1. The game starts with an intro screen displaying game instructions.
2. Players can choose to start the game or play a tutorial demo.
3. Upon starting the game or demo, it initializes with initGame().
4. A level is generated with random word positions and walls using a seeded RNG.
5. Players use arrow keys, WASD, or touch gestures to move the selection rectangle and Enter/Space to select/deselect words.
6. Selected words can be moved using arrow keys, WASD, or swipe gestures.
7. The game checks for level completion after each move.
8. Upon level completion, a level completion animation is shown with move count and best moves.
9. Players can choose to try the level again or proceed to the next level.
10. After completing all levels, a game completion animation is shown.
11. Players can restart the game after completion.

6. User Interaction:
- Keyboard input: Handled through event listeners for arrow keys, WASD, Enter, Space, 'R' for restarting, and 'N' for next level.
- Mouse input: Click to select/deselect words on desktop.
- Touch input: Supported for mobile devices, allowing swipe gestures to move words and taps to select/deselect.
- Responsive design ensures playability on both desktop and mobile devices.
- Automatic game mode detection (desktop/mobile) for optimized controls and instructions.

7. Animations and Visual Feedback:
- Word movements are animated using CSS transitions for a smooth user experience.
- Selection rectangle provides visual feedback for the current focus.
- Confetti animations are displayed upon level and game completion using the canvas-confetti library.
- Gentle pulsing animation for the target sentence and selected words.

8. Level Completion and Best Moves:
- The game tracks the number of moves taken to complete each level.
- Minimum moves for each level are predefined (where applicable).
- Players' best moves for each level are stored locally.
- Level completion screen shows current moves, minimum moves (if defined), and the player's best moves.
- Players are encouraged to complete levels in the minimum number of moves.

9. Level Generation and Difficulty Progression:
- Levels are predefined with sentences and seeds for the random number generator.
- The number of walls varies as levels progress, affecting difficulty.
- For levels 1-4, wall percentage starts at 30% and decreases by 2% per level, with a minimum of 11%.
- For levels 5 and beyond, wall percentage increases by 2% per level, up to a maximum of 30%.
- Word and wall positions are generated using a seeded RNG for consistency across game sessions.

10. Sound Effects:
Simple sound effects are implemented for word selection and deselection using the Web Audio API:
- Selection sound: A3 note (220 Hz)
- Deselection sound: G3 note (196 Hz)
Note: Sound effects are currently disabled in the code.

11. Performance Considerations:
- Efficient DOM manipulation using document fragments for board creation.
- Use of CSS classes for styling instead of inline styles for better performance.
- Debouncing of move checks to prevent excessive calculations.

12. Accessibility Features:
- Keyboard navigation support for players who cannot use a mouse or touch screen.
- High contrast colors used for better visibility.
- Text-based representation of the game state for potential screen reader support.

13. Error Handling and Debugging:
- Try-catch blocks implemented in critical functions to catch and log errors.
- Console error logging for debugging purposes.

14. Future Improvements and Expansions:
- Implement more levels and refine dynamic difficulty progression.
- Add background music with volume controls.
- Create a global leaderboard system for best moves using a backend server.
- Optimize performance for larger grid sizes and more complex levels.
- Implement a hint system or power-ups to assist players.
- Enhance accessibility features, including full screen reader support.
- Add localization support for multiple languages.
- Implement user accounts for cross-device progress saving.
- Create a level editor for user-generated content.
- Add social sharing features for completed levels and best moves.
- Implement achievements or badges for completing levels with minimum moves or beating personal bests.
- Add a daily challenge feature with unique levels.

This documentation provides a comprehensive overview of the Wordix game, its implementation details, and potential areas for future development. It serves as a guide for understanding the game's structure, mechanics, and technical considerations.

15. Level Data:
The following is a CSV dump of all levels, including word and wall positions:

Level,Sentence,Type,Row,Col
1,The quick brown fox,Word 1,7,0
1,The quick brown fox,Word 2,2,4
1,The quick brown fox,Word 3,1,0
1,The quick brown fox,Word 4,2,7
1,The quick brown fox,Wall 1,7,2
1,The quick brown fox,Wall 2,3,6
1,The quick brown fox,Wall 3,4,2
1,The quick brown fox,Wall 4,0,6
1,The quick brown fox,Wall 5,7,6
1,The quick brown fox,Wall 6,3,7
1,The quick brown fox,Wall 7,7,7
1,The quick brown fox,Wall 8,5,1
1,The quick brown fox,Wall 9,3,2
1,The quick brown fox,Wall 10,6,6
1,The quick brown fox,Wall 11,5,6
1,The quick brown fox,Wall 12,6,5
1,The quick brown fox,Wall 13,3,0
1,The quick brown fox,Wall 14,2,1
1,The quick brown fox,Wall 15,6,0
1,The quick brown fox,Wall 16,2,0
1,The quick brown fox,Wall 17,5,7
1,The quick brown fox,Wall 18,0,5
1,The quick brown fox,Wall 19,4,5
2,Jumps over the lazy dog,Word 1,1,1
2,Jumps over the lazy dog,Word 2,5,7
2,Jumps over the lazy dog,Word 3,2,7
2,Jumps over the lazy dog,Word 4,3,5
2,Jumps over the lazy dog,Word 5,4,3
2,Jumps over the lazy dog,Wall 1,2,0
2,Jumps over the lazy dog,Wall 2,4,2
2,Jumps over the lazy dog,Wall 3,6,0
2,Jumps over the lazy dog,Wall 4,7,4
2,Jumps over the lazy dog,Wall 5,5,2
2,Jumps over the lazy dog,Wall 6,4,1
2,Jumps over the lazy dog,Wall 7,5,1
2,Jumps over the lazy dog,Wall 8,3,4
2,Jumps over the lazy dog,Wall 9,7,5
2,Jumps over the lazy dog,Wall 10,7,6
2,Jumps over the lazy dog,Wall 11,2,6
2,Jumps over the lazy dog,Wall 12,7,7
2,Jumps over the lazy dog,Wall 13,7,0
2,Jumps over the lazy dog,Wall 14,1,2
2,Jumps over the lazy dog,Wall 15,6,5
2,Jumps over the lazy dog,Wall 16,3,0
2,Jumps over the lazy dog,Wall 17,2,4
3,Better late than never friend,Word 1,3,5
3,Better late than never friend,Word 2,4,7
3,Better late than never friend,Word 3,5,0
3,Better late than never friend,Word 4,6,7
3,Better late than never friend,Word 5,2,7
3,Better late than never friend,Wall 1,6,5
3,Better late than never friend,Wall 2,7,4
3,Better late than never friend,Wall 3,7,6
3,Better late than never friend,Wall 4,4,3
3,Better late than never friend,Wall 5,1,7
3,Better late than never friend,Wall 6,7,2
3,Better late than never friend,Wall 7,5,5
3,Better late than never friend,Wall 8,5,6
3,Better late than never friend,Wall 9,1,6
3,Better late than never friend,Wall 10,6,1
3,Better late than never friend,Wall 11,0,3
3,Better late than never friend,Wall 12,1,0
3,Better late than never friend,Wall 13,1,1
3,Better late than never friend,Wall 14,3,2
3,Better late than never friend,Wall 15,1,4
3,Better late than never friend,Wall 16,4,5
4,Make hay while the sun shines,Word 1,7,7
4,Make hay while the sun shines,Word 2,7,5
4,Make hay while the sun shines,Word 3,1,6
4,Make hay while the sun shines,Word 4,7,3
4,Make hay while the sun shines,Word 5,1,4
4,Make hay while the sun shines,Word 6,3,2
4,Make hay while the sun shines,Wall 1,2,7
4,Make hay while the sun shines,Wall 2,0,1
4,Make hay while the sun shines,Wall 3,2,6
4,Make hay while the sun shines,Wall 4,7,2
4,Make hay while the sun shines,Wall 5,5,2
4,Make hay while the sun shines,Wall 6,6,3
4,Make hay while the sun shines,Wall 7,1,1
4,Make hay while the sun shines,Wall 8,1,3
4,Make hay while the sun shines,Wall 9,2,5
4,Make hay while the sun shines,Wall 10,6,0
4,Make hay while the sun shines,Wall 11,4,3
4,Make hay while the sun shines,Wall 12,5,7
4,Make hay while the sun shines,Wall 13,6,6
4,Make hay while the sun shines,Wall 14,0,3
4,Make hay while the sun shines,Wall 15,3,0
5,A stitch in time saves nine,Word 1,1,6
5,A stitch in time saves nine,Word 2,1,0
5,A stitch in time saves nine,Word 3,1,2
5,A stitch in time saves nine,Word 4,3,5
5,A stitch in time saves nine,Word 5,4,2
5,A stitch in time saves nine,Word 6,3,0
5,A stitch in time saves nine,Wall 1,6,2
5,A stitch in time saves nine,Wall 2,0,5
5,A stitch in time saves nine,Wall 3,0,3
5,A stitch in time saves nine,Wall 4,0,7
5,A stitch in time saves nine,Wall 5,5,1
5,A stitch in time saves nine,Wall 6,5,7
5,A stitch in time saves nine,Wall 7,7,1
5,A stitch in time saves nine,Wall 8,6,1
5,A stitch in time saves nine,Wall 9,5,2
5,A stitch in time saves nine,Wall 10,7,3
5,A stitch in time saves nine,Wall 11,3,6
5,A stitch in time saves nine,Wall 12,2,7
5,A stitch in time saves nine,Wall 13,2,6
5,A stitch in time saves nine,Wall 14,0,1
5,A stitch in time saves nine,Wall 15,1,3
5,A stitch in time saves nine,Wall 16,6,4
5,A stitch in time saves nine,Wall 17,5,0
5,A stitch in time saves nine,Wall 18,2,5
5,A stitch in time saves nine,Wall 19,6,6
6,Actions speak louder than words,Word 1,2,3
6,Actions speak louder than words,Word 2,3,5
6,Actions speak louder than words,Word 3,2,5
6,Actions speak louder than words,Word 4,3,7
6,Actions speak louder than words,Word 5,4,0
6,Actions speak louder than words,Wall 1,6,7
6,Actions speak louder than words,Wall 2,6,2
6,Actions speak louder than words,Wall 3,4,6
6,Actions speak louder than words,Wall 4,1,6
6,Actions speak louder than words,Wall 5,1,4
6,Actions speak louder than words,Wall 6,4,7
6,Actions speak louder than words,Wall 7,7,7
6,Actions speak louder than words,Wall 8,5,1
6,Actions speak louder than words,Wall 9,2,2
6,Actions speak louder than words,Wall 10,7,0
6,Actions speak louder than words,Wall 11,5,4
6,Actions speak louder than words,Wall 12,4,1
6,Actions speak louder than words,Wall 13,1,1
6,Actions speak louder than words,Wall 14,0,1
6,Actions speak louder than words,Wall 15,6,0
6,Actions speak louder than words,Wall 16,5,6
6,Actions speak louder than words,Wall 17,0,6
6,Actions speak louder than words,Wall 18,7,2
6,Actions speak louder than words,Wall 19,4,3
7,All that glitters is not gold,Word 1,2,3
7,All that glitters is not gold,Word 2,7,5
7,All that glitters is not gold,Word 3,7,4
7,All that glitters is not gold,Word 4,7,6
7,All that glitters is not gold,Word 5,3,2
7,All that glitters is not gold,Word 6,4,7
7,All that glitters is not gold,Wall 1,2,4
7,All that glitters is not gold,Wall 2,4,2
7,All that glitters is not gold,Wall 3,0,6
7,All that glitters is not gold,Wall 4,1,0
7,All that glitters is not gold,Wall 5,0,4
7,All that glitters is not gold,Wall 6,5,3
7,All that glitters is not gold,Wall 7,6,6
7,All that glitters is not gold,Wall 8,4,0
7,All that glitters is not gold,Wall 9,0,5
7,All that glitters is not gold,Wall 10,2,7
7,All that glitters is not gold,Wall 11,0,0
7,All that glitters is not gold,Wall 12,0,7
7,All that glitters is not gold,Wall 13,7,3
7,All that glitters is not gold,Wall 14,5,4
7,All that glitters is not gold,Wall 15,1,4
7,All that glitters is not gold,Wall 16,6,5
7,All that glitters is not gold,Wall 17,6,1
7,All that glitters is not gold,Wall 18,7,0
7,All that glitters is not gold,Wall 19,5,0
8,A rolling stone gathers no moss,Word 1,4,2
8,A rolling stone gathers no moss,Word 2,1,6
8,A rolling stone gathers no moss,Word 3,3,3
8,A rolling stone gathers no moss,Word 4,1,3
8,A rolling stone gathers no moss,Word 5,2,0
8,A rolling stone gathers no moss,Word 6,1,4
8,A rolling stone gathers no moss,Wall 1,5,0
8,A rolling stone gathers no moss,Wall 2,7,3
8,A rolling stone gathers no moss,Wall 3,2,6
8,A rolling stone gathers no moss,Wall 4,3,1
8,A rolling stone gathers no moss,Wall 5,3,2
8,A rolling stone gathers no moss,Wall 6,7,1
8,A rolling stone gathers no moss,Wall 7,3,0
8,A rolling stone gathers no moss,Wall 8,4,6
8,A rolling stone gathers no moss,Wall 9,0,2
8,A rolling stone gathers no moss,Wall 10,6,6
8,A rolling stone gathers no moss,Wall 11,0,7
8,A rolling stone gathers no moss,Wall 12,7,5
8,A rolling stone gathers no moss,Wall 13,0,5
8,A rolling stone gathers no moss,Wall 14,5,7
8,A rolling stone gathers no moss,Wall 15,6,7
8,A rolling stone gathers no moss,Wall 16,5,1
8,A rolling stone gathers no moss,Wall 17,2,7
8,A rolling stone gathers no moss,Wall 18,6,3
8,A rolling stone gathers no moss,Wall 19,7,4
9,Fortune always favors the brave soul,Word 1,6,0
9,Fortune always favors the brave soul,Word 2,6,3
9,Fortune always favors the brave soul,Word 3,1,6
9,Fortune always favors the brave soul,Word 4,7,0
9,Fortune always favors the brave soul,Word 5,1,3
9,Fortune always favors the brave soul,Word 6,5,7
9,Fortune always favors the brave soul,Wall 1,1,0
9,Fortune always favors the brave soul,Wall 2,6,5
9,Fortune always favors the brave soul,Wall 3,0,5
9,Fortune always favors the brave soul,Wall 4,0,1
9,Fortune always favors the brave soul,Wall 5,2,5
9,Fortune always favors the brave soul,Wall 6,3,7
9,Fortune always favors the brave soul,Wall 7,4,3
9,Fortune always favors the brave soul,Wall 8,0,2
9,Fortune always favors the brave soul,Wall 9,3,1
9,Fortune always favors the brave soul,Wall 10,3,6
9,Fortune always favors the brave soul,Wall 11,0,7
9,Fortune always favors the brave soul,Wall 12,7,2
9,Fortune always favors the brave soul,Wall 13,5,5
9,Fortune always favors the brave soul,Wall 14,2,1
9,Fortune always favors the brave soul,Wall 15,7,6
9,Fortune always favors the brave soul,Wall 16,2,7
9,Fortune always favors the brave soul,Wall 17,5,1
9,Fortune always favors the brave soul,Wall 18,4,0
9,Fortune always favors the brave soul,Wall 19,0,0
10,Practice makes perfect or at least less embarrassing,Word 1,5,4
10,Practice makes perfect or at least less embarrassing,Word 2,6,1
10,Practice makes perfect or at least less embarrassing,Word 3,3,4
10,Practice makes perfect or at least less embarrassing,Word 4,5,5
10,Practice makes perfect or at least less embarrassing,Word 5,5,2
10,Practice makes perfect or at least less embarrassing,Word 6,7,2
10,Practice makes perfect or at least less embarrassing,Word 7,3,0
10,Practice makes perfect or at least less embarrassing,Word 8,7,0
10,Practice makes perfect or at least less embarrassing,Wall 1,7,5
10,Practice makes perfect or at least less embarrassing,Wall 2,3,2
10,Practice makes perfect or at least less embarrassing,Wall 3,7,3
10,Practice makes perfect or at least less embarrassing,Wall 4,4,6
10,Practice makes perfect or at least less embarrassing,Wall 5,4,0
10,Practice makes perfect or at least less embarrassing,Wall 6,7,4
10,Practice makes perfect or at least less embarrassing,Wall 7,3,1
10,Practice makes perfect or at least less embarrassing,Wall 8,4,7
10,Practice makes perfect or at least less embarrassing,Wall 9,6,3
10,Practice makes perfect or at least less embarrassing,Wall 10,2,6
10,Practice makes perfect or at least less embarrassing,Wall 11,7,7
10,Practice makes perfect or at least less embarrassing,Wall 12,0,3
10,Practice makes perfect or at least less embarrassing,Wall 13,3,3
10,Practice makes perfect or at least less embarrassing,Wall 14,7,6
10,Practice makes perfect or at least less embarrassing,Wall 15,5,1
10,Practice makes perfect or at least less embarrassing,Wall 16,6,5
10,Practice makes perfect or at least less embarrassing,Wall 17,0,6
10,Practice makes perfect or at least less embarrassing,Wall 18,2,3
10,Practice makes perfect or at least less embarrassing,Wall 19,2,7

This level data provides a detailed layout for each level, including the positions of words and walls on the game grid. This information is crucial for level generation and can be used for debugging or recreating specific game scenarios.
