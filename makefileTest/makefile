outA.txt outB.txt: inA.txt inB.txt inC.txt
	cat inA.txt inB> outA.txt
	cat inB.txt inC.txt > outB.txt

finalA.txt: outB.txt
	cat outA.txt > finalA.txt

finalB.txt: outA.txt
	cat outB.txt > finalB.txt



clean:
	rm -rf out* final*
