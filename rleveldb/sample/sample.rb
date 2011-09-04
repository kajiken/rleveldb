require './../leveldb'
require 'pp'

db = LevelDB::DB.new

db.open "/tmp/testdb"

db.put "tokyo", "hogehgoe"

pp db.get "tokyo"

db.del "tokyo"

pp db.get "tokyo"

db.close 
