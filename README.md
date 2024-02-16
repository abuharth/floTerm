# floTerm
A simple and intuitive terminal drawing library. Featuring unicode characters and color!

# running floTerm
Clone the repository, or just download main.c. In the terminal, run:
```
  $ cc main.c
  $./a.out
```
Currently, you can just press WASD to move the sprite and text around.

# Planned Additions
I am planning to make this more modular so it can be included in any project that is writing to the
terminal in this way. Since it has no external dependencies, it should be very portable and easy to use
by just importing one header file.
1. add better input handling (current setup with termios can use some work)
2. support for background colors?
