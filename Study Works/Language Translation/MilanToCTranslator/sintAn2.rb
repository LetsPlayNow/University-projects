#Синтаксический анализатор
class SintAn
	def initialize (lexemsTable)
		#Входные данные
		@lexemsTable = lexemsTable
		#Промежуточные данные
		@i = 0
		@size = lexemsTable.size
		#Выходные данные
		@syntTree = []
		@err = []
	end

	#Входные данные
	@lexemsTable
	#Промежуточные данные
	@i #Итератор
	@size #Размер таблицы лексем
	#Выходные данные
	@err         # Синтаксическое дерево
	@syntTree    # Ошибки


	#^^^^^^
	public
	#^^^^^^
	def syntTree
		@syntTree
	end

	#Снитаксический анализатор
	def	sintAn()
		i = 0
		#Идем через условия
		if @lexemsTable[0][0] == 1 #BEGIN
			if @lexemsTable[@size - 1][0] == 4 #END
				@syntTree.push ("<программа> = BEGIN <последовательность операторов> END")
				return sequenceOfStatements()
			end
		else 
			@err.push ("Отстутствует BEGIN")
			return false
		end
	end


	def printResults()
		puts @err.join("\n")
		puts @syntTree.join("\n")
	end



	#^^^^^^^
	private
	#^^^^^^^
	#Разбор нетерминала <оператор>
	def operator()
		#Если идентификатор
		if @lexemsTable[@i][0] == 19
			#Возможно разветвление, если добавлять объявление переменных
			@syntTree.push ("<оператор> = <присваивание>")
			return assignment()

		#Если OUTPUT
		elsif @lexemsTable[@i][0] == 8 && @lexemsTable[@i+1][0] == 17 #OUTPUT(
			@syntTree.push ("<оператор> = OUTPUT(<выражение>)")
			@i+=2 #Переходим к разбору нетерминала <выражение>
			trueExpression = expression() && @lexemsTable[@i][0] == 18 #Проверка выражения и закрывающей скобки
			return trueExpression
		end
	end

	#Разбор нетерминала <присваивание>
	def assignment()
		#Если оператор имеет вид "id := <выражение>"
		if @i < @size-2 && @lexemsTable[@i][0] == 19 && @lexemsTable[@i+1][0] == 16 && (@lexemsTable[@i+2][0] == 19 || @lexemsTable[@i+2][0] == 20)
			@syntTree.push ("<присваивание> = идентификатор := <выражение>")
			@i+=2 #Перескакиваем на <выражение>
			return expression()

		#Случай с READ	
		elsif @i < @size-2 && @lexemsTable[@i][0] == 19 && @lexemsTable[@i+1][0] == 16 && @lexemsTable[@i+2][0] == 9
			@syntTree.push ("<присваивание> = идентификатор := READ")
			@i+=3 #Заканчиваем разбор оператора (переходим на ;)
			return true
		else
			@err.push ("Ошибка <присваивание>")
			return false
		end
	end


	#Разбор нетерминала <выражение>
	def expression()
		trueExpression = @i<@size-2 && (19..20).include?(@lexemsTable[@i][0]) && (19..20).include?(@lexemsTable[@i+2][0]) && @lexemsTable[@i+1][0]!=12
		#"id || const := id || const"
		if trueExpression
			case @lexemsTable[@i][0]
				when 19
					case @lexemsTable[@i+2][0]
						when 19
							@syntTree.push ("<выражение> = идентификатор <арифметическая операция> идентификатор")
						when 20
							@syntTree.push ("<выражение> = идентификатор <арифметическая операция> константа")
					end
				when 20
					case @lexemsTable[@i+2][0]
						when 19
							@syntTree.push ("<выражение> = константа <арифметическая операция> идентификатор")
						when 20
							@syntTree.push ("<выражение> = константа <арифметическая операция> константа")
					end
			end

		@i+=1 #Переходим к обработке арифметической операции
		return arithmeticOperation()



		#<выражение> = id || const
		#Признак этого ; или ) после id || const
		elsif (@lexemsTable[@i+1][0]==12 || @lexemsTable[@i+1][0]==18) && (19..20).include?(@lexemsTable[@i][0])
			case @lexemsTable[@i][0]
			when 19
				@syntTree.push ("<выражение> = идентификатор")
			when 20
				@syntTree.push ("<выражение> = константа")
			end
			
			case @lexemsTable[@i+1][0]
			when 12
				@i+=1 #;
			when 18
				@i+=2 #(;
			end
			return true
		else
			@err.push ("Ошибка <выражение>")
			return false
		end
	end

	#Разбор нетерминала <арифм. операция>
	def arithmeticOperation()
		trueArithmeticOperation = (14..15).include?(@lexemsTable[@i][0])
		if trueArithmeticOperation
			case @lexemsTable[@i][0]
				when 14
					case @lexemsTable[@i][1] 
						when 0
							@syntTree.push ("<арифметическая операция> = +")
						when 1
							@syntTree.push ("<арифметическая операция> = -")
					end
							
				when 15
					case @lexemsTable[@i][1] 
						when 0
							@syntTree.push ("<арифметическая операция> = *")
						when 1
							@syntTree.push ("<арифметическая операция> = /")
					end
			end

			@i+=2 #+id;
			return true
		else
			@err.push ("Ошибка <арифметическая операция>")
			return false
		end
	end

	#Разбор нетерминала <структура> (if/if-else/while)
	def struct()
		case @lexemsTable[@i][0]
		when 7 #IF
			#Определяем if/if-else (Ищем else после if)
			iter = @i
			includeElse = 0
			includeENDIF = 0
			while iter < @size do
				includeElse = @lexemsTable[iter][0] == 3 || includeElse
				includeENDIF = @lexemsTable[iter][0] == 6 || includeENDIF
				iter += 1
			end

			#В тексте программы после IF имеется ENDIF
			correctIfstruct = @i < @size - 4 && includeENDIF 
			if correctIfstruct
				case includeElse

				#Разбор струкруры IF-THEN-ELSE-ENDIF
				when true
					#Если после условия идет THEN, и все завершается ENDIF
					@syntTree.push ("<структура> = IF <условие> THEN <последовательность операторов> ELSE <последовательность операторов> ENDIF")
					#Обработка IF-ELSE
					trueStruct = 1

					@i+=1 #Переходим к разбору условия
					trueStruct = condition() && trueStruct
					#Наличие после условия слова THEN
					trueStruct = @lexemsTable[@i] == 10 && trueStruct



					@i+=1  #Переход к <последовательность операторов>
					#Разбор последовательности операторов после THEN
					trueStruct = sequenceOfStatements() && trueStruct


					#Наличие слова ElSE после последовательности операторов
					trueStruct = @lexemsTable[@i] == 3 && trueStruct
					@i+=1


					#Последовательность операторов после ELSE
					trueStruct = sequenceOfStatements() && trueStruct
					#Ключевое слово ENDIF
					trueStruct = @lexemsTable[@i] == 6 && trueStruct

				#Разбор структуры IF-THEN-ENDIF
				when false
					@syntTree.push ("<структура> = IF <условие> THEN <последовательность операторов> ENDIF")
					#Обработка IF
					trueStruct = 1 


					@i+=1
					#Переходим к разбору условия
					trueStruct = condition() && trueStruct
					#Наличие после условия слова THEN
					trueStruct = @lexemsTable[@i] == 10 && trueStruct


					@i+=1 #Переход к разбору последовательности операторов
					#Разбор последовательности операторов после THEN
					trueStruct = sequenceOfStatements() && trueStruct
					#Ключевое слово ENDIF
					trueStruct = @lexemsTable[@i] == 6 && trueStruct
				end

				return trueStruct
			else
				return false
				err.push ("Ошибка <структура> (IF/IF_ELSE)")
			end


		#WHILE
		when 11
			#Проверяем корректность WHILE-DO-ENDDO
			iter = @i
			includeDO = 0
			includeENDDO = 0
			while iter < @size do
				includeDO = includeDO || @lexemsTable[iter][0] == 2
				includeENDDO = includeENDDO || @lexemsTable[iter][0] == 5
				iter+=1
			end

			correctWhileStruct = includeDO && includeENDDO
			
			if correctWhileStruct
				@syntTree.push ("<структура> = WHILE <условие> DO <последовательность операторов> ENDDO")
			else
				err.push ("Ошибка <структура> (WHILE)")
				return false
			end
		end

		#Разбор <условие> и <последовательность операторов>
		@i+=1 #(Переходим к разбору условия)
		trueWhile = 1
		trueWhile = condition() && trueWhile
		trueWhile = sequenceOfStatements() && trueWhile
		return trueWhile
	end

	#Разбор нетерминала <условие>
	def condition()
		#Если <условие> имеет вид "id || const <операция отношения> id || const"
		trueCondition = @i<@size-2 && (19..20).include?(@lexemsTable[@i][0]) && (19..20).include?(@lexemsTable[@i+2][0])
		if trueCondition
			case @lexemsTable[@i][0]
			when 19
				case @lexemsTable[@i+2][0]
					when 19
						@syntTree.push ("<условие> = идентификатор <знак отношения> идентификатор")
					when 20
						@syntTree.push ("<условие> = идентификатор <знак отношения> константа")
				end

			when 20	
				case @lexemsTable[@i+2][0]
					when 19
						@syntTree.push ("<условие> = константа <знак отношения> идентификатор")
					when 20
						@syntTree.push ("<условие> = константа <знак отношения> константа")
				end
			end

			@i+=1 #Переходим к разбору <операция отношения>
			return signOfTheRatio()

		else
			@err.push ("Ошибка <условие>")
			return false
		end
	end

	#Разбор нетерминала <знак отношения>
	def signOfTheRatio()
		trueSignOfTheRatio = (@lexemsTable[@i][0] == 13)
		if trueSignOfTheRatio
			case @lexemsTable[@i][1]
				when 0	
					@syntTree.push ("<знак отношения> = =")
				when 1
					@syntTree.push ("<знак отношения> = <>")
				when 2
					@syntTree.push ("<знак отношения> = >")
				when 3
					@syntTree.push ("<знак отношения> = <")
				when 4
					@syntTree.push ("<знак отношения> = >=")
				when 5
					@syntTree.push ("<знак отношения> = <=")
			end

			@i+=2 #Переход к разбору следующего оператора
			return true
		else
			@err.push ("Ошибка <знак отношения>")
			return false
		end
	end

	#Разбор нетерминала <последовательность операторов>
	def sequenceOfStatements()
		trueSequenceOfStatements = true
		#Пока не встретили ENDIF, ENDDO, ELSE, IF, WHILE
		while (@i < @size) && trueSequenceOfStatements #&& @lexemsTable[@i][0]!=5 # && @lexemsTable[@i][0]!=6  && @lexemsTable[@i][0]!=3 && @lexemsTable[@i][0]!=7 && @lexemsTable[@i][0]!=11 do
			case @lexemsTable[@i][0]
				when 19 #Идентификатор (скорее всего это присваивание)
					@syntTree.push ("<последовательность операторов> = <оператор>; <последовательность операторов>") if trueSequenceOfStatements
					trueSequenceOfStatements = trueSequenceOfStatements && operator()
					@syntTree.push ("<последовательность операторов> = ничего") unless trueSequenceOfStatements
				when 8 #OUTPUT
					@syntTree.push ("<последовательность операторов> = <оператор>; <последовательность операторов>") if trueSequenceOfStatements
					trueSequenceOfStatements = trueSequenceOfStatements && operator()
					@syntTree.push ("<последовательность операторов> = ничего") unless trueSequenceOfStatements
				when 7 #IF
					@syntTree.push ("<последовательность операторов> = <структура> <последовательность операторов>") if trueSequenceOfStatements
					trueSequenceOfStatements = trueSequenceOfStatements && struct()
					@syntTree.push ("<последовательность операторов> = ничего") unless trueSequenceOfStatements
				when 11 #WHILE
					@syntTree.push ("<последовательность операторов> = <структура> <последовательность операторов>") if trueSequenceOfStatements
					trueSequenceOfStatements = trueSequenceOfStatements && struct()
					@syntTree.push ("<последовательность операторов> = ничего") unless trueSequenceOfStatements
			end
			@i+=1 #Переходим к разбору следующего нетерминала
		end

		if trueSequenceOfStatements
			return true
		else
			return false
			@err.push ("Ошибка <последовательность операторов>")
		end
	end
end