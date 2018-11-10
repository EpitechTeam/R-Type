g++ -I ../../include -L ../../lib/ -Lbuild chat_client.cpp -lboost_thread -lpthread -lboost_regex -lboost_system -o client
export LD_LIBRARY_PATH=../../lib
