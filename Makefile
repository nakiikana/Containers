CC=g++
CPPFLAGS=-Wall -Wextra -Werror -std=c++17 -g
LDFLAGS=-lgtest -lgtest_main -lpthread
GCOV_FLAGS=
TEST_SOURCES=tests/*.cc

all: test

test:
	$(CC) $(CPPFLAGS) $(TEST_SOURCES) $(LDFLAGS) $(GCOV_FLAGS) -o test
	./test

gcov_report: GCOV_FLAGS += -fprofile-arcs -ftest-coverage
gcov_report: clean test
	# ./test > /dev/null
	# geninfo --ignore-errors mismatch .
	lcov --no-external -c -d . -o report.info
	lcov --remove report.info "$(PWD)/tests/*" -o report.info
	genhtml -o "html_report" report.info
	open html_report/index.html

style:
	# clang-format --style=google -i *.h tests/*.cc trees/*.h
	clang-format --style=google -n *.h tests/*.cc trees/*.h

clean:
	rm -rf *.o *.gch *.a *.out test *.gcno *.gcda *.info html_report

.PHONY: all test style clean gcov_report
