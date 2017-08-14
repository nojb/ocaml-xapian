CAMLC=ocamlc

test: xapian_stubs.o xapian.cmo test.cmo
	$(CAMLC) $^ -o $@

xapian_stubs.o: xapian_stubs.cc
	$(CC) -I$(shell $(CAMLC) -where) -I/usr/local/include -c $<

xapian.cmi xapian.cmo: xapian.mli xapian.ml
	$(CAMLC) -c $^

test.cmo: test.ml xapian.cmi
	$(CAMLC) -c $<

clean:
	rm -f *.cm* *.o

.PHONY: clean
