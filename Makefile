CXX = g++
CPPFLAGS = -std=c++11 -g -I/opt/vc/include -L/opt/vc/lib -lbcm_host
RM = rm -f

CPUSRCS=cpu_temp.cc
GPUSRCS=gpu_temp.cc
TESTSRCS=test.cc

default: all

what: 
	@echo check_cpu_temp check_gpu_temp test

all: check_cpu_temp check_gpu_temp test

check_cpu_temp: cpu_temp.cc
	$(CXX) $(CPPFLAGS) -o check_cpu_temp $(CPUSRCS)
check_gpu_temp: gpu_temp.cc gpu_temp.h
	$(CXX) $(CPPFLAGS) -o check_gpu_temp $(GPUSRCS)
test: test.cc gpu_temp.h
	$(CXX) $(CPPFLAGS) -o test $(TESTSRCS)

clean:
	$(RM) check_cpu_temp check_gpu_temp test

install: check_cpu_temp check_gpu_temp
	cp check_cpu_temp /usr/local/nagios/libexec/
	cp check_gpu_temp /usr/local/nagios/libexec/
	chown nagios:nagios /usr/local/nagios/libexec/check_cpu_temp
	chown nagios:nagios /usr/local/nagios/libexec/check_gpu_temp
