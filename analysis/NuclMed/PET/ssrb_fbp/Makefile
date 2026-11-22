sources = $(wildcard src/*.cc)
objects = $(subst src/,tmp/,$(patsubst %.cc,%.o,$(sources)))
exec = $(subst src/,bin/,$(subst .cxx,,$(wildcard src/*.cxx)))

CC = g++
INCLUDES = -Iinclude/ 

CFLAGS = $(INCLUDES) 

LDFLAGS = 

FFTWFLAGS = -lfftw3 -lm

all: $(objects) $(exec)
	@echo all done!

$(objects): tmp/%.o: src/%.cc  
	@echo compiling $@
	@$(CC) -c $(CFLAGS) $< -o $@

$(exec): bin/% : src/%.cxx $(objects)
	@echo compiling $@
	@$(CC) $(objects) $(CFLAGS) $< -o $@ $(LDFLAGS) $(FFTWFLAGS)
	@echo $(@F) compiled.

$(notdir $(exec)): % : bin/%

$(notdir $(objects)):%:tmp/%

tmp/cc_%.d: src/%.cc 
	@$(CC) -MM $(CFLAGS) $< | sed 's:$*\.o[ \:]:tmp/& $@:g' >  $@

tmp/cxx_%.d: src/%.cxx
	@$(CC) -MM $(CFLAGS) $< | sed 's:\.o::g' | sed 's:$*[ \:]:bin/& $@:g'  > $@

-include $(subst src/,tmp/cc_,$(patsubst %.cc,%.d,$(wildcard src/*.cc)))
-include $(subst src/,tmp/cxx_,$(patsubst %.cxx,%.d,$(wildcard src/*.cxx)))

.PHONY: nobin
nobin: $(objects)

.PHONY: bin
bin: $(exec)

.PHONY: cleanemacs
cleanemacs:
	@-rm *~ */*~ &> /dev/null

.PHONY: clean
clean:
	@-rm tmp/*.o tmp/*.d bin/* lib/* &> /dev/null

.PHONY: cleanall
cleanall:clean cleanemacs
