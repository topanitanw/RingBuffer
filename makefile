prog: ring_buffer.c
	gcc -o $@ -O3 $^


test: test.cpp ring_buffer.c
	g++ -o $@ -O3 $^

format:
	clang-format -i *.c *.h *.cpp
.PHONY: format

clean:
	rm -f prog test
.PHONY: clean
