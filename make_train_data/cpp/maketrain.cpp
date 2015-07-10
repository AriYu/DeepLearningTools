#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>
#include <cstdio>
#include <iomanip>

#include "argumentsparser.h"

using namespace std;
using namespace cv;
#define ATTRIBUTES 64
#define CLASSES 10
//話ごとに添字を変える
int imagenum=0;
void detectAndDisplay(Mat image, string savepath);

CascadeClassifier face_cascade;

//XMLを読み込んでニューラルネットワークの構築
CvANN_MLP nnetwork;
CvFileStorage* storage;
CvFileNode *n;
VideoWriter output;

int main(int argc, char* argv[]){

  ArgumentsParser argparser;
  argparser.add("--param");
  argparser.add("--input");
  argparser.add("--output");
  argparser.add("--cascade");
  std::map<std::string, std::string> options;
  options = argparser.parse_args(argc, argv, '=');
  argparser.show();

  int framenum = 0;
  //ネットワークのロード
  storage = cvOpenFileStorage( options["--param"].c_str(), 0, CV_STORAGE_READ );
  n = cvGetFileNodeByName(storage,0,"DigitOCR");
  nnetwork.read(storage,n);
  cvReleaseFileStorage(&storage);
    
  //カスケードのロード
  face_cascade.load(options["--cascade"].c_str());

  //動画の読み込み
  Mat frame;
  VideoCapture input(options["--input"]);
  if(!input.isOpened()){
	cout << "Video not found!" << endl;
	return -1;
  }
    
  for(;;){
	framenum++;
	input >> frame;
	if (frame.empty()) {
	  cout << "End of video" << endl;
	  break;
	};
	if(framenum%50==0) detectAndDisplay(frame, options["--output"]);
  }
  return 0;
}

//認識と表示を行う関数
void detectAndDisplay(Mat image, string savepath)
{
    vector<Rect> faces;
    Mat frame_gray;
    stringstream name;

    //画像のグレースケール化
    cvtColor(image, frame_gray, COLOR_BGR2GRAY );
    //ヒストグラムの平坦化
    equalizeHist(frame_gray, frame_gray);
    //顔の認識　小さい顔は除外
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, 0, Size(50,50));
    for(int i = 0; i<faces.size(); i++){
        //顔部分に注目したMatをROIで作る
        Mat Face = image(Rect(faces[i].x, faces[i].y,faces[i].width, faces[i].height));
        Mat norm(Face.size(), Face.type());
        int index;
        float train[64];
        for(int j=0; j<64; j++) train[j] = 0;
        normalize(Face, norm, 0, 255, NORM_MINMAX, CV_8UC3);
        for(int y=0; y<norm.rows; y++){
            for(int x=0; x<norm.cols; x++){
                //Vec3b tmp = norm.at<Vec3b>(y, x);
                index = y*norm.step+x*norm.elemSize();
                int color = (norm.data[index+0]/64)+
                    (norm.data[index+1]/64)*4+
                    (norm.data[index+2]/64)*16;
                train[color]+=1;
            }
        }
        float pixel = norm.cols * norm.rows;
        for(int j=0; j<64; j++){
            train[j] /= pixel;
        }

        //分類の実行
        Mat data(1, ATTRIBUTES, CV_32F);
        for(int col=0; col<=ATTRIBUTES; col++){
            data.at<float>(0,col) = train[col];
        }
        int maxIndex = 0;
        Mat classOut(1,CLASSES,CV_32F);
        nnetwork.predict(data, classOut);
        float value;
        float maxValue=classOut.at<float>(0,0);
        for(int index=1;index<CLASSES;index++){
            value = classOut.at<float>(0,index);
            if(value > maxValue){
                maxValue = value;
                maxIndex=index;
            }
        }
        stringstream name;
        name.str("");
        //分類結果ごとにフォルダ分け
        switch(maxIndex){
            case 0:
			  cout << "eri tyan!" << endl;
			  name << savepath << "/eri/eri" << setw(5) << setfill('0') << imagenum << ".png";
                break;
            case 1:
			  cout << "hanayo tyan!" << endl;
                name <<  savepath << "/hanayo/hanayo" << setw(5) << setfill('0') << imagenum << ".png";
                break;
            case 2:
			  cout << "honoka tyan!" << endl;
                name <<  savepath << "/honoka/honoka" << setw(5) << setfill('0') << imagenum << ".png";
                break;
            case 3:
			  cout << "kotori tyan!" << endl;
                name <<  savepath << "/kotori/kotori" << setw(5) << setfill('0') << imagenum << ".png";
                break;
            case 4:
			  cout << "maki tyan!" << endl;
                name <<  savepath << "/maki/maki" << setw(5) << setfill('0') << imagenum << ".png";
                break;
		    case 5:
			  cout << "niko tyan!" << endl;
                name <<  savepath << "/niko/niko" << setw(5) << setfill('0') << imagenum << ".png";
                break;
		    case 6:
			  cout << "nozomi tyan!" << endl;
                name <<  savepath << "/nozomi/nozomi" << setw(5) << setfill('0') << imagenum << ".png";
                break;
		    case 7:
			  cout << "rin tyan!" << endl;
                name <<  savepath << "/rin/rin" << setw(5) << setfill('0') << imagenum << ".png";
                break;
		    case 8:
			  cout << "umi tyan!" << endl;
                name <<  savepath << "/umi/umi" << setw(5) << setfill('0') << imagenum << ".png";
                break;
		    case 9:
			  cout << "etc" << endl;
                name <<  savepath << "/etc/etc" << setw(5) << setfill('0') << imagenum << ".png";
                break;
        }
        imwrite(name.str(), Face);
        imagenum++;
    }
}
