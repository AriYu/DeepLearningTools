#ifndef ARGUMENTSPARSER_H_
#define ARGUMENTSPARSER_H_

#include <iostream>
#include <string>
#include <map>
#include <sstream>

class ArgumentsParser
{
 public:
 ArgumentsParser():description_(""){}
 ArgumentsParser(std::string description):description_(description){}
  ~ArgumentsParser(){}
  void add(std::string option){
	options_[option] = "";
  }
  std::map<std::string, std::string> parse_args(int argc, char* argv[], const char delim){
	for(int i = 1; i < argc; i++){
	  std::string value(argv[i]);
	  std::vector<std::string> values = split_equal(argv[i], delim);
	  if(values.size() == 1){
		std::cerr << "WARNING : Cannot find delimiter -> " << delim << std::endl;
		continue;
	  }
	  //キーから値を検索
	  std::map<std::string, std::string>::iterator itr;
	  itr = options_.find(values[0]);
	  if(itr !=  options_.end()){ // キーが見つかったら
		options_[values[0]] = values[1];
	  }else{
		std::cerr << "WARNING : Unknown option -> " << argv[i] << std::endl;
	  }
	}
	return options_;
  }
  void show(){
	std::map<std::string, std::string>::iterator itr;
	for (itr = options_.begin(); itr != options_.end(); itr++)
	  {
		std::cout << itr->first << " : " << itr->second << std::endl;
	  }
  }
 private:
  std::vector<std::string> split_equal(char* argv, const char delim){
	std::vector<std::string> results;
	std::istringstream stream(argv);
	std::string field;
	while(std::getline(stream, field, delim)){
	  results.push_back(field);
	}
	if(results.size() > 2){
	  std::cerr << "[WARNING] : delim is over 2" << std::endl;
	}
	return results;
  }

  std::string description_;
  std::map<std::string, std::string> options_;
};

#endif
