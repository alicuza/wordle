# wordle

Wordle in your terminal, written in C.

Guess the secret 5-letter word in 6 tries. After each guess, the letters get colored:

- **green**: right letter, right spot
- **yellow**: the letter is in the word, but somewhere else
- **no color**: the letter isn't in the word (or you've already used up all copies of it)

Like the real Wordle, there's one word per day. Everyone running it on the same day gets the same word.

## Requirements

- `cc` (gcc or clang)
- `make`
- GNU readline (`libreadline-dev` on Debian/Ubuntu, `readline` on Arch/macOS)

## Build & run

```sh
make
./wordle
```

Run it from the repo root, since it reads `./words.txt` from the current directory.

Other make rules: `make clean`, `make fclean`, `make re`.

## Example

```
Enter a 5 letter word: crane
 c r a n e
 _ _ _ _ _
 _ _ _ _ _
 ...
```

Guesses are case-insensitive. Words that are too short, too long, or not in the word list get rejected and don't cost you a try. Press `Ctrl+D` to quit.

## How it works

| File | What it does |
| --- | --- |
| `src/main.c` | Game loop: 6 rounds of guess → check → print |
| `src/init.c` | Loads `words.txt` and picks today's word (hashes the date) |
| `src/input.c` | Reads input with readline and checks it's a valid word |
| `src/check.c` | Marks each letter green/yellow/grey, handling repeated letters correctly |
| `src/print.c` | Draws the board with colors and the win/lose messages |
| `inc/wordle.h` | Settings (word length, max guesses, messages, colors) and types |
| `words.txt` | The word list (~13k five-letter words) |

## Checking for leaks

readline keeps some memory around on purpose, which valgrind reports as leaks. `readline.supp` hides that noise:

```sh
valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./wordle
```
## Resources

`man ascii(7)`
`man console_codes(4)`
`man readline(3)`
`man time(3)`
`man ctime(3)`

## Authors

- sancuta
- kseltenr
