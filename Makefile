CAMLFLAGS=
CAMLC=ocamlc $(CAMLFLAGS)
CAMLOPT=ocamlopt $(CAMLFLAGS)

test: xapian_stubs.o xapian.cmx test.cmx
	$(CAMLOPT) -cclib -lstdc++ -cclib '$(shell pkg-config xapian-core --libs)' $^ -o $@

xapian_stubs.o: xapian_stubs.cc
	$(CC) -I$(shell $(CAMLC) -where) $(shell pkg-config xapian-core --cflags) -c $<

xapian.cmi: xapian.mli
	$(CAMLC) -c $^

xapian.cmo: xapian.ml xapian.cmi
	$(CAMLC) -c $<

xapian.cmx: xapian.ml xapian.cmi
	$(CAMLOPT) -c $<

test.cmo: test.ml xapian.cmi
	$(CAMLC) -c $<

test.cmx: test.ml xapian.cmi
	$(CAMLOPT) -c $<

clean:
	rm -f *.cm* *.o

.PHONY: clean
