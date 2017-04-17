#Лексический анализатор
class LexAn
	#Входные данные
	@input #Програма на языке Милан

	#------------------------------------------------
	#Промежуточные данные
	#Таблица лексем языка Милан
	#@table = {"BEGIN"=>[[1,0]], "DO"=>[[2,0]], "ELSE" =>[[3,0]], "END" =>[[4,0]], "ENDDO" =>[[5,0]], "ENDIF" =>[[6,0]], "IF" => [[7,0]], "OUTPUT" => [[8,0]], "READ" => [[9,0]], "THEN" => [[10,0]], "WHILE" => [[11,0]], ";" => [[12,0]], "=" => [[13,0]], "<>" => [[13,1]], ">" => [[13,2]], "<" => [[13,3]], ">=" => [[13,4]], "<=" => [[13,5]],"+" => [[14,0]], "-" => [[14,1]], "*" => [[15,0]], "/" => [[15,1]], ":=" => [[16,1]], "(" => [[17,0]], ")" => [[18,0]]}
	@table
	#------------------------------------------------
	#Выходные данные
	@lexemsTable #Таблица лексем
	@ids  #Таблица идентификаторов
	@consts  #Таблица констант

	#Счетчики
	@lexemsCount #Число выделенных лексем
	@idsCount  #Число выделенных идентификаторов
	@constsCount #Число выделенных констант

	@OutProg #Выходная программа


	def lexemsTable
		@lexemsTable #для передачи ее синтксическому анализатору
	end

	public
	#==================================================================================
	def initialize(input)
		if (input == nil)
			@input = "BEGIN number1:=0; number2:=1;  index:=2; fibnum:=0; fibid:=READ; IF fibid<=2 THEN OUTPUT(0); ELSE WHILE index<fibid DO fibnum:=number1 + number2; number1:=number2; number2:=fibnum; index:=index+1; ENDDO; ENDIF; OUTPUT(fibnum); END"
		else
			@input = input
		end

		#Входные данные
		@input #Програма на языке Милан

		#------------------------------------------------
		#Промежуточные данные
		#Таблица лексем языка Милан
		@table = {"BEGIN"=>[[1,0]], "DO"=>[[2,0]], "ELSE" =>[[3,0]], "END" =>[[4,0]], "ENDDO" =>[[5,0]], "ENDIF" =>[[6,0]], "IF" => [[7,0]], "OUTPUT" => [[8,0]], "READ" => [[9,0]], "THEN" => [[10,0]], "WHILE" => [[11,0]], ";" => [[12,0]], "=" => [[13,0]], "<>" => [[13,1]], ">" => [[13,2]], "<" => [[13,3]], ">=" => [[13,4]], "<=" => [[13,5]],"+" => [[14,0]], "-" => [[14,1]], "*" => [[15,0]], "/" => [[15,1]], ":=" => [[16,0]], "(" => [[17,0]], ")" => [[18,0]]}

		#------------------------------------------------
		#Выходные данные
		@lexemsTable = [] #Таблица лексем
		@ids = [] #Таблица идентификаторов
		@consts = [] #Таблица констант

		#Счетчики
		@lexemsCount = 0 #Число выделенных лексем
		@idsCount = 0    #Число выделенных идентификаторов
		@constsCount = 0 #Число выделенных констант

		#Выходная программа
		@OutProg = []
	end


	#==================================================================================
	def scan()
		#Прежде, чем запускать сам сканнер, выполним проверку на ошибки
		return if searchForErrors()
		#Если ошибок не найдено, запускаем анализатор
		#Анализ выполняем в цикле
		#Подготовка
		#====================================================================
		i = 0
		size = @input.length - 1 #Учитываем нумерацию массива с 0 элемента

		#Выполнение
		while (i<=size)
			#Подготовка к выполнению
			#Вычисляем, есть ли у нас составной опреатор
			if (i<size) #Иначе мы выйдем за границы массива при проверке
			twoPart = (@input[i] + @input[i+1]) == ":=" || (@input[i] + @input[i+1]) == ">=" || (@input[i] + @input[i+1]) == "<=" || (@input[i] + @input[i+1]) == "<>"
			end

			#Вычисляем, есть ли у нас одинарный опреатор
			single = (@input[i] == '+' || @input[i] == '-' || @input[i] == ';'|| @input[i] == '*' || @input[i] == '/' || @input[i] == '(' || @input[i] == ')' || @input[i] == '=' || (@input[i] == '>' && @input[i+1] != '=') || (@input[i] == '<' && @input[i+1] != '=' && @input[i+1] != '>'))

		    #Игнорируем пропуски
			if (@input[i] == ' ')
				i += 1
				next
			

		    #Обработка двухсоставного оператора 
			elsif (twoPart == true)
			@lexemsTable += @table[(@input[i]+@input[i+1])]
			#Два символа обработали, переходим к следующему за ними символу
			i += 2


			#Обработка односоставного оператора
			elsif (single == true)
		    @lexemsTable += @table[@input[i]]
			#Один символ обработали, переходим к следующему за ним символу
		    i += 1


			#Выделяем ключевые слова и идентификаторы в строке
			elsif (isalpha?(@input[i]))
				#Выделяем беспрерывный набор букв и цифр в строке, начинающийся с буквы

				#Подготовка
				word = ""

				#Выполнение
				while (isalpha?(@input[i]) || isdigit?(@input[i]))
					word += @input[i]
					i+=1
					break if i > size
				end

				
				if (@table[word] != nil)    #word - ключевое слово
					@lexemsTable = @lexemsTable + @table[word]

				else              #word - идентификатор
					#Добавляем иднтификатор в таблицу идентификаторов в случае, если его там пока нет
					if (!@ids.include?(word))
						@ids += [word]
						@idsCount += 1
					end

					#Добавим идентификатор в таблицу лексем
				    @lexemsTable += [[19,@ids.index(word)]]
				end

			#Выделяем число в строке
			elsif (isdigit?(@input[i]))
				number = ""  #Обнуляем временную переменную
				#Выделяем беспрерывный набор цифр в строке
				while (isdigit?(@input[i]))
					number += @input[i]
					i+=1
					break if i>size
				end

				#из 0a нельзя выделить число
				if (i<=size && !isalpha?(@input[i]) || i>size && !isalpha(@input[i-1]))
					#Добавим число в таблицу констант в том случае, если его там пока еще нет
					if (!@consts.include?(number))
						@consts += [number]
						@constsCount += 1
					end
					#Добавим число в таблицу лексем
					@lexemsTable += [[20,@consts.index(number)]]
				end
			end

		    #Приготовления к следующей итерации
			@twoPart = false
			@single = false
			@lexemsCount += 1;  #Число лексем возрасло
		end

		#Расстановка ссылок
		placementLinks()
		#Надо как-то вернуть 3 целочисленных параметра
		#Выводим результаты
		printResults()
	end

	#+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+
	#Генератор кода	
	def codeGen
		#Добавим предопределения для корректной работы программы на языке С
		@OutProg.push("#include <stdio.h>\n\nint main(void)\n")
		#Трансляцию будем проводить в цикле
		#Подготовка к циклу
		i = 0
		while (i<=@lexemsCount-1)
			case(@lexemsTable[i][0])

			when 1  #begin
				@OutProg.push("{\n")

			when 2  #do
				@OutProg.push(" )\n") #Закрывающая скобка для условия цикла
				@OutProg.push("{\n")

			when 3  #else
				@OutProg.push("{\n")

			when 4  #end
				@OutProg.push("return 0;\n")
				@OutProg.push("}\n")

			when 5  #enddo
				@OutProg.push("}\n")

			when 6  #endif
				@OutProg.push("}\n")

			when 7  #if
				@OutProg.push("if")
				@OutProg.push(" ( ") #Открывающая скобка для условия ветвления

			when 8  #output
				@OutProg.push("putchar") #Вывертелся)

			when 9  #read
				@OutProg.push("getchar()\n") #Вывертелся)

			when 10 #then
				@OutProg.push(" )\n") #Закрывающая скобка для условия ветвления
				@OutProg.push("{\n") 

			when 11 #while
				@OutProg.push("while ")
				@OutProg.push(" ( ") #Открывающая скобка для условия цикла

			when 12 #;
				@OutProg.push(";\n")

			when 13 #Знаки отношения
				case @lexemsTable[i][1]
				when 0
					@OutProg.push("==")
				when 1
					@OutProg.push("!=")
				when 2
					@OutProg.push(">")
				when 3
					@OutProg.push("<")
				when 4
					@OutProg.push(">=")
				when 5
					@OutProg.push("<=")
				end
					
			when 14 #Операции типа сложение
				case @lexemsTable[i][1]
				when 0
					@OutProg.push("+")
				when 1
					@OutProg.push("-")
				end
					
			when 15 #Операции типа умножение
				case @lexemsTable[i][1]
				when 0
					@OutProg.push("*")
				when 1
					@OutProg.push("/")
				end

			when 16 #:=
				@OutProg.push(" = ")

			when 17 #(
				@OutProg.push("(")

			when 18 #)
				@OutProg.push(")")

			when 19 #Identifiers
				@OutProg.push("#{@ids[@lexemsTable[i][1]]}")
			when 20 #Constants
				@OutProg.push("#{@consts[@lexemsTable[i][1]]}")
			end	

			#Переход к следующей лексеме
			i+=1 
		end

		puts @OutProg.join
	end
	#Генератор кода
	#+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+





	private
	#==================================================================================
	#Проверка на ошибки
	#==================================================================================
	def searchForErrors()
		#Сделаем мы это в цикле
		#================================================================
		#Подготовка к циклу
		i = 0
		size = @input.length - 1 #Учитываем нумерацию массива с 0 элемента
		#Сам цикл
		#Не закрыта кавычка у строковой константы (в нашем варианте таких, вроде, нет)
		invalidIdName = false
		invalidCharacter = false
		while (i<=size && !invalidIdName && !invalidCharacter)
			#Недопустимое имя идентификатора (1abc)
			invalidIdName = isdigit?(@input[i]) && isalpha?(@input[i+1]) if i<size
			#Недопустимый символ 
			#invalidCharacter = input[i] == тут должна быть регулярка по всем допустимым символам языка Милан

			#Итератор цикла
			i+=1
		end

		if invalidIdName == true
			puts "Error: Invalid identificator name form #{i}"
		end

		if invalidCharacter==true
			puts "Error: Invalid character is on #{i}"
		end

		return invalidIdName || invalidCharacter
	end

	#==================================================================================
	#Расстановка ссылок
	#==================================================================================
	def placementLinks()
		#Подготовка
		#================================================================
		i = 0
		stackDo = [] #while
		stackIf = [] #if
		s = 0 #
		r = 0 #
		size = @lexemsTable.size - 1

		#================================================================
		#Выполнение
		while i<=size #Наверное, придется изменить на while или do while
		case @lexemsTable[i][0] #Определяем что у нас в руках по id
			when 11 #while
				stackDo+=[i]
				#Читаем лексемы, пока не встретим DO
				while (i<size && @lexemsTable[i][0]!=2)
					i+=1
				end

				if @lexemsTable[i][0]==2
					stackDo.push(i)
				end

			when 5 #enddo
				s = stackDo.pop
				r = stackDo.pop
				@lexemsTable[i][1] = r+1
				@lexemsTable[s][1] = i+1

			when 7 #if
				#Читаем лексемы, пока не встретим THEN
				while (i<size && @lexemsTable[i][0]!=10)
					i+=1
				end

				if @lexemsTable[i][0]==10
					stackIf.push(i)
				end


			when 3 #else
				r = stackIf.pop
				@lexemsTable[r][1] = i+1
				stackIf.push(i)


			when 6 #endif
				r = stackIf.pop
				@lexemsTable[r][1]= i+1
				stackIf.push(i)
			end

		#Если не конец строки i++
		i+=1 if i<=size
		end
	end


	#==================================================================================
	#Вывод результатов на экран
	#==================================================================================
	def printResults()
		puts "Таблица лексем"
		puts "Число лексем #{@lexemsCount}"
		p @lexemsTable
		puts"================================================================================"
		puts ""

		puts "Таблица идентификаторов"
		puts "Число идентификаторов #{@idsCount}"
		p @ids
		puts"================================================================================"
		puts ""

		puts "Таблица констант"
		puts "Число констант #{@constsCount}"
		p @consts
		puts ""
	end

	#==================================================================================
	def isalpha? (symbol)
	    !!symbol.match(/^[[:alpha:]]$/)
	end

	def isdigit? (symbol)
	    !!symbol.match(/^\d$/)
	end
	#==================================================================================


	public
	#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	#Готовит пирожки для генератора кода
	#А именно заносит все константы и идентификаторы в стек в порядке их использования
	def consts_and_ids_to_stack
		stack_of_consts_and_ids = []
		stack_of_consts = []
		stack_of_ids = []
		@lexemsTable.each { |i| stack_of_ids.push(@ids[i[1]]) if (i[0] == 19)
								stack_of_consts.push(@consts[i[1]]) if (i[0] == 20)}
		#Перевернем стек, для более удобного использования в будущем

		stack_of_consts_and_ids[0] = stack_of_ids.reverse
		stack_of_consts_and_ids[1] = stack_of_consts.reverse
		stack_of_consts_and_ids
	end
	#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end


PROGRAM = "BEGIN number1:=0; number2:=1;  index:=2; fibnum:=0; fibid:=READ; IF fibid<=2 THEN OUTPUT(0); ELSE WHILE index<fibid DO fibnum:=number1 + number2; number1:=number2; number2:=fibnum; index:=index+1; ENDDO; ENDIF; OUTPUT(fibnum); END"
