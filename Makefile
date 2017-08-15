all:
	jbuilder build @install

examples:
	jbuilder build examples/csvtest.exe

clean:
	jbuilder clean

install:
	jbuilder install

uninstall:
	jbuilder uninstall

reinstall: uninstall install

doc:
	jbuilder build @doc

.PHONY: all clean doc install uninstall
