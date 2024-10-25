run_simples_suffix_tree: clear helper_first
	./build/app
run_suffix_array: clear helper_second
	./build/app
run_suffix_tree: clear helper_third
	./build/app
clear:
	rm -rf build
	mkdir build
helper_first:
	cd ./build;	g++ ./../SimplestSuffixTree.cpp -o app
helper_second:
	cd ./build;	g++ ./../SuffixArray.cpp -o app
helper_third:
	cd ./build;	g++ ./../SuffixTree.cpp -o app
