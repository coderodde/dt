dt (directory tagger)
=====================

A funky system for tagging directories and switching in bash through all of them! :3

Installing dt
-------------

    # Get the repository
    git clone git@github.com:coderodde/dt.git
    
    # ... or alternatively load the zip file in case you don't have git
    ???
    
    # Now run this from the directory containing dt files:
    make
    
    # Reload:
    source ~/.bashrc
    
    # The previous spell will create everything needed in '$HOME/.dt/' - directory.
    # Edit the '$HOME/.dt/table' - file by populating tag/path - pairs. For instance:
    
    root /
    mybin /home/meh/bin
    
    # There is no need for hacking directly into table file. Use these:
    add -a bin /usr/bin
    # or..
    add --append bin /usr/bin
    
    add -p [REGEX]
    # or..
    add --print [REGEX]
    
    add -r bin
    # or..
    add --remove bin
    
    # Now you are ready to go. Type in bash "dt root" or "dt mybin" or whatever tags you may specify.
    
Enjoy! :3

What's new?
-----------

* Added Levenshtein distance support in order to do approximate tag matching.
* Now you can use '~' as to denote you home directory.
* There is a command line API for adding, listing and removing tags + ping pong between 2 directories.

Limitations
-----------

In the ~/.dt/table file, all paths must be absolute, so no '~'-characters for now. I will do a workaround for this soon enough. (Done! See above.)
