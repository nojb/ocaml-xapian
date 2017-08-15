open Xapian

let () =
  Printexc.record_backtrace true

let main () =
  let ic = open_in Sys.argv.(1) in
  let csv_ic = Csv.of_channel ic in
  let db = WritableDatabase.create "db" in
  let tg = TermGenerator.create () in
  TermGenerator.set_stemmer tg (Stem.create "en");
  let titles = Csv.next csv_ic in
  let f fields =
    let row = List.combine titles fields in
    let title = List.assoc "TITLE" row in
    let description = List.assoc "DESCRIPTION" row in
    let id_number = List.assoc "id_NUMBER" row in
    let doc = Document.create () in
    TermGenerator.set_document tg doc;
    TermGenerator.index_text tg ~wdf_inc:1 ~prefix:"S" title;
    TermGenerator.index_text tg ~wdf_inc:2 ~prefix:"XD" description;
    TermGenerator.index_text tg title;
    TermGenerator.increase_termpos tg;
    TermGenerator.index_text tg description;
    Document.set_data doc (String.concat "," fields);
    let idterm = "Q" ^ id_number in
    Document.add_boolean_term doc idterm;
    WritableDatabase.replace_document db idterm doc;
  in
  Csv.iter ~f csv_ic;
  WritableDatabase.commit db

let () =
  Printf.printf "Xapian version: %s\n" version_string;
  try
    main ()
  with e ->
    Printf.eprintf "** Fatal error: %s\n%!" (Printexc.to_string e);
    Printexc.print_backtrace stderr
