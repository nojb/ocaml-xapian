open Xapian

let () =
  let db = WritableDatabase.create "foo" in
  let termgenerator = TermGenerator.create () in
  TermGenerator.set_stemmer termgenerator (Stem.create "en");
  let doc = Document.create () in
  TermGenerator.set_document termgenerator doc;
  WritableDatabase.replace_document db "id" doc
