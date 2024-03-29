# Labirynth test base

This is a folder hosting an array of test labirynths in a representation customised for the project's needs.

## Naming convention:

All elements within this folder follow a simple naming convention:
- **MM_TEST[X]** - test labirynths, ordered.
- **MM_VIS[X]** - visual representation file of the [X] numbered test labirynth (no cyan).

## File structure:

- All test labirynths are uncompressed BMP files with 4-bit RGB color representation.
- The files are 33x33px bitmaps representing 16x16 cell labirinth and its walls layout.
- Each labiryth cell is 3x3px with its center square [cyan] representing the empty space inside the cell and 8 surrounding pixels representing the walls
	- Wall positioning is in truth defined by the middle pixels on each side - the corners are only matched for readability.
	- Walls are defined by [white] - no wall and [black] - wall present.
	- The walls between the neighbouring cells are shared. As such the cyan fields are separated by 1 pixel only.
- The starting and ending points of the labirynth are marked respectively with [red] and [green] colors.

##### Colors:

The colors mentioned above are defined as:
- white: R:16, G:16, B:16
- black: R: 0, G: 0, B: 0
- cyan:  R: 0, G:16, B:16
- red:   R:16, G: 0, B: 0
- green: R: 0, G:16, B: 0

## Additional:

##### Cyan fields in submitted labirynths:

The labirynth files can be developed and submitted as black-white with start and ending marked (no cyan squares) as it is easy to interpret and overlay them later. These files are saved as **MM_VIS[X]** files
