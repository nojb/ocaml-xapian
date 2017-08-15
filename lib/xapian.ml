type writableDatabase
type termGenerator
type stem
type document

external stub_version_string: unit -> string = "caml_xapian_version_string"
external stub_TermGenerator: unit -> termGenerator = "caml_xapian_TermGenerator"
external stub_Stem: string -> stem = "caml_xapian_Stem"
external stub_TermGenerator_set_stemmer: termGenerator -> stem -> unit = "caml_xapian_TermGenerator_set_stemmer"
external stub_Document: unit -> document = "caml_xapian_Document"
external stub_TermGenerator_set_document: termGenerator -> document -> unit = "caml_xapian_TermGenerator_set_document"
external stub_TermGenerator_index_text: termGenerator -> string -> int -> string -> unit = "caml_xapian_TermGenerator_index_text"
external stub_TermGenerator_increase_termpos: termGenerator -> unit = "caml_xapian_TermGenerator_increase_termpos"
external stub_Document_set_data: document -> string -> unit = "caml_xapian_Document_set_data"
external stub_Document_add_boolean_term: document -> string -> unit = "caml_xapian_Document_add_boolean_term"
external stub_WritableDatabase_replace_document: writableDatabase -> string -> document -> unit = "caml_xapian_WritableDatabase_replace_document"

let version_string () =
  stub_version_string ()

module Stem = struct
  type t = stem

  let create str =
    stub_Stem str
end

module Document = struct
  type t = document

  let create () =
    stub_Document ()

  let set_data doc data =
    stub_Document_set_data doc data

  let add_boolean_term doc term =
    stub_Document_add_boolean_term doc term
end

module TermGenerator = struct
  type t = termGenerator

  let create () =
    stub_TermGenerator ()

  let set_stemmer tg stem =
    stub_TermGenerator_set_stemmer tg stem

  let set_document tg doc =
    stub_TermGenerator_set_document tg doc

  let index_text tg ?(wdf_inc = 1) ?(prefix = "") txt =
    stub_TermGenerator_index_text tg txt wdf_inc prefix

  let increase_termpos tg =
    stub_TermGenerator_increase_termpos tg
end

module WritableDatabase = struct
  type t = writableDatabase

  type mode =
    | DB_CREATE_OR_OPEN
    | DB_CREATE_OR_OVERWRITE
    | DB_CREATE
    | DB_OPEN

  external stub_WritableDatabase: string -> mode -> writableDatabase = "caml_xapian_WritableDatabase"

  let create ?(mode = DB_OPEN) path =
    stub_WritableDatabase path mode

  let replace_document db id doc =
    stub_WritableDatabase_replace_document db id doc

  external commit: t -> unit = "caml_Xapian_WritableDatabase_commit"

  external delete_document: t -> string -> unit = "caml_Xapian_WritableDatabase_delete_document"
end
