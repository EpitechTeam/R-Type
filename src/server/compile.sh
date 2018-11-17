g++ -I ../../include -L ../../lib/ -Lbuild src/*.cpp -lboost_thread -lpthread -lboost_regex -lboost_system -o server
export LD_LIBRARY_PATH=../../lib
