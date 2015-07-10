# animefacedetect
アニメの顔検出を行って保存する

### compile

```bash
$ cd cpp
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### setting

出力ディレクトリを作る

```bash
$ pwd
hogehoge/animefacedetect
$ mkdir output
$ ls
cpp  lbpcascade_animeface.xml  output
```

### usage

```bash
$ ./cpp/build/animefacedetect --input=fugaguga/ラブライブ！第1話.mp4 --output=./output --cascade=./lbpcascade_animeface.xml
```

### 参考
- [kivantium活動日記 - ご注文は機械学習ですか？](http://kivantium.hateblo.jp/entry/2014/11/25/230658)
