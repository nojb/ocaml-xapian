let read_csv path =
  let ic = open_in_bin path in
  let titles = String.split_on_char ',' (input_line ic) in
  let rec loop lines =
    match input_line ic with
    | exception End_of_file ->
        close_in ic;
        List.rev lines
    | line ->
        let fields = String.split_on_char ',' line in
        if List.length fields = List.length titles then begin
          loop (List.combine titles fields :: lines)
        end else
          loop lines
  in
  loop []

open Xapian

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
