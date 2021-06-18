SRCS := $(wildcard *.cpp)
OBJS := $(foreach wrd,$(SRCS),$(patsubst %.cpp,%.o,$(wrd)))
EXEC =
EXEC := $(if $(EXEC),$(EXEC),risc)

all: $(EXEC) clean 

$(EXEC): $(OBJS)
	g++ -o $(EXEC) $(OBJS)

%.o: %.c
	g++ -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o
