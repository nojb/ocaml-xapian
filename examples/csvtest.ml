open Xapian

let read_csv _ =
  assert false

let () =
  let csv = read_csv Sys.argv.(1) in
  let db = WritableDatabase.create "db" in
  let tg = TermGenerator.create () in
  TermGenerator.set_stemmer tg (Stem.create "en");
  List.iter (fun row ->
      let doc = Document.create () in
      TermGenerator.set_document tg doc;
      TermGenerator.index_text tg ~wdf_inc:1 ~prefix:"S" (List.assoc "TITLE" row);
      TermGenerator.index_text tg ~wdf_inc:2 ~prefix:"XD" (List.assoc "DESCRIPTION" row);

      TermGenerator.index_text tg (List.assoc "TITLE" row);
      TermGenerator.increase_termpos tg;
      TermGenerator.index_text tg (List.assoc "DESCRIPTION" row);

      Document.set_data doc "jjaja";

      let idterm = "Q" ^ List.assoc "id_NUMBER" row in
      Document.add_boolean_term doc idterm;
      WritableDatabase.replace_document db idterm doc
    ) csv
