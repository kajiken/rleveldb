#include "rleveldb.h"

VALUE leveldb_open(VALUE self, VALUE _pathname)
{
    Check_Type(_pathname, T_STRING);
    Leveldb *ldb;
    Data_Get_Struct(self, Leveldb, ldb);
    
    char* pathname_c = RSTRING_PTR(_pathname);
    std::string pathname = std::string((char*)RSTRING_PTR(_pathname));
    leveldb::Options options;
    leveldb::Status status = leveldb::DB::Open(options, pathname, &ldb->db);
    if(status.ok()) {
        return Qtrue;
    }
    return Qnil;
}

VALUE leveldb_put(VALUE self, VALUE _key, VALUE _val)
{
    Check_Type(_key, T_STRING);
    Check_Type(_val, T_STRING);

    Leveldb *ldb;
    Data_Get_Struct(self, Leveldb, ldb);

    leveldb::Slice key = leveldb::Slice(RSTRING_PTR(_key), RSTRING_LEN(_key));
    leveldb::Slice val = leveldb::Slice(RSTRING_PTR(_val), RSTRING_LEN(_val));
    leveldb::Status status = ldb->db->Put(leveldb::WriteOptions(), key, val);
    if(status.ok()) {
        return Qtrue;
    }
    return Qnil;
}

VALUE leveldb_get(VALUE self, VALUE _key)
{
    Check_Type(_key, T_STRING);

    Leveldb *ldb;
    Data_Get_Struct(self, Leveldb, ldb);

    leveldb::Slice key = leveldb::Slice(RSTRING_PTR(_key), RSTRING_LEN(_key));
    std::string val;
    leveldb::Status status = ldb->db->Get(leveldb::ReadOptions(), key, &val);
    if(status.ok()) {
        return rb_str_new(val.data(), val.size());
    }
    return Qnil;
}

VALUE leveldb_del(VALUE self, VALUE _key)
{
    Check_Type(_key, T_STRING);

    Leveldb *ldb;
    Data_Get_Struct(self, Leveldb, ldb);

    leveldb::Slice key = leveldb::Slice(RSTRING_PTR(_key), RSTRING_LEN(_key));
    std::string val;
    leveldb::Status status = ldb->db->Get(leveldb::ReadOptions(), key, &val);
    if (status.IsNotFound()) return Qnil;

    status = ldb->db->Delete(leveldb::WriteOptions(), key);
    if(status.ok()) {
        return rb_str_new(val.data(), val.size());
    }
    return Qnil;

}

static VALUE leveldb_close(VALUE self) {
  Leveldb* ldb;
  Data_Get_Struct(self, Leveldb, ldb);

  if(ldb->db != NULL) {
    delete ldb->db;
    ldb->db = NULL;
  }
  return Qtrue;
}

static void leveldb_obj_free(Leveldb *ldb)
{
    if(ldb->db != NULL) {
        delete ldb->db;
        ldb->db = NULL;
    }
    delete ldb;
}

static VALUE leveldb_obj_alloc(VALUE klass)
{
    Leveldb *ldb = new Leveldb;
    return Data_Wrap_Struct(klass, 0, leveldb_obj_free, ldb);
}

extern "C" {
    void Init_leveldb()
    { 
        static VALUE leveldb = rb_define_module("LevelDB");
        VALUE klass = rb_define_class_under(leveldb, "DB", rb_cObject);
        rb_define_singleton_method(klass, "new", reinterpret_cast<VALUE(*)(...)>(leveldb_obj_alloc), 0);
        rb_define_method(klass, "open", reinterpret_cast<VALUE(*)(...)>(leveldb_open), 1);
        rb_define_method(klass, "close", reinterpret_cast<VALUE(*)(...)>(leveldb_close), 0);
        rb_define_method(klass, "put", reinterpret_cast<VALUE(*)(...)>(leveldb_put), 2);
        rb_define_method(klass, "get", reinterpret_cast<VALUE(*)(...)>(leveldb_get), 1);
        rb_define_method(klass, "del", reinterpret_cast<VALUE(*)(...)>(leveldb_del), 1);
    }
}
