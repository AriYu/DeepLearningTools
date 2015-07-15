# -*- coding: utf-8 -*-
import cv2
import sys
import os.path
import caffe
from caffe.proto import caffe_pb2
import numpy as np

cascade = cv2.CascadeClassifier("./lbpcascade_animeface.xml")

class NameRGB:
    def __init__(self,name,B,G,R):
        self.name = name
        self.B = B
        self.G = G
        self.R = R

def input_arg(argvs, argc):
    if (argc != 3):   # 引数が足りない場合は、その旨を表示
        print 'Usage: # python %s srcdirectory outputdirectory' % argvs[0]
        quit()        # プログラムの終了

    print 'Input filename = %s' % argvs[1]
    print 'Output filename = %s' % argvs[2]
    # 引数でとったディレクトリの文字列をリターン
    return argvs


def detect(frame):
    # メンバーの名前と矩形の色定義
    MemberList = []
    MemberList.append(NameRGB("eri",11,134,170))
    MemberList.append(NameRGB("hanayo",38,103,150))
    MemberList.append(NameRGB("honoka",70,134,189))
    MemberList.append(NameRGB("kotori",133, 192, 189))
    MemberList.append(NameRGB("maki",114, 114, 237))
    MemberList.append(NameRGB("niko",83, 80, 82))
    MemberList.append(NameRGB("nozomi",128, 92, 104))
    MemberList.append(NameRGB("rin",116, 160, 253))
    MemberList.append(NameRGB("umi",119, 78, 79))
    #MemberList.append(NameRGB("etc",27, 26, 253))
    #顔の認識
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.equalizeHist(gray)

    faces = cascade.detectMultiScale(gray,
                                     scaleFactor = 1.1,
                                     minNeighbors = 1,
                                     minSize = (50,50),
                                     maxSize = (500, 500))
    for (x, y, w, h) in faces:
        image = frame[y:y+h, x:x+w]
        cv2.imwrite("face.png", image)
        image = caffe.io.load_image('face.png')
        predictions = classifier.predict([image], oversample=False)
        sorted_prediction_ind = sorted(range(len(predictions[0])),key=lambda x:predictions[0][x],reverse=True)
        # print sorted_prediction_ind
        pred = np.argmax(predictions)
        for i, value in enumerate(MemberList):
            if pred == i:
                # 確率を代入，文字列変換
                probability = int(predictions[0][int(i)]*100)
                probability = str(probability) + "%"
                # 矩形設置
                cv2.rectangle(frame, (x, y), (x + w, y + h), (value.B, value.G, value.R), 2)
                cv2.rectangle(frame,(x, y),(x+w,y-int(h*0.2)),(value.B, value.G, value.R),-1)
                cv2.putText(frame,value.name,(x, y), cv2.FONT_HERSHEY_SIMPLEX, float(h*0.007),(255, 255, 255),2,cv2.CV_AA)
                cv2.putText(frame,probability,(x, y + int(h*1.1)), cv2.FONT_HERSHEY_SIMPLEX, float(h*0.007),(255,255,255),2,cv2.CV_AA)
       
    return frame


if __name__ == "__main__":
    argvs = sys.argv   # コマンドライン引数を格納したリストの取得
    argc = len(argvs)  # 引数の個数

    filepath = input_arg(argvs, argc)
    #元動画の読み込み
    cap = cv2.VideoCapture(filepath[1])
    # 出力動画の設定
    fourcc = cv2.cv.CV_FOURCC(*'DIVX')
    out = cv2.VideoWriter(filepath[2], 0, 25.0, (720,576))
    # in_image = cv2.imread(filepath[1])
    # out_image = filepath[2]
    mean_blob = caffe_pb2.BlobProto()
    with open('lovelive_mean.binaryproto') as f:
        mean_blob.ParseFromString(f.read())
    mean_array = np.asarray(
    mean_blob.data,
    dtype=np.float32).reshape(
        (mean_blob.channels,
        mean_blob.height,
        mean_blob.width))
    classifier = caffe.Classifier(
        'lovelive_cifar10_quick.prototxt',
        'lovelive_cifar10_quick_iter_4000.caffemodel',
        mean=mean_array,
        raw_scale=255)

    # frame = detect(in_image)
    # cv2.imwrite(out_image, frame)
    while(cap.isOpened()):
        ret, frame = cap.read()
        if ret == True:
            frame = detect(frame)
            out.write(frame)
            cv2.imshow("show video", frame)
            # キー待ち
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        else:
            break
    cap.release()
    out.release()
    cv2.destroyAllWindows()
