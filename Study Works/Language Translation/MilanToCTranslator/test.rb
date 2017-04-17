require_relative "sintAn2"
require_relative "lexAn2"
require_relative "codeGen"

#====================================================================================
#Лексический анализатор
obj = LexAn.new("BEGIN number1:=0; number2:=1;  index:=2; fibnum:=0; fibid:=READ; IF fibid<=2 THEN OUTPUT(0); ELSE WHILE index<fibid DO fibnum:=number1 + number2; number1:=number2; number2:=fibnum; index:=index+1; ENDDO; ENDIF; OUTPUT(fibnum); END")
obj.scan()
obj.codeGen()


#Сохраним стек использованных переменных и констант для генератора кода
stack_of_consts_and_ids = obj.consts_and_ids_to_stack
#|||||||||||||||||||||||||
#VVVVVVVVVVVVVVVVVVVVVVVVV
#Г_е_н_е_р_а_т_о_р к_о_д_а
#====================================================================================


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#Синтаксический анализатор
sint = SintAn.new(obj.lexemsTable)
sint.sintAn
sint.printResults

#Передаем дерево Генератору кода
syntTree = sint.syntTree
#|||||||||||||||||||||||||
#VVVVVVVVVVVVVVVVVVVVVVVVV
#Г_е_н_е_р_а_т_о_р к_о_д_а
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



#№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№
#Генератор кода
cdg = CodeGen.new(syntTree, stack_of_consts_and_ids)
cdg.codeGen()
cdg.printResults()
#№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№№