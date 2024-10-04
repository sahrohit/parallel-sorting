CXX = g++
CXXFLAGS = -Wall -fopenmp
TARGETS = qss qsp mss msp bbs bbp reference

.PHONY: all clean

all: $(TARGETS)

qss: qss.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

qsp: qsp.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

mss: mss.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

msp: msp.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

bbs: bbs.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

bbp: bbp.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

reference: reference.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)