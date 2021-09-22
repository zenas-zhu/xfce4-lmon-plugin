### xfce4-lmon-plugin

Loads monitor for the Xfce4 panel, shows cpu and memory usage as percentages

##### Installation

    ./configure # optionally specify destination directory
    ninja
    sudo ninja install

`./configure` uses the environment variable `$DESTDIR`, as well as the argument `--destdir=DESTDIR` (passing by argument takes precedence) for its installation directory.
it also uses `$CC` and `--cc=CC` to determine c compiler.

##### Uninstall

    sudo ninja uninstall

##### Cleanup

    ./configure --clean
