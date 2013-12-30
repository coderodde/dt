all:
	gcc -Os -ansi dt_engine.c -o ~/.dt/dt_engine && ./dtinstall

clean:
	rm ~/.dt/dt_engine ~/.dt/dt_script ~/.dt/633.directory.tagger.tag.633
