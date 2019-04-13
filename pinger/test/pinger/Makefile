test:
	g++ ./leveldb_test.cc -std=c++11 -I../../../../leveldb/include/  -L../../../../leveldb/out-static/ -Wl,-Bstatic -lleveldb -lmemenv -Wl,-Bdynamic -lpthread
	g++ ./import_ip_list.cc -o ip_reader -std=c++11 -I../../../../leveldb/include/  -L../../../../leveldb/out-static/ -Wl,-Bstatic -lleveldb -lmemenv -Wl,-Bdynamic -lpthread
