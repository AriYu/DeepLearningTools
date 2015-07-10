#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>
#include <iomanip>

#include "argumentsparser.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
  ArgumentsParser argparser;
  argparser.add("--input");
  argparser.add("--start");
  argparser.add("--end");

  std::map<std::string, std::string> options;
  options = argparser.parse_args(argc, argv, '=');
  argparser.show();

  int index;
  float train[64];
  stringstream name;

  const int files = std::stoi(options["--end"].c_str());
  for(int filenum=std::stoi(options["--start"].c_str()); filenum<=files; filenum++){
	name.str("");
	name << options["--input"] << "/image"<< setw(3) << setfill('0') << filenum << ".png";
	Mat src = imread(name.str());

	if(src.empty()){
	  cout << "Image not found!" << endl;
	  return -1;
	}
	for(int i=0; i<64; i++) train[i] = 0;
	Mat norm(src.size(), src.type());
	Mat sample(src.size(), src.type());
	normalize(src, norm, 0, 255, NORM_MINMAX, CV_8UC3);

	for(int y=0; y<sample.rows; y++){
	  for(int x=0; x<sample.cols; x++){
		index = y*sample.step+x*sample.elemSize();
		int color = (norm.data[index+0]/64)+
		  (norm.data[index+1]/64)*4+
		  (norm.data[index+2]/64)*16;
		train[color]+=1;
	  }
	}
	int pixel = sample.cols * sample.rows;
	for(int i=0; i<64; i++){
	  train[i] /= pixel;
	  cout << train[i] << " ";
	}
	cout << endl;
  }
  return 0;
}
