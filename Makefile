MYCC	= g++
CXX	= $(MYCC) -MMD -MP -O3 -g -c

LINK	= $(MYCC) -o $@ $^

all: funnelhashing

clean:
	rm -f	\
		*.o	\
		*.d	\
		funnelhashing

.PHONY: all clean

murmur_hash_64a.o: murmur_hash_64a.cc murmur_hash_64a.h
	$(CXX) $<

funnelhashing.o: funnelhashing.cc funnelhash.h murmur_hash_64a.h
	$(CXX) $<

funnelhashing: funnelhashing.o murmur_hash_64a.o
	$(LINK)

