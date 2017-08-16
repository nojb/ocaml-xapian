#include <string>
#include <cstdio>

#include <xapian.h>

using namespace std;
using namespace Xapian;

#ifdef __cplusplus
extern "C" {
#endif

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/alloc.h>
#include <caml/callback.h>

static value xapian_handle_error(const Xapian::Error& err, const char *s)
{
  CAMLparam0();
  CAMLlocalN(args, 5);

  static value *exn = NULL;

  if (exn == NULL) exn = caml_named_value("Xapian.Error");
  if (exn == NULL) caml_failwith("could not find Xapian.Error");

  args[0] = caml_copy_string(s);
  args[1] = caml_copy_string(err.get_type());
  args[2] = caml_copy_string(err.get_msg().c_str());
  args[3] = caml_copy_string(err.get_description().c_str());
  args[4] = caml_copy_string(err.get_error_string());

  caml_raise_with_args(*exn, 5, args);

  CAMLreturn(Val_unit);
}

#define XAPIAN_TYPE(name)                                            \
  static void xapian_ ## name ## _finalize(value v)                  \
  {                                                                  \
    delete *reinterpret_cast<name **>(Data_custom_val(v));           \
  }                                                                  \
                                                                     \
  static struct custom_operations xapian_ ## name ## _custom_ops = { \
    .identifier  = (char *)("xapian." #name),                        \
    .finalize    = &xapian_ ## name ## _finalize,                    \
    .compare     = custom_compare_default,                           \
    .hash        = custom_hash_default,                              \
    .serialize   = custom_serialize_default,                         \
    .deserialize = custom_deserialize_default                        \
  }

#define Xapian_alloc(res, name, ...)                                                \
  do {                                                                              \
    name *tmp = NULL;                                                               \
    try {                                                                           \
      tmp = new name(__VA_ARGS__);                                                  \
    } catch (const Error& err) {                                                    \
      xapian_handle_error(err, #name ".create");                                    \
    } catch (...) {                                                                 \
      printf("Hola\n");                                                             \
      caml_failwith("foo");                                                         \
    }                                                                               \
    res = caml_alloc_custom(&xapian_ ## name ## _custom_ops, sizeof(name *), 0, 1); \
    *reinterpret_cast<name **>(Data_custom_val(res)) = tmp;                         \
  } while (0)

#define Xapian_val(name, v) (*reinterpret_cast<name **>(Data_custom_val(v)))

XAPIAN_TYPE(WritableDatabase);
XAPIAN_TYPE(TermGenerator);
XAPIAN_TYPE(Stem);
XAPIAN_TYPE(Document);

CAMLprim value caml_Xapian_version_string(value unit)
{
  CAMLparam0();
  CAMLlocal1(s);

  s = caml_copy_string(version_string());

  CAMLreturn(s);
}

CAMLprim value caml_Xapian_WritableDatabase(value path, value mode)
{
  CAMLparam1(path);
  CAMLlocal1(res);

  Xapian_alloc(res, WritableDatabase, String_val(path), Int_val(mode));

  CAMLreturn(res);
}

CAMLprim value caml_Xapian_TermGenerator(value unit)
{
  CAMLparam0();
  CAMLlocal1(res);

  Xapian_alloc(res, TermGenerator);

  CAMLreturn(res);
}

CAMLprim value caml_Xapian_Stem(value str)
{
  CAMLparam1(str);
  CAMLlocal1(res);

  Xapian_alloc(res, Stem, String_val(str));

  CAMLreturn(res);
}

CAMLprim value caml_Xapian_TermGenerator_set_stemmer(value ml_tg, value ml_stem)
{
  CAMLparam1(ml_stem);

  Stem *stem = Xapian_val(Stem, ml_stem);
  TermGenerator *tg = Xapian_val(TermGenerator, ml_tg);

  try {
    tg->set_stemmer(*stem);
  } catch (const Error& err) {
    xapian_handle_error(err, "TermGenerator.set_stemmer");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_Document(value unit)
{
  CAMLparam0();
  CAMLlocal1(res);

  Xapian_alloc(res, Document);

  CAMLreturn(res);
}

CAMLprim value caml_Xapian_TermGenerator_set_document(value ml_tg, value ml_doc)
{
  CAMLparam0();

  TermGenerator *tg = Xapian_val(TermGenerator, ml_tg);
  Document *doc = Xapian_val(Document, ml_doc);

  try {
    tg->set_document(*doc);
  } catch (const Error& err) {
    xapian_handle_error(err, "TermGenerator.set_document");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_TermGenerator_index_text(value ml_tg, value text, value num, value prefix)
{
  CAMLparam4(ml_tg, text, num, prefix);

  TermGenerator *tg = Xapian_val(TermGenerator, ml_tg);

  try {
    tg->index_text(String_val(text), Int_val(num), String_val(prefix));
  } catch (const Error& err) {
    xapian_handle_error(err, "TermGenerator.index_text");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_TermGenerator_increase_termpos(value ml_tg)
{
  CAMLparam1(ml_tg);

  TermGenerator *tg = Xapian_val(TermGenerator, ml_tg);

  try {
    tg->increase_termpos();
  } catch (const Error& err) {
    xapian_handle_error(err, "TermGenerator.increase_termpos");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_Document_set_data(value ml_doc, value data)
{
  CAMLparam2(ml_doc, data);

  Document *doc = Xapian_val(Document, ml_doc);

  try {
    doc->set_data(String_val(data));
  } catch (const Error& err) {
    xapian_handle_error(err, "Document.set_data");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_Document_add_boolean_term(value ml_doc, value ml_term)
{
  CAMLparam2(ml_doc, ml_term);

  Document *doc = Xapian_val(Document, ml_doc);

  try {
    doc->add_boolean_term(String_val(ml_term));
  } catch (const Error& err) {
    xapian_handle_error(err, "Document.add_boolean_term");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_WritableDatabase_replace_document(value ml_db, value ml_term, value ml_doc)
{
  CAMLparam3(ml_db, ml_term, ml_doc);

  WritableDatabase *db = Xapian_val(WritableDatabase, ml_db);
  Document *doc = Xapian_val(Document, ml_doc);

  try {
    db->replace_document(String_val(ml_term), *doc);
  } catch (const Error& err) {
    xapian_handle_error(err, "WritableDatabase.replace_document");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_WritableDatabase_commit(value ml_db)
{
  CAMLparam1(ml_db);

  WritableDatabase *db = Xapian_val(WritableDatabase, ml_db);

  try {
    db->commit();
  } catch (const Error& err) {
    xapian_handle_error(err, "WritableDatabase.commit");
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_Xapian_WritableDatabase_delete_document(value vdb, value vid)
{
  CAMLparam2(vdb, vid);

  WritableDatabase *db = Xapian_val(WritableDatabase, vdb);

  try {
    db->delete_document(String_val(vid));
  } catch (const Error& err) {
    xapian_handle_error(err, "WritableDatabase.delete_document");
  }

  CAMLreturn(Val_unit);
}

#ifdef __cplusplus
}
#endif
