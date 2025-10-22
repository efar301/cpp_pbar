# pbar

pbar is inspired by the python library tqdm, where you can wrap an iterable and get a fancy animated progressbar in the terminal. 

I created my own version for c++ loops. It currently performs best with `for` loops, and a `while` version will come soon.

## Demonstration

![gif](/content/pbar.gif)

The logic takes between 60ns and 80ns, but depends on your hardware. Benchmarking does not include the terminal operations (coloring, writing, and updating the progressbar).

## Personalization
The color of the progressbar, the filler character for the progressbar, and the width is customizable. Colors are under `Progressbar::Colors` enum, the filler character can be changed with `SetFiller()`, and the width can be set with `SetWidth()`.

## Requirements
As long as your terminal supports ANSI escape characters, `carriage return: \r`, and `newline: \n` then this will work.
