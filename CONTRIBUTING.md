[Just something incomplete to start off with so that I don't put off writing this forever]

# Bug Reports
[Put something here later]

# Style Guide
## Code
### Casing
The codebase tries to conform to the following casing conventions for various tokens in the source:
- **PascalCase**: Class Names, Types, Namespaces
- **camelCase**: Functions, Methods
- **snake_case**: Variables
- **SCREAMING_SNAKE_CASE**: Preprocessor

### General Principles
- "Verbose and precise" is preferred to "terse and vague". For example, a `variable_with_a_very_long_name_but_obvious_meaning` is more preferable than `var`.
- [Allman Style](https://en.wikipedia.org/wiki/Indentation_style#Allman_style) indentation preferred to [K&R](https://en.wikipedia.org/wiki/Indentation_style#Variant:_Stroustrup)
- Prefix members of a class used within that class with `this->` wherever possible and wherever it makes sense.

## Git Commit Messages
- Present tense (`Create File.txt`, not `Created File.txt`)
- Imperative mood (`Create File.txt`, not `Creates File.txt`)
