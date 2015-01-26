vpath main.cpp src
vpath %.cpp src/core
vpath %.h src/core
vpath %.cpp src/stiq
vpath %.h src/stiq

briqs: main.cpp
briqs: interpreter.cpp scope.cpp
briqs: abstractor.cpp tokenizer.cpp stiqplate.cpp stiqbucket.cpp
briqs: smbl.cpp func.cpp spfm.cpp rule.cpp tokn.cpp
briqs: baseplate.cpp bucket.cpp
briqs: text.cpp
briqs: list.cpp cell.cpp
briqs: fval.cpp tval.cpp ui64.cpp none.cpp sval.cpp
briqs: briq.cpp
	clang++ -I./src/core -I./src/stiq $^ -o $@ -g -O0
