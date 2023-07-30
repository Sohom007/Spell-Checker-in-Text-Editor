# Spell Checker in Text Editor (C)

This is a simple Text Editor program written in C that includes a Spell Checker feature. The program allows you to create, display, append, and delete text files, and it also has a spell checker that can identify misspelled words and save the results in a separate file.

## Features

- Create a new text file
- Display the content of a text file
- Append text to an existing text file
- Delete a text file
- Spell Checker to identify misspelled words in the text
- Save misspelled words and their positions in a separate file

## How to Use

1. Clone the repository to your local machine.
2. Compile the `project.c` file using a C compiler.
   ```
   gcc project.c -o text_editor
   ```
3. Run the program.
   ```
   ./text_editor
   ```

## Menu Options

1. **CREATE**: Allows you to create a new text file. Type the text and use '.' to save the file.
2. **DISPLAY**: Displays the content of an existing text file. Enter the file name when prompted.
3. **APPEND**: Appends text to an existing text file. Enter the file name when prompted and type the new text.
4. **DELETE**: Deletes an existing text file. Enter the file name when prompted.
5. **SPELL_CHECKER**: Checks the text for misspelled words and saves the results in the `misspelling.txt` file.

## Spell Checker

The spell checker uses a dictionary file (`dictionary.txt`) to build a trie data structure. It then compares the words in the text with the words in the dictionary to identify misspelled words. The results are saved in the `misspelling.txt` file.

## Dependencies

This program uses the following standard C libraries:
- stdio.h
- conio.h
- process.h
- string.h
- ctype.h
- stdlib.h

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Note

This program is for educational purposes only and may not be suitable for large-scale production use.

Feel free to explore and modify the code according to your needs. If you encounter any issues or have any suggestions, please let me know!

Happy coding!
