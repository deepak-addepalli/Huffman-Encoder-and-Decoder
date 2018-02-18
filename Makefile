all:encoder decoder Comparision

encoder:encoder.cpp
	g++ encoder.cpp -o encoder


decoder:decoder.cpp
	g++ decoder.cpp -o decoder


Comparision:Comparision.cpp
	g++ Comparision.cpp -o Comparision
