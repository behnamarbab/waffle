CXX = $(HOME)/AFL/afl-clang-fast++
# AFLCXX = $(HOME)/Shared/Dropbox/UNB/Thesis/S1/MemLock/tool/MemLock/memlock-stack-clang++
AFLCXX = $(HOME)/Dropbox/UNB/Thesis/S1/Waffle_Memlock/tool/MemLock/memlock-stack-clang++
# AFLCXX = $(HOME)/Fuzzer/perffuzz/afl-clang-fast++
# AFLCXX = afl-clang-fast++
CXXFLAGS = -Wall -g -v

main: test1.o
	@echo 'Main'
	$(CXX) $(CXXFLAGS) -o test1 test1.o

afl: test1-afl.o
	@echo 'AFL'
	$(AFLCXX) $(CXXFLAGS) -o test-program test1.o

test1.o: example3.cpp
	$(CXX) $(CXXFLAGS) -c example3.cpp

test1-afl.o: example3.cpp
	$(AFLCXX) $(CXXFLAGS) -c example3.cpp
	$(AFLCXX) $(CXXFLAGS) -S -c example3.cpp

clean:
	rm test1 test1.o
