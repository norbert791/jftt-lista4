# jftt-lista4
Lista 4 na kurs języki formalne i techniki translacji (kompilator)
# kompilacja
cmake -S compiler -B [folder docelowy]
cmake --build [folder docelowy]

np.

cmake -S compiler -B build
cmake --build build
# Uruchomienie
[folder docelowy]/compiler [plik wejścia/kod] [plik wyjścia/skompilowany kod] 

# Platforma testowa

Kompilator C: GNU 11.3.0
Kompilator CXX: GNU 11.3.0
FLEX: "2.6.4"
BISON: "3.8.2"
Do wybudowania CMake uży GNU Make