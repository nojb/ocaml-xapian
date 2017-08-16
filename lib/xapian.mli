exception Error of string * string * string * string * string

val version_string: unit -> string

module Stem : sig
  type t

  val create: string -> t
end

module Document : sig
  type t

  val create: unit -> t

  val set_data: t -> string -> unit

  val add_boolean_term: t -> string -> unit
end

module TermGenerator : sig
  type t

  val create: unit -> t

  val set_stemmer: t -> Stem.t -> unit

  val set_document: t -> Document.t -> unit

  val index_text: t -> ?wdf_inc:int -> ?prefix:string -> string -> unit

  val increase_termpos: t -> unit
end

module WritableDatabase : sig
  type t

  type mode =
    | DB_CREATE_OR_OPEN
    | DB_CREATE_OR_OVERWRITE
    | DB_CREATE
    | DB_OPEN

  val create: ?mode:mode -> string -> t

  val replace_document: t -> string -> Document.t -> unit

  val commit: t -> unit

  val delete_document: t -> string -> unit
end
