NAME := clock

PWD := $(shell pwd)
INCS := -I$(PWD)/inc
SRCDIR := $(PWD)/src
LIBDIR := $(PWD)/lib
LIBNAME := $(LIBDIR)/lib$(NAME).a


CSRC := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst %.c,%.o,$(CSRC))

CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPOBJS := $(patsubst %.cpp,%.o,$(CPPS))

all: $(OBJS) $(CPPOBJS)
	$(shell if [ ! -e $(LIBDIR) ];then mkdir -p $(LIBDIR); fi)
	$(AR) -rcD $(LIBNAME) $(OBJS) $(CPPOBJS)  

$(OBJS): %.o:%.c
	$(CC) -c $(CFLAGS) $(INCS) $< -o $@

$(CPPOBJS): %.o:%.cpp
	$(CXX) -c $(CFLAGS) $(INCS) $< -o $@

.PHNOY:clean
clean:
	@rm -f $(OBJS)
	@rm -f $(CPPOBJS)
	@rm -f $(LIBNAME)
