require 'json'
# This program make convolution
# input in input.json (see example below)
# output in output.json 
# By Titov Alex

# Format of json is 
# {
#   "weights":
#     {
#     "property_1" : property_weight1,
#     "property_2" : property_weight2,
#     "property_n" : property_weightn,
#     },
#   "choices":
#   {
#   	"name1" : 
#     {
# 	    "property_1" : value_1,
# 	    "property_2" : value_2,
# 	    "property_n" : value_n,
#     },
#     "name2" :
#     {
# 	    "property_1" : value_1,
# 	    "property_2" : value_2,
# 	    "property_n" : value_n,
#     }
#   }
# }

def main
	str_data = File.read('input.json',
											 :external_encoding => 'utf-8',
											 :internal_encoding => 'cp866') # hack for encoding. Don't knot what it is
	data = JSON.parse(str_data)

	# Calculate Convolutions
	wrong_chooses_count = 0
	data['choices'].each do |name, choice|
		convolution = 1
		data['weights'].each do |property, weight|
			begin
				convolution *= choice[property] ** weight
			rescue Exception => e # Some of alternatives have no some properties
				convolution = 0
				wrong_chooses_count += 1
				break
			end
		end
		choice['weight'] = convolution * 100
	end


	IO.write('output.json', nice_output_2( data ))
	print_instructions
	puts "There are #{wrong_chooses_count} wrong alternatives"
end

# Output
def nice_output( data )
	choices = data['choices'].sort_by {|name, choice| choice['weight']}.reverse!
	JSON.pretty_generate(choices)
end

def nice_output_2( data )
	output = ''
	choices = data['choices'].sort_by {|name, choice| choice['weight']}.reverse!
	choices.each_with_index  do |choice, index|
		output += "#{index+1}) #{choice[0]} : #{choice[1]['weight']}\n"
	end
	output
end

def print_instructions
	puts 'See weight in output.json'
	puts 'Results are sorted by weight'
end

main


