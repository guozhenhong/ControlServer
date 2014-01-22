main : ManageServerCreateBatchVDMessageTester.o ManageServerCreateBatchVDACKMessageTester.o BatchDeleteMsgTester.o BatchDeleteACKMsgTester.o test.o 
	g++ -I/homegzh/gtest-1.6.0/include -o main ManageServerCreateBatchVDMessageTester.o ManageServerCreateBatchVDACKMessageTester.o BatchDeleteMsgTester.o BatchDeleteACKMsgTester.o test.o /home/gzh/gtest-1.6.0/libgtest.a -g -lpthread
	rm *.o

test.o : test.cpp
	g++ -I/home/gzh/gtest-1.6.0/include -o test.o -c test.cpp -g

ManageServerCreateBatchVDMessageTester.o : ManageServerCreateBatchVDMessageTester.cpp
	g++ -I/home/gzh/gtest-1.6.0/include -o ManageServerCreateBatchVDMessageTester.o -c ManageServerCreateBatchVDMessageTester.cpp -g

ManageServerCreateBatchVDACKMessageTester.o : ManageServerCreateBatchVDACKMessageTester.cpp
	g++ -I/home/gzh/gtest-1.6.0/include -o ManageServerCreateBatchVDACKMessageTester.o -c ManageServerCreateBatchVDACKMessageTester.cpp -g
	
BatchDeleteMsgTester.o : BatchDeleteMsgTester.cpp
	g++ -I/home/gzh/gtest-1.6.0/include -o BatchDeleteMsgTester.o -c BatchDeleteMsgTester.cpp -g

BatchDeleteACKMsgTester.o : BatchDeleteACKMsgTester.cpp
	g++ -I/home/gzh/gtest-1.6.0/include -o BatchDeleteACKMsgTester.o -c BatchDeleteACKMsgTester.cpp -g