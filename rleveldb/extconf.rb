require 'mkmf'

CONFIG['LDSHARED'] = "$(CXX) -shared"
$CFLAGS += " -I/usr/local/include "
$LIBS += " -L/usr/local/lib -lleveldb "
create_makefile('leveldb/leveldb')
