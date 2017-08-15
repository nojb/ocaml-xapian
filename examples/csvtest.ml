open Xapian

let () =
  Printexc.record_backtrace true

let delete_docs dbpath identifiers =
  (* Open the database we are going to be deleting from. *)
  let db = WritableDatabase.(create ~mode:DB_OPEN dbpath) in
  List.iter (fun identifier ->
      let idterm = "Q" ^ identifier in
      WritableDatabase.delete_document db idterm
    ) identifiers

let with_open_in path f =
  let ic = open_in path in
  match f ic with
  | r ->
      close_in ic;
      r
  | exception e ->
      close_in_noerr ic;
      raise e

let index datapath dbpath =
  let doit ic =
    let ic = Csv.of_channel ic in
    let db = WritableDatabase.create dbpath in
    let tg = TermGenerator.create () in
    TermGenerator.set_stemmer tg (Stem.create "en");
    let titles = Csv.next ic in
    let json_of_row row =
      Ezjsonm.(to_string ~minify:false (dict (List.map (fun (k, v) -> k, encode_string v) row)))
    in
    let f row =
      let row = List.combine titles row in
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
      Document.set_data doc (json_of_row row);
      let idterm = "Q" ^ id_number in
      Document.add_boolean_term doc idterm;
      WritableDatabase.replace_document db idterm doc;
    in
    Csv.iter ~f ic;
    WritableDatabase.commit db
  in
  with_open_in datapath doit

let main () =
  index Sys.argv.(1) Sys.argv.(2)

let () =
  Printf.printf "Xapian version: %s\n" (version_string ());
  try
    main ()
  with e ->
    Printf.eprintf "** Fatal error: %s\n%!" (Printexc.to_string e);
    Printexc.print_backtrace stderr
