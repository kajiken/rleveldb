extern "C" {
    #include <ruby.h>
}

#include "leveldb/db.h"
#include "leveldb/slice.h"


#ifndef RSTRING_PTR
#define RSTRING_PTR(v) RSTRING(v)->ptr
#endif

static VALUE klass;

typedef struct {
    leveldb::DB* db;
} Leveldb;

VALUE leveldb_open(VALUE self, VALUE _pathname);
VALUE leveldb_put(VALUE self, VALUE _key, VALUE _val);
VALUE leveldb_get(VALUE self, VALUE _key);
VALUE leveldb_del(VALUE self, VALUE _key);
static VALUE leveldb_close(VALUE self);
static void leveldb_obj_free(Leveldb *ldb);
static VALUE leveldb_obj_alloc(VALUE klass);
