dt (directory tagger)
=====================

A funky system for tagging directories and switching in bash through all of them! :3

Installing dt
-------------

    # Get the repository
    git clone git@github.com:coderodde/dt.git
    
    # ... or alternatively load the zip file in case you don't have git
    ???
    
    # Now run this:
    ./dtinstall
    
    # The previous spell will create everything needed in '$HOME/.dt/' - directory.
    # Edit the '$HOME/.dt/table' - file by populating tag/path - pairs. For instance:
    
    root /
    mybin /home/meh/bin
    
    # Last, but not least, reload .bashrc
    source ~/.bashrc
    
    # Now you are ready to go. Type in bash "dt root" or "dt mybin" or whatever tags you may specify.
    
Enjoy! :3
