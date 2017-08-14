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

  val index_text: t -> string -> int -> string -> unit

  val increase_termpos: t -> unit
end

module WritableDatabase : sig
  type t

  val create: string -> t

  val replace_document: t -> string -> Document.t -> unit
end
