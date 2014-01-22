main : CCtrlAgentTester.o test.o
	g++ -I/home/gzh/gtest-1.6.0/include -o main CCtrlAgentTester.o test.o /home/gzh/gtest-1.6.0/libgtest.a  -L../lib/ -lctrlserver -L../build/src/controlDB/ -ldb -L../../lab_common/trunk/lib/ -lcommon -g -lpthread -lrt -lsqlite3
	rm *.o

CCtrlAgentTester.o : CCtrlAgentTester.cpp
	g++ -I/home/gzh/gtest-1.6.0/include -o CCtrlAgentTester.o -c CCtrlAgentTester.cpp  -I../include -I../../lab_common/trunk/include -g

test.o : test.cpp
	g++ -I/home/gzh/gtest-1.6.0/include -o test.o -c test.cpp -I../include -I../../lab_common/trunk/include  -g

