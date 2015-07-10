# make train data
`train`で作った`param.xml`を用いてDeeplearning用のデータセットを作る

## compile

```bash
$ cd cpp
$ mkdir build
$ cmake ..
$ make
```

## usage

出力するディレクトリに

```bash
.
├── eri
├── etc
├── hanayo
├── honoka
├── kotori
├── maki
├── niko
├── nozomi
├── rin
├── umi
```

このようなディレクトリ構造を作っておく.たとえば、上記のサブディレクトリが`./images`にあるとすれば

```bash
$ ./cpp/build/maketrain --param=param3.xml --cascade=lbpcascade_animeface.xml --input=/home/yuta/Videos/lovelive_encoded/ラブライブ！第1話.mp4 --output=./images
```

とする。あとは手で間違ったやつを仕分ける。
