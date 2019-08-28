CXX = g++
CPPFLAGS = -std=c++11
RM = rm -f

CPUSRCS=cpu_temp.cc
GPUSRCS=gpu_temp.cc

default: all

all: cpu_temp gpu_temp
cpu_temp: cpu_temp.cc
	$(CXX) $(CPPFLAGS) -o check_cpu_temp $(CPUSRCS)

gpu_temp: gpu_temp.cc
	$(CXX) $(CPPFLAGS) -o check_gpu_temp $(GPUSRCS)

clean:
	$(RM) check_cpu_temp check_gpu_temp
