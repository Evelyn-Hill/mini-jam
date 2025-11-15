cmake -B build
cmake --build build

cp build/compile_commands.json ..

echo ""
echo "== PROGRAM OUT =="
./build/mini-jam
