#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/fail.h>

#include <xapian.h>

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

static struct custom_operations xapian_custom_ops = {
  .identifier = (char *)"xapian handling",
  .finalize = custom_finalize_default,
  .compare = custom_compare_default,
  .hash = custom_hash_default,
  .serialize = custom_serialize_default,
  .deserialize = custom_deserialize_default
};

using namespace std;
using namespace Xapian;

#define Xapian_val(ty,v) (*reinterpret_cast<ty *>(Data_custom_val(v)))

CAMLprim value caml_xapian_WritableDatabase(value path)
{
  CAMLparam1(path);
  CAMLlocal1(res);

  res = caml_alloc_custom(&xapian_custom_ops, sizeof(WritableDatabase), 0, 1);
  new (Data_custom_val(res)) WritableDatabase(String_val(path));

  CAMLreturn(res);
}

CAMLprim value caml_xapian_TermGenerator(value unit)
{
  CAMLparam0();
  CAMLlocal1(res);

  res = caml_alloc_custom(&xapian_custom_ops, sizeof(TermGenerator), 0, 1);
  new (Data_custom_val(res)) TermGenerator();

  CAMLreturn(res);
}

CAMLprim value caml_xapian_Stem(value str)
{
  CAMLparam1(str);
  CAMLlocal1(res);

  res = caml_alloc_custom(&xapian_custom_ops, sizeof(Stem), 0, 1);
  new (Data_custom_val(res)) Stem(String_val(str));

  CAMLreturn(res);
}

CAMLprim value caml_xapian_TermGenerator_set_stemmer(value ml_tg, value ml_stem)
{
  CAMLparam1(ml_stem);

  Stem stem = Xapian_val(Stem, ml_stem);
  TermGenerator tg = Xapian_val(TermGenerator, ml_tg);

  try {
    tg.set_stemmer(stem);
  } catch (const Error& err) {
    caml_failwith(err.get_msg().c_str());
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_xapian_Document(value unit)
{
  CAMLparam0();
  CAMLlocal1(res);

  res = caml_alloc_custom(&xapian_custom_ops, sizeof(Document), 0, 1);
  new (Data_custom_val(res)) Document();

  CAMLreturn(res);
}

CAMLprim value caml_xapian_TermGenerator_set_document(value ml_tg, value ml_doc)
{
  CAMLparam0();

  TermGenerator tg = Xapian_val(TermGenerator, ml_tg);
  Document doc = Xapian_val(Document, ml_doc);

  try {
    tg.set_document(doc);
  } catch (const Error& err) {
    caml_failwith(err.get_msg().c_str());
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_xapian_TermGenerator_index_text(value ml_tg, value text, value num, value prefix)
{
  CAMLparam4(ml_tg, text, num, prefix);

  TermGenerator tg = Xapian_val(TermGenerator, ml_tg);

  try {
    tg.index_text(String_val(text), Int_val(num), String_val(prefix));
  } catch (const Error& err) {
    caml_failwith(err.get_msg().c_str());
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_xapian_TermGenerator_increase_termpos(value ml_tg)
{
  CAMLparam1(ml_tg);

  TermGenerator tg = Xapian_val(TermGenerator, ml_tg);

  try {
    tg.increase_termpos();
  } catch (const Error& err) {
    caml_failwith(err.get_msg().c_str());
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_xapian_Document_set_data(value ml_doc, value data)
{
  CAMLparam2(ml_doc, data);

  Document doc = Xapian_val(Document, ml_doc);

  try {
    doc.set_data(String_val(data));
  } catch (const Error& err) {
    caml_failwith(err.get_msg().c_str());
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_xapian_Document_add_boolean_term(value ml_doc, value ml_term)
{
  CAMLparam2(ml_doc, ml_term);

  Document doc = Xapian_val(Document, ml_doc);

  try {
    doc.add_boolean_term(String_val(ml_term));
  } catch (const Error& err) {
    caml_failwith(err.get_msg().c_str());
  }

  CAMLreturn(Val_unit);
}

CAMLprim value caml_xapian_WritableDatabase_replace_document(value ml_db, value ml_term, value ml_doc)
{
  CAMLparam3(ml_db, ml_term, ml_doc);

  WritableDatabase db = Xapian_val(WritableDatabase, ml_db);
  Document doc = Xapian_val(Document, ml_doc);

  try {
    db.replace_document(String_val(ml_term), doc);
  } catch (const Error& err) {
    caml_failwith(err.get_msg().c_str());
  }

  CAMLreturn(Val_unit);
}

#ifdef __cplusplus
}
#endif
